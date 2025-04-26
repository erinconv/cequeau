//****************************************************************************
// Fichier: FonteCemaNeige.h
//
// Creation date: 2014-04-15
// Created by: 
//                Rio Tinto Alcan                     
//                Energie electrique                  
//                1954 Davis, Saguenay arr. Jonquiere,
//                G7S 4R7, QC, Canada                 
// 
//****************************************************************************
#pragma once

#include "Fonte.h"

class FonteCemaNeige: public Fonte
{
public:
  //! Constructeur.
  FonteCemaNeige();
  FonteCemaNeige(int latitudeMoyenneBV, int nbCE);
  ~FonteCemaNeige();

  int calculerFonte(
  // IN
  const DateChrono& datePasDeTemps,
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

  // State variables
  class EtatFonteCE
  {
    public:
      float eTg;
      float G;
      float fonte_reel;
      float SNC_stockNeigeForet;
      float SND_stockNeigeClairiere;
  };
  // Pour le calcul de la temperature de l'eau de Cequeau qualite
  const std::vector<std::vector<EtatFonteCE> >& etatsFonte() const {return etatsFonte_;};

private:

  class Params
  {
   public:
  //  Melt factor (mm day-1 degC-1)
    float Kf;
    // Thereshold temperature (degC)
    float Tf;
    // Thereshold to convert rain to snow
    float seuilTranformationPluieNeige;
    // Coefficient of the therman state of the snowpack
    float CTg;
    float theta;
    float Gseuil;
    float Zmed;
    // le pourcentage de f K correspondant à la vitesse de fonte minimale (c'est-à-dire la
    // vitesse atteinte quand le stock est très faible)
    float Vmin;
    // Initial values for the state variables
    
    // Thermal of the snowpack
    float eTg;
    // Snow accumulation
    float G;
  } params_;
  
  std::tuple<float, float> split_precip_cequeau(Meteo meteo);
  // Donnees a assimiler
  class EtatFonteAssimCE
  {
  public:
    int idCarreauEntier;
    TypeAssim SNC_stockNeigeForetType;
    float SNC_stockNeigeForet[2];
    TypeAssim SND_stockNeigeClairiereType;
    float SND_stockNeigeClairiere[2];
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
