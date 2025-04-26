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
#include "Interpolateur.h"
//#include <boost/math/special_functions/fpclassify.hpp> // Pour std::isnan

//------------------------------------------------------------------
float DonneesInterpolation::calculerAltitudeMoyStation()
{
  int nbStation = (int)stations.size();
  Stations::const_iterator iterStation = stations.begin();
  float moyenne = 0.0f;

  while (iterStation != stations.end()) {
    moyenne += iterStation->get()->altitude();
    iterStation++;
  }

  moyenne /= (float)nbStation;
  return moyenne;
}

//------------------------------------------------------------------
float DonneesInterpolation::calculerSommePrecipitaionStation()
{
  Stations::const_iterator iterStation = stations.begin();
  float somme = 0.0f;

  while (iterStation != stations.end()) {
    somme += iterStation->get()->moyAnPrecipitation();
    iterStation++;
  }

  return somme;
}

//------------------------------------------------------------------
Interpolateur::Interpolateur(const BassinVersant& bv, const ListeStation& stations, const Parametres& parametres, 
                             const ParametresInterpolation& paramInterpolation, const DonneesMeteo& meteoStations, 
                             const std::vector<std::string>& nomsAutresMeteo, int nbStation)
    : bassinVersant_(bv), stations_(stations), parametres_(parametres),
      paramInterpolation_(paramInterpolation), meteoStations_(meteoStations), nomsAutresMeteo_(nomsAutresMeteo) ,nbStation_(nbStation)
{
}

//------------------------------------------------------------------
Interpolateur::~Interpolateur() 
{
}

//------------------------------------------------------------------
void Interpolateur::initialiserListeCE()
{
  std::vector<CarreauEntierPtr>::const_iterator iterCE = bassinVersant_.carreauxEntiers().begin();
  DonneesCarreaux donneesCarreaux;

  while (iterCE != bassinVersant_.carreauxEntiers().end()) {
    DonneesInterpolation donneesInterpolation;
    donneesInterpolation.CE = *iterCE;
    donneesCarreaux.push_back(donneesInterpolation);

    if (iterCE->get()->altitude() < 0.0f) {
      // L'altitude ne semble pas avoir ete initialisee
      throw std::runtime_error("Une altitude de carreau entier est negative");
    }

    iterCE++;
  }

  // Insertion pour chaque pas de temps 
  std::vector<DonneesCarreaux> donneesCarreauxTemps;
  int nbPasDeTemps = meteoStations_.nbPasDeTemps();
  for (int i = 0; i < nbPasDeTemps; i++) {
    donneesCarreauxTemps.push_back(donneesCarreaux);
  }

  donneesCarreauxTemps_.push_back(donneesCarreauxTemps);
  donneesCarreauxTemps_.push_back(donneesCarreauxTemps);

  int nbAutreMeteo = obtenirNombreAutreMeteo();
  for (int i = 0; i < nbAutreMeteo; i++) {
    donneesCarreauxTemps_.push_back(donneesCarreauxTemps);
  }
}

//------------------------------------------------------------------
void Interpolateur::trouverStationPlusPres(int idxTypeMeteo, int pasDeTemps)
{

  const unsigned long DIST_INI = ULONG_MAX - 2; 
  const unsigned long DIST_SEUIL = DIST_INI  + 1;
  const unsigned long DIST_EXCLU = DIST_INI  + 2;

  int xCE, yCE, xStation, yStation, distance, idMin;
  unsigned long distanceMin;
  std::vector<unsigned long> distanceStation; 
  float distanceReelle; 
  StationPtr station;
  
  const int quantiteStations = stations_.quantiteStations();
  const int nbCE = (int)donneesCarreauxTemps_.back().back().size();

  // Distance des stations selon le type de mesure
  for (int ce = 0; ce < nbCE; ce++) {
    xCE = donneesCarreauxTemps_[idxTypeMeteo][pasDeTemps][ce].CE->i();
    yCE = donneesCarreauxTemps_[idxTypeMeteo][pasDeTemps][ce].CE->j();

    distanceStation.assign(quantiteStations, DIST_INI);

    // Distance de chaque station par rapport au CE
    for (int i = 0; i < quantiteStations; i++) {
      station = stations_.stations()[i];
      xStation = station->i();
      yStation = station->j();
      // Distance au carre
      distance = (xCE - xStation) * (xCE - xStation) + (yCE - yStation) * (yCE - yStation);
        
      // Y'a-t-il des donnes de temperature pour cette station et ce pas de temps
      // Si non, on ne considere pas cette station
      switch (idxTypeMeteo) {
      case IDX_TEMPERATURE:
        if(!std::isnan(meteoStations_.valeurs()[pasDeTemps][i].get()->tMin()) &&
           !std::isnan(meteoStations_.valeurs()[pasDeTemps][i].get()->tMax())) {
          distanceStation[i] = distance;
        }
        break;
      // Y'a-t-il des donnes de precipitation pour cette station et ce pas de temps
      // Si non, on ne considere pas cette station
      case  IDX_PRECIPITATION: 
        if(!std::isnan(meteoStations_.valeurs()[pasDeTemps][i].get()->pluie()) &&
           !std::isnan(meteoStations_.valeurs()[pasDeTemps][i].get()->neige())) {
          distanceStation[i] = distance;
        }
        break;
      // idxTypeMeteo-2: Mois temperatures et precipitations
      default:
        if (!std::isnan(meteoStations_.valeurs()[pasDeTemps][i].get()->meteoAutre()[idxTypeMeteo-2])) {
        distanceStation[i] = distance;
        }
        break;
      }
    }

    // nbStation_ = nombre de stations le plus pres qu'on veut trouver 
    // Note: Dans le cas ou l'on aurait moins de nbStation, 1 ou des stations avec NaN
    // seront selectionnees mais avec tellement une grande distance qu'apres ponderation 
    // leur effet sera negligeable. 
    for (int j = 0; j < nbStation_; j++) {
      distanceMin = DIST_SEUIL;

      for (int k = 0; k < quantiteStations; k++) {
        if (distanceStation[k] < distanceMin) {
          distanceMin = distanceStation[k];
          idMin = k;
        }
      }

      // Distance reelle 
      distanceReelle = (distanceMin == 0 ? 0.25f : (float)distanceMin);
      distanceReelle = std::sqrt(distanceReelle * bassinVersant_.superficieCarreauEntier());
 
      station = stations_.trouverStation(idMin);
      donneesCarreauxTemps_[idxTypeMeteo][pasDeTemps][ce].stations.push_back(station);
      donneesCarreauxTemps_[idxTypeMeteo][pasDeTemps][ce].distancesCE.push_back(distanceReelle);
      // Pour ne pas selectionner a nouveau cette station.
      distanceStation[idMin] = DIST_EXCLU;
    }
  }
}

//------------------------------------------------------------------
int Interpolateur::obtenirNombreAutreMeteo() {
  return (int)nomsAutresMeteo_.size();
}


//------------------------------------------------------------------
mxArray* Interpolateur::obtenirMeteoInterpolee() 
{
  const int nbPasDeTemps = (int)meteoInterpolee_.size();
  const int nbCE = (int)meteoInterpolee_.back().size();
  int nbAutreMeteo = obtenirNombreAutreMeteo();

  mxArray* tMin = mxCreateDoubleMatrix(nbPasDeTemps, nbCE, mxREAL);
  mxArray* tMax = mxCreateDoubleMatrix(nbPasDeTemps, nbCE, mxREAL);
  mxArray* pluie = mxCreateDoubleMatrix(nbPasDeTemps, nbCE, mxREAL);
  mxArray* neige = mxCreateDoubleMatrix(nbPasDeTemps, nbCE, mxREAL);

  std::vector<mxArray**> donnesAutres;
  // Allocation de la memoire pour chaque champ. 
  for (int idxAutreMeteo = 0; idxAutreMeteo < nbAutreMeteo; idxAutreMeteo++) {
    donnesAutres.push_back((mxArray**)mxMalloc(nbPasDeTemps * sizeof(mxArray*)));
    donnesAutres.back()[0] = mxCreateDoubleMatrix(nbPasDeTemps, nbCE, mxREAL);
  }

  double tMinData, tMaxData, pluieData, neigeData;
  std::vector<double> autreData;
  int index;
  for (int idxPasDeTemps = 0; idxPasDeTemps < nbPasDeTemps; idxPasDeTemps++) {
    for (int idxCE = 0; idxCE < nbCE; idxCE++) {
      tMinData = meteoInterpolee_[idxPasDeTemps][idxCE]->tMin();
      tMaxData = meteoInterpolee_[idxPasDeTemps][idxCE]->tMax();
      pluieData = meteoInterpolee_[idxPasDeTemps][idxCE]->pluie();
      neigeData = meteoInterpolee_[idxPasDeTemps][idxCE]->neige();
      autreData.clear();
      for (int idxAutreMeteo = 0; idxAutreMeteo < nbAutreMeteo; idxAutreMeteo++) {
        autreData.push_back(meteoInterpolee_[idxPasDeTemps][idxCE]->meteoAutre()[idxAutreMeteo]);
      }
      
      // Le stockage interne d'un mxArray est colonne/ligne
      index = idxPasDeTemps + idxCE * nbPasDeTemps;
      memcpy((void*)(mxGetPr(tMin) + index), (void*)&tMinData , sizeof(double));
      memcpy((void*)(mxGetPr(tMax) + index), (void*)&tMaxData , sizeof(double));
      memcpy((void*)(mxGetPr(pluie) + index), (void*)&pluieData , sizeof(double));
      memcpy((void*)(mxGetPr(neige) + index), (void*)&neigeData , sizeof(double));
      for (int i = 0; i < nbAutreMeteo; i++) {
        memcpy((void*)(mxGetPr(donnesAutres.at(i)[0]) + index), (void*)&autreData[i] , sizeof(double));
      }
    }
  }

  int nbMeteoBase = 4; // tMin, tMax, pluie, neige
  if (meteoStations_.estPtot()) {
    nbMeteoBase = 3; // tMin, tMax, pTot
  }
  // Assignation des nom de champs
  const char **nomChamps = new const char*[nbMeteoBase + nbAutreMeteo]; 
  nomChamps[0] = "tMin";
  nomChamps[1] = "tMax";
  if (meteoStations_.estPtot()) {
    nomChamps[2] = "pTot";
  }
  else {
    nomChamps[2] = "pluie";
    nomChamps[3] = "neige";
  }

  // Assignation des autres noms de champs 
  std::vector<std::string>::const_iterator champsIter;
  int cntChamp = nbMeteoBase;
  for (champsIter = nomsAutresMeteo_.begin(); champsIter != nomsAutresMeteo_.end(); champsIter++) {
    nomChamps[cntChamp] = (*champsIter).c_str();
    cntChamp++;
  }

  // allocation de la memoire pour la structure de sortie
  int nbChamps = nbMeteoBase + nbAutreMeteo;
  mxArray* meteo = mxCreateStructMatrix(1, 1, nbChamps, nomChamps);

  // Assignation des listes de donnees a la structure de sortie
  mxSetField(meteo, 0, "tMin", tMin);
  mxSetField(meteo, 0, "tMax", tMax);
  if (meteoStations_.estPtot()) {
    mxSetField(meteo, 0, "pTot", pluie);
  }
  else {
    mxSetField(meteo, 0, "pluie", pluie);
    mxSetField(meteo, 0, "neige", neige);
  }

  if (nbAutreMeteo > 0) {
    cntChamp = 0;
    for (champsIter = nomsAutresMeteo_.begin(); champsIter != nomsAutresMeteo_.end(); champsIter++) {
      mxSetField(meteo, 0, (*champsIter).c_str(), donnesAutres.at(cntChamp)[0]);
      cntChamp++;
    }
  }

  return meteo;
}


