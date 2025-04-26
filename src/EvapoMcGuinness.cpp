//****************************************************************************
// Fichier: EvapoMcGuinness.cpp
//
// Creation date: 2014-04-09
// Created by: 
//                Rio Tinto Alcan                     
//                Energie electrique                  
//                1954 Davis, Saguenay arr. Jonquiere,
//                G7S 4R7, QC, Canada                 
// 
// Usage: Use this template to create you own snow melt module:
//        1- Copy and paste EvapoTemplate.h and EvapoTemplate.cpp
//        2- Rename the new files after your module name (please keep "Evapo" at the beginning). 
//           Example: EvapoSomething.h and EvapoSomething.cpp
//        3- Search for "// TODO New module" in the project. These are placeholders for you code.  
//           All methods MUST be implemented (see interface Evapo.h).
//
//****************************************************************************
#include "stdafx.h"
#include "EvapoMcGuinness.h"

#define PI 3.14159265

//------------------------------------------------------------------
EvapoMcGuinness::EvapoMcGuinness()
{
}

//------------------------------------------------------------------
EvapoMcGuinness::EvapoMcGuinness(int latitudeMoyenneBV, int nbCE, int pasParJour)
                           : Evapo(nbCE, "EvapoCMcGuiness"), pasParJour_(pasParJour)
{ 
  // TODO New module: Field names of your state variables
  /*** Example
  nomChamps_.push_back("stateVar1");
  nomChamps_.push_back("stateVar2");
  ***/

  // TODO New module: Field names of the specific weather field(s)  
  // needed by your module. By default the available values are:
  // tMin, tMax, pluie, neige. See meteo.h for details.
  // If you want to acces your field from the meteo object:
  //   meteo.meteoEvapo().at(i)
  // where i is the order of you fields. First i is 0.
  /*** Example
  nomsChampsMeteo().push_back("rayonemment");
  ***/

	nomsChampsMeteo().push_back("rayExtra");

}

//------------------------------------------------------------------
EvapoMcGuinness::~EvapoMcGuinness()
{
}

//------------------------------------------------------------------
int EvapoMcGuinness::calculerEvapo(
  // IN
  const int& noJour,
  const Meteo& meteo,
  const CarreauEntier& carreauEntier,
  const float niveauEauNappe,
  // OUT
  float& evapotranspirationSol,
  float& evapotranspirationNappe,
  float& evaporationLac,
  float& evapotranspirationPotentielle
)
{

  const int indexCE = carreauEntier.id() - 1;
  // Nouveau pas de temps
  if (indexCE == 0) {
    etatsEvapoCE_.clear();
    etatsEvapoCE_.reserve(nbCE_);
  }

  EtatEvapoCE& etatEvapoCE = etatsEvapo_.back().at(indexCE);

  // TODO New module: Put your calculation below
  /***
  Your module core computation here!
  ***/
  float TJE_tempMoy = meteo.calculerTempMoy();
  
  // Initialisation des variables météo - même ordre que lors de la déclaration des champs plus haut
  float rayExtra = meteo.meteoEvapo().at(0);

  float ARR27_coeffPonderation = carreauEntier.calculerCoeffPonderation();
  float HNAPS_seuilVidangeHauteNappe = carreauEntier.param().seuilVidangeHauteNappe;
 
  float HN_niveauEauNappe = niveauEauNappe;

  float EMCG_evapoPotJour = 0.0f;
  float ETRSOL_evapoSol = 0.0f, ETRNAP_evapoNappe = 0.0f, ETRLAC_evapoReelleLac = 0.0f;
  // New state preservation
  // TODO New module: Add your variables
  /*** Example
  etatEvapoCE_.stateVar1 = ???;
  etatEvapoCE_.stateVar2 = ???;
  ***/

    if (TJE_tempMoy > 0.0f) {
    EMCG_evapoPotJour = calculerMcGuinness(TJE_tempMoy, rayExtra);
    float EVNAP_fractionEvapoNappe = params_.fractionEvapoNappe;
    ETRLAC_evapoReelleLac = 0.8f * EMCG_evapoPotJour;
    float ETOT_evapoPotSol = EMCG_evapoPotJour * ARR27_coeffPonderation;
    float evapoCalcul = 
      minf(EVNAP_fractionEvapoNappe, 
                      EVNAP_fractionEvapoNappe * HN_niveauEauNappe / (HNAPS_seuilVidangeHauteNappe + 25.4f));

    ETRNAP_evapoNappe = minf(HN_niveauEauNappe, ETOT_evapoPotSol * evapoCalcul);
    HN_niveauEauNappe = HN_niveauEauNappe - ETRNAP_evapoNappe;
    ETRSOL_evapoSol = ETOT_evapoPotSol - ETRNAP_evapoNappe;
  }

  etatsEvapoCE_.push_back(etatEvapoCE_);
  
  // TODO New module: Your result
  evapotranspirationSol = ETRSOL_evapoSol;
  evapotranspirationNappe = ETRNAP_evapoNappe;
  evaporationLac = ETRLAC_evapoReelleLac;
  evapotranspirationPotentielle = EMCG_evapoPotJour;
  

  return 0;
}

//------------------------------------------------------------------
int EvapoMcGuinness::assimiler(const DateChrono& datePasDeTemps)
{
  if (assimilationsCE_.count(datePasDeTemps) > 0) {
    std::vector<EtatEvapoCE> etatsSimules;
    std::vector<EtatEvapoCE>::iterator iterCE;
    // Toutes les assimilations CE pour ce pas de temps
    std::vector<EtatEvapoAssimCE> assimilations = assimilationsCE_.find(datePasDeTemps)->second;

    int idCE;
    // Parcourt des assimilations
    std::vector<EtatEvapoAssimCE>::const_iterator assimilationsIter = assimilations.begin();
    for (; assimilationsIter != assimilations.end(); assimilationsIter++) {
      // Parcourt des etats
      idCE = 0;
      for (iterCE = etatsEvapoCE_.begin(); iterCE != etatsEvapoCE_.end(); iterCE++) {
        // Si id egaux, on fait la mise a jour
        idCE++;
        if (idCE == assimilationsIter->idCarreauEntier) {

          // Sauvegarde de l'etat avant l'assimilation
          etatsSimules.push_back(*iterCE);
          // TODO New module: Your fields here
          /*** Example
          AssimilationHelper::assimilerValeur(assimilationsIter->stateVar1, 
                          assimilationsIter->stateVar1Type, iterCE->stateVar1);

          AssimilationHelper::assimilerValeur(assimilationsIter->stateVar2, 
                          assimilationsIter->stateVar2Type, iterCE->stateVar2);

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
int EvapoMcGuinness::initialiserAssimilations(const mxArray* assimilations)
{
  int retCode = 0;

  if (assimilations == NULL) {
    return retCode;
  }
  
  size_t nbDonnees, nbDonneesCE;
  mxArray *etatsEvapo, *idCE;
  double pasDeTempsData, *idCEData; 
  DateChrono datePasDeTemps;
  EtatEvapoAssimCE etatEvapoAssimCE;
  std::vector<EtatEvapoAssimCE> etatsEvapoAssimCE;

  // TODO New module: Your variables here.
  /*** Example
  mxArray* stateVar1, *stateVar2;
  double *stateVar1Data, *stateVar2Data;
  ***/

  // Nombre de donnees d'assimilation (pas de temps distincts)
  nbDonnees = mxGetNumberOfElements(assimilations);
  for (size_t i = 0; i < nbDonnees; i++) {
    MexHelper::chargerValeurs(assimilations, "pasDeTemps", pasDeTempsData, i);
    //datePasDeTemps = MexHelper::datenumToDate(&pasDeTempsData);
    datePasDeTemps = DateChrono::fromMatlabDatenum(pasDeTempsData);
    etatsEvapo = MexHelper::mhMxGetField(assimilations, i, "etatsEvapo");
    
    /***** Carreaux Entiers *****/
    // Donnees d'assimilation relatives aux CE pour ce pas de temps d'assimilation
    if (mxGetNumberOfElements(etatsEvapo) > 0) {
      idCE = MexHelper::mhMxGetField(etatsEvapo, 0, "id");
      idCEData = MexHelper::mhMxGetPr(idCE, "id");

      // Obtention des pointeurs de donnees
      // On utilise mxGetPr plutot que MexHelper::mhMxGetPr pour la possibilite 
      // d'avoir un pointeur null
      // TODO New module: Your fields here.
      /*** Example 
      stateVar1 = MexHelper::mhMxGetField(etatsEvapo, 0, "stateVar1");
      etatEvapoAssimCE.stateVar1Type = AssimilationHelper::obtenirTypeAssim(stateVar1);
      stateVar1Data = mxGetPr(stateVar1);

      stateVar2 = MexHelper::mhMxGetField(etatsEvapo, 0, "stateVar2");
      etatEvapoAssimCE.stateVar2Type = AssimilationHelper::obtenirTypeAssim(stateVar2);
      stateVar2Data = mxGetPr(stateVar2);
      ***/

      // Nombre de CE vises.
      nbDonneesCE = mxGetNumberOfElements(idCE);
      etatsEvapoAssimCE.clear();
      for (size_t j = 0; j < nbDonneesCE; j++) {
        etatEvapoAssimCE.idCarreauEntier = (int)idCEData[j];

        // TODO New module: Your fields here.
        /*** Example 
        AssimilationHelper::obtenirValeursAssim(stateVar1Data, etatEvapoAssimCE.stateVar1Type, 
                            j, etatEvapoAssimCE.stateVar1);

        AssimilationHelper::obtenirValeursAssim(stateVar2Data, etatEvapoAssimCE.stateVar2Type, 
                            j, etatEvapoAssimCE.stateVar2);
        ***/

        etatsEvapoAssimCE.push_back(etatEvapoAssimCE);
      }

      assimilationsCE_.insert(std::make_pair(datePasDeTemps, etatsEvapoAssimCE));
    }
  }

  return retCode;
}

//------------------------------------------------------------------
void EvapoMcGuinness::initialiserEtats(const mxArray* etatsInitiaux)
{
  etatsEvapoCE_.reserve(nbCE_);

  // Pas d'etats precedents, on initialise avec les valeurs par defaut des parametres
  if (etatsInitiaux == NULL) {

    for (int i = 0; i < nbCE_; i++) {
      // TODO New module: Your fields here.
      /*** Example
      etatEvapoCE_.stateVar1 = 0.0f;
      etatEvapoCE_.stateVar2 = 0.0f;
      ***/
      etatsEvapoCE_.push_back(etatEvapoCE_);
    }

    etatsEvapo_.push_back(etatsEvapoCE_);
  }
  else {
    // Association entre le nom du champ des intrants et une reference sur la variable correspondante
    std::map<std::string, float*> mapChamps;
    // TODO New module: Your fields here.
    /*** Example
    mapChamps.insert(std::make_pair("stateVar1", &etatEvapoCE_.stateVar1));
    mapChamps.insert(std::make_pair("stateVar2", &etatEvapoCE_.stateVar2));
    ***/

    initialiserEtatsHelper(etatsInitiaux, mapChamps, etatEvapoCE_, etatsEvapoCE_);

    etatsEvapo_.push_back(etatsEvapoCE_);
  }
}
  
//------------------------------------------------------------------
void EvapoMcGuinness::lireParametres(const mxArray* paramsEvapo)
{
  // TODO New module: Your parameters here.
	lireParametresHelper(paramsEvapo, "evnap" , params_.fractionEvapoNappe);
}

//------------------------------------------------------------------
mxArray* EvapoMcGuinness::obtenirEtatsAvantAssimilations(const std::vector<bool> filtreCE)
{
  return obtenirEtatsAvantAssimHelper(filtreCE, nomChamps_, avantAssimilationsCE_);
}

//------------------------------------------------------------------
mxArray* EvapoMcGuinness::obtenirEtats(const std::vector<bool> filtreCE)
{
  return obtenirEtatsHelper(nomChamps_, etatsEvapo_, filtreCE);
}

//------------------------------------------------------------------
void EvapoMcGuinness::preserverEtatsPasDeTemps()
{
  etatsEvapo_.push_back(etatsEvapoCE_);
}

float EvapoMcGuinness::calculerMcGuinness(float tempMoy, float rayExtra) const
{
	float rho = 0.9981f;
	float latentHeatVap = 2.501f - 0.002361f * (float) tempMoy;

  float calcul = (float) rayExtra/(latentHeatVap*rho)*((float) tempMoy+5.0f)/68.0f;
  
  return calcul;
}
