 //****************************************************************************
// Fichier: SimulationQualite.cpp
//
// Date creation: 2012-04-11
// Auteur: 
//                Rio Tinto Alcan                     
//                Energie electrique                  
//                1954 Davis, Saguenay arr. Jonquiere,
//                G7S 4R7, QC, Canada                 
//
//****************************************************************************
#include "stdafx.h"
#include "SimulationQualite.h"

//------------------------------------------------------------------
SimulationQualite::SimulationQualite(Simulation& simulation, ParametresQualite& param, DLI& dli,
  const mxArray* etatsPrecedents, const mxArray* assimilations) 
  : simulation_(simulation), bassinVersant_(simulation.bassinVersant_), donneesMeteo_(simulation.donneesMeteo_),
    parametresQualite_(param), dli_(dli), etatsPrecedents_(etatsPrecedents)
{
  FILE_LOG(logDEBUG) << "SimulationQualite::SimulationQualite(Simulation& simulation)";
  initialiserAssimilations(assimilations);
}

//------------------------------------------------------------------
SimulationQualite::~SimulationQualite()
{
  FILE_LOG(logDEBUG) << "SimulationQualite::~SimulationQualite()";
}

//------------------------------------------------------------------
int SimulationQualite::executer(bool estMexFile)
{
  FILE_LOG(logDEBUG) << "SimulationQualite::executer(bool estMexFile)";
  int retVal;
  
  initialiserEtats();
  
  DateChrono dateDebut = simulation_.parametres_.dateDebut();
  DateChrono dateFin = simulation_.parametres_.dateFin();

  //date_duration unJour(1);
  int nbPasParJourSimulation = simulation_.parametres_.nbPasParJourSimulation();
  int unJour = 1;
  for (DateChrono dateSimulation = dateDebut; dateSimulation <= dateFin; dateSimulation += unJour) {
    for (int noPasSimulation = 1; noPasSimulation <= nbPasParJourSimulation; noPasSimulation++) {
      
      // Donnees CP a assimiler pour cette date?
      if (assimilationsCP_.count(dateSimulation) > 0) {
        // FILE_LOG(logINFO) << "    Assimilation CP: " << dateSimulation;
        retVal = assimiler(dateSimulation, qualite_.back());
      }

      calculerTemperatureEau(dateSimulation, noPasSimulation);
    }
  }

  return 0;
}

//------------------------------------------------------------------
void SimulationQualite::initialiserEtats()
{
  // Initialisation etats carreaux partiels.
  int nbCP = bassinVersant_.nbCarreauxPartiels(), nbChargementsCP;
  Qualite::EtatsCarreauxPartiels etatsInitiauxCP;
  etatsInitiauxCP.reserve(nbCP);

  // Chargement des etats initiaux a partir d'une sauvegarde, si elle existe.
  nbChargementsCP = chargerEtatsCP(etatsInitiauxCP);
  
  // Pas d'etats precedents charges, valeurs par defaut
  if (nbChargementsCP == 0) {
    for (int i = 0; i < nbCP; i++) {
      Qualite::EtatCarreauPartiel etatInitial; 
    
      etatInitial.idCarreauPartiel = bassinVersant_.carreauxPartiels()[i]->id();
      etatInitial.temperature = parametresQualite_.temperature().tinit;
      etatsInitiauxCP.push_back(etatInitial);
    }
  }
  // Erreur: Le nombre de CP des etats precedents n'est pas le meme que pour le bassin 
  else if (nbChargementsCP != nbCP) {
    std::stringstream erreur;
    erreur << "Le nombre de CP des etats initiaux Qualite: " << nbChargementsCP << std::endl;
    erreur << "n'est pas le meme que pour le bassin versant: " << nbCP;
    throw std::runtime_error(erreur.str());
  }

  qualite_.reserve(simulation_.parametres_.nbPasSimulation() + 1);
  qualite_.push_back(etatsInitiauxCP);

}

int SimulationQualite::calculerIndexPasDeTemps(const DateChrono& datePasDeTemps, const int& noPasDeTemps) {
    FILE_LOG(logDEBUG) << "SimulationQualite::calculerIndexPasDeTemps(const date& datePasDeTemps, const int& noPasDeTemps)";
    int index = simulation_.parametres_.nbPasParJourSimulation() *
        (datePasDeTemps.getDayNumber() - simulation_.parametres_.dateDebut().getDayNumber()) +
        (noPasDeTemps - 1);

    return index;
}

int SimulationQualite::calculerTemperatureEau(const DateChrono& datePasDeTemps, const int& noPasDeTemps) {
    FILE_LOG(logDEBUG) << "calculerTemperatureEau(const date& datePasDeTemps)";

    const int nbCP = bassinVersant_.nbCarreauxPartiels();
    const int noJour = datePasDeTemps.getDayOfYear();
    // const int indexPasDeTemps = datePasDeTemps.getDayNumber() - simulation_.parametres_.dateDebut().getDayNumber();
    const int indexPasDeTemps = calculerIndexPasDeTemps(datePasDeTemps, noPasDeTemps);

    // FILE_LOG(logINFO) << "datePasDeTemps = " << datePasDeTemps << " noPasDeTemps = " << noPasDeTemps << " idx = " << indexPasDeTemps; 

    const ParamTemperature& paramTemperature = parametresQualite_.temperature();

    //! variable de surface
    float surfaceTotal = 0.0f, surfaceRiviere = 0.0f;

    //! variable de fonte
    float stockageMoyen;

    //! variable d'Ã©nergie (e....)
    float eAMONT, eLOCAL, eRESEAU;

    //! variable d'enthalpie (q....)
    float qRUISSEAU, qNAPPE, qHYPO, qLACMA, qEVAP, qCONV, qRADSO, qRADIN;

    //! variable de volume
    float volume, volumeAMONT, volumeLOCAL, volumeRESEAU, volumeDIL;

    //! variable de tempÃ©rature (T...)
    float TEAU, TAIR, TAIRMoyen, TRUISSEAU, TNAPPE, TSOL, temperature;

    //! variable de tempÃ©rature en Kelvin (TK...)
    float TKAIR, TKEAU;

    //! variable de atmospherique
    float RADIN, RADSOC;

    int idCPAmont, idCE;
    CarreauEntierPtr carreauEntierPtr;
    CarreauPartielPtr carreauPartielPtr;

    // Index = 0 est pour l'initialisation, donc on debute a 1
    const EtatsCarreauxEntiers& etatsActuelsCE = simulation_.bilans_[indexPasDeTemps + 1];
    EtatCarreauEntier etatActuelCE;

    const EtatsCarreauxPartiels& etatsActuelsCP = simulation_.debits_[indexPasDeTemps + 1];
    EtatCarreauPartiel etatActuelCP;

    // Declarer ici les modules de fonte ayant des etats pouvant participer
    // au calcul de la temperature
    const std::vector<FonteCequeau::EtatFonteCE>* etatsActuelsFonteCE_cequeau;
    const FonteCequeau::EtatFonteCE* etatActuelFonteCE_cequeau;
    
    const std::vector<FonteCemaNeige::EtatFonteCE>* etatsActuelsFonteCE_cemaneige;
    const FonteCemaNeige::EtatFonteCE* etatActuelFonteCE_cemaneige;

    const std::vector<FonteUEB::EtatFonteCE>* etatsActuelsFonteCE_ueb;
    const FonteUEB::EtatFonteCE* etatActuelFonteCE_ueb;

    // Declarer ici les modules d'evapo ayant des etats pouvant participer
    // au calcul de la temperature
    const std::vector<EvapoCequeau::EtatEvapoCE>* etatsActuelsEvapoCE_Cequeau;
    const EvapoCequeau::EtatEvapoCE* etatActuelEvapoCE_Cequeau;

    const std::vector<EvapoPriestleyTaylor::EtatEvapoCE>* etatsActuelsEvapoCE_PriestleyTaylor;
    const EvapoPriestleyTaylor::EtatEvapoCE* etatActuelEvapoCE_PriestleyTaylor;

    float valCalculEvapo;

    const Qualite::EtatsCarreauxPartiels& etatsPrecedentsQualite = qualite_.back();
    Qualite::EtatCarreauPartiel etatPrecCarreauPartiel;

    Qualite::EtatsCarreauxPartiels etatsCarreauxPartielsQualite;
    etatsCarreauxPartielsQualite.assign(nbCP, etatPrecCarreauPartiel);

    MeteoPtr meteo;
    float RADSO, ENNUAG, PREVAP, VITVEN, DLI, LAI, LAI_NORM;

    // initialisation des donnees pour de position du soleil
    // pour calculer le ratio d'ombre sur un cours d'eau
    PositionSolaire positionSolaire;
    float ratioOmbre = 0.0f;
    float cCanope = parametresQualite_.temperature().ccanope;

    for (int i = nbCP - 1; i >= 0; i--) {

        surfaceTotal = 0.0f, surfaceRiviere = 0.0f;
        stockageMoyen = 0.0f;
        eAMONT = 0.0f, eLOCAL = 0.0f, eRESEAU = 0.0f;
        qRUISSEAU = 0.0f, qNAPPE = 0.0f, qHYPO = 0.0f, qLACMA = 0.0f, qEVAP = 0.0f, qCONV = 0.0f, qRADSO = 0.0f, qRADIN = 0.0f;
        volume = 0.0f, volumeAMONT = 0.0f, volumeLOCAL = 0.0f, volumeRESEAU = 0.0f, volumeDIL = 0.0f;
        TEAU = 0.0f, TAIR = 0.0f, TAIRMoyen = 0.0f, TRUISSEAU = 0.0f, TNAPPE = 0.0f, TSOL = 0.0f, temperature = 0.0f;
        TKAIR = 0.0f, TKEAU = 0.0f;
        RADIN = 0.0f, RADSOC = 0.0f;

        carreauPartielPtr = bassinVersant_.carreauxPartiels()[i];
        idCE = carreauPartielPtr->idCarreauEntier();
        carreauEntierPtr = bassinVersant_.trouverCarreauEntierParId(idCE);
        etatActuelCE = etatsActuelsCE[idCE - 1];

        // etatActuelFonteCE = etatsActuelsFonteCE[idCE - 1];
        etatActuelCP = etatsActuelsCP[i];
        etatPrecCarreauPartiel = etatsPrecedentsQualite[i];

        // Meteo pour ce CP
        // 0- Nebulosite, 1- Pression vapeur, 2- Radiation solaire 3- Vitesse vent
        meteo = donneesMeteo_.valeurs()[indexPasDeTemps][idCE - 1];
        ENNUAG = meteo->meteoAutre()[0];
        PREVAP = meteo->meteoAutre()[1];
        RADSO = meteo->meteoAutre()[2];
        VITVEN = meteo->meteoAutre()[3];
        DLI = meteo->meteoAutre()[4]; //(Mostafa)
        LAI = meteo->meteoAutre()[5];
        LAI_NORM = meteo->meteoAutre()[6];

        // Calcul des apports de l'amont
        for (int j = 0; j < NB_CP_AMONT; j++) {
            idCPAmont = carreauPartielPtr->idCarreauxPartielsAmont()[j];

            // Un 0 indique la fin de la liste des CP en amont.
            if (idCPAmont == 0) {
                break;
            } else {
                eAMONT += etatsCarreauxPartielsQualite[idCPAmont - 1].temperature *
                    etatsActuelsCP[idCPAmont - 1].Qualite.volumeSortant * 4.187f;
                volumeAMONT += etatsActuelsCP[idCPAmont - 1].Qualite.volumeSortant;
            }
        }

        // Cumul des apports locaux d'energie
        TEAU = etatPrecCarreauPartiel.temperature;
        
        // Retrieve snowmelt data according to the model choice
        switch (simulation_.parametres_.option().moduleFonte)
        {
          case ModuleFonte::CEQUEAU:
            etatsActuelsFonteCE_cequeau = &((FonteCequeau*)simulation_.fonte_.get())->etatsFonte()[indexPasDeTemps + 1];
            etatActuelFonteCE_cequeau = &(*etatsActuelsFonteCE_cequeau)[idCE -1];
            stockageMoyen = etatActuelFonteCE_cequeau->SNC_stockNeigeForet * carreauEntierPtr->pctForet() +
              etatActuelFonteCE_cequeau->SND_stockNeigeClairiere * (1.0f - carreauEntierPtr->pctForet());
            break;
          case ModuleFonte::CEMA_NEIGE:
            etatsActuelsFonteCE_cemaneige = &((FonteCemaNeige*)simulation_.fonte_.get())->etatsFonte()[indexPasDeTemps + 1];
            etatActuelFonteCE_cemaneige = &(*etatsActuelsFonteCE_cemaneige)[idCE -1];
            stockageMoyen = etatActuelFonteCE_cemaneige->G;
            break;
          case ModuleFonte::UEB:
            etatsActuelsFonteCE_ueb = &((FonteUEB*)simulation_.fonte_.get())->etatsFonte()[indexPasDeTemps + 1];
            etatActuelFonteCE_ueb = &(*etatsActuelsFonteCE_ueb)[idCE -1];
            stockageMoyen = 1000*etatActuelFonteCE_ueb->w;
            break;
          default:
            throw std::runtime_error("The chosen snowmelt routine is not yet available for water temperature simulations");
					  break;
        }

        if (TEAU != 0.0f || stockageMoyen <= paramTemperature.crigel) {
            TAIR = simulation_.donneesMeteo_.valeurs()[indexPasDeTemps][idCE - 1]->calculerTempMoy();
            TAIRMoyen = maxf(0.0f, TAIR);

            surfaceTotal = 1000.0f * bassinVersant_.superficieCarreauEntier() * carreauPartielPtr->pctSurface();

            surfaceRiviere = ((carreauPartielPtr->largeurCoursEauPrincipal() / 10.0f) * parametresQualite_.colarg()) *
                (carreauPartielPtr->longueurCoursEauPrincipal() / 10.0f) * 1000.0f;

            volume = surfaceRiviere * ((carreauPartielPtr->profondeurMinimum() / 100.0f) * parametresQualite_.coprom());

            volumeLOCAL = ((etatActuelCE.Qualite.RUISST + etatActuelCE.Qualite.SOLT + etatActuelCE.Qualite.SONAPT) *
                carreauEntierPtr->pctSol() + etatActuelCE.Qualite.SLAMAT * carreauEntierPtr->pctEau()) *
                surfaceTotal;

            volumeRESEAU = volume + etatActuelCP.volume;
            volumeDIL = volumeRESEAU + volumeAMONT + volumeLOCAL;

            /*** Calcul des energies associees aux apports advectifs locaux d'eau ***/
            // 1- Ruissellement de surface
            TRUISSEAU = TAIRMoyen;
            if (stockageMoyen > 1.0) {
                TRUISSEAU = TAIRMoyen * (1.0f - stockageMoyen / (paramTemperature.crigel + 1.0f));
                TRUISSEAU = maxf(0.0f, TRUISSEAU);
            }
            qRUISSEAU = etatActuelCE.Qualite.RUISST * carreauEntierPtr->pctSol() * surfaceTotal * TRUISSEAU * 4.187f;

            // 2- Eaux provenant de la nappe
            TNAPPE = paramTemperature.panap * paramTemperature.tnap + (1.0f - paramTemperature.panap) * TAIRMoyen;
            qNAPPE = etatActuelCE.Qualite.SONAPT * carreauEntierPtr->pctSol() * surfaceTotal * TNAPPE * 4.187f;

            // 3- Ruissellement hypodermique
            //---> Module original <---
            TSOL = (TNAPPE + TRUISSEAU) / 2.0f;
            qHYPO = etatActuelCE.Qualite.SOLT * carreauEntierPtr->pctSol() * surfaceTotal * TSOL * 4.187f;
            /***
                  float CC = carreauPartielPtr->pctFermetureFutaie(); //CC = 0.5; // Crown Closure (0-1)
                  int LAI = carreauPartielPtr->indiceFoliaire(); //LAI = 3; // Leaf Area Index (0-10)
                  float t1 = 365; //annual period (365 d)
                  float A0 = 12.5; // maximum amplitude of soil surface temperature (ï¿½C)
                  float z1 = 0.5; // depth used for soil temperature measurements (cm)
                  //float c1 = 1.56; // specific heat of soil (J/(g/ï¿½C))
                  //float rho_s = 2.65;// soil density (g/cm3)
                  float t0 = 30; //nb of days between jan 1 and coldest soil temperature (d)
                  float Dh = 0.026; // Thermal diffusivity (m2/d)
                  float w = (2*PI)/365;
                  float d = std::pow((2*Dh/w),0.5f);

                  // Soil Temperature from Hillel (1982)
                  TSOL = A0*exp(-z1/d)*sin((2*PI*(noJour-t0))/t1-(z1/d)-(PI/2));

                  qHYPO = etatActuelCE.Qualite.SOLT * carreauEntierPtr->pctSol() * surfaceTotal *
                      sin((PI*noJour)/t1)*(1+exp(-CC*LAI))*TSOL* 4.187f;
            ***/
            // 4- Debordement des lacs et marais
            qLACMA = etatActuelCE.Qualite.SLAMAT * carreauEntierPtr->pctEau() * surfaceTotal * TEAU * 4.187f;

            /*** Calcul des echanges d'energies a la surface de l'eau ***/
            // 5- Radiation solaire
            if (etatActuelCE.Qualite.PPTTOT == 0.0f) {
                RADSOC = RADSO * (1.0f + paramTemperature.corsol);
            } else if (etatActuelCE.Qualite.PPTTOT > paramTemperature.bassol) {
                RADSOC = RADSO * (1.0f - paramTemperature.corsol);
            } else {
                RADSOC = RADSO;
            }
            
            int moduleOmbrage = simulation_.parametres_.option().moduleOmbrage;

            if (moduleOmbrage != 0) {
                float heure;
                double latitude, longitude, altitude;
                double largeurRiviere, hauteurArbre, azimRiviere;
                double pctConifer, pctFeuillu;

                latitude = (double)carreauPartielPtr->latitude();
                longitude = (double)carreauPartielPtr->longitude();
                altitude = (double)carreauPartielPtr->altitudeMoyenne();
                largeurRiviere = (double)carreauPartielPtr->largeurCoursEauPrincipal() / 10.0f;
                azimRiviere = (double)carreauPartielPtr->azimutCoursEau();
                
                hauteurArbre = (double)carreauPartielPtr->hautMoyenneArbre();

                pctConifer = (double)carreauPartielPtr->pctConifer();
                pctFeuillu = (double)carreauPartielPtr->pctFeuillu();

                if (simulation_.parametres_.nbPasParJourSimulation() == 1) {
                    heure = parametresQualite_.temperature().hombre; // constant
                    
                    // -------------calcul de la position du soleil Ã  cette date------------------
                    Qualite::spa_data spa{ datePasDeTemps, heure, latitude, longitude, altitude, FUSEAU_HORAIRE };
                    positionSolaire.calcul_altitude_et_azimut_du_soleil(spa);
                    
                    // -------------ratio d'ombre bloquant la radiation du soleil-----------------
                    if (moduleOmbrage == 1) {
                      ratioOmbre = (float) calculerRatioOmbre(spa.altitude, spa.azimuth, azimRiviere, largeurRiviere, hauteurArbre);
                      ratioOmbre = minf(1.0f, ratioOmbre);

                      qRADSO = paramTemperature.crayso * surfaceRiviere * RADSOC * (1.0f - ratioOmbre);

                    } else if (moduleOmbrage == 2) {
                        float max_lai_norm = minf(LAI_NORM, 0.8f);

                        float scaled_crayso = paramTemperature.crayso - max_lai_norm;               

                        qRADSO = scaled_crayso * surfaceRiviere * RADSOC;
                    } 

                }
                
                //TODO: Rajouter des options de calculs de ratios selon le pas de temps
            } else {
              qRADSO = paramTemperature.crayso * surfaceRiviere * RADSOC;  
            }

            // 6- Radiation infrarouge
            TKAIR = TAIR + 273.2f;
            TKEAU = TEAU + 273.2f;
            double ea = PREVAP;
            double B = ENNUAG;
            double sigma = 4.8992E-9f;

            RADIN = static_cast<float>(dli_.calculeRadiation(sigma, ENNUAG, TKAIR, PREVAP, TKEAU, LAI, DLI));

            qRADIN = paramTemperature.crayin * RADIN * surfaceRiviere;

            // 7- Evaporation
            //***test*** etatActuelEvapoCE.stateEauLibre OU etatActuelEvapoCE.statePot 
            switch (simulation_.parametres_.option().moduleEvapo) {
            case ModuleEvapo::PRIESTLEYTAYLOR:
                etatsActuelsEvapoCE_PriestleyTaylor =
                    &((EvapoPriestleyTaylor*)simulation_.evapo_.get())->etatsEvapo()[indexPasDeTemps + 1];

                etatActuelEvapoCE_PriestleyTaylor = &(*etatsActuelsEvapoCE_PriestleyTaylor)[idCE - 1];
                valCalculEvapo = etatActuelEvapoCE_PriestleyTaylor->stateEauLibre;
                break;
            case ModuleEvapo::CEQUEAU:
                etatsActuelsEvapoCE_Cequeau =
                    &((EvapoCequeau*)simulation_.evapo_.get())->etatsEvapo()[indexPasDeTemps + 1];

                etatActuelEvapoCE_Cequeau = &(*etatsActuelsEvapoCE_Cequeau)[idCE - 1];
                valCalculEvapo = etatActuelEvapoCE_Cequeau->statePot;
                break;
            default:
                valCalculEvapo = etatActuelCE.evapoPotJour;
                break;
            }

            qEVAP = -paramTemperature.cevapo * valCalculEvapo * surfaceRiviere * 2.480f;

            // 8- Convection
            qCONV = paramTemperature.cconve * 0.2f * VITVEN * (TAIR - TEAU) * surfaceRiviere;

            /*** Bilan local ***/
            eRESEAU = volumeRESEAU * TEAU * 4.187f;
            eLOCAL = qRUISSEAU + qHYPO + qNAPPE + qRADSO + qRADIN + qEVAP + qCONV;

            temperature = (eRESEAU + eAMONT + eLOCAL) / (volumeDIL * 4.187f);
            temperature = maxf(0.0f, temperature);
            temperature = minf(35.0f, temperature);

        } // if (TEAU != 0.0f || stockageMoyen <= CRIGEL)

        // Sauvegarde de l'etat du CP
        etatsCarreauxPartielsQualite[i].idCarreauPartiel = i + 1;
        etatsCarreauxPartielsQualite[i].temperature = temperature;
        etatsCarreauxPartielsQualite[i].qruiss = qRUISSEAU;
        etatsCarreauxPartielsQualite[i].qnappe = qNAPPE;
        etatsCarreauxPartielsQualite[i].qhypo = qHYPO;
        etatsCarreauxPartielsQualite[i].qlacma = qLACMA;
        etatsCarreauxPartielsQualite[i].qradso = qRADSO;
        etatsCarreauxPartielsQualite[i].qradin = qRADIN;
        etatsCarreauxPartielsQualite[i].qevap = qEVAP;
        etatsCarreauxPartielsQualite[i].qconv = qCONV;
        etatsCarreauxPartielsQualite[i].enerlo = eLOCAL;
        etatsCarreauxPartielsQualite[i].eneram = eAMONT;
        etatsCarreauxPartielsQualite[i].eneres = eRESEAU;
    }

    // Sauvegarde du pas de temps
    qualite_.push_back(etatsCarreauxPartielsQualite);

    return 0;
}

//------------------------------------------------------------------
int SimulationQualite::assimiler(const DateChrono& datePasDeTemps, Qualite::EtatsCarreauxPartiels& etatsCarreauxPartiels)
{
  FILE_LOG(logDEBUG) << "SimulationQualite::assimiler(const date& datePasDeTemps, EtatsCarreauxPartiels& etatsCarreauxPartiels)";

  if (assimilationsCP_.count(datePasDeTemps) > 0) {
    Qualite::EtatsCarreauxPartiels etatsSimules;
    Qualite::EtatsCarreauxPartiels::iterator iterCP;
    // Toutes les assimilations CP pour ce pas de temps
    Qualite::EtatsCarreauxPartielsAssim assimilations = assimilationsCP_.find(datePasDeTemps)->second;
    
    // Parcourt des assimilations
    Qualite::EtatsCarreauxPartielsAssim::const_iterator assimilationsIter = assimilations.begin();
    for (; assimilationsIter != assimilations.end(); assimilationsIter++) {

      // Parcourt des etats
      for (iterCP = etatsCarreauxPartiels.begin(); iterCP != etatsCarreauxPartiels.end(); iterCP++) {

        // Si id egaux, on fait la mise a jour
        if (iterCP->idCarreauPartiel == assimilationsIter->idCarreauPartiel) {
          // Sauvegarde de l'etat avant l'assimilation
          etatsSimules.push_back(*iterCP);

          AssimilationHelper::assimilerValeur(assimilationsIter->temperature, assimilationsIter->temperatureType, iterCP->temperature);

          break;
        }
      } // for
    } // for

    // Conservation des etats simules pour comparaison avec etats assimiles
    if (etatsSimules.size() > 0) {
      avantAssimilationsCP_.insert(std::make_pair(datePasDeTemps, etatsSimules));
    }

  } // if (assimilationsCP_.count(datePasDeTemps) > 0)

  return 0;
}


double SimulationQualite::calculerCanopeLAI(double LAI) {
    return LAI;
}

double SimulationQualite::calculerCanopeTypeArbre(double pctConifer, double pctFeuillu) {
    double cOmbrageCanope;
    double cOmbreFeuillu, cOmbreConifer;
    cOmbreConifer = 0.42;
    cOmbreFeuillu = 0.55;

    double cIndiceFolliaire = ((pctConifer / 100.0f) * cOmbreConifer + (pctFeuillu / 100.0f) * cOmbreFeuillu);
    
    cOmbrageCanope = cIndiceFolliaire;
    
    return cOmbrageCanope;
}


double SimulationQualite::calculerRatioOmbre(double altitudeSolaire, double azimutSolaire, double azimutRiviere, double largeurRiviere, double hauteurArbre) const {
    double theta, longueurOmbre, longueurPerpendiculaireOmbre, ratioOmbre;
    //! coefficient pour convertir des degrÃ©s en radians
    double coefRad = PI / 180;
    //! coefficient pour convertir des radians en degrÃ©s
    double coefDeg = 180 / PI;

    // ------angle entre l'azimut et l'orientation du cours d'eau------
    theta = azimutSolaire - azimutRiviere;

    // conversion en radians
    theta = theta * coefRad;
    altitudeSolaire = altitudeSolaire * coefRad;

    // ----------------longueur d'ombre moyenne------------------------
    /*On veut connaÃ®tre la couverture de l'ombre sur la riviere. Pour ce faire
    il faut alors connaÃ®tre la longueur de l'ombre moyen sur la riviere, voilÃ  pourquoi
    cette longueur doit Ãªtre perpendiculaire.
    */
    longueurOmbre = hauteurArbre / tan(altitudeSolaire);
    longueurPerpendiculaireOmbre = sin(theta) * longueurOmbre;

    // ------------------couverture d'ombre sur la riviere--------------
    /*En moyenne, quel pourcentage de la riviere est habituellement couvert par
    l'ombre des arbres, bloquant ainsi les rayons du soleil.
    */
    ratioOmbre = (longueurPerpendiculaireOmbre / largeurRiviere);
    return abs(ratioOmbre);

}
