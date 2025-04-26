//****************************************************************************
// Fichier: Evapo.h
//
// Date creation: 2014-04-02
// Auteur: 
//                Rio Tinto Alcan                     
//                Energie electrique                  
//                1954 Davis, Saguenay arr. Jonquiere,
//                G7S 4R7, QC, Canada                 
//
//****************************************************************************
#pragma once
#include "CarreauEntier.h"
#include "Meteo.h"
#include "ModuleBase.hpp"
#include "DateChrono.h"

//! Interface module d'evapotranspiration
class Evapo: public ModuleBase
{
public:
  //! Constructeur.
  Evapo() {};
  
  Evapo(int nbCE, std::string nomModele)
    : ModuleBase(nbCE, nomModele) {}
  
  virtual ~Evapo() {};
  
  virtual int calculerEvapo(
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
  ) = 0;

  virtual int assimiler(const DateChrono& datePasDeTemps) = 0;

  virtual int initialiserAssimilations(const mxArray* assimilations) = 0;

  virtual void initialiserEtats(const mxArray* etatsInitiaux) = 0;

  virtual void lireParametres(const mxArray* paramSimul) = 0;

  virtual mxArray* obtenirEtats(const std::vector<bool> filtreCE) = 0;

  virtual mxArray* obtenirEtatsAvantAssimilations(const std::vector<bool> filtreCE) = 0;

  virtual void preserverEtatsPasDeTemps() = 0;

};
