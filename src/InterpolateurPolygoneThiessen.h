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

#include "Interpolateur.h"

class InterpolateurPolygoneThiessen : public Interpolateur 
{
public:
  InterpolateurPolygoneThiessen(const BassinVersant& bv, const ListeStation& stations, const Parametres& parametres,
                                const ParametresInterpolation& paramInterpolation, const DonneesMeteo& meteoStations,
                                const std::vector<std::string>& nomsAutresMeteo);
  ~InterpolateurPolygoneThiessen();

  void executer();

private:
  void calculerCorrections(int idxTypeMeteo, int pasDeTemps);
  void interpoler(int pasDeTemps);
};

