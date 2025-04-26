//****************************************************************************
// Fichier: Parametres.cpp
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
#include "Parametres.h"

//------------------------------------------------------------------
Parametres::Parametres()
{ 
  FILE_LOG(logDEBUG) << "Parametres::Parametres()";
}

//------------------------------------------------------------------
Parametres::Parametres(DateChrono dateDebut, DateChrono dateFin)
  : dateDebut_(dateDebut), dateFin_(dateFin)
{
  FILE_LOG(logDEBUG) << "Parametres::Parametres(date dateDebut, date dateFin)";
}

//------------------------------------------------------------------
Parametres::~Parametres()
{
  FILE_LOG(logDEBUG) << "Parametres::~Parametres()";
}

//------------------------------------------------------------------
const DateChrono& Parametres::dateDebut() const
{
  FILE_LOG(logDEBUG) << "Parametres::dateDebut()";
  return dateDebut_;
}

//------------------------------------------------------------------
const DateChrono& Parametres::dateFin() const
{
  FILE_LOG(logDEBUG) << "Parametres::dateFin()";
  return dateFin_;
}

//------------------------------------------------------------------
int Parametres::dureeHeuresPasSimulation() const
{
  FILE_LOG(logDEBUG) << "Parametres::dureeHeuresPasSimulation()";
  return dureeHeuresPasSimulation_;
}

//------------------------------------------------------------------
const std::vector<bool>& Parametres::resultatsIdCE() const
{
  FILE_LOG(logDEBUG) << "Parametres::resultatsIdCE()";
  return resultatsIdCE_;
}

//------------------------------------------------------------------
const std::vector<bool>& Parametres::resultatsIdCP() const
{
  FILE_LOG(logDEBUG) << "Parametres::resultatsIdCP()";
  return resultatsIdCP_;
}

//------------------------------------------------------------------
const ParamSol& Parametres::sol() const
{
  FILE_LOG(logDEBUG) << "Parametres::sol()";
  return sol_;
}

//------------------------------------------------------------------
const ParamSolInitial& Parametres::solInitial() const
{
  FILE_LOG(logDEBUG) << "Parametres::solInitial()";
  return solInitial_;
}

//------------------------------------------------------------------
const ParamOption& Parametres::option() const
{
  FILE_LOG(logDEBUG) << "Parametres::option()";
  return option_;
}

//------------------------------------------------------------------
const ParamTransfert& Parametres::transfert() const
{
  FILE_LOG(logDEBUG) << "Parametres::transfert()";
  return transfert_;
}

//------------------------------------------------------------------
const ParamFacultatifs& Parametres::facultatifs() const
{
  FILE_LOG(logDEBUG) << "Parametres::facultatifs()";
  return facultatifs_;
}
const ParamPompage& Parametres::pompage() const
{
  FILE_LOG(logDEBUG) << "Parametres::pompage()";
  return pompage_;
}
//------------------------------------------------------------------
#ifdef ENV_MATLAB
void Parametres::initialiserFichier(std::string nomFichierParamExec, std::string nomFichierParamSimul, int nbCE, int nbCP)
{
  FILE_LOG(logDEBUG) << "Parametres::initialiserFichier(std::string nomFichierParamExec, std::string nomFichierParamSimul, int nbCE, int nbCP)";
  
  MATFile* pFichierParamExec = MexHelper::mhMatOpen(nomFichierParamExec, "r");
  mxArray* parametresExec = MexHelper::mhMatGetStructVariable(pFichierParamExec, "execution");
  MATFile* pFichierParamSimul = MexHelper::mhMatOpen(nomFichierParamSimul, "r");
  mxArray* parametresSimul = MexHelper::mhMatGetStructVariable(pFichierParamSimul, "parametres");

  initialiser(parametresExec, parametresSimul, nbCE, nbCP);

  matClose(pFichierParamExec);
  mxDestroyArray(parametresExec);
  matClose(pFichierParamSimul);
  mxDestroyArray(parametresSimul);
}
#endif

//------------------------------------------------------------------
void Parametres::initialiser(const mxArray* paramExec, const mxArray* paramSimul, int nbCE, int nbCP)
{
  FILE_LOG(logDEBUG) << "Parametres::initialiser(const mxArray* paramExec, const mxArray* paramSimul, int nbCE, int nbCP)";
  int nbValeurs;
  
  double datenum = 730851.0; // Resultat de datenum(2000, 12, 31) en Matlab
  //date dateTest = MexHelper::datenumToDate(&datenum);

  DateChrono dateTest = DateChrono::fromMatlabDatenum(datenum);

  // Validation de la difference entre datenum Matlab et la representation 
  // interne de boost::gregorian.
  if (dateTest.getYear() != 2000 || dateTest.getMonth() != 12 ||  dateTest.getDay() != 31) {
    std::string erreur = "Valeur de OFFSET_DATENUM_MATLAB invalide.";
    throw std::runtime_error(erreur);
  }

  // Traitement du fichier des parametres d'execution
  // dateDebut
  mxArray* pValeur = MexHelper::mhMxGetField(paramExec, 0, "dateDebut");
  double* valeur   = MexHelper::mhMxGetPr(pValeur, "dateDebut");
  dateDebut_       = MexHelper::datenumToDate(valeur);

  // dateFin
  pValeur  = MexHelper::mhMxGetField(paramExec, 0, "dateFin");
  valeur   = MexHelper::mhMxGetPr(pValeur, "dateFin");
  dateFin_ = MexHelper::datenumToDate(valeur);

  // Initialisation des listes de CE et CP qu'on desire en sortie
  std::vector<int> ids;
  std::vector<int>::const_iterator idsIter;
  int id;

  if (mxGetField(paramExec, 0, "resultatsIdCE") != NULL) {
    resultatsIdCE_.assign(nbCE, false);
    MexHelper::chargerValeurs(paramExec, "resultatsIdCE", ids);
    idsIter = ids.begin();
    // Les id dans la liste seront presents en sortie
    while (idsIter != ids.end()) {
      id = *idsIter;
      // On s'assure que le id est ok...
      if (id > 0 && id <= nbCE) {
        resultatsIdCE_[id-1] = true;
      }
      idsIter++;
    }
  }
  else {
    // Par defaut on affiche tout
    resultatsIdCE_.assign(nbCE, true);
  }

  // Cette fois pour les CP
  if (mxGetField(paramExec, 0, "resultatsIdCP") != NULL) {
    resultatsIdCP_.assign(nbCP, false);
    MexHelper::chargerValeurs(paramExec, "resultatsIdCP", ids);
    idsIter = ids.begin();
    // Les id dans la liste seront presents en sortie
    while (idsIter != ids.end()) {
      id = *idsIter;
      // On s'assure que le id est ok...
      if (id > 0 && id <= nbCP) {
        resultatsIdCP_[id-1] = true;
      }
      idsIter++;
    }
  }
  else {
    // Par defaut on affiche tout
    resultatsIdCP_.assign(nbCP, true);
  }

  /** option **/
  mxArray* option = MexHelper::mhMxGetField(paramSimul, 0, "option");
  MexHelper::chargerValeurs(option, "ipassim", dureeHeuresPasSimulation_);
  // 24 = 24 heures (1 jour)
  if (24 % dureeHeuresPasSimulation_ != 0) {
    std::stringstream erreur;
    erreur << "Duree du pas de simulation invalide: " << dureeHeuresPasSimulation_ << " heures. Doit etre 1,2,3,4,6,8,12 ou 24";
    throw std::runtime_error(erreur.str());
  }
  MexHelper::chargerValeurs(option, "moduleFonte", option_.moduleFonte);
  MexHelper::chargerValeurs(option, "moduleEvapo", option_.moduleEvapo);
  MexHelper::chargerValeurs(option, "calculQualite", option_.calculQualite);
  
  
  if (MexHelper::hasField(option, 0, "moduleDLI")) {
      MexHelper::chargerValeurs(option, "moduleDLI", option_.moduleDLI);
  } else {
      option_.moduleDLI = 0;
  }

  if (MexHelper::hasField(option, 0, "logNeigeAjustee")) {
    MexHelper::chargerValeurs(option, "logNeigeAjustee", option_.logNeigeAjustee);
  }

  if (MexHelper::hasField(option, 0, "moduleOmbrage")) {
      MexHelper::chargerValeurs(option, "moduleOmbrage", option_.moduleOmbrage);
  } else {
    option_.moduleOmbrage = 0;
  }

  if (MexHelper::hasField(option, 0, "modulePompage")) {
      MexHelper::chargerValeurs(option, "modulePompage", option_.modulePompage);
  } else {
    option_.modulePompage = 0;
  }

  if (MexHelper::hasField(option, 0, "moduleOmbrage")) {
      MexHelper::chargerValeurs(option, "moduleOmbrage", option_.moduleOmbrage);
  } else {
    option_.moduleOmbrage = 0;
  }

  /** sol **/
  mxArray* sol        = MexHelper::mhMxGetField(paramSimul, 0, "sol");
  MexHelper::chargerValeurs(sol, "cin_s"  , sol_.coeffInfiltrationNappe);
  validerNombreValeurs("sol.cin_s", (int)sol_.coeffInfiltrationNappe.size(), nbCE);

  MexHelper::chargerValeurs(sol, "cvmar"  , sol_.coeffVidangeLacsMarais);
  MexHelper::chargerValeurs(sol, "cvnb_s" , sol_.coeffVidangeBasseNappe);
  validerNombreValeurs("sol.cvnb_s", (int)sol_.coeffVidangeBasseNappe.size(), nbCE);

  MexHelper::chargerValeurs(sol, "cvnh_s" , sol_.coeffVidangeHauteNappe);
  validerNombreValeurs("sol.cvnh_s", (int)sol_.coeffVidangeHauteNappe.size(), nbCE);

  MexHelper::chargerValeurs(sol, "cvsb"   , sol_.coeffVidangeBasseSol);
  MexHelper::chargerValeurs(sol, "cvsi_s" , sol_.coeffVidangeIntermediaireSol);
  validerNombreValeurs("sol.cvsi_s", (int)sol_.coeffVidangeIntermediaireSol.size(), nbCE);

  MexHelper::chargerValeurs(sol, "xinfma" , sol_.infiltrationMax);
  MexHelper::chargerValeurs(sol, "hinf_s" , sol_.seuilInfiltrationSolVersNappe);
  validerNombreValeurs("sol.hinf_s", (int)sol_.seuilInfiltrationSolVersNappe.size(), nbCE);

  MexHelper::chargerValeurs(sol, "hint_s" , sol_.seuilVidangeIntermediaireSol);
  validerNombreValeurs("sol.hint_s", (int)sol_.seuilVidangeIntermediaireSol.size(), nbCE);

  MexHelper::chargerValeurs(sol, "hmar"   , sol_.seuilVidangeLacsMarais);
  MexHelper::chargerValeurs(sol, "hnap_s" , sol_.seuilVidangeHauteNappe);
  validerNombreValeurs("sol.hnap_s", (int)sol_.seuilVidangeHauteNappe.size(), nbCE);

  MexHelper::chargerValeurs(sol, "hpot_s" , sol_.seuilPrelevementEauTauxPotentiel);
  validerNombreValeurs("sol.hpot_s", (int)sol_.seuilPrelevementEauTauxPotentiel.size(), nbCE);

  MexHelper::chargerValeurs(sol, "hsol_s" , sol_.hauteurReservoirSol);
  validerNombreValeurs("sol.hsol_s", (int)sol_.hauteurReservoirSol.size(), nbCE);

  MexHelper::chargerValeurs(sol, "hrimp_s", sol_.lameEauDebutRuisellement);
  validerNombreValeurs("sol.hrimp_s", (int)sol_.lameEauDebutRuisellement.size(), nbCE);

  MexHelper::chargerValeurs(sol, "tri_s"  , sol_.fractionImpermeableCE);
  validerNombreValeurs("sol.tri_s", (int)sol_.fractionImpermeableCE.size(), nbCE);

  MexHelper::chargerValeurs(sol, "xla"    , sol_.latitudeMoyenneBV);

  /** solInitial **/
  mxArray* solInitial = MexHelper::mhMxGetField(paramSimul, 0, "solInitial");
  MexHelper::chargerValeurs(solInitial, "hsini" , solInitial_.niveauInitialSol);
  MexHelper::chargerValeurs(solInitial, "hnini" , solInitial_.niveauInitialNappe);
  MexHelper::chargerValeurs(solInitial, "hmini" , solInitial_.niveauInitialLacsMarais);
  MexHelper::chargerValeurs(solInitial, "q0"    , solInitial_.debitInitialExutoire);

  /** lacExutoire **/
  if (MexHelper::hasField(paramSimul, 0, "lacExutoire")) {
    mxArray* lacExutoire = MexHelper::mhMxGetField(paramSimul, 0, "lacExutoire");
    MexHelper::chargerValeurs(paramSimul, "lacExutoire", facultatifs_.lacExutoire);
    validerNombreValeurs("facultatifs.lacExutoire", (int)facultatifs_.lacExutoire.size(), nbCE);

    // Aucune zone lac exutoire
    if ((int)facultatifs_.lacExutoire.size() == 1) {
      facultatifs_.lacExutoire.assign(nbCE, 0);
    }
  }
  else {
    // Aucune zone lac exutoire
    facultatifs_.lacExutoire.assign(nbCE, 0);
  }

  /** transfert **/
  mxArray* transfert  = MexHelper::mhMxGetField(paramSimul, 0, "transfert");
  MexHelper::chargerValeurs(transfert, "exxkt", transfert_.paramCalculCoeffTransfertCP);
  MexHelper::chargerValeurs(transfert, "zn"   , transfert_.tempsConcentrationBV);

  /** ctp **/
  mxArray* ctp = MexHelper::mhMxGetField(paramSimul, 0, "ctp");
  nbValeurs = (int)mxGetNumberOfElements(ctp);
  
  if (validerNombreValeurs("ctp", nbValeurs, nbCP)) {
    if (nbValeurs > 1) {
      double* valeur = MexHelper::mhMxGetPr(ctp, "ctp");

      for (int i = 0; i < nbValeurs; i++) {
       if (valeur[i] != 0.0) {
          facultatifs_.coeffTxParticuliers.insert(std::make_pair(i + 1, valeur[i] / 10000.0));
        }
      }
    }
  }

  /** lac **/
  mxArray* lac = MexHelper::mhMxGetField(paramSimul, 0, "lac");
  nbValeurs = (int)mxGetNumberOfElements(lac);

  if (validerNombreValeurs("lac", nbValeurs, nbCP)) {
    if (nbValeurs > 1) {
      double* valeur = MexHelper::mhMxGetPr(lac, "lac");

      for (int i = 0; i < nbValeurs; i++) {
        facultatifs_.lacs.insert(std::make_pair(i + 1, (int)valeur[i]));
      }
    }
  }

  /** surface **/
  mxArray* surface = MexHelper::mhMxGetField(paramSimul, 0, "surface");
  nbValeurs = (int)mxGetNumberOfElements(surface);

  if (validerNombreValeurs("surface", nbValeurs, nbCP)) {
    if (nbValeurs > 1) {
      double* valeur = MexHelper::mhMxGetPr(surface, "surface");

      for (int i = 0; i < nbValeurs; i++) {
       if (valeur[i] != 0.0) {
         facultatifs_.superficieCPAmontCorriges.insert(std::make_pair(i + 1, (float)valeur[i]));
        }
      }
    }
  }

    /** pompage **/
  bool possedePompage = MexHelper::hasField(paramSimul, 0, "pompage");
  if (possedePompage) {
    mxArray* pompage  = MexHelper::mhMxGetField(paramSimul, 0, "pompage");
    MexHelper::chargerValeurs(pompage, "delai"  , pompage_.delai);

    if (MexHelper::hasField(pompage, 0, "coeffPompage")) {
      MexHelper::chargerValeurs(pompage, "coeffPompage"  , pompage_.coeffPompage);
    } else {
      pompage_.coeffPompage = -1;
    }

    if (MexHelper::hasField(pompage, 0, "conductiviteHydraulique_s")) {
      MexHelper::chargerValeurs(pompage, "conductiviteHydraulique_s"  , pompage_.conductiviteHydraulique);
      validerNombreValeurs("pompage.conductiviteHydraulique_s", (int)pompage_.conductiviteHydraulique.size(), nbCE);
    } else {
      pompage_.conductiviteHydraulique.push_back(1);
    }

    if (MexHelper::hasField(pompage, 0, "coeffEmmagasinement_s")) {
      // mexPrintf("\ncoeffEmmagasinement_s exists \n");
      MexHelper::chargerValeurs(pompage, "coeffEmmagasinement_s"  , pompage_.coeffEmmagasinement);
      validerNombreValeurs("pompage.coeffEmmagasinement_s", (int)pompage_.coeffEmmagasinement.size(), nbCE);

      int idx = 1;
      for (float v : pompage_.coeffEmmagasinement) {
        FILE_LOG (logINFO) << "coeffEmmagasinement_s []" << idx << "] = " <<  v;
        idx++;
      }

    } else {
      // mexPrintf("\ncoeffEmmagasinement_s does not exists \n");
      pompage_.coeffEmmagasinement.push_back(1);
    }

  } else {
    pompage_.delai = 0;
    pompage_.coeffPompage = -1;
    pompage_.conductiviteHydraulique.push_back(1);
    pompage_.coeffEmmagasinement.push_back(1);
  }

  // TODO: Releves neige
  // Vecteurs DATERELEVE et RELEVEMOY
  bool possedeReleves = MexHelper::hasField(paramSimul, 0, "relevesNeige");
  facultatifs_.typeAjustementNeige = AUCUN; 
  if (possedeReleves) {
    mxArray* relevesNeige = MexHelper::mhMxGetField(paramSimul, 0, "relevesNeige");
    int iMin, iMax, jMin, jMax, nbReleves = (int)mxGetNumberOfElements(relevesNeige);
    float valeur;
    double pasDeTemps;
    DateChrono uneDate;
    ValeurZone releveNeige;
    facultatifs_.typeAjustementNeige = STATIONS_3;
    
    for (int cptReleves = 0; cptReleves < nbReleves; cptReleves++) {
      MexHelper::chargerValeurs(relevesNeige, "pasDeTemps", pasDeTemps, cptReleves);
      MexHelper::chargerValeurs(relevesNeige, "iMin", iMin, cptReleves);
      MexHelper::chargerValeurs(relevesNeige, "iMax", iMax, cptReleves);
      MexHelper::chargerValeurs(relevesNeige, "jMin", jMin, cptReleves);
      MexHelper::chargerValeurs(relevesNeige, "jMax", jMax, cptReleves);
      MexHelper::chargerValeurs(relevesNeige, "valeur", valeur, cptReleves);
      uneDate = MexHelper::datenumToDate(&pasDeTemps);
      releveNeige.iMin = iMin;
      releveNeige.iMax = iMax;
      releveNeige.jMin = jMin;
      releveNeige.jMax = jMax;
      releveNeige.valeur = valeur;
      facultatifs_.relevesNeige.insert(std::make_pair(uneDate, releveNeige));
      
      // On considere une application par zone si les min et max sont differents
      if (iMax != 0 && iMin != iMax || jMax != 0 && jMin != jMax) {
        facultatifs_.typeAjustementNeige = ZONE;
      }

    }
  }
}

//------------------------------------------------------------------
bool Parametres::validerNombreValeurs(const std::string& nomChamp, int nbValeur, int nbValeurOK) 
{
  FILE_LOG(logDEBUG) << "Parametres::validerNombreValeurs(...)";

  bool retVal = true;
  // Si plus d'une valeur, on doit avoir un nombre coherent de valeur (nbCE ou bien nbCP).
  if (nbValeur > 1 && nbValeur != nbValeurOK) {
    std::stringstream avertissement;
    
    avertissement << "Avertissement: Le nombre de valeurs de " << nomChamp << " est incoherent: " << nbValeur << std::endl;
    avertissement << "  Seule la premiere valeur sera utilisee." << std::endl;
    avertissement << "  Doit egaler 1 ou bien nombre de carreaux: " << nbValeurOK << std::endl;

    mexPrintf(avertissement.str().c_str());
    FILE_LOG(logWARNING) << avertissement.str();
    retVal = false;
  }
  
  return retVal;
}

//------------------------------------------------------------------
int Parametres::nbJoursSimulation() const
{
  FILE_LOG(logDEBUG) << "Parametres::nbJoursSimulation()";
      
  DateChrono dateDebut = dateDebut_;
  DateChrono dateFin = dateFin_;

  // Calculate the duration between dateDebut and dateFin
  std::chrono::duration<double> duration = dateFin.getTimePoint() - dateDebut.getTimePoint();

  // Convert the duration to days
  int nbDays = static_cast<int>(duration.count() / (60 * 60 * 24)) + 1;

  return nbDays;
}

//------------------------------------------------------------------
int Parametres::nbPasSimulation() const
{
  FILE_LOG(logDEBUG) << "Parametres::nbPasSimulation()";
  return nbJoursSimulation() * nbPasParJourSimulation();
}

//------------------------------------------------------------------
int Parametres::nbPasParJourSimulation() const
{
  FILE_LOG(logDEBUG) << "Parametres::nbPasParJourSimulation()";
  return (24 / dureeHeuresPasSimulation_);
}

//------------------------------------------------------------------
ParamCE::ParamCE()
{
  FILE_LOG(logDEBUG) << "ParamCE::ParamCE()";

  float infini = std::numeric_limits<float>::infinity();

  seuilTranformationPluieNeige =      infini; 
  tauxPotentielFonteForet =           infini; 
  tauxPotentielFonteClairiere =       infini; 
  seuilTempFonteForet =               infini; 
  seuilTempFonteClairiere =           infini; 
  tempMurissementNeige =              infini; 
  coeffInfiltrationNappe =            infini; 
  coeffVidangeBasseNappe =            infini;  
  coeffVidangeHauteNappe =            infini;  
  coeffVidangeIntermediaireSol =      infini;  
  seuilInfiltrationSolVersNappe =     infini;  
  seuilVidangeIntermediaireSol =      infini;  
  seuilVidangeHauteNappe =            infini;  
  seuilPrelevementEauTauxPotentiel =  infini;  
  hauteurReservoirSol =               infini;  
  lameEauDebutRuisellement =          infini;  
  fractionImpermeableCE =             infini;  
  conductiviteHydraulique =           infini;
  coeffEmmagasinement =               infini;
}
