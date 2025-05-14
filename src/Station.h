//****************************************************************************
// Fichier:  Station.h
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
#include <string>

//! Definition d'une station meteo.
class Station 
{
public:
  //! Constructeur.
  Station(std::string no, std::string nom, int id, int i, int j, float moyAnPrecipitation_, float altitude_);
  ~Station();

  int i() const;
  int j() const;
  int id() const;
  float moyAnPrecipitation() const;
  float altitude() const;

private:
  //! Nom de la station meteo.
  std::string no_;
  //! Nom de la station meteo.
  std::string nom_;
  //! Id interne de la station.
  int id_;
  //! Position i de la station dans le système de coordonnées "I et J" du bassin.
  int i_;
  //! Postion j de la station dans le système de coordonnées "I et J" du bassin.
  int j_;
  //! Precipitation moyenne interannuelle de la station (mm)
  float moyAnPrecipitation_;
  //! Altitude de la station (m)
  float altitude_;
};
