//****************************************************************************
// Fichier: CarreauEntier.h
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
#include "Parametres.h" 

//! Definition d'un carreau entier.
/*! 
 *  Definition d'un carreau entier
 *  selon les donnees physiographiques PREPAREES.
 */
class CarreauEntier
{
public:
  //! Constructeur
  CarreauEntier(int id, int pctLacRiviere, int pctForet, int pctMarais, int pctLacExutoire);
  //! Constructeur
  CarreauEntier(int id, int i, int j);
  //! Constructeur
  CarreauEntier(int id, int i, int j, int pctLacRiviere, int pctForet, int pctMarais, int pctLacExutoire, float altitude);
  //! Constructeur
  CarreauEntier(int id, int i, int j, int pctLacRiviere, int pctForet, int pctMarais, int pctLacExutoire, float altitude, float latitude, float longitude);
  //! Constructeur
  CarreauEntier(int id, int i, int j, int pctLacRiviere, int pctForet, int pctMarais, int pctLacExutoire, float altitude, float latitude, float longitude, float meanSlope);
  ~CarreauEntier();
  
  //! Getter id
  int id() const;
  //! Getter pctLacRiviere
  float pctLacRiviere() const;
  //! Getter pctForet
  float pctForet() const;
  //! Getter pctMarais
  float pctMarais() const;
  //! Getter pctLacExutoire
  float pctLacExutoire() const;
  //! Setter pctLacRiviere
  void pctLacRiviere(int pctLacRiviere);
  //! Setter pctForet
  void pctForet(int pctForet);
  //! Setter pctMarais
  void pctMarais(int pctMarais);
  //! Setter pctLacExutoire
  void pctLacExutoire(int pctLacExutoire) ;
  //! Getter i (abscise) du carreau sur la grille
  int i() const;
  //! Getter j (ordonnee) du carreau sur la grille
  int j() const;
  //! Setter i
  void i(int val);
  //! Setter j
  void j(int val);

  //! Getter/Setter parametres spatialisables
  ParamCE& param();
  //! Getter parametres spatialisables
  const ParamCE& param() const;

  //! Obtention du poucentage d'eau sur le carreau entier.
  float pctEau() const;
  //! Obtention du pourcentage de sol sur le carreau entier.
  float pctSol() const;
  //! Coefficient de ponderation.
  /*!
    Coefficient de ponderation utilise dans les calculs
    d'evapotranspiration et d'infiltration.
    varie de 0.8 si le carreau entier est completement deboise, 
    a 1.0 si la foret recouvre totalement le carreau.
  */
  float calculerCoeffPonderation() const;

  // L'altitude est utilisee seulement par le module d'interpolation.
  // ! Getter altitude
  float altitude() const;
  //! Setter altitude
  void altitude(float val);
  //! Getter latitude 
  float latitude() const;
  //! Getter longitude
  float longitude() const;
    //! Getter meanSlope
  float meanSlope() const;

private:
  //! Id du carreau entier. 1 a nbCarreauxEntiers du bassin versant.
  int id_;
  int i_;
  int j_;
  int pctLacRiviere_;
  int pctForet_;
  int pctMarais_;
  int pctLacExutoire_;
  //! Parametres spatialisables
  ParamCE param_;
  //! Utilise par le module d'interpolation
  float altitude_;
  float latitude_;
  float longitude_;
  float meanSlope_;
};

