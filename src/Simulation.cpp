 //****************************************************************************
// Fichier: Simulation.cpp
//
// Date creation: 2012-10-01
// Auteur: 
//                Rio Tinto Alcan                     
//                Energie electrique                  
//                1954 Davis, Saguenay arr. Jonquiere,
//                G7S 4R7, QC, Canada                 
//
//****************************************************************************
#include "stdafx.h"
#include "Simulation.h"

//------------------------------------------------------------------
Simulation::Simulation(BassinVersant& bv, const Parametres& param, const DonneesMeteo& meteo, 
    const mxArray* etatsPrecedents, const std::string& fichierResultats, FontePtr moduleFonte,
    EvapoPtr moduleEvapo, const mxArray* assimilations)
    : bassinVersant_(bv), parametres_(param), donneesMeteo_(meteo), 
    etatsPrecedents_(etatsPrecedents), fichierResultats_(fichierResultats), fonte_(moduleFonte),
    evapo_(moduleEvapo), hasNeigeAjustee_(false) // TODO: Releves neige
{
  FILE_LOG(logDEBUG) << "Simulation::Simulation(...)";

  initialiserAssimilations(assimilations);

  // Validation du nombre de donnees de debit des barrages externes, s'il en existe.
  if (bassinVersant_.possedeBarrage()) {
    std::vector<BarragePtr>::const_iterator barragesIter = bassinVersant_.barrages().begin();
    for (; barragesIter != bassinVersant_.barrages().end(); barragesIter++) {
      if (barragesIter->get()->typeBarrage() == EVACUATION_MESUREE_EXT) {
        if ((int)barragesIter->get()->debitsMesures().size() != parametres_.nbPasSimulation()) {
          std::stringstream erreur;
          erreur << "Le nombre de debit du barrage: " << barragesIter->get()->debitsMesures().size() << "\n";
          erreur << "n'est pas le meme que le nombre de pas de temps: " << parametres_.nbPasSimulation();
          throw std::runtime_error(erreur.str());
        }
      }
    }
  }
}

//------------------------------------------------------------------
Simulation::~Simulation()
{
  FILE_LOG(logDEBUG) << "Simulation::~Simulation()";
}

//------------------------------------------------------------------
// TODO: Releves neige
bool Simulation::hasNeigeAjustee() const {
  return hasNeigeAjustee_;
}

//------------------------------------------------------------------
//------------------------------------------------------------------
int Simulation::executer(bool estMexFile) {
    FILE_LOG(logDEBUG) << "Simulation::executer()";

    // CSV_LOG() << "idCE, idx, Qp, K, B, ho, rayon, dRiv, coeffPompage, Qextrait, AICE, hauteur_mm, HN_avant, HN_apres,";
    int retVal;

    traiterParamFacultatifs();

    float NT_nbCarreauxPartielsCheminLong = (float)bassinVersant_.nbCarreauxPartielsCheminLong();
    float ZN_tempsConcentrationBV = parametres_.transfert().tempsConcentrationBV;
    int NPJO_nbTransfertsParJour = (int)maxf(1.0, NT_nbCarreauxPartielsCheminLong / ZN_tempsConcentrationBV);
    // Duree d'un transfert
    float dureeTransfert = ((float)parametres_.dureeHeuresPasSimulation() * 3600.0f) / (float)NPJO_nbTransfertsParJour;
    initialiserCoeffTransfertCP(NPJO_nbTransfertsParJour);

    initialiserEtats();
    mxArray* etatsFonte = NULL;
    mxArray* etatsEvapo = NULL;
    if (etatsPrecedents_ != NULL) {
        etatsFonte = mxGetField(etatsPrecedents_, 0, "etatsFonte");
        etatsEvapo = mxGetField(etatsPrecedents_, 0, "etatsEvapo");
    }
    fonte_->initialiserEtats(etatsFonte);
    evapo_->initialiserEtats(etatsEvapo);

    DateChrono dateDebut = parametres_.dateDebut();
    DateChrono dateFin = parametres_.dateFin();

    // Profilage de base
    //Util::profiler("Avant boucle sur pas de temps");

    // TODO: Releves neige
    std::ofstream fsAjustement;
    if (parametres_.option().logNeigeAjustee) {
        fsAjustement.open("relevesNeige.csv");
        fsAjustement << std::setprecision(3) << std::fixed;
    }

    // Boucle pas de temps
    int nbPasParJourSimulation = parametres_.nbPasParJourSimulation();
    int unJour = 1;
    int i = 0;
    for (DateChrono dateSimulation = dateDebut; dateSimulation <= dateFin; dateSimulation += unJour) {
        for (int noPasSimulation = 1; noPasSimulation <= nbPasParJourSimulation; noPasSimulation++) {

            //FILE_LOG(logINFO) << "dateSimulation: " << dateSimulation << " - " << noPasSimulation;

            // Donnees CE a assimiler pour cette date?
            if (assimilationsCE_.count(dateSimulation) > 0) {
                // FILE_LOG(logINFO) << "    Assimilation CE: " << dateSimulation << " - " << noPasSimulation;
                retVal = assimiler(dateSimulation, bilans_.back());
            }

            i++;
            //FILE_LOG(logINFO) << "  Production: " << dateSimulation << " - " << noPasSimulation;
            retVal = executerProduction(dateSimulation, noPasSimulation);

            // TODO: Releves neige
            // Releves de neige pour cette date?
            if (parametres_.facultatifs().relevesNeige.count(dateSimulation) != 0) {
                hasNeigeAjustee_ = true;
                if (parametres_.facultatifs().typeAjustementNeige == STATIONS_3) {
                    ajusterNeige3Releves(dateSimulation, fsAjustement);
                }
                else {
                    ajusterNeigeZone(dateSimulation, fsAjustement);
                }
            }

            // Donnees CP a assimiler pour cette date?
            if (assimilationsCP_.count(dateSimulation) > 0) {
                // FILE_LOG(logINFO) << "    Assimilation CP: " << dateSimulation << " - " << noPasSimulation;
                retVal = assimiler(dateSimulation, debits_.back());
            }
            // Donnees Barrage a assimiler pour cette date?
            if (assimilationsBarrages_.count(dateSimulation) > 0) {
                // FILE_LOG(logINFO) << "    Assimilation Barrages: " << dateSimulation << " - " << noPasSimulation;
                retVal = assimiler(dateSimulation, barrages_.back());
            }

            //FILE_LOG(logINFO) << "  Transfert:  " << dateSimulation << " - " << noPasSimulation;
            retVal = executerTransfert(NPJO_nbTransfertsParJour, dateSimulation, noPasSimulation, dureeTransfert);
        }
    }

    // Profilage de base
    //Util::profiler("Apr�s boucle sur pas de temps");

    // Si l'execution se fait a partir d'un mexFile, on n'ecrit pas les resultats,
    // ils seront disponibles dans des variables
    if (!estMexFile) {
        #ifdef ENV_MATLAB
        FILE_LOG(logINFO) << "Ecriture des resultats.";
        ecrireResultats();
        #endif
    }
    return 0;
}


//------------------------------------------------------------------
void Simulation::traiterParamFacultatifs()
{
  FILE_LOG(logDEBUG) << "Simulation::traiterParamFacultatifs()";

  // Vecteur SURFACE
  if (parametres_.facultatifs().superficieCPAmontCorriges.size() > 0) {
    
    const std::map<int, float>* listeCorrections = &parametres_.facultatifs().superficieCPAmontCorriges;
    std::map<int, float>::const_iterator listeCorrectionsIter;
    
    int idCP;
    float superficieCorrigee;
    listeCorrectionsIter = listeCorrections->begin();
    for (; listeCorrectionsIter!= listeCorrections->end(); listeCorrectionsIter++) {
      idCP = listeCorrectionsIter->first;
      superficieCorrigee = listeCorrectionsIter->second;
      // idCP - 1 car l'index est de 0 a nbcp - 1
      bassinVersant_.carreauxPartiels()[idCP-1]->superficieCPAmontCorrige(superficieCorrigee); 
    }
  }

  // Initialisation des parametres spatialisables des carreaux entiers.
  int nbCE = bassinVersant_.nbCarreauxEntiers(), idCE, j;
  CarreauEntierPtr carreauEntierPtr;
  for (int i = 0; i < nbCE; i++) {
    idCE = i + 1;

    // Si le parametre est unique, il s'apllique a l'ensemble des carreaux entiers.
    // Dans ce cas on prend la premiere valeur du vecteur.
    j = (parametres_.sol().fractionImpermeableCE.size() == nbCE ? i : 0);
    bassinVersant_.trouverCarreauEntierParId(idCE)->param().fractionImpermeableCE = 
      parametres_.sol().fractionImpermeableCE[j];

    j = (parametres_.sol().coeffInfiltrationNappe.size() == nbCE ? i : 0);
    bassinVersant_.trouverCarreauEntierParId(idCE)->param().coeffInfiltrationNappe = 
      parametres_.sol().coeffInfiltrationNappe[j];

    j = (parametres_.sol().coeffVidangeBasseNappe.size() == nbCE ? i : 0);
    bassinVersant_.trouverCarreauEntierParId(idCE)->param().coeffVidangeBasseNappe = 
      parametres_.sol().coeffVidangeBasseNappe[j];

    j = (parametres_.sol().coeffVidangeHauteNappe.size() == nbCE ? i : 0);
    bassinVersant_.trouverCarreauEntierParId(idCE)->param().coeffVidangeHauteNappe = 
      parametres_.sol().coeffVidangeHauteNappe[j];

    j = (parametres_.sol().coeffVidangeIntermediaireSol.size() == nbCE ? i : 0);
    bassinVersant_.trouverCarreauEntierParId(idCE)->param().coeffVidangeIntermediaireSol = 
      parametres_.sol().coeffVidangeIntermediaireSol[j];

    j = (parametres_.sol().seuilInfiltrationSolVersNappe.size() == nbCE ? i : 0);
    bassinVersant_.trouverCarreauEntierParId(idCE)->param().seuilInfiltrationSolVersNappe = 
      parametres_.sol().seuilInfiltrationSolVersNappe[j];

    j = (parametres_.sol().seuilVidangeIntermediaireSol.size() == nbCE ? i : 0);
    bassinVersant_.trouverCarreauEntierParId(idCE)->param().seuilVidangeIntermediaireSol = 
      parametres_.sol().seuilVidangeIntermediaireSol[j];

    j = (parametres_.sol().seuilVidangeHauteNappe.size() == nbCE ? i : 0);
    bassinVersant_.trouverCarreauEntierParId(idCE)->param().seuilVidangeHauteNappe = 
      parametres_.sol().seuilVidangeHauteNappe[j];

    j = (parametres_.sol().seuilPrelevementEauTauxPotentiel.size() == nbCE ? i : 0);
    bassinVersant_.trouverCarreauEntierParId(idCE)->param().seuilPrelevementEauTauxPotentiel = 
      parametres_.sol().seuilPrelevementEauTauxPotentiel[j];

    j = (parametres_.sol().hauteurReservoirSol.size() == nbCE ? i : 0);
    bassinVersant_.trouverCarreauEntierParId(idCE)->param().hauteurReservoirSol = 
      parametres_.sol().hauteurReservoirSol[j];

    j = (parametres_.sol().lameEauDebutRuisellement.size() == nbCE ? i : 0);
    bassinVersant_.trouverCarreauEntierParId(idCE)->param().lameEauDebutRuisellement = 
      parametres_.sol().lameEauDebutRuisellement[j];
    
    j = (parametres_.pompage().conductiviteHydraulique.size() == nbCE ? i : 0);
    bassinVersant_.trouverCarreauEntierParId(idCE)->param().conductiviteHydraulique = 
      parametres_.pompage().conductiviteHydraulique[j];

    j = (parametres_.pompage().coeffEmmagasinement.size() == nbCE ? i : 0);
    bassinVersant_.trouverCarreauEntierParId(idCE)->param().coeffEmmagasinement = 
      parametres_.pompage().coeffEmmagasinement[j];
    
    // TODO: Pour l'instant on donne 100% de lac exutoire au CE.
    if (parametres_.facultatifs().lacExutoire[i] > 0) {
      carreauEntierPtr = bassinVersant_.trouverCarreauEntierParId(idCE);
      carreauEntierPtr->pctForet(0);
      carreauEntierPtr->pctLacRiviere(0);
      carreauEntierPtr->pctMarais(0);
      carreauEntierPtr->pctLacExutoire(100);
    }
  }
}

//------------------------------------------------------------------
void  Simulation::initialiserEtats() 
{
  FILE_LOG(logDEBUG) << "Simulation::initialiserEtats()";

  if (bassinVersant_.possedeBarrage()) {
    // only load previous state if type barrage != EVACUATION_MESUREE_EXT
     bool chargerEtats = std::any_of(
        bassinVersant_.barrages().begin(), 
        bassinVersant_.barrages().end(),
        [](const std::shared_ptr<Barrage>& barrage) { 
          return barrage->typeBarrage() != EVACUATION_MESUREE_EXT; 
        }
      );

    // Initialisation etats barrages.
    EtatsBarrages etatsInitiauxBarrages;
    
  if (chargerEtats) {

    // Chargement du volume initial des barrages a partir d'une sauvegarde, 
    // si elle existe.
    chargerEtatsBarrages();

	int nbBarrages = bassinVersant_.nbBarrages();
    for (int i = 0; i < nbBarrages; i++) {
	  // Pas d'etats initiaux pour un barrage externe
      if (bassinVersant_.barrages()[i]->typeBarrage() != EVACUATION_MESUREE_EXT) {
        EtatBarrage etatInitial;
        etatInitial.idCarreauPartiel = bassinVersant_.barrages()[i]->idCP();
        etatInitial.volume = bassinVersant_.barrages()[i]->volume();
        // Le niveau initial n'a pas d'incidence sur les calculs des etats suivants.
        etatInitial.niveau = - (std::numeric_limits<float>::max)();
        etatsInitiauxBarrages.push_back(etatInitial);
      }
    }

  }
    
	if (etatsInitiauxBarrages.size() > 0) {
      barrages_.reserve(parametres_.nbPasSimulation() + 1);
      barrages_.push_back(etatsInitiauxBarrages);
    }
  }

  // Initialisation etats carreaux entiers.
  int nbCE = bassinVersant_.nbCarreauxEntiers(), nbChargementsCE;
  EtatsCarreauxEntiers etatsInitiauxCE;
  etatsInitiauxCE.reserve(nbCE);

  // Chargement des etats initiaux a partir d'une sauvegarde, si elle existe.
  nbChargementsCE = chargerEtatsCE(etatsInitiauxCE);
  // Pas d'etats precedents charges, valeurs par defaut
  if (nbChargementsCE == 0) {
    for (int i = 0; i < nbCE; i++) {
      EtatCarreauEntier etatInitial = 
        {i+1, 0, 0, 
        parametres_.solInitial().niveauInitialSol, 
        parametres_.solInitial().niveauInitialNappe, 
        parametres_.solInitial().niveauInitialLacsMarais,
        0.0f, 0.0f, 0.0f};
    
        etatsInitiauxCE.push_back(etatInitial);
    }
  }
  // Erreur: Le nombre de CE des etats precedents n'est pas le meme que pour le bassin 
  else if (nbChargementsCE != nbCE) {
    std::stringstream erreur;
    erreur << "Le nombre de CE des etats initiaux: " << nbChargementsCE << std::endl;
    erreur << "n'est pas le meme que pour le bassin versant: " << nbCE;
    throw std::runtime_error(erreur.str());
  }
  
  bilans_.reserve(parametres_.nbPasSimulation() + 1);
  bilans_.push_back(etatsInitiauxCE);

  // Initialisation etats carreaux partiels.
  int nbCP = bassinVersant_.nbCarreauxPartiels(), nbChargementsCP;
  EtatsCarreauxPartiels etatsInitiauxCP;
  etatsInitiauxCP.reserve(nbCP);
  
  // Chargement des etats initiaux a partir d'une sauvegarde, si elle existe.
  nbChargementsCP = chargerEtatsCP(etatsInitiauxCP);
  // Pas d'etats precedents charges, valeurs par defaut
  if (nbChargementsCP == 0) {
    for (int i = 0; i < nbCP; i++) {
      EtatCarreauPartiel etatInitial; 
    
      etatInitial.idCarreauPartiel = bassinVersant_.carreauxPartiels()[i]->id();
      etatInitial.volume = bassinVersant_.carreauxPartiels()[i]->volumeInitial();
      etatInitial.apport = 0.0f;
      etatInitial.debit = (i == 0 ? parametres_.solInitial().debitInitialExutoire : 0.0f);
      // QUALITE
      etatInitial.Qualite.volumeSortant = 0.0f;

      etatsInitiauxCP.push_back(etatInitial);
    }
  }
  // Erreur: Le nombre de CP des etats precedents n'est pas le meme que pour le bassin 
  else if (nbChargementsCP != nbCP) {
    std::stringstream erreur;
    erreur << "Le nombre de CP des etats initiaux: " << nbChargementsCP << std::endl;
    erreur << "n'est pas le meme que pour le bassin versant: " << nbCP;
    throw std::runtime_error(erreur.str());
  }

  debits_.reserve(parametres_.nbPasSimulation() + 1);
  debits_.push_back(etatsInitiauxCP);
}

//------------------------------------------------------------------
int Simulation::calculerIndexMeteo(const DateChrono& datePasDeTemps, const int& noPasDeTemps)
{
  FILE_LOG(logDEBUG) << "Simulation::calculerIndexMeteo(const date& datePasDeTemps, const int& noPasDeTemps)";
  int index = parametres_.nbPasParJourSimulation() * 
              (datePasDeTemps.getDayNumber() - parametres_.dateDebut().getDayNumber()) +
              (noPasDeTemps - 1); 

  return index;
}

int Simulation::calculerIndexPompage(const DateChrono& datePasDeTemps, const int& noPasDeTemps) {
    FILE_LOG(logDEBUG) << "Simulation::calculerIndexPompage(const date& datePasDeTemps, const int& noPasDeTemps)";
    int index = parametres_.nbPasParJourSimulation() *
        (datePasDeTemps.getDayNumber() - parametres_.dateDebut().getDayNumber()) +
        (noPasDeTemps - 1);

    return index;
}
