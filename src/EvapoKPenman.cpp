//****************************************************************************
// Fichier: EvapoKPenman.cpp
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
#include "EvapoKPenman.h"

//------------------------------------------------------------------
EvapoKPenman::EvapoKPenman()
{
}

//------------------------------------------------------------------
EvapoKPenman::EvapoKPenman(int latitudeMoyenneBV, int nbCE, int pasParJour)
                           : Evapo(nbCE, "EvapoCequeau"), pasParJour_(pasParJour)
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

nomsChampsMeteo().push_back("wind");
nomsChampsMeteo().push_back("airPressure");
nomsChampsMeteo().push_back("rayonnement");
nomsChampsMeteo().push_back("humidity");
}

//------------------------------------------------------------------
EvapoKPenman::~EvapoKPenman()
{
}

//------------------------------------------------------------------
int EvapoKPenman::calculerEvapo(
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
  float wind = meteo.meteoEvapo().at(0);
  float airPressure = meteo.meteoEvapo().at(1);;
  float rayonnement = meteo.meteoEvapo().at(2);;
  float humidity = meteo.meteoEvapo().at(3);;

  float ARR27_coeffPonderation = carreauEntier.calculerCoeffPonderation();
  float HNAPS_seuilVidangeHauteNappe = carreauEntier.param().seuilVidangeHauteNappe;
 
  float HN_niveauEauNappe = niveauEauNappe;

  float EKPEN_evapoPotJour = 0.0f;
  float ETRSOL_evapoSol = 0.0f, ETRNAP_evapoNappe = 0.0f, ETRLAC_evapoReelleLac = 0.0f;
  // New state preservation
  // TODO New module: Add your variables
  /*** Example
  etatEvapoCE_.stateVar1 = ???;
  etatEvapoCE_.stateVar2 = ???;
  ***/

    if (TJE_tempMoy > 0.0f) {
    EKPEN_evapoPotJour = calculerKPenman(TJE_tempMoy, airPressure, rayonnement, wind, humidity, (float)noJour);
    float EVNAP_fractionEvapoNappe = params_.fractionEvapoNappe;
    ETRLAC_evapoReelleLac = 0.8f * EKPEN_evapoPotJour;
    float ETOT_evapoPotSol = EKPEN_evapoPotJour * ARR27_coeffPonderation;
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
  evapotranspirationPotentielle = EKPEN_evapoPotJour;
  

  return 0;
}

//------------------------------------------------------------------
int EvapoKPenman::assimiler(const DateChrono& datePasDeTemps)
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
int EvapoKPenman::initialiserAssimilations(const mxArray* assimilations)
{
  int retCode = 0;

  if (assimilations == NULL) {
    return retCode;
  }

  // Pour isnan
  
  
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
    
    /*datePasDeTemps = MexHelper::datenumToDate(&pasDeTempsData);*/
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
void EvapoKPenman::initialiserEtats(const mxArray* etatsInitiaux)
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
void EvapoKPenman::lireParametres(const mxArray* paramsEvapo)
{
  // TODO New module: Your parameters here.
	lireParametresHelper(paramsEvapo, "evnap" , params_.fractionEvapoNappe);
}

//------------------------------------------------------------------
mxArray* EvapoKPenman::obtenirEtatsAvantAssimilations(const std::vector<bool> filtreCE)
{
  return obtenirEtatsAvantAssimHelper(filtreCE, nomChamps_, avantAssimilationsCE_);
}

//------------------------------------------------------------------
mxArray* EvapoKPenman::obtenirEtats(const std::vector<bool> filtreCE)
{
  return obtenirEtatsHelper(nomChamps_, etatsEvapo_, filtreCE);
}

//------------------------------------------------------------------
void EvapoKPenman::preserverEtatsPasDeTemps()
{
  etatsEvapo_.push_back(etatsEvapoCE_);
}

float EvapoKPenman::calculerSlopeSatVapPressure(float tempMoy) const
{
  /***
  Slope of saturated vapour pressure (Pa/°C)
  4098*[0.6108*exp((17.27*Tmean_air)/(Tmean_air+237.3))])/((Tmean_air+237.3)^2
  ***/
  //float calcul = 4098 * (0.6108 * static_cast<float>(std::exp((17.27 * tempMoy))/(tempMoy+237.3))) / std::pow((tempMoy+237.3),2);
  //float calcul = 4098 * (0.6108 * static_cast<float>(std::exp((17.27 * static_cast<float>(tempMoy)) / (static_cast<float>(tempMoy) + 237.3)))) / std::pow((static_cast<float>(tempMoy) + 237.3), 2);
    float calcul = 4098.0f * (0.6108f * static_cast<float>(std::exp((17.27f * tempMoy) / (tempMoy + 237.3f)))) / std::pow((tempMoy + 237.3f), 2.0f);


  return calcul; 
}

float EvapoKPenman::calculerPsychometricConstant(float airPressure) const
{

  float calcul = 0.665f * 0.001f * airPressure; // Proulx-McInnis et al. (2013)

  return calcul; 
}

float EvapoKPenman::calculerKPenman(float tempMoy, float airPressure, float rayonnement, float wind, float humidity, float noJour) const
{
	float rho = 998.1f; // Water density 1000 kg/L
	float latentHeatVap = 2.501f-0.002361f*(float) tempMoy;
	float W1 = 0.4f + 0.14f * static_cast<float>(std::exp(std::pow(-((noJour-173)/58),2)));
	float W2 = 0.605f + 0.345f * static_cast<float>(std::exp(std::pow(-((noJour-243)/80),2)));
	float W = W1 + W2 * (float)wind;

	float satPres = (6.108f * std::pow(10.0f,((7.5f * tempMoy)/(237.3f + tempMoy)))) / 10.0f; // divided by 10 to convert from hPa to kPa

	// Actual vapour pressure
	float vapPres = (float) humidity/100*satPres;
  float calcul =   ((calculerSlopeSatVapPressure((float) tempMoy)*(float) rayonnement)+ 

					(calculerPsychometricConstant((float) airPressure)*(vapPres-satPres)* W))/

					(latentHeatVap*(calculerSlopeSatVapPressure((float) tempMoy)+calculerPsychometricConstant((float) airPressure)));


		//********************FONCTIONNE SI ON RETIRE RHO AU DÉNOMINATEUR - régler avant d'utiliser********************//

      if (calcul < 0.0f) {
		  calcul = 0.0f;
  }

  return calcul;
}
