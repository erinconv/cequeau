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
#include "InterpolateurPonderationStations.h"

//------------------------------------------------------------------
InterpolateurPonderationStations::InterpolateurPonderationStations(const BassinVersant& bv, const ListeStation& stations, const Parametres& parametres,
                                  const ParametresInterpolation& paramInterpolation, const DonneesMeteo& meteoStations,
                                  const std::vector<std::string>& nomsAutresMeteo)
  : Interpolateur(bv, stations, parametres, paramInterpolation, meteoStations, nomsAutresMeteo, 3)
{
}

//------------------------------------------------------------------
InterpolateurPonderationStations::~InterpolateurPonderationStations()
{
}

//------------------------------------------------------------------
void InterpolateurPonderationStations::executer()
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
void InterpolateurPonderationStations::calculerCorrections(int idxTypeMeteo, int pasDeTemps)
{
  float correction, diffAltitude, varTravail;
  float distanceStn1, distanceStn2, distanceStn3;
  DonneesCarreaux::iterator iterCE;
  
  iterCE = donneesCarreauxTemps_[idxTypeMeteo][pasDeTemps].begin();
  while(iterCE != donneesCarreauxTemps_[idxTypeMeteo][pasDeTemps].end()) {
    diffAltitude = (iterCE->CE->altitude() - iterCE->calculerAltitudeMoyStation());
    
    if (idxTypeMeteo == IDX_TEMPERATURE) {
      correction = diffAltitude * paramInterpolation_.coet() / 1000.0f;
    }
    else if (idxTypeMeteo == IDX_PRECIPITATION) {
      correction = 1.0f + diffAltitude * paramInterpolation_.coep() * nbStation_ / 
                           iterCE->calculerSommePrecipitaionStation();
    }
    else  {
      correction = 0.0f;
    }

    iterCE->correction = correction;

    // Calcul des facteurs de ponderation de chaque station
    distanceStn1 = iterCE->distancesCE[0];
    distanceStn2 = iterCE->distancesCE[1];
    distanceStn3 = iterCE->distancesCE[2];
    varTravail = 1.0f / (1.0f / distanceStn1 + 1.0f / distanceStn2 + 1.0f / distanceStn3);

    iterCE->facteurPonderationCE.push_back(varTravail / distanceStn1);
    iterCE->facteurPonderationCE.push_back(varTravail / distanceStn2);
    iterCE->facteurPonderationCE.push_back(varTravail / distanceStn3);

    iterCE++;
  }
}

//------------------------------------------------------------------
void InterpolateurPonderationStations::interpoler(int pasDeTemps)
{
 
  // Boucle sur les pas de temps de la meteo des stations
  int nbCE = bassinVersant_.nbCarreauxEntiers(), idStation;
  int nbAutreMeteo = obtenirNombreAutreMeteo();
  float tMin, tMax, pluie, neige, autre, ponderationStation;
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
      autre = 0.0f;

      ponderationStation = donneesInterpolation->facteurPonderationCE[0];
      idStation = donneesInterpolation->stations[0].get()->id();
      meteo = meteoStations_.valeurs()[pasDeTemps][idStation].get();
        
      if (idxMeteo == IDX_TEMPERATURE) {
        tMin += ponderationStation * meteo->tMin();
        tMax += ponderationStation * meteo->tMax();
      }
      else if (idxMeteo == IDX_PRECIPITATION) {
        pluie += ponderationStation * meteo->pluie();
        neige += ponderationStation * meteo->neige();
      }
      else if (nbAutreMeteo > 0 && idxMeteo > IDX_PRECIPITATION) {
        autre += ponderationStation * meteo->meteoAutre()[idxMeteo-2];
      }


      for (int idxStation = 1; idxStation < 3; idxStation++) {
        
        ponderationStation = donneesInterpolation->facteurPonderationCE[idxStation];
        idStation = donneesInterpolation->stations[idxStation].get()->id();
        meteo = meteoStations_.valeurs()[pasDeTemps][idStation].get();
        
        if (idxMeteo == IDX_TEMPERATURE) {
          tMin += ponderationStation * (std::isnan(meteo->tMin()) ? 0.0f : meteo->tMin());
          tMax += ponderationStation * (std::isnan(meteo->tMax()) ? 0.0f : meteo->tMax());
        }
        else if (idxMeteo == IDX_PRECIPITATION) {
          pluie += ponderationStation * (std::isnan(meteo->pluie()) ? 0.0f : meteo->pluie());
          neige += ponderationStation * (std::isnan(meteo->neige()) ? 0.0f : meteo->neige());
        }
        else if (nbAutreMeteo > 0 && idxMeteo > IDX_PRECIPITATION) {
          autre += ponderationStation * (std::isnan(meteo->meteoAutre()[idxMeteo-2]) ? 0.0f : meteo->meteoAutre()[idxMeteo-2]);
        }
      }

      if (idxMeteo == IDX_TEMPERATURE) {
        tMin += donneesInterpolation->correction;
        tMax += donneesInterpolation->correction;
      }
      else if (idxMeteo == IDX_PRECIPITATION) {
        pluie *= donneesInterpolation->correction;
        neige *= donneesInterpolation->correction;
      }
      else if (nbAutreMeteo > 0 && idxMeteo > IDX_PRECIPITATION) {
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
