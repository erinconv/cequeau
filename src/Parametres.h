//****************************************************************************
// Fichier: Parametres.h
//
// Date creation: 2012-10-01
// Auteur: 
//                Rio Tinto Alcan                     
//                Energie electrique                  
//                1954 Davis, Saguenay arr. Jonquiere,
//                G7S 4R7, QC, Canada                 
//
//****************************************************************************
#pragma once

#include "DateChrono.h"

//! Classe de donnees des parametres relatifs reservoirs SOL, NAPPE et MARAIS.
class ParamSol
{
public:
  //! Coefficient d'infiltration dans le r�servoir NAPPE. 
  // CIN
  std::vector<float> coeffInfiltrationNappe;
  //! Coefficient de vidange du r�servoir LACS et MARAIS.
  // CVMAR
  float coeffVidangeLacsMarais;
  //! Coefficient de vidange basse du r�servoir NAPPE.
  // CVNB
  std::vector<float> coeffVidangeBasseNappe; 
  //! Coefficient de vidange haute du r�servoir NAPPE.
  // CVNH
  std::vector<float> coeffVidangeHauteNappe; 
  //! Coefficient de vidange basse du r�servoir SOL.
  // CVSB
  float coeffVidangeBasseSol; 
  //! Coefficient de vidange interm�diaire du r�servoir SOL.
  // CVSI
  std::vector<float> coeffVidangeIntermediaireSol; 
  //! Infiltration maximale (mm/jour).
  // XINFMA
  float infiltrationMax; 

  //! Seuil d'infiltration du r�servoir SOL vers le r�servoir NAPPE (mm).
  // HINF
  std::vector<float> seuilInfiltrationSolVersNappe; 
  //! Seuil de vidange interm�diaire du r�servoir SOL (mm).
  // HINT
  std::vector<float> seuilVidangeIntermediaireSol; 
  //! Seuil de vidange du r�servoir LACS et MARAIS (mm).
  // HMAR
  float seuilVidangeLacsMarais; 
  //! Seuil de vidange sup�rieure du r�servoir NAPPE (mm).
  // HNAP
  std::vector<float> seuilVidangeHauteNappe; 
  //! Seuil de pr�l�vement de l'eau � taux potentiel, par �vapotranspiration (mm).
  // HPOT
  std::vector<float> seuilPrelevementEauTauxPotentiel; 
  //! Hauteur du r�servoir SOL (mm).
  // HSOL
  std::vector<float> hauteurReservoirSol; 
  //! Lame d'eau n�cessaire pour que d�bute le ruissellement sur les surfaces impermeables (mm).
  // HRIMP
  std::vector<float> lameEauDebutRuisellement; 
 
  //! Coefficient de correction des pr�cipitations annuelles en fonction de l'altitude (mm/m�tre/an).
  // COEP
  float coeffCorrectionPluieAnAltitude; 
  //! Fraction de l'�vapotranspiration prise dans le r�servoir NAPPE (de 0.0 a 1.0);
  // EVNAP
  
  //float fractionEvapoNappe; 
  //! Fraction de surface imperm�able des carreaux entiers (de 0.0 � 1.0).
  // TRI
  std::vector<float> fractionImpermeableCE; 
  //! Latitude moyenne du bassin versant en degr�s et minutes sexag�simales
  //! (ex.: XLA = 4245 pour une latitude de 42o45').
  // XLA
  float latitudeMoyenneBV; 
  //! Correction des temp�ratures en fonction de l'altitude (degC/1 000 m).
  // COET
  float correctionTempAltitude; 

};

//! Classe de donnees pour les valeurs initiales des reservoirs.
class ParamSolInitial
{
public:
  //! Niveau d'eau initial dans le r�servoir SOL (mm).
  // HSINI
  float niveauInitialSol;
  //! Niveau d'eau initial dans le r�servoir NAPPE (mm).
  // HNINI
  float niveauInitialNappe;
  //! Niveau d'eau initial dans le r�servoir LACS et MARAIS (mm).
  // HMINI
  float niveauInitialLacsMarais;
  //! Debit initial � l'exutoire du bassin versant (m3/s).
  // QO
  float debitInitialExutoire;
};

//! Classe de donnees pour les parametres optionnels.
class ParamOption
{
public:
  //! Module de fonte a utiliser
  int moduleFonte;
  //! Module d'evapotranspiration a utiliser
  int moduleEvapo;
  //! Execution de la qualite?
  bool calculQualite;
  //! Module DLI %%%%%
  int moduleDLI;
  // TODO: Releves neige
  bool logNeigeAjustee;
    //! Utilisation du module d'ombrage
  int moduleOmbrage;
  //! Utilisation du module de pompage
  int modulePompage;
 }; 

//! Classe de donnees pour les parametres relatifs a la fonction de transfert.
class ParamTransfert
{
public:
  //! Parametre de calcul des coefficients de transfert des carreaux partiels, pour
  //! le pas de temps de la simulation.
  // EXXKT
  float paramCalculCoeffTransfertCP;
  //! Temps de concentration du bassin versant (en pas de temps).
  // ZN
  float tempsConcentrationBV;
 }; 

// TODO: Releves neige
enum TypeAjustementNeige {AUCUN, ZONE, STATIONS_3};
/*! valeur specifique d'un parametre pour la zone d�finie par
    iMin, iMax, jMin et jMax
*/
class ValeurZone
{
public:
  int iMin;
  int iMax;
  int jMin;
  int jMax;
  float valeur;
};

//! Classe de donnees pour les parametres facultatifs.
class ParamFacultatifs
{
public:
  //! Corrections des superficies des carreaux partiels.
  std::map<int, float> superficieCPAmontCorriges;
  //! Coefficients de transfert particuliers
  std::map<int, double> coeffTxParticuliers;
  /*!
    Permettent de tenir compte, pour le calcul des coefficients de transfert,
    des lacs chevauchant plusieurs carreaux partiels, ou encore d'exclure du carreau
    partiel les lacs non situes sur le reseau d'�coulement
  */
  std::map<int, int> lacs;
  //! Liste fraction de surface impermeable de carreau entier
  std::map<int, float> surfacesImper;

  //! Liste de l'appartenance des carreaux entiers a une zone de lac exutoire (precipitations directes)
  std::vector<int> lacExutoire;

  // TODO: Releves neige
  //! Releves de neige remplacant les stocks de neige calcules aux dates donnees.
  std::multimap<DateChrono, ValeurZone> relevesNeige;
  TypeAjustementNeige typeAjustementNeige;
 }; 

class ParamPompage {
public:
    int delai;
    double coeffPompage;
    std::vector<float> conductiviteHydraulique;
    std::vector<float> coeffEmmagasinement;  // coefficient d'emmagasinement
};

// TODO: Releves neige
typedef std::multimap<DateChrono, ValeurZone>::const_iterator RelevesNeigeIter;

//! Classe des parametres de la simulation
class Parametres
{
public:
   Parametres();
   Parametres(DateChrono dateDebut, DateChrono dateFin);
   ~Parametres();

   //! Getter Date de debut de la simulation
   const DateChrono& dateDebut() const;
   //! Getter Date de fin de la simulation
   const DateChrono& dateFin() const;
   //! Getter Duree en heure d'un pas de simulation: 1,2,3,4,6,8,12,24 heures
   int dureeHeuresPasSimulation() const;
   //! Getter nombre jours de simulation
   int nbJoursSimulation() const;
   //! Getter nombre pas de simulation
   int nbPasSimulation() const;
   //! Getter nombre pas pas jour de simulation
   int nbPasParJourSimulation() const;
   //! Getter selection des carreaux entiers qu'on desire en sortie
   const std::vector<bool>& resultatsIdCE() const;
   //! Getter selection des carreaux partiels qu'on desire en sortie
   const std::vector<bool>& resultatsIdCP() const;
   //! Getter param sol
   const ParamSol& sol() const;
   //! Getter param solInitial
   const ParamSolInitial& solInitial() const;
   //! Getter param option
   const ParamOption& option() const;
   //! Getter param transfert
   const ParamTransfert& transfert() const;
   //! Getter param facultatifs
   const ParamFacultatifs& facultatifs() const;
      //! Getter param pompage
   const ParamPompage& pompage() const;

   //! Initialisation des parametres a partir de fichiers d'intrants Matlab
   void initialiserFichier(std::string nomFichierParamExec, std::string nomFichierParamSimul, int nbCE, int nbCP);
   //! Initialisation des parametres a partir de variables Matlab (mxArray)
   void initialiser(const mxArray* paramExec, const mxArray* paramSimul, int nbCE, int nbCP);

private:
   DateChrono dateDebut_;
   DateChrono dateFin_;
   // 1,2,3,4,6,8,12,24 heures
   int dureeHeuresPasSimulation_;
   // Selection des carreaux entiers qu'on desire en sortie
   std::vector<bool> resultatsIdCE_;
   // Selection des carreaux partiels qu'on desire en sortie
   std::vector<bool> resultatsIdCP_;
   ParamSol   sol_;
   ParamSolInitial solInitial_;
   ParamOption option_;
   ParamTransfert transfert_;
   ParamFacultatifs facultatifs_;
   ParamPompage pompage_;

   //! Validation du nombre de valeurs d'un parametre spatialisable. 
   bool validerNombreValeurs(const std::string& nomChamp, int nbValeur, int nbValeursOK);
};

//! Parametres spatialisables.
/*! 
  Parametres obligatoires dont la valeur peut etre modifiee
  pour chaque carreau entier a l'aide de vecteurs facultatifs.
*/
class ParamCE {
public:
  ParamCE();
  // NEIGE
  //! Seuil de transformation pluie-neige (degC).
  // STRNE
  float seuilTranformationPluieNeige; 
  //! Taux potentiel de fonte en for�t (mm/degC/jour).
  // TFC
  float tauxPotentielFonteForet;
  //! Taux potentiel de fonte en clairi�re (mm/degC/jour).
  // TFD
  float tauxPotentielFonteClairiere;
  //! Seuil de temp�rature de fonte en foret (degC).
  // TSC
  float seuilTempFonteForet;
  //! Seuil de temp�rature de fonte en clairiere (degC).
  // TSD
  float seuilTempFonteClairiere;
  //! Temperature du murissement du stock de neige (degC).
  // TTS
  float tempMurissementNeige;
  // SOL
  //! Coefficient d'infiltration dans le r�servoir NAPPE. 
  // CIN
  float coeffInfiltrationNappe;
  //! Coefficient de vidange basse du r�servoir NAPPE.
  // CVNB
  float coeffVidangeBasseNappe; 
  //! Coefficient de vidange haute du r�servoir NAPPE.
  // CVNH
  float coeffVidangeHauteNappe; 
  //! Coefficient de vidange interm�diaire du r�servoir SOL.
  // CVSI
  float coeffVidangeIntermediaireSol; 
  //! Seuil d'infiltration du r�servoir SOL vers le r�servoir NAPPE (mm).
  // HINF
  float seuilInfiltrationSolVersNappe; 
  //! Seuil de vidange interm�diaire du r�servoir SOL (mm).
  // HINT
  float seuilVidangeIntermediaireSol; 
  //! Seuil de vidange sup�rieure du r�servoir NAPPE (mm).
  // HNAP
  float seuilVidangeHauteNappe; 
  //! Seuil de pr�l�vement de l'eau � taux potentiel, par �vapotranspiration (mm).
  // HPOT
  float seuilPrelevementEauTauxPotentiel; 
  //! Hauteur du r�servoir SOL (mm).
  // HSOL
  float hauteurReservoirSol; 
  //! Lame d'eau n�cessaire pour que d�bute le ruissellement sur les surfaces impermeables (mm).
  // HRIMP
  float lameEauDebutRuisellement; 
  //! Fraction de surface imperm�able des carreaux entiers (de 0.0 � 1.0).
  // TRI
  float fractionImpermeableCE;
  // conductivite hydraulique
  float conductiviteHydraulique;
  // coefficient d’emmagasinnement : C’est la quantité d’eau libéré sous une variation unitaire de la charge hydraulique
  float coeffEmmagasinement;
};
