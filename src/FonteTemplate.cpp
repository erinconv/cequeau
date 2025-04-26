//****************************************************************************
// Fichier: FonteTemplate.cpp
//
// Creation date: 2014-04-09
// Created by: 
//                Rio Tinto Alcan                     
//                Energie electrique                  
//                1954 Davis, Saguenay arr. Jonquiere,
//                G7S 4R7, QC, Canada                 
// 
// Usage: Use this template to create you own snow melt module:
//        1- Copy and paste FonteTemplate.h and FonteTemplate.cpp
//        2- Rename the new files after your module name (please keep "Fonte" at the beginning). 
//           Example: FonteSomething.h and FonteSomething.cpp
//        3- Search for "// TODO New module" in the project. These are placeholders for you code.  
//           All methods MUST be implemented (see interface fonte.h).
//
//****************************************************************************
#include "stdafx.h"
#include "FonteTemplate.h"

//------------------------------------------------------------------
FonteTemplate::FonteTemplate()
{
}

//------------------------------------------------------------------
FonteTemplate::FonteTemplate(int latitudeMoyenneBV, int nbCE)
                           : Fonte(nbCE, "FonteTemplate")
{ 
  // TODO New module: Field names of your state variables
  /*** Example
  nomChamps_.push_back("stockNeigeForet");
  nomChamps_.push_back("stockNeigeClairiere");
  nomChamps_.push_back("indexMurissementNeige");
  nomChamps_.push_back("indexTempNeige"); 
  ***/

  // TODO New module: Field names of the specific weather field(s)  
  // needed by your module. By default the available values are:
  // tMin, tMax, pluie, neige. See meteo.h for details.
  // If you want to acces your field from the meteo object:
  //   meteo.meteoFonte().at(i)
  // where i is the order of you fields. First i is 0.
  /*** Example
  nomsChampsMeteo().push_back("rayonemment");
  ***/
}

//------------------------------------------------------------------
FonteTemplate::~FonteTemplate()
{
}

//------------------------------------------------------------------
int FonteTemplate::calculerFonte(
    // IN
    const int& noJour,
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

  // TODO New module: Put your calculation below
  /***
  Your module core computation here!
  ***/

  // New state preservation
  // TODO New module: Add your variables
  /*** Example
  etatFonteCE_.SNC_stockNeigeForet = stockNeigeForet;
  etatFonteCE_.SND_stockNeigeClairiere = stockNeigeClairiere;
  etatFonteCE_.QNUI3_indexMurissementNeige =QNUI3_indexMurissementNeige;
  etatFonteCE_.QNUI4_indexTempNeige = QNUI4_indexTempNeige;
  ***/

  etatsFonteCE_.push_back(etatFonteCE_);
  
  // Available water 
  // TODO New module: Your result
  // eauDisponible = ??? 
  
  // Total precipitation
  // TODO New module: Your result
  // precipationsTotales = ???

  return 0;
}

//------------------------------------------------------------------
int FonteTemplate::assimiler(const DateChrono& datePasDeTemps)
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
int FonteTemplate::initialiserAssimilations(const mxArray* assimilations)
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
void FonteTemplate::initialiserEtats(const mxArray* etatsInitiaux)
{
  etatsFonteCE_.reserve(nbCE_);

  // Pas d'etats precedents, on initialise avec les valeurs par defaut des parametres
  if (etatsInitiaux == NULL) {

    for (int i = 0; i < nbCE_; i++) {
      // TODO New module: Your fields here.
      /*** Example
      etatFonteCE_.QNUI3_indexMurissementNeige = params_.indexMurissementNeigeIni;
      etatFonteCE_.QNUI4_indexTempNeige = params_.indexTempNeigeIni;
      etatFonteCE_.SNC_stockNeigeForet = 0.0f;
      etatFonteCE_.SND_stockNeigeClairiere = 0.0f;
      ***/
      etatsFonteCE_.push_back(etatFonteCE_);
    }

    etatsFonte_.push_back(etatsFonteCE_);
  }
  else {
    // Association entre le nom du champ des intrants et une reference sur la variable correspondante
    std::map<std::string, float*> mapChamps;
    // TODO New module: Your fields here.
    /*** Example
    mapChamps.insert(std::make_pair("stockNeigeForet"       , &etatFonteCE_.SNC_stockNeigeForet));
    mapChamps.insert(std::make_pair("stockNeigeClairiere"   , &etatFonteCE_.SND_stockNeigeClairiere));
    mapChamps.insert(std::make_pair("indexMurissementNeige" , &etatFonteCE_.QNUI3_indexMurissementNeige));
    mapChamps.insert(std::make_pair("indexTempNeige"        , &etatFonteCE_.QNUI4_indexTempNeige));
    ***/

    initialiserEtatsHelper(etatsInitiaux, mapChamps, etatFonteCE_, etatsFonteCE_);

    etatsFonte_.push_back(etatsFonteCE_);
  }
}
  
//------------------------------------------------------------------
void FonteTemplate::lireParametres(const mxArray* paramsFonte)
{
  // TODO New module: Your parameters here.
  /*** Example
  lireParametresHelper(paramsFonte, "jonei"  , params_.jourSoleilMaxFonteNeige);
  lireParametresHelper(paramsFonte, "tsd_s"  , params_.seuilTempFonteClairiere);
  ***/
}

//------------------------------------------------------------------
mxArray* FonteTemplate::obtenirEtatsAvantAssimilations(const std::vector<bool> filtreCE)
{
  return obtenirEtatsAvantAssimHelper(filtreCE, nomChamps_, avantAssimilationsCE_);
}

//------------------------------------------------------------------
mxArray* FonteTemplate::obtenirEtats(const std::vector<bool> filtreCE)
{
  return obtenirEtatsHelper(nomChamps_, etatsFonte_, filtreCE);
}

//------------------------------------------------------------------
void FonteTemplate::preserverEtatsPasDeTemps()
{
  etatsFonte_.push_back(etatsFonteCE_);
}
