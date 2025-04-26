//****************************************************************************
// Fichier: EvapoKPenman.h
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
#pragma once

#include "Evapo.h"

class EvapoKPenman: public Evapo
{
public:
  //! Constructeur.
  EvapoKPenman();
  EvapoKPenman(int latitudeMoyenneBV, int nbCE, int pasParJour);
  ~EvapoKPenman();

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

private:
  //! Nombre de pas de temps par jour
  int pasParJour_;

  class Params
  {
   public:
    // TODO New module: Add your parameters
    // Use a vector for parameter different for each CE
    /*** Example:
    int param1;
    std::vector<float> param2;
    ***/
	float fractionEvapoNappe;
  } params_;
  
  // State variables
  class EtatEvapoCE
  {
    public:
      // TODO New module: Add your state variables
      // Type MUST BE float
      /*** Example:
      float stateVar1;
      float stateVar2;
      ****/

  };

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
  float calculerKPenman(float tempMoy, float airPressure, float rayonnement, float wind, float humidity, float noJour) const;
};
