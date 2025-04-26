//****************************************************************************
// Fichier:  Station.cpp
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
#include "InterpolateurPolygoneThiessen.h"

//------------------------------------------------------------------
InterpolateurPolygoneThiessen::InterpolateurPolygoneThiessen(const BassinVersant& bv, const ListeStation& stations, const Parametres& parametres,
                               const ParametresInterpolation& paramInterpolation, const DonneesMeteo& meteoStations, 
                               const std::vector<std::string>& nomsAutresMeteo)
  : Interpolateur(bv, stations, parametres, paramInterpolation, meteoStations, nomsAutresMeteo, 1)
{
}

//------------------------------------------------------------------
InterpolateurPolygoneThiessen::~InterpolateurPolygoneThiessen()
{
}

//------------------------------------------------------------------
void InterpolateurPolygoneThiessen::executer()
{
  initialiserListeCE();
  int nbPasDeTemps = meteoStations_.nbPasDeTemps();

  int nbAutreMeteo = obtenirNombreAutreMeteo();
  for (int pasDeTemps = 0; pasDeTemps < nbPasDeTemps; pasDeTemps++) {
    for (int idxMeteo = 0; idxMeteo < nbAutreMeteo + 2; idxMeteo++) {
      trouverStationPlusPres(idxMeteo, pasDeTemps);
      calculerCorrections(idxMeteo, pasDeTemps);
    }
    interpoler(pasDeTemps);
  }
}

//------------------------------------------------------------------
void InterpolateurPolygoneThiessen::calculerCorrections(int idxTypeMeteo, int pasDeTemps)
{
  DonneesCarreaux::iterator iterCE;
  float correction, diffAltitude;

  iterCE = donneesCarreauxTemps_[idxTypeMeteo][pasDeTemps].begin();
  while(iterCE != donneesCarreauxTemps_[idxTypeMeteo][pasDeTemps].end()) {
    diffAltitude = (iterCE->CE->altitude() - iterCE->stations[0]->altitude());
    
    if (idxTypeMeteo == IDX_TEMPERATURE) {
      correction = diffAltitude * paramInterpolation_.coet() / 1000.0f;
    }
    else if (idxTypeMeteo == IDX_PRECIPITATION) {
      correction = 1.0f + diffAltitude * paramInterpolation_.coep() / 
                            iterCE->stations[0]->moyAnPrecipitation();
    }
    else {
      correction = 0.0f;
    }

    iterCE->correction = correction;

    iterCE++;
  }
}

//------------------------------------------------------------------
void InterpolateurPolygoneThiessen::interpoler(int pasDeTemps)
{
  int nbCE = bassinVersant_.nbCarreauxEntiers(), idStation;
  int nbAutreMeteo = obtenirNombreAutreMeteo();
  float tMin, tMax, pluie, neige, autre;
  std::vector<float> meteoAutre;

  MeteoGrille meteoGrille;
  meteoGrille.reserve(nbCE);
  DonneesInterpolation* donneesInterpolation;
  Meteo* meteo;
  MeteoPtr meteoPtr;
  for (int idxCE = 0; idxCE < nbCE; idxCE++) {
    tMin = tMax = pluie = neige = 0.0f;
    meteoAutre.clear();

    for (int idxMeteo = 0; idxMeteo < nbAutreMeteo + 2; idxMeteo++) {
      donneesInterpolation = &(donneesCarreauxTemps_[idxMeteo][pasDeTemps][idxCE]);
      idStation = donneesInterpolation->stations[0].get()->id();
      meteo = meteoStations_.valeurs()[pasDeTemps][idStation].get();
      autre = 0.0f;
      
      if (idxMeteo == IDX_TEMPERATURE) {
        tMin = meteo->tMin();
        tMax = meteo->tMax();
        tMin += donneesInterpolation->correction;
        tMax += donneesInterpolation->correction;
      } 
      else if (idxMeteo == IDX_PRECIPITATION) {
        pluie = meteo->pluie();
        neige = meteo->neige();
        pluie *= donneesInterpolation->correction;
        neige *= donneesInterpolation->correction;
      }
      else if (nbAutreMeteo > 0 && idxMeteo > IDX_PRECIPITATION) {
        autre = meteo->meteoAutre()[idxMeteo-2];
        autre += donneesInterpolation->correction;
        meteoAutre.push_back(autre);
      }
    }
    meteoPtr = MeteoPtr(new Meteo(tMin, tMax, pluie, neige));
    meteoPtr->meteoAutre(meteoAutre);

    meteoGrille.push_back(meteoPtr);
  }

  meteoInterpolee_.push_back(meteoGrille);
}
