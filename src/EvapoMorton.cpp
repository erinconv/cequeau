//****************************************************************************
// Fichier: EvapoMorton.cpp
//
// Creation date: 2014-04-09
// Created by: 
//                Rio Tinto Alcan                     
//                Energie electrique                  
//                1954 Davis, Saguenay arr. Jonquiere,
//                G7S 4R7, QC, Canada                 
// 
// Usage: Use this template to create you own snow melt module:
//        1- Copy and paste EvapoMorton.h and EvapoMorton.cpp
//        2- Rename the new files after your module name (please keep "Evapo" at the beginning). 
//           Example: EvapoSomething.h and EvapoSomething.cpp
//        3- Search for "// TODO New module" in the project. These are placeholders for you code.  
//           All methods MUST be implemented (see interface Evapo.h).
//
//****************************************************************************
#include "stdafx.h"
#include "EvapoMorton.h"

//------------------------------------------------------------------
EvapoMorton::EvapoMorton()
{
}

//------------------------------------------------------------------
EvapoMorton::EvapoMorton(int latitudeMoyenneBV, int nbCE, int pasParJour)
                           : Evapo(nbCE, "EvapoMorton"), pasParJour_(pasParJour)
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
  nomsChampsMeteo().push_back("rayonnement");
  ***/

nomsChampsMeteo().push_back("airPressure");
nomsChampsMeteo().push_back("humidity");
nomsChampsMeteo().push_back("longwave");
nomsChampsMeteo().push_back("inShortwave");
nomsChampsMeteo().push_back("rayonnement");
nomsChampsMeteo().push_back("vitesseVent");
}

//------------------------------------------------------------------
EvapoMorton::~EvapoMorton()
{
}

//------------------------------------------------------------------
int EvapoMorton::calculerEvapo(
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
  // Pour faciliter la comprehension, les variables sont prefixees du nom des variables
  // telles que definies dans le manuel Cequeau.

  float TJE_tempMoy = meteo.calculerTempMoy();
  
  // Initialisation des variables météo - même ordre que lors de la déclaration des champs plus haut
  float airPressure = meteo.meteoEvapo().at(0);
  float humidity = meteo.meteoEvapo().at(1);;
  float longwave = meteo.meteoEvapo().at(2);;  
  float inShortwave = meteo.meteoEvapo().at(3);;
  float rayonnement = meteo.meteoEvapo().at(4);;
  float vitesseVent = meteo.meteoEvapo().at(5);;

  float ARR27_coeffPonderation = carreauEntier.calculerCoeffPonderation();
  float HNAPS_seuilVidangeHauteNappe = carreauEntier.param().seuilVidangeHauteNappe;
 
  float HN_niveauEauNappe = niveauEauNappe;

  float MORTON_evapoPotJour = 0.0f;
  float ETRSOL_evapoSol = 0.0f, ETRNAP_evapoNappe = 0.0f, ETRLAC_evapoReelleLac = 0.0f;

  if (TJE_tempMoy > 0.0f) {
    MORTON_evapoPotJour = calculerMorton(TJE_tempMoy, airPressure, rayonnement, humidity, longwave, inShortwave);
    float EVNAP_fractionEvapoNappe = params_.fractionEvapoNappe;

    ETRLAC_evapoReelleLac = 0.8f * MORTON_evapoPotJour;

    float ETOT_evapoPotSol = MORTON_evapoPotJour * ARR27_coeffPonderation;
    float evapoCalcul = 
      minf(EVNAP_fractionEvapoNappe, 
                      EVNAP_fractionEvapoNappe * HN_niveauEauNappe / (HNAPS_seuilVidangeHauteNappe + 25.4f));

    ETRNAP_evapoNappe = minf(HN_niveauEauNappe, ETOT_evapoPotSol * evapoCalcul);
    HN_niveauEauNappe = HN_niveauEauNappe - ETRNAP_evapoNappe;
    ETRSOL_evapoSol = ETOT_evapoPotSol - ETRNAP_evapoNappe;
  }
  
  // New state preservation
  // TODO New module: Add your variables
  /*** Example
  etatEvapoCE_.stateVar1 = ???;
  etatEvapoCE_.stateVar2 = ???;
  ***/

  etatsEvapoCE_.push_back(etatEvapoCE_);
  
  // TODO New module: Your result
  
  // resultats
  evapotranspirationSol = ETRSOL_evapoSol;
  evapotranspirationNappe = ETRNAP_evapoNappe;
  evaporationLac = ETRLAC_evapoReelleLac;
  evapotranspirationPotentielle = MORTON_evapoPotJour;
 

  return 0;
}

//------------------------------------------------------------------
int EvapoMorton::assimiler(const DateChrono& datePasDeTemps)
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
int EvapoMorton::initialiserAssimilations(const mxArray* assimilations)
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
void EvapoMorton::initialiserEtats(const mxArray* etatsInitiaux)
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
void EvapoMorton::lireParametres(const mxArray* paramsEvapo)
{
  // TODO New module: Your parameters here.
   
	lireParametresHelper(paramsEvapo, "evnap" , params_.fractionEvapoNappe);
	lireParametresHelper(paramsEvapo, "alpha", params_.paramALPHA); 
  /*** Example
  lireParametresHelper(paramsEvapo, "anotherParam", params_.param2);
  ***/
}

//------------------------------------------------------------------
mxArray* EvapoMorton::obtenirEtatsAvantAssimilations(const std::vector<bool> filtreCE)
{
  return obtenirEtatsAvantAssimHelper(filtreCE, nomChamps_, avantAssimilationsCE_);
}

//------------------------------------------------------------------
mxArray* EvapoMorton::obtenirEtats(const std::vector<bool> filtreCE)
{
  return obtenirEtatsHelper(nomChamps_, etatsEvapo_, filtreCE);
}

//------------------------------------------------------------------
void EvapoMorton::preserverEtatsPasDeTemps()
{
  etatsEvapo_.push_back(etatsEvapoCE_);
}


float EvapoMorton::calculerSlopeSatVapPressure(float tempMoy) const
{

    //float calcul = 4098*(0.6108*std::exp((17.27*(float)tempMoy)/((float)tempMoy+237.3)))/std::pow(((float)tempMoy+237.3),2);
    float calcul = static_cast<float>(4098 * (0.6108 * std::exp((17.27 * tempMoy) / (tempMoy + 237.3))) / std::pow((tempMoy + 237.3), 2));


  return calcul; 
}


float EvapoMorton::calculerPsychometricConstant(float airPressure) const
{

  float calcul = 0.665f*0.001f*airPressure; // Proulx-McInnis et al. (2013)

  return calcul; 
}

/*** Morton (1983) in Barr et al. (1997) ***/
float EvapoMorton::calculerMorton(float tempMoy, float airPressure, float rayonnement, float humidity, float longwave, float inShortwave) const
{
	// Latent heat of vaporisation (MJ
	float latentHeatVap = 2.501f-0.002361f*(float) tempMoy;
    
	// Priestley-Taylor alpha
	float alpha = params_.paramALPHA;

	// Saturation vapour pressure (MJ/K/kg)
	float gazConst = 0.4615f;

	//T0 (K)
	float T0 = 273.2f;

	float TaK = tempMoy + T0;

	//Saturated Vapour Pressure (kPa)
	float satPres = (6.108f*std::pow(10.0f,((7.5f*(float) tempMoy)/(237.3f+(float) tempMoy))))/10.0f; // divided by 10 to convert from hPa to kPa
	//float satPres = 0.611*std::exp(latentHeatVap/gazConst*(1/T0-1/TaK));

	// Actual vapour pressure (kPa)
	float vapPres = (float) humidity/100*satPres;

	// Wind function
	float psi = 1.98f; // for air temperatures at or above 0.0°C, 

	// Core equation
	float slopeSVP = calculerSlopeSatVapPressure((float) tempMoy)*((float) rayonnement);
	float psyConst = 1;

	float M = 1.37f* (float) longwave - 0.394f * (float) inShortwave;

	float ET = (slopeSVP*((2*alpha-1)* (float) rayonnement+2*alpha*M)) / (latentHeatVap*(slopeSVP+psyConst))-
		(psyConst*psi*(satPres-vapPres))/(latentHeatVap*(slopeSVP+psyConst));
	float calcul = ET;

	return calcul;
}
