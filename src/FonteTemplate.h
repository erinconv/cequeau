//****************************************************************************
// Fichier: FonteTemplate.h
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
#pragma once

#include "Fonte.h"

class FonteTemplate: public Fonte
{
public:
  //! Constructeur.
  FonteTemplate();
  FonteTemplate(int latitudeMoyenneBV, int nbCE);
  ~FonteTemplate();

  int calculerFonte(
  // IN
  const int& noJour,
  const Meteo& meteo,
  const CarreauEntier& carreauEntier,
  // OUT
  float& precipationsTotales,
  float& eauDisponible
  );

  int assimiler(const DateChrono& datePasDeTemps);

  int initialiserAssimilations(const mxArray* assimilations);

  void initialiserEtats(const mxArray* etatsInitiaux);

  void lireParametres(const mxArray* paramSimul);

  mxArray* obtenirEtats(const std::vector<bool> filtreCE);

  mxArray* obtenirEtatsAvantAssimilations(const std::vector<bool> filtreCE);

  void preserverEtatsPasDeTemps();

private:

  class Params
  {
   public:
    // TODO New module: Add your parameters
    /*** Example:
    int jourSoleilMaxFonteNeige;
    std::vector<float> seuilTempFonteClairiere;
    ***/
  } params_;
  
  // State variables
  class EtatFonteCE
  {
    public:
      // TODO New module: Add your state variables
      // Type MUST BE float
      /*** Example:
      float SNC_stockNeigeForet;
      float SND_stockNeigeClairiere;
      float QNUI3_indexMurissementNeige;
      float QNUI4_indexTempNeige;
      ****/

  };

  // Donnees a assimiler
  class EtatFonteAssimCE
  {
  public:
    int idCarreauEntier;
    // TODO New module: Add your variables
    /*** Example
    TypeAssim SNC_stockNeigeForetType;
    float SNC_stockNeigeForet[2];
    TypeAssim SND_stockNeigeClairiereType;
    float SND_stockNeigeClairiere[2];
    TypeAssim QNUI3_indexMurissementNeigeType;
    float QNUI3_indexMurissementNeige[2];
    TypeAssim QNUI4_indexTempNeigeType;
    float QNUI4_indexTempNeige[2];
    ***/
  };

  //! Noms des variable d'etat dans les intrants
  std::vector<std::string> nomChamps_;
  //! Etat pour un carreau entier
  EtatFonteCE etatFonteCE_;
  //! Etats des carreaux entiers pour un pas de temps
  std::vector<EtatFonteCE> etatsFonteCE_;
  //! Etats des carreaux entiers pour tous les pas de temps
  std::vector<std::vector<EtatFonteCE> > etatsFonte_;

  // Donnees d'assimilation
  std::map<DateChrono, std::vector<EtatFonteAssimCE> > assimilationsCE_;
  // conservation des etats avant assimilation
  std::map<DateChrono, std::vector<EtatFonteCE> > avantAssimilationsCE_;

};
