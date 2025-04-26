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

// TODO: Releves neige
#include "FonteCequeau.h"

//------------------------------------------------------------------
int Simulation::executerProduction(const DateChrono& datePasDeTemps, const int& noPasDeTemps)
{
  FILE_LOG(logDEBUG) << "Simulation::executerProduction(int jourPasDeTemps)";
  

  // Determiner la quantite d'eau disponible a chacun des CE
  // pour un pas de temps.
  //int jourPasDeTemps = datePasDeTemps.day_of_year();
  int jourPasDeTemps = datePasDeTemps.getDayOfYear();
  int nbCE = bassinVersant_.nbCarreauxEntiers();
  EtatsCarreauxEntiers etatsPasDeTemps;
  etatsPasDeTemps.reserve(nbCE);

  int indexMeteo = calculerIndexMeteo(datePasDeTemps, noPasDeTemps);
  int indexPompage = calculerIndexPompage(datePasDeTemps, noPasDeTemps);
  int indexPasDeTemps = calculerIndexMeteo(datePasDeTemps, noPasDeTemps);

  // Variable de travail contenant les etats du pas de temps precedent
  EtatsCarreauxEntiers etatsPasDeTempsPrecedent = bilans_.back(); 

  // Boucle carreaux entiers
  for (int indexCE = 0; indexCE < nbCE; indexCE++) {
    Meteo& meteo = *donneesMeteo_.valeurs()[indexMeteo][indexCE]; 
    // double donneePompage = pompageEau_.donnees()[indexPompage][indexCE];

    EtatCarreauEntier& etatPrecCarreauEntier = etatsPasDeTempsPrecedent[indexCE]; 
    CarreauEntier& carreauEntier = *bassinVersant_.carreauxEntiers()[indexCE];
    EtatCarreauEntier etatCarreauEntier;

    const std::vector<PuitsPtr>& listePuits = bassinVersant_.puitsParIdCE(carreauEntier.id());
  
    float pluieEtFonteDisponible = 0.0, precipitationsModele = 0.0;
    fonte_->calculerFonte(datePasDeTemps, meteo, carreauEntier, precipitationsModele, pluieEtFonteDisponible);

    // QUALITE
    etatCarreauEntier.Qualite.PPTTOT = precipitationsModele;

    /*calculerBilanReservoirs(jourPasDeTemps, carreauEntier, etatPrecCarreauEntier, 
                            etatCarreauEntier, meteo, pluieEtFonteDisponible);*/

    calculerBilanReservoirs(jourPasDeTemps, carreauEntier, etatPrecCarreauEntier,
        etatCarreauEntier, meteo, pluieEtFonteDisponible, listePuits, indexPasDeTemps);
  
    etatsPasDeTemps.push_back(etatCarreauEntier);
  } // FIN Boucle carreaux entiers

  // Donnes de fonte a assimiler
  fonte_->assimiler(datePasDeTemps);

  // Donnes d'evapo a assimiler
  evapo_->assimiler(datePasDeTemps);
  
  fonte_->preserverEtatsPasDeTemps();
  evapo_->preserverEtatsPasDeTemps();
  bilans_.push_back(etatsPasDeTemps);

  return 0;
}


//------------------------------------------------------------------
int Simulation::calculerBilanReservoirs(int noJour, CarreauEntier& carreauEntier, const EtatCarreauEntier& etatPrecCarreauEntier,
  EtatCarreauEntier& etatCarreauEntier, const Meteo& meteo, float PLUFON_eauDisponible, const std::vector<PuitsPtr>& listePuits,  int indexPasDeTemps)
{
  FILE_LOG(logDEBUG) << "Simulation::calculerBilanReservoirs(...)";

  // Pour faciliter la comprehension, les variables sont prefixees du nom des variables
  // telles que definies dans le manuel Cequeau.

  // Initialisation
  float HS_niveauEauSol = etatPrecCarreauEntier.niveauEauSol;
  float HN_niveauEauNappe = etatPrecCarreauEntier.niveauEauNappe;
  // double HN_niveauEauNappe = static_cast<double>(etatPrecCarreauEntier.niveauEauNappe);
  float HM_niveauEauLacsMarais = etatPrecCarreauEntier.niveauEauLacsMarais;
  float RUISS_ruissellementSurface = 0.0f, VIDINT_ruissellement1 = 0.0f;
  float VIDFON_ruissellement2 = 0.0f, ETRLAC_evapoReelleLac = 0.0f, ETRNAP_evapoNappe = 0.0f;
  float ETRSOL_evapoSol = 0.0f, ETOT_evapoPotSol = 0.0f, PRODU_eauDisponibleSolNappe = 0.0f; 

  // *** RUISSELLEMENT ***
  // Possibilite variation spatiale (TRI): COEFIMP ou SURFIMPERM
  float PCIMP_pctSurfaceImper = carreauEntier.param().fractionImpermeableCE;
  // Possibilite variation spatiale (HRIMP): HAURIMP
  float HRIMP_lameEauDebutRuisellement = carreauEntier.param().lameEauDebutRuisellement;
  // La lame RIMP est immediatement disponible pour l'�coulement en riviere. 
  float RIMP_ruissellementSufaceImper = 
  maxf(0.0, PCIMP_pctSurfaceImper * (PLUFON_eauDisponible - HRIMP_lameEauDebutRuisellement));
  // La lame restante EAUTER est ajoutee au contenu anterieur du reservoir SOL.
  float EAUTER_eauNonRuissellement = PLUFON_eauDisponible - RIMP_ruissellementSufaceImper;
  HS_niveauEauSol += EAUTER_eauNonRuissellement;

  // Calcul de l'infiltration de sol vers nappe
  // Possibilite variation spatiale (CIN): COEFINF (perimetre) et COEFINFILT (par carreau)
  float TONIF_coeffInfiltrationNappe = carreauEntier.param().coeffInfiltrationNappe;
  // Possibilite variation spatiale (HINF): HAUINF 
  float HINFS_seuilInfiltrationSolVersNappe = carreauEntier.param().seuilInfiltrationSolVersNappe;
  float XINFMA_infiltrationMax = parametres_.sol().infiltrationMax;
  float ARR27_coeffPonderation = carreauEntier.calculerCoeffPonderation();

  float XINF_infiltration = TONIF_coeffInfiltrationNappe * 
            maxf(0.0, HS_niveauEauSol - HINFS_seuilInfiltrationSolVersNappe);
  XINF_infiltration = minf(XINFMA_infiltrationMax, XINF_infiltration) * ARR27_coeffPonderation;

  // *** RESERVOIR NAPPE ***

  // Possibilite variation spatiale (CVNH): COEFVNH
  float CVNHS_coeffVidangeHauteNappe = carreauEntier.param().coeffVidangeHauteNappe;
  // Possibilite variation spatiale (HNAP): HAUNAP
  float HNAPS_seuilVidangeHauteNappe = carreauEntier.param().seuilVidangeHauteNappe;
  float SNAPH_vidangeHaute = CVNHS_coeffVidangeHauteNappe * 
            maxf(0.0, HN_niveauEauNappe - HNAPS_seuilVidangeHauteNappe);
  HN_niveauEauNappe -= SNAPH_vidangeHaute;

  // Possibilite variation spatiale (CVNB): COEFVNB
  float CVNBS_coeffVidangeBasseNappe = carreauEntier.param().coeffVidangeBasseNappe;
  float SNAPB_vidangeBasse = CVNBS_coeffVidangeBasseNappe * HN_niveauEauNappe;

  HN_niveauEauNappe = HN_niveauEauNappe - SNAPB_vidangeBasse + XINF_infiltration;

  // Soustraire l'eau pompee de l'haute nappe
  float rayon = 0.0f;
  // float recharge = 0.0f;
  if (parametres_.option().modulePompage) {

    if (!listePuits.empty()) {
      float conductiviteHydraulique = carreauEntier.param().conductiviteHydraulique;
      double coeffPompage = parametres_.pompage().coeffPompage;
      double gradientHydraulique = carreauEntier.meanSlope();

      int delai = parametres_.pompage().delai;
      int idxDelai = indexPasDeTemps - delai;

      for (PuitsPtr pPtr : listePuits) {

        if (pPtr->getActive() == 1) {
          double Qp = pPtr->getDebitPompageParIndex(idxDelai);

          double distanceRiviere = pPtr->getDistanceRiviere();

          double niveauInitial = pPtr->getNiveauInitial();
          double niveauEauSuivant = pPtr->getNiveauPuitsParIndex(idxDelai+1); // ht+1
          double niveauEau = pPtr->getNiveauPuitsParIndex(idxDelai);          // ht
          double niveauEauPrecedent = pPtr->getNiveauPuitsParIndex(idxDelai-1); // ht-1

          double AICE = 1000000 * bassinVersant_.superficieCarreauEntier();
          
          rayon = static_cast<float>(Qp / (2 * PI * conductiviteHydraulique * niveauInitial * gradientHydraulique));
    
          double Qextrait = Qp * (1 - exp(-1 * coeffPompage * rayon));

          double hauteur_mm = 1000.0 * (Qextrait / AICE);

          double HN_avant_Q = HN_niveauEauNappe;
          double HN_apres_Q = HN_avant_Q - hauteur_mm;
          HN_niveauEauNappe = static_cast<float>(HN_apres_Q);
          
          HN_niveauEauNappe = maxf(HN_niveauEauNappe, 0.0f);

        } // if (pPtr->getActive() == 1)

      } // for (PuitsPtr pPtr : listePuits)

    } // if (!listePuits.empty())
    
  } // if (parametres_.option().modulePompage)

  // HN_niveauEauNappe = HN_niveauEauNappe - SNAPB_vidangeBasse + XINF_infiltration;
  float SONAP_eauDisponibleNappe = SNAPB_vidangeBasse + SNAPH_vidangeHaute;

  // *** EVAPOTRANSPIRATION ***
  float evapoPotJour = 0.0f;
  evapo_->calculerEvapo(noJour, meteo, carreauEntier, HN_niveauEauNappe, ETRSOL_evapoSol, 
        ETRNAP_evapoNappe, ETRLAC_evapoReelleLac, evapoPotJour);

  HN_niveauEauNappe = maxf(0, HN_niveauEauNappe - ETRNAP_evapoNappe);

  // *** RESERVOIR SOL ***
  // Possibilite variation spatiale (HPOT): HAUPOT 
  float HPOTS_seuilPrelevementEauTauxPotentiel = carreauEntier.param().seuilPrelevementEauTauxPotentiel;
  // Possibilite variation spatiale (HSOL): HAUSOL 
  float HSOLS_hauteurReservoirSol = carreauEntier.param().hauteurReservoirSol;
  float CVSB_coeffVidangeBasseSol = parametres_.sol().coeffVidangeBasseSol;
  // Possibilite variation spatiale (CVSI): COEFVSI 
  float CVSIS_coeffVidangeIntermediaireSol = carreauEntier.param().coeffVidangeIntermediaireSol;
  // Possibilite variation spatiale (HINT): HAUINT
  float HINTS_seuilVidangeIntermediaireSol = carreauEntier.param().seuilVidangeIntermediaireSol;

  if (HS_niveauEauSol <= HPOTS_seuilPrelevementEauTauxPotentiel) {
  ETRSOL_evapoSol = ETRSOL_evapoSol * HS_niveauEauSol / 
    (HPOTS_seuilPrelevementEauTauxPotentiel + 0.245f);
  }
  HS_niveauEauSol -= XINF_infiltration;

  if (HS_niveauEauSol <= 0.0f) {
  HS_niveauEauSol = 0.0f;
  ETRSOL_evapoSol = 0.0f;
  PRODU_eauDisponibleSolNappe = RIMP_ruissellementSufaceImper + RUISS_ruissellementSurface +
                  VIDINT_ruissellement1 + VIDFON_ruissellement2 + SONAP_eauDisponibleNappe;
  }
  else {
  ETRSOL_evapoSol = minf(HS_niveauEauSol, ETRSOL_evapoSol);
  HS_niveauEauSol -= ETRSOL_evapoSol;

  if (HS_niveauEauSol <= HSOLS_hauteurReservoirSol ){
  VIDINT_ruissellement1 = maxf(0.0, (HS_niveauEauSol - HINTS_seuilVidangeIntermediaireSol) * 
                      CVSIS_coeffVidangeIntermediaireSol);
  VIDFON_ruissellement2 = (HS_niveauEauSol - VIDINT_ruissellement1) * CVSB_coeffVidangeBasseSol;
  }
  else {
  RUISS_ruissellementSurface = HS_niveauEauSol - HSOLS_hauteurReservoirSol;
  VIDINT_ruissellement1 = (HSOLS_hauteurReservoirSol - HINTS_seuilVidangeIntermediaireSol) * 
              CVSIS_coeffVidangeIntermediaireSol;
  VIDFON_ruissellement2 = (HSOLS_hauteurReservoirSol - VIDINT_ruissellement1) *
              CVSB_coeffVidangeBasseSol;
  }

  HS_niveauEauSol = HS_niveauEauSol - RUISS_ruissellementSurface - 
      VIDINT_ruissellement1 - VIDFON_ruissellement2;
  PRODU_eauDisponibleSolNappe = RIMP_ruissellementSufaceImper + RUISS_ruissellementSurface +
                  VIDINT_ruissellement1 + VIDFON_ruissellement2 + SONAP_eauDisponibleNappe;
  }

  // *** RESERVOIR LACS ET MARAIS ***
  HM_niveauEauLacsMarais += PLUFON_eauDisponible;
  ETRLAC_evapoReelleLac = minf(ETRLAC_evapoReelleLac, HM_niveauEauLacsMarais);
  HM_niveauEauLacsMarais -= ETRLAC_evapoReelleLac;

  float HMAR_seuilVidangeLacsMarais = parametres_.sol().seuilVidangeLacsMarais;
  float CVMAR_coeffVidangeLacsMarais = parametres_.sol().coeffVidangeLacsMarais;
  float SLAMA_eauDisponibleLacsMarais = 
  maxf(0.0, (HM_niveauEauLacsMarais - HMAR_seuilVidangeLacsMarais) * CVMAR_coeffVidangeLacsMarais);
  HM_niveauEauLacsMarais -= SLAMA_eauDisponibleLacsMarais;

  // *** RESERVOIR LAC EXUTOIRE ***
  float eauDisponibleLacExutoire =  maxf(0.0, (meteo.neige() + meteo.pluie() - (0.8f * evapoPotJour)));
  float REST_eauDisponibleCE = (PRODU_eauDisponibleSolNappe * carreauEntier.pctSol()) + 
              (SLAMA_eauDisponibleLacsMarais * (carreauEntier.pctLacRiviere() + carreauEntier.pctMarais())) +
              (eauDisponibleLacExutoire * carreauEntier.pctLacExutoire());

  float CEKM2_superficieKm2 = bassinVersant_.superficieCarreauEntier();

  // Preservation de l'etat
  etatCarreauEntier.idCarreauEntier = carreauEntier.id();
  etatCarreauEntier.iCarreauEntier = carreauEntier.i();
  etatCarreauEntier.jCarreauEntier = carreauEntier.j();
  etatCarreauEntier.niveauEauLacsMarais = HM_niveauEauLacsMarais;
  etatCarreauEntier.niveauEauNappe = HN_niveauEauNappe;
  etatCarreauEntier.niveauEauSol = HS_niveauEauSol;
  etatCarreauEntier.evapoPotJour = evapoPotJour;
  // Stockage en metres cube
  etatCarreauEntier.production = 1000.0f * CEKM2_superficieKm2 * REST_eauDisponibleCE;
  etatCarreauEntier.recharge = 0.0f;

  // QUALITE
  etatCarreauEntier.Qualite.RUISST = RIMP_ruissellementSufaceImper + RUISS_ruissellementSurface;
  etatCarreauEntier.Qualite.SOLT = VIDINT_ruissellement1 + VIDFON_ruissellement2;
  etatCarreauEntier.Qualite.SONAPT = SONAP_eauDisponibleNappe;
  etatCarreauEntier.Qualite.SLAMAT = SLAMA_eauDisponibleLacsMarais;
  etatCarreauEntier.Qualite.ETRSOL = ETRSOL_evapoSol;
  etatCarreauEntier.Qualite.ETRNAP = ETRNAP_evapoNappe;
  etatCarreauEntier.Qualite.ETRLAC = ETRLAC_evapoReelleLac;

  return 0;
}

// TODO: Releves neige
//------------------------------------------------------------------
int Simulation::ajusterNeigeZone(const DateChrono& datePasDeTemps, std::ofstream& fOut)
{
  FILE_LOG(logDEBUG) << "Simulation::ajusterNeige(date datePasDeTemps)";
  
  std::string sep = ";";
  if (fOut.is_open()) {
    fOut << std::endl << "CORRECTION DES STOCKS DE NEIGE LE " << datePasDeTemps << "(PAR ZONE)\n";
    fOut << "IMIN;IMAX;JMIN;JMAX;CORRECTION\n";
  }

  std::vector<FonteCequeau::EtatFonteCE>& etatsActuelsFonteCE = 
    ((FonteCequeau*)fonte_.get())->etatsFonte_.back();
  
  RelevesNeigeIter relevesUneDateIter; 
  // Tous les releves pour une date.
  std::pair<RelevesNeigeIter, RelevesNeigeIter> relevesUneDate = 
    parametres_.facultatifs().relevesNeige.equal_range(datePasDeTemps);

  float sommeStockForet, moyenneZone, facteurMul, facteurAdd;
  int nbCarreauxZone, i, j, nbCE = bassinVersant_.nbCarreauxEntiers();
  const std::vector<CarreauEntierPtr>& carreauxEntiersPtr = bassinVersant_.carreauxEntiers();
  // On parcourt les releves pour une date.
  for (relevesUneDateIter =  relevesUneDate.first; relevesUneDateIter != relevesUneDate.second; relevesUneDateIter++) {
    sommeStockForet = 0.0;
    nbCarreauxZone = 0;
    // Calcul de la moyenne de neige pour les carreaux couvert par la zone du releve.
    for (int idxCE = 0; idxCE < nbCE; idxCE++) {
      i = carreauxEntiersPtr[idxCE]->i();
      j = carreauxEntiersPtr[idxCE]->j();;
      if (i >=  relevesUneDateIter->second.iMin && i <= relevesUneDateIter->second.iMax 
          && j >=  relevesUneDateIter->second.jMin && j <= relevesUneDateIter->second.jMax) 
      {
        nbCarreauxZone++;  
        sommeStockForet += etatsActuelsFonteCE[idxCE].SNC_stockNeigeForet;
      }
    }

    moyenneZone = sommeStockForet / (float)nbCarreauxZone;
    if (moyenneZone < 0.001f) {
      facteurMul = 1.0;
      facteurAdd = relevesUneDateIter->second.valeur;
    }
    else {
      facteurMul = relevesUneDateIter->second.valeur / moyenneZone;
      facteurAdd = 0.0;
    }

    if (fOut.is_open()) {
      fOut << relevesUneDateIter->second.iMin << sep << relevesUneDateIter->second.iMax << sep;
      fOut << relevesUneDateIter->second.jMin << sep << relevesUneDateIter->second.jMax << sep;
      fOut << facteurMul << std::endl;
    }
    // Assignation du nouveau stock de neige aux carreaux de la zone.
    for (int idxCE = 0; idxCE < nbCE; idxCE++) {
      i = carreauxEntiersPtr[idxCE]->i();
      j = carreauxEntiersPtr[idxCE]->j();;
      if (i >=  relevesUneDateIter->second.iMin && i <= relevesUneDateIter->second.iMax 
          && j >=  relevesUneDateIter->second.jMin && j <= relevesUneDateIter->second.jMax) 
      {
        etatsActuelsFonteCE[idxCE].SNC_stockNeigeForet = 
          (etatsActuelsFonteCE[idxCE].SNC_stockNeigeForet * facteurMul + facteurAdd) ;

        etatsActuelsFonteCE[idxCE].SND_stockNeigeClairiere = 
          (etatsActuelsFonteCE[idxCE].SND_stockNeigeClairiere * facteurMul + facteurAdd) ;
      }
    }

  }
  return 0;
}
// TODO: Releves neige
bool sortByDistance(const ReleveNeige &lhs, const ReleveNeige &rhs)
{ 
   return lhs.distance < rhs.distance; 
}

//------------------------------------------------------------------
int Simulation::ajusterNeige3Releves(const DateChrono& datePasDeTemps, std::ofstream& fOut)
{
  
  FILE_LOG(logDEBUG) << "Simulation::ajusterNeige(date datePasDeTemps)";
  
  std::string sep = ";";
  if (fOut.is_open()) {
    fOut << std::endl << "CORRECTION DES STOCKS DE NEIGE LE " << datePasDeTemps << "(PONDERATION DE 3 RELEVES)\n";
    fOut << "NO_REL;ICE;I;J;STOCK_SOL;STOCK_MESURE;CORRECTION\n";
  }

  std::vector<FonteCequeau::EtatFonteCE>& etatsActuelsFonteCE = 
    ((FonteCequeau*)fonte_.get())->etatsFonte_.back();
  
  RelevesNeigeIter relevesUneDateIter; 
  // Tous les releves pour une date.
  std::pair<RelevesNeigeIter, RelevesNeigeIter> relevesUneDate = 
    parametres_.facultatifs().relevesNeige.equal_range(datePasDeTemps);

  float correction, varTemp;
  int i, j, noReleve = 0, nbCE = bassinVersant_.nbCarreauxEntiers();
  const std::vector<CarreauEntierPtr>& carreauxEntiersPtr = bassinVersant_.carreauxEntiers();
  RelevesNeige relevesNeige;

  // On parcourt les releves pour une date.
  for (relevesUneDateIter =  relevesUneDate.first; relevesUneDateIter != relevesUneDate.second; relevesUneDateIter++) {
    ReleveNeige releveNeige;
    // Calcul de la correction de chaque releve
    for (int idxCE = 0; idxCE < nbCE; idxCE++) {
      i = carreauxEntiersPtr[idxCE]->i();
      j = carreauxEntiersPtr[idxCE]->j();;
      if (i == relevesUneDateIter->second.iMin && j == relevesUneDateIter->second.jMin) {
        correction = 1.0f;
        // Coorection = releve / neige calculee
        if (etatsActuelsFonteCE[idxCE].SNC_stockNeigeForet >= 0.01f) {
          correction = relevesUneDateIter->second.valeur / etatsActuelsFonteCE[idxCE].SNC_stockNeigeForet;
        }
        releveNeige.no = ++noReleve;
        releveNeige.idCE = idxCE + 1;
        releveNeige.i = i;
        releveNeige.j = j;
        releveNeige.valeur = relevesUneDateIter->second.valeur;
        releveNeige.correction = correction;
        relevesNeige.push_back(releveNeige);

        varTemp = etatsActuelsFonteCE[idxCE].SNC_stockNeigeForet;
        if (fOut.is_open()) {
          fOut << releveNeige.no << sep << releveNeige.idCE << sep << i << sep << j << sep << varTemp << sep << releveNeige.valeur << sep << correction << std::endl;
        }
      }
    }
  }
  
  if (fOut.is_open()) {
    fOut << "ICE;I;J;REL1;REL2;REL3;DIST1;DIST2;DIST3;FACT_POND1;FACT_POND2;FACT_POND3;COR;CLAIR_AV;CLAIR_AP;FORET_AV;FORET_AP\n";
  }
  int xCE, yCE, xReleve, yReleve, distanceTmp;
  float distance, varTravail, stockNeigeForetAvant, stockNeigeClairierAvant;
  float facteursPonderation[3], corrections[3], facteurCorrection;
  RelevesNeige::iterator relevesNeigeIter;
  // Correction de la neige pour tous les carreaux
  for (int idxCE = 0; idxCE < nbCE; idxCE++) {
    xCE = carreauxEntiersPtr[idxCE]->i();
    yCE = carreauxEntiersPtr[idxCE]->j();

    // Calcul de la distance de chaque releve par rapport au CE
    for (relevesNeigeIter = relevesNeige.begin(); relevesNeigeIter != relevesNeige.end(); relevesNeigeIter++) {
      xReleve = relevesNeigeIter->i;
      yReleve = relevesNeigeIter->j;
      distanceTmp = (xReleve - xCE) * (xReleve - xCE) + (yReleve - yCE) * (yReleve - yCE);
      distance = std::sqrt((float)distanceTmp * bassinVersant_.superficieCarreauEntier());
      relevesNeigeIter->distance = distance;

      // Le releve est sur le CE
      if (distanceTmp == 0) {
        break;
      }
    }

    // Intialisation
    facteursPonderation[0] = 0.0f;
    facteursPonderation[1] = 0.0f;
    facteursPonderation[2] = 0.0f;
    corrections[0] = 0.0f;
    corrections[1] = 0.0f;
    corrections[2] = 0.0f;

    // Ordonnancement des releves par distance
    std::sort(relevesNeige.begin(), relevesNeige.end(), sortByDistance);

    if (distanceTmp == 0 || relevesNeige.size() == 1) {
      facteursPonderation[0] = 1.0f;
      corrections[0] = relevesNeige.at(0).correction;
    }
    else if (relevesNeige.size() == 2) {
      varTravail = 1.0f / (1.0f / relevesNeige.at(0).distance + 1.0f / relevesNeige.at(1).distance);
      facteursPonderation[0] = varTravail / relevesNeige.at(0).distance;
      facteursPonderation[1] = varTravail / relevesNeige.at(1).distance;
      corrections[0] = relevesNeige.at(0).correction;
      corrections[1] = relevesNeige.at(1).correction;
    }
    else {
      varTravail = 1.0f / (1.0f / relevesNeige.at(0).distance + 1.0f / relevesNeige.at(1).distance + 1.0f / relevesNeige.at(2).distance);
      facteursPonderation[0] = varTravail / relevesNeige.at(0).distance;
      facteursPonderation[1] = varTravail / relevesNeige.at(1).distance;
      facteursPonderation[2] = varTravail / relevesNeige.at(2).distance;
      corrections[0] = relevesNeige.at(0).correction;
      corrections[1] = relevesNeige.at(1).correction;
      corrections[2] = relevesNeige.at(2).correction;
    }

    facteurCorrection = facteursPonderation[0] * corrections[0] + 
                        facteursPonderation[1] * corrections[1] + 
                        facteursPonderation[2] * corrections[2];

    stockNeigeForetAvant = etatsActuelsFonteCE[idxCE].SNC_stockNeigeForet; 
    stockNeigeClairierAvant = etatsActuelsFonteCE[idxCE].SND_stockNeigeClairiere;
    etatsActuelsFonteCE[idxCE].SNC_stockNeigeForet = stockNeigeForetAvant * facteurCorrection;
    etatsActuelsFonteCE[idxCE].SND_stockNeigeClairiere = stockNeigeClairierAvant * facteurCorrection;

    if (fOut.is_open()) {
      fOut << idxCE+1 << sep << xCE << sep << yCE << sep << relevesNeige[0].no << sep << relevesNeige[1].no << sep << relevesNeige[2].no << sep 
        << relevesNeige[0].distance << sep << relevesNeige[1].distance << sep << relevesNeige[2].distance << sep 
        << facteursPonderation[0]  << sep << facteursPonderation[1] << sep << facteursPonderation[2] << sep << facteurCorrection << sep 
        << stockNeigeClairierAvant << sep << etatsActuelsFonteCE[idxCE].SND_stockNeigeClairiere << sep
        << stockNeigeForetAvant << sep << etatsActuelsFonteCE[idxCE].SNC_stockNeigeForet << std::endl;
    }
  }
  return 0;
}
