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
#include "stdafx.h"
#include "ListeStation.h"

ListeStation::ListeStation()
{

}
 
ListeStation::~ListeStation()
{

}

const StationPtr ListeStation::trouverStation(int id) const
{
  Stations::const_iterator iterStation = stations_.begin();

  while (iterStation != stations_.end()) {
    
    if (iterStation->get()->id() == id) {
      break;
    }
    
    iterStation++;
  }

  return *iterStation;
  
}

const Stations& ListeStation::stations() const
{
  return stations_;
}

int ListeStation::quantiteStations() const
{
  return (int)stations_.size();
}

void ListeStation::initialiser()
{
  stations_.push_back(StationPtr(new Station("CDP", "CDP", 0, 27, 28, 911.0f, 399.0f)));
  stations_.push_back(StationPtr(new Station("CD", "CD", 1, 23, 16, 838.0f, 174.0f)));
  stations_.push_back(StationPtr(new Station("MISBI", "MISBI", 2, 22, 22, 870.0f, 244.0f)));
}

void ListeStation::initialiser(const mxArray* stations)
{
  std::string no, nom;
  int iPos, jPos;
  float tp, altitude;
  
  int nbStations = (int)mxGetNumberOfElements(stations);

  for (int i = 0; i < nbStations; i++) {
    MexHelper::chargerValeurs(stations, "id", no, i);
    MexHelper::chargerValeurs(stations, "nom", nom, i);
    MexHelper::chargerValeurs(stations, "i", iPos, i);
    MexHelper::chargerValeurs(stations, "j", jPos, i);
    MexHelper::chargerValeurs(stations, "tp", tp, i);
    MexHelper::chargerValeurs(stations, "altitude", altitude, i);
    stations_.push_back(StationPtr(new Station(no, nom, i, iPos, jPos, tp, altitude)));
  }
}

