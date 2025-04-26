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
#include "Station.h"
//#include <boost/shared_ptr.hpp>
#include <vector>

//! "Smart pointer" de station. 
typedef std::shared_ptr<Station> StationPtr;
typedef std::vector<StationPtr> Stations;

//! Definition d'une liste de stations meteo.
class ListeStation 
{
public:
  //! Constructeur.
  ListeStation();
  ~ListeStation();

  const Stations& stations() const;
  
  const StationPtr trouverStation(int id) const;

  void initialiser();

  void initialiser(const mxArray* stations);

  int quantiteStations() const;

private:
  Stations stations_;
};

