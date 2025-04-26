//****************************************************************************
// Fichier: Meteo.h
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

//! Definition d'une donnee meteo.
class Meteo
{
public:
  //! Constructeur.
  /*!
    \param tMin Temperature manimale journaliere de l'air en degC
    \param tMax Temperature maximale journaliere de l'air en degC
    \param pluie Precipitation liquide en mm
    \param neige Precipitation solide en mm d'equivalent eau
  */
  Meteo(float tMin, float tMax, float pluie, float neige);
  //! Constructeur.
  /*!
    \param tMin Temperature manimale journaliere de l'air en dixieme de degC
    \param tMax Temperature maximale journaliere de l'air en dixieme de degC
    \param pluie Precipitation liquide en dixieme de mm
    \param neige Precipitation solide en dixieme de mm d'equivalent eau
  */
  Meteo(int tMin, int tMax, int pluie, int neige);
  //! Constructeur par defaut.
  Meteo();
  ~Meteo();

  //! Getter tMin
  float tMin() const;
  //! Getter tMax
  float tMax() const;
  //! Getter pluie
  float pluie() const;
  //! Getter neige
  float neige() const;
  //! Getter idCarreauEntier
  int   idCarreauEntier() const;
  //! Getter donnees meteo specifique au modele de fonte 
  const std::vector<float>& meteoFonte() const;
  //! Getter donnees meteo specifique au modele d'evapotranspiration 
  const std::vector<float>& meteoEvapo() const;
  //! Getter donnees meteo autres (exemple: qualite)
  const std::vector<float>& meteoAutre() const;
  //! Setter idCarreauEntier
  void  idCarreauEntier(int idCarreauEntier);
  //! Setter donnees meteo specifique au modele de fonte 
  void meteoFonte(const std::vector<float>& donneesMeteo);
  //! Setter donnees meteo specifique au modele d'evapotranspiration 
  void meteoEvapo(const std::vector<float>& donneesMeteo);
  //! Setter donnees meteo autres (exemple: qualite)
  void meteoAutre(const std::vector<float>& donneesMeteo);

  //! Obtetion de la temperature moyenne d'une donnee meteo
  float calculerTempMoy() const;

private:
  float tMin_;
  float tMax_;
  float pluie_;
  float neige_;
  int   idCarreauEntier_;
  std::vector<float> meteoFonte_;
  std::vector<float> meteoEvapo_;
  std::vector<float> meteoAutre_;
};
