//****************************************************************************
// Fichier: FonteCemaNeige.cpp
//
// Creation date: 2014-04-15
// Created by: 
//                Rio Tinto Alcan                     
//                Energie electrique                  
//                1954 Davis, Saguenay arr. Jonquiere,
//                G7S 4R7, QC, Canada                 
// 
//****************************************************************************
#include "stdafx.h"
#include "FonteCemaNeige.h"

//------------------------------------------------------------------
FonteCemaNeige::FonteCemaNeige()
{
}

//------------------------------------------------------------------
FonteCemaNeige::FonteCemaNeige(int latitudeMoyenneBV, int nbCE)
                           : Fonte(nbCE, "FonteCemaNeige")
{ 
  nomChamps_.push_back("eTg");
  nomChamps_.push_back("G");
  nomChamps_.push_back("fonte_reel");
}

//------------------------------------------------------------------
FonteCemaNeige::~FonteCemaNeige()
{
}

//------------------------------------------------------------------
int FonteCemaNeige::calculerFonte(
    // IN
    const DateChrono& datePasDeTemps,
    const Meteo& meteo,
    const CarreauEntier& carreauEntier,
    // OUT
    float& precipationsTotales,
    float& eauDisponible
    )
{

  const int indexCE = carreauEntier.id() - 1;
  // Nouveau pas de temps
  if (indexCE == 0) {
    etatsFonteCE_.clear();
    etatsFonteCE_.reserve(nbCE_);
  }

  EtatFonteCE& etatFonteCE = etatsFonte_.back().at(indexCE);

  /***** CemaNeige calculations *****/
	float Fpot;

  // Initialize states
  float snow, rain;
  std::tie(snow, rain) = split_precip_cequeau(meteo);
  float eTg = etatFonteCE.eTg;
  float G = etatFonteCE.G;
  float Tz = meteo.calculerTempMoy() + params_.theta * (carreauEntier.altitude() - params_.Zmed) / 100.0f;
  float SNC_stockNeigeForet = etatFonteCE.SNC_stockNeigeForet;
  float SND_stockNeigeClairiere = etatFonteCE.SND_stockNeigeClairiere;

  // Snow accumulation
  G = G + snow;
  // eTg = std::max<float>(0.0f,params_.CTg * eTg + (1.0f - params_.CTg) * Tz);  
  eTg = params_.CTg * eTg + (1.0f - params_.CTg) * Tz;
  eTg = std::min<float>(0.0f,eTg);
  // Potential snowmelt computation
  if (eTg == 0.0f && Tz > params_.Tf)
  {
    Fpot = params_.Kf*(Tz - params_.Tf);
    if (Fpot > G)
    {
      Fpot = G;
    }
  }
	else
  {
    Fpot = 0.0f;
  }
  // Calcul pourcentage de la zone enneigée
  float Gseuil = params_.Gseuil;
  float pct_snow = std::min<float>(G/Gseuil,1.0f);
  // Calcul de la fonte effective
  float Vmin;
  if (G == 0.0f)
  {
    Vmin = 0.1f*params_.Kf;
  }
  else
  {
    Vmin = params_.Vmin;
  }
  float fonte_reel = ((1.0f - Vmin)*pct_snow + Vmin)*Fpot;
  // Actualisation réservoir neige
  G = G - fonte_reel;
  
  /***** End CemaNeige calculations *****/

  // New state preservation
  etatFonteCE_.G = G;
  etatFonteCE_.eTg = eTg;
  etatFonteCE_.fonte_reel = fonte_reel;


  etatsFonteCE_.push_back(etatFonteCE_);
  
  // Available water 
  eauDisponible = fonte_reel + rain;
  
  // Total precipitation
  precipationsTotales = meteo.pluie() + meteo.neige();

  return 0;
}

std::tuple<float, float> FonteCemaNeige::split_precip_cequeau(Meteo meteo)
{
    float PJE_pluie, PJN_neige;
    float TJE_tempMoy = meteo.calculerTempMoy();
    float seuilTranformationPluieNeige = params_.seuilTranformationPluieNeige;
    float neigeMeteo = meteo.neige(), pluieMeteo = meteo.pluie();

    // Si la temperature moyenne est inferieure au seuil de transformation - 2 deg,
    // la pluie est transformee en neige.
    float facteurTransformation;
    if (TJE_tempMoy <= params_.seuilTranformationPluieNeige - 2.0f)
    {
        PJE_pluie = 0.0f;
        PJN_neige = neigeMeteo + pluieMeteo;
    }
    // Si la temperature moyenne est comprise entre le seuil de transformation +/- 2 deg,
    // la pluie est transformee partiellement en neige.
    else if (TJE_tempMoy <= params_.seuilTranformationPluieNeige + 2.0f)
    {
        facteurTransformation = std::abs(TJE_tempMoy - params_.seuilTranformationPluieNeige - 2.0f) / 4.0f;
        PJE_pluie = pluieMeteo * (1.0f - facteurTransformation);
        PJN_neige = pluieMeteo * facteurTransformation + neigeMeteo;
    }
    else
    {
        PJE_pluie = pluieMeteo;
        PJN_neige = neigeMeteo;
    }

    // Output values
    // snow = PJN_neige;
    // rain = PJE_pluie;
    return std::make_tuple(PJN_neige, PJE_pluie);
}


//------------------------------------------------------------------
int FonteCemaNeige::assimiler(const DateChrono& datePasDeTemps)
{
  if (assimilationsCE_.count(datePasDeTemps) > 0) {
    std::vector<EtatFonteCE> etatsSimules;
    std::vector<EtatFonteCE>::iterator iterCE;
    // Toutes les assimilations CE pour ce pas de temps
    std::vector<EtatFonteAssimCE> assimilations = assimilationsCE_.find(datePasDeTemps)->second;

    int idCE;
    // Parcourt des assimilations
    std::vector<EtatFonteAssimCE>::const_iterator assimilationsIter = assimilations.begin();
    for (; assimilationsIter != assimilations.end(); assimilationsIter++) {
      // Parcourt des etats
      idCE = 0;
      for (iterCE = etatsFonteCE_.begin(); iterCE != etatsFonteCE_.end(); iterCE++) {
        // Si id egaux, on fait la mise a jour
        idCE++;
        if (idCE == assimilationsIter->idCarreauEntier) {

          // Sauvegarde de l'etat avant l'assimilation
          etatsSimules.push_back(*iterCE);
          // TODO New module: Your fields here
          /*** Example
          AssimilationHelper::assimilerValeur(assimilationsIter->SNC_stockNeigeForet, 
                          assimilationsIter->SNC_stockNeigeForetType, iterCE->SNC_stockNeigeForet);

          AssimilationHelper::assimilerValeur(assimilationsIter->SND_stockNeigeClairiere, 
                          assimilationsIter->SND_stockNeigeClairiereType, iterCE->SND_stockNeigeClairiere);

          AssimilationHelper::assimilerValeur(assimilationsIter->QNUI3_indexMurissementNeige, 
                          assimilationsIter->QNUI3_indexMurissementNeigeType, iterCE->QNUI3_indexMurissementNeige);

          AssimilationHelper::assimilerValeur(assimilationsIter->QNUI4_indexTempNeige, 
                          assimilationsIter->QNUI4_indexTempNeigeType, iterCE->QNUI4_indexTempNeige);
          ***/

          break;
        }
      } // for
    } // for

    // Conservation des etats simules pour comparaison avec etats assimiles
    if (etatsSimules.size() > 0) {
      avantAssimilationsCE_.insert(std::make_pair(datePasDeTemps, etatsSimules));
    }

  } // if (assimilationsCE_.count(datePasDeTemps) > 0)

  return 0;
}

//------------------------------------------------------------------
int FonteCemaNeige::initialiserAssimilations(const mxArray* assimilations)
{
  int retCode = 0;

  if (assimilations == NULL) {
    return retCode;
  }

  size_t nbDonnees, nbDonneesCE;
  mxArray *etatsFonte, *idCE;
  double pasDeTempsData, *idCEData;
  DateChrono datePasDeTemps;
  EtatFonteAssimCE etatFonteAssimCE;
  std::vector<EtatFonteAssimCE> etatsFonteAssimCE;

  // TODO New module: Your variables here.
  /*** Example
  mxArray *stockNeigeForet, *stockNeigeClairiere, *indexMurissementNeige, *indexTempNeige;
  double *stockNeigeForetData, *stockNeigeClairiereData, *indexMurissementNeigeData, *indexTempNeigeData;
  ***/

  // Nombre de donnees d'assimilation (pas de temps distincts)
  nbDonnees = mxGetNumberOfElements(assimilations);
  for (size_t i = 0; i < nbDonnees; i++) {
    MexHelper::chargerValeurs(assimilations, "pasDeTemps", pasDeTempsData, i);
    //datePasDeTemps = MexHelper::datenumToDate(&pasDeTempsData);
    datePasDeTemps = DateChrono::fromMatlabDatenum(pasDeTempsData);
    etatsFonte = MexHelper::mhMxGetField(assimilations, i, "etatsFonte");
    
    /***** Carreaux Entiers *****/
    // Donnees d'assimilation relatives aux CE pour ce pas de temps d'assimilation
    if (mxGetNumberOfElements(etatsFonte) > 0) {
      idCE = MexHelper::mhMxGetField(etatsFonte, 0, "id");
      idCEData = MexHelper::mhMxGetPr(idCE, "id");

      // Obtention des pointeurs de donnees
      // On utilise mxGetPr plutot que MexHelper::mhMxGetPr pour la possibilite 
      // d'avoir un pointeur null
      // TODO New module: Your fields here.
      /*** Example
      stockNeigeForet = MexHelper::mhMxGetField(etatsFonte, 0, "stockNeigeForet");
      etatFonteAssimCE.SNC_stockNeigeForetType = AssimilationHelper::obtenirTypeAssim(stockNeigeForet);
      stockNeigeForetData = mxGetPr(stockNeigeForet);

      stockNeigeClairiere = MexHelper::mhMxGetField(etatsFonte, 0, "stockNeigeClairiere");
      etatFonteAssimCE.SND_stockNeigeClairiereType = AssimilationHelper::obtenirTypeAssim(stockNeigeClairiere);
      stockNeigeClairiereData = mxGetPr(stockNeigeClairiere);

      indexMurissementNeige = MexHelper::mhMxGetField(etatsFonte, 0, "indexMurissementNeige");
      etatFonteAssimCE.QNUI3_indexMurissementNeigeType = AssimilationHelper::obtenirTypeAssim(indexMurissementNeige);
      indexMurissementNeigeData = mxGetPr(indexMurissementNeige);

      indexTempNeige = MexHelper::mhMxGetField(etatsFonte, 0, "indexTempNeige");
      etatFonteAssimCE.QNUI4_indexTempNeigeType = AssimilationHelper::obtenirTypeAssim(indexTempNeige);
      indexTempNeigeData = mxGetPr(indexTempNeige);
      ***/

      // Nombre de CE vises.
      nbDonneesCE = mxGetNumberOfElements(idCE);
      etatsFonteAssimCE.clear();
      for (size_t j = 0; j < nbDonneesCE; j++) {
        etatFonteAssimCE.idCarreauEntier = (int)idCEData[j];

        // TODO New module: Your fields here.
        /*** Example
        AssimilationHelper::obtenirValeursAssim(stockNeigeForetData, etatFonteAssimCE.SNC_stockNeigeForetType, 
                            j, etatFonteAssimCE.SNC_stockNeigeForet);

        AssimilationHelper::obtenirValeursAssim(stockNeigeClairiereData, etatFonteAssimCE.SND_stockNeigeClairiereType, 
                            j, etatFonteAssimCE.SND_stockNeigeClairiere);

        AssimilationHelper::obtenirValeursAssim(indexMurissementNeigeData, etatFonteAssimCE.QNUI3_indexMurissementNeigeType, 
                            j, etatFonteAssimCE.QNUI3_indexMurissementNeige);

        AssimilationHelper::obtenirValeursAssim(indexTempNeigeData, etatFonteAssimCE.QNUI4_indexTempNeigeType, 
                            j, etatFonteAssimCE.QNUI4_indexTempNeige);
        ***/

        etatsFonteAssimCE.push_back(etatFonteAssimCE);
      }

      assimilationsCE_.insert(std::make_pair(datePasDeTemps, etatsFonteAssimCE));
    }
  }

  return retCode;
}

//------------------------------------------------------------------
void FonteCemaNeige::initialiserEtats(const mxArray* etatsInitiaux)
{
  etatsFonteCE_.reserve(nbCE_);

  // Pas d'etats precedents, on initialise avec les valeurs par defaut des parametres
  if (etatsInitiaux == NULL) {
    
  for (int i = 0; i < nbCE_; i++) {
      etatFonteCE_.eTg = params_.eTg;
      etatFonteCE_.G = params_.G;
      etatFonteCE_.fonte_reel = 0.0f;
      etatFonteCE_.SNC_stockNeigeForet = 0.0f;
      etatFonteCE_.SND_stockNeigeClairiere = 0.0f;

      etatsFonteCE_.push_back(etatFonteCE_);
    }
    etatsFonte_.push_back(etatsFonteCE_);
  }
  else {
    // Association entre le nom du champ des intrants et une reference sur la variable correspondante
    std::map<std::string, float*> mapChamps;
    mapChamps.insert(std::make_pair("stockNeigeForet"       , &etatFonteCE_.SNC_stockNeigeForet));
    mapChamps.insert(std::make_pair("stockNeigeClairiere"   , &etatFonteCE_.SND_stockNeigeClairiere));

    initialiserEtatsHelper(etatsInitiaux, mapChamps, etatFonteCE_, etatsFonteCE_);

    etatsFonte_.push_back(etatsFonteCE_);
  }
}
  
//------------------------------------------------------------------
void FonteCemaNeige::lireParametres(const mxArray* paramsFonte)
{
  lireParametresHelper(paramsFonte, "strne"  , params_.seuilTranformationPluieNeige);
  lireParametresHelper(paramsFonte, "Kf"  , params_.Kf);
  lireParametresHelper(paramsFonte, "Tf"  , params_.Tf);
  lireParametresHelper(paramsFonte, "CTg"  , params_.CTg);
  lireParametresHelper(paramsFonte, "theta"  , params_.theta);
  lireParametresHelper(paramsFonte, "Gseuil"  , params_.Gseuil);
  lireParametresHelper(paramsFonte, "Vmin"  , params_.Vmin);
  lireParametresHelper(paramsFonte, "Zmed"  , params_.Zmed);
  // Initial values for the state variables
  lireParametresHelper(paramsFonte, "eTg"  , params_.eTg);
  lireParametresHelper(paramsFonte, "G"  , params_.G);
}

//------------------------------------------------------------------
mxArray* FonteCemaNeige::obtenirEtatsAvantAssimilations(const std::vector<bool> filtreCE)
{
  return obtenirEtatsAvantAssimHelper(filtreCE, nomChamps_, avantAssimilationsCE_);
}

//------------------------------------------------------------------
mxArray* FonteCemaNeige::obtenirEtats(const std::vector<bool> filtreCE)
{
  return obtenirEtatsHelper(nomChamps_, etatsFonte_, filtreCE);
}

//------------------------------------------------------------------
void FonteCemaNeige::preserverEtatsPasDeTemps()
{
  etatsFonte_.push_back(etatsFonteCE_);
}