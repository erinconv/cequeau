//****************************************************************************
// Fichier:  Interpolation.h
//
// Date creation: 2013-05-13
// Auteur: 
//                Rio Tinto Alcan                     
//                Energie electrique                  
//                1954 Davis, Saguenay arr. Jonquiere,
//                G7S 4R7, QC, Canada                 
//
//****************************************************************************
#pragma once

#include "ListeStation.h"
#include "BassinVersant.h"
#include "ParametresInterpolation.h"
#include "DonneesMeteo.h"

const int NB_TYPE_STATION = 2;
const int IDX_TEMPERATURE = 0;
const int IDX_PRECIPITATION = 1;

//! Donnees necessaires pour interpoler la meteo a chaque CE
class DonneesInterpolation
{
public:
  //! Un carreau entier
  CarreauEntierPtr CE;
  //! Stations attribuees au CE selon la methode d'interpolation
  Stations stations;
  //! Distance des stations par rapport au CE
  std::vector<float> distancesCE;
  //! Facteur de ponderation des stations par rapport au CE
  std::vector<float> facteurPonderationCE;
  //! Valeur calculee selon la methode d'interpolation
  float correction;

  float calculerAltitudeMoyStation();
  float calculerSommePrecipitaionStation();
};

// Liste des donnees necessaires pour interpoler la meteo a chaque CE 
typedef std::vector<DonneesInterpolation> DonneesCarreaux;

//! Classe Interpolation.
class Interpolateur 
{
public:
  //! Constructeur.
  Interpolateur(const BassinVersant& bv, const ListeStation& stations, const Parametres& parametres, 
                const ParametresInterpolation& paramInterpolation, const DonneesMeteo& meteoStations, 
                const std::vector<std::string>& nomsAutresMeteo, int nbStation);
  virtual ~Interpolateur();

  virtual void executer() = 0;
  mxArray* obtenirMeteoInterpolee();

protected:
  // Nombre de station necessaire pour l'interpolation
  const int nbStation_;
  //! Definition du bassin versant.
  const BassinVersant& bassinVersant_;
  //! Liste des donnees necessaires pour interpoler la meteo a chaque CE et pas de temps
  std::vector<std::vector<DonneesCarreaux> > donneesCarreauxTemps_;
  //! Liste des stations meteo.
  const ListeStation& stations_;
  //! Parametres de Cequeau Quantite
  const Parametres& parametres_;
  //! Parametres de l'interpolation
  const ParametresInterpolation& paramInterpolation_;
  //! Donnees meteo aux stations
  const DonneesMeteo& meteoStations_;
  //! Donnees meteo interpolees
  std::vector<MeteoGrille> meteoInterpolee_;
  //! Noms des champs meteo supplementaires
  const std::vector<std::string>& nomsAutresMeteo_;

  void initialiserListeCE();
  void trouverStationPlusPres(int idxTypeMeteo, int pasDeTemps);
  int obtenirNombreAutreMeteo();
  virtual void calculerCorrections(int idxTypeMeteo, int pasDeTemps) = 0;
  virtual void interpoler(int pasDeTemps) = 0;
};
