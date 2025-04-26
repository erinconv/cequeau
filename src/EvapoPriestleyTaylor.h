//****************************************************************************
// Fichier: EvapoPriestleyTaylor.h
//
// Creation date: 2014-04-09
// Created by: 
//                Rio Tinto Alcan                     
//                Energie electrique                  
//                1954 Davis, Saguenay arr. Jonquiere,
//                G7S 4R7, QC, Canada                 
// 
// Usage: Use this template to create you own evapotranspiration module:
//        1- Copy and paste EvapoPriestleyTaylor.h and EvapoPriestleyTaylor.cpp
//        2- Rename the new files after your module name (please keep "Evapo" at the beginning). 
//           Example: EvapoSomething.h and EvapoSomething.cpp
//        3- Search for "// TODO New module" in the project. These are placeholders for you code.  
//           All methods MUST be implemented (see interface Evapo.h).
//
//****************************************************************************
#pragma once

#include "Evapo.h"

class EvapoPriestleyTaylor: public Evapo
{
public:
  //! Constructeur.
  EvapoPriestleyTaylor();
  EvapoPriestleyTaylor(int latitudeMoyenneBV, int nbCE, int pasParJour);
  ~EvapoPriestleyTaylor();
  
  int calculerEvapo(
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
  );

  int assimiler(const DateChrono& datePasDeTemps);

  int initialiserAssimilations(const mxArray* assimilations);

  void initialiserEtats(const mxArray* etatsInitiaux);

  void lireParametres(const mxArray* paramSimul);

  mxArray* obtenirEtats(const std::vector<bool> filtreCE);

  mxArray* obtenirEtatsAvantAssimilations(const std::vector<bool> filtreCE);

  void preserverEtatsPasDeTemps();

  // Etat de l'evapo pour un carreau entier
  // Est publique uniquement pour les besoins de Cequeau Qualite.
  // TODO: Devrait etre prive. A voir selon les autres modeles d'evapo si besoin.
  // State variables
  class EtatEvapoCE
  {
    public:
      // TODO New module: Add your state variables
      // Type MUST BE float
      /*** Example:****/
      float stateSol;
      float stateNappe;
      float stateEauLibre;
      float statePot;      
  };

  // Pour le calcul de la temperature de l'eau de Cequeau qualite
  // TODO: Generaliser selon les autres modeles de fonte, si besoin.
  const std::vector<std::vector<EtatEvapoCE> >& etatsEvapo() const {return etatsEvapo_;};

private:
  //! Nombre de pas de temps par jour
  int pasParJour_;

  class Params
  {
   public:
    // TODO New module: Add your parameters
    float paramALPHA;
	float fractionEvapoNappe;
     // Use a vector for parameter different for each CE 
	/*** Example:
    std::vector<float> param2;
    ***/
  } params_;
  
  // Donnees a assimiler
  class EtatEvapoAssimCE
  {
  public:
    int idCarreauEntier;
    // TODO New module: Add your variables
    /*** Example  
    TypeAssim stateVar1Type;
    float stateVar1[2];
    TypeAssim stateVar2Type;
    float stateVar2[2];
    ***/
  };

  //! Noms des variable d'etat dans les intrants
  std::vector<std::string> nomChamps_;
  //! Etat pour un carreau entier
  EtatEvapoCE etatEvapoCE_;
  //! Etats des carreaux entiers pour un pas de temps
  std::vector<EtatEvapoCE> etatsEvapoCE_;
  //! Etats des carreaux entiers pour tous les pas de temps
  std::vector<std::vector<EtatEvapoCE>> etatsEvapo_;

  // Donnees d'assimilation
  std::map<DateChrono, std::vector<EtatEvapoAssimCE>> assimilationsCE_;
  // conservation des etats avant assimilation
  std::map<DateChrono, std::vector<EtatEvapoCE>> avantAssimilationsCE_;


  float calculerSlopeSatVapPressure(float tempMoy) const;
  float calculerPsychometricConstant(float airPressure) const;
  float calculerPriestleyTaylor(float tempMoy, float airPressure, float rayonnement, float heatStorage) const;
  float calculerFonctionVent(float tempMoy, float humidity, float vitesseVent) const;
};
