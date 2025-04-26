//****************************************************************************
// Fichier: DonneesMeteo.h
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

#include "Meteo.h"
#include "BassinVersant.h"

//! "Smart pointer" sur une donnee meteo.
typedef std::shared_ptr<Meteo> MeteoPtr;
//! Liste des donnees meteo.
typedef std::vector<MeteoPtr> MeteoGrille;

//! Ensemble des donnees meteo.
/*! 
    Donnees meteo grille pour chaque pas de temps de la simulation.
 */
class DonneesMeteo
{
public:
  //! Constructeur
  DonneesMeteo();
  DonneesMeteo(int nbCarreauxEntiers, int nbPasDeTemps);
  ~DonneesMeteo();

  // Getter nbCarreauxEntiers
  int nbCarreauxEntiers() const;
  // Getter nbPasDeTemps
  int nbPasDeTemps() const; 
  // Getter estPtot
  bool estPtot() const; 

  //! Getter des donnees meteo
  /*!
    Retourne une reference sur un vecteur de vecteur de meteo.
    Pour chacun des jours on a un vecteur de meteo de chaque carreau entier.
  */
  const std::vector<MeteoGrille>& valeurs() const;

  //! Initialisation des donnes meteo a partir d'un fichier Matlab.
  void initialiserFichier(const std::string fichierEntree);
  //! Initialisation des donnes meteo a partir d'un mxArray.
  void initialiser(const mxArray* meteo);
  //! Initialisation des donnes meteo a partir d'un mxArray.
  void initialiser(const mxArray* meteo, const std::vector<std::string>& champsFonte, 
                   const std::vector<std::string>& champsEvapo, const std::vector<std::string>& champsAutre);
  //! Tri des donnees meteo selon les id des carreaux entiers.
  void trierParId(const std::vector<CarreauEntierPtr>& carreauxEntiers);

private:
  int nbCarreauxEntiers_;
  int nbPasDeTemps_;
  //! Est-ce que la pluie et neige est donne sous forme liquide?
  bool estPtot_;
  //! Vecteur #1: Pas de temps. Vecteur #2: Meteo aux points de grille.
  std::vector<MeteoGrille> valeurs_;
  
  //! Initialisation des donnes meteo a partir d'un mxArray selon un type.
  template <typename Type>
  void initialiser(const mxArray* meteo, const std::vector<std::string>& champsFonte, 
                   const std::vector<std::string>& champsEvapo, const std::vector<std::string>& champsAutre);

  //! Validation de la quantite de donnees meteo en fonction du nombre de jours et CE.
  int validerDonneesMeteo();

};

//! Classe utilitaire pour le tri des carreaux entiers.
/*
  Le tri est fait par ordre des index i et j. 
*/
class TrieurCEGrille
{
public:
  bool operator ()(const CarreauEntierPtr carreauEntier1, const CarreauEntierPtr carreauEntier2);
};

//! Classe utilitaire pour le tri des donnees meteo.
/*
  Le tri est fait par id de carreau entier. 
*/
class TrieurMeteoId
{
public:
  bool operator ()(const MeteoPtr meteo1, const MeteoPtr meteor2);
};
