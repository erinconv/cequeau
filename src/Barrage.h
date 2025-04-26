//****************************************************************************
// Fichier:  Barrage.h
//
// Date creation: 2012-11-16
// Auteur: 
//                Rio Tinto Alcan                     
//                Energie electrique                  
//                1954 Davis, Saguenay arr. Jonquiere,
//                G7S 4R7, QC, Canada                 
//
//****************************************************************************
#pragma once 

#include "stdafx.h"

//! Types de barrage. La valeur numerique represente le type selon Cequeau original.
enum TypeBarrage {EVACUATION_MESUREE_INT = 1, EVACUATION_CALCULEE = 2, 
                  EVACUATION_MESUREE_EXT = 3, EVACUATION_CALCULEE_SPECIAL = 5};

//! Definition d'un barrage.
class Barrage 
{
public:
  //! Constructeur barrage type 2.
  Barrage(int idCP, int idCPAval, float volume, const ParamEquation& paramNiveau, const ParamEquation& paramDebit);
  //! Constructeur barrage type 5.
  Barrage(int idCP, int idCPAval, float volume, const ParamEquation& paramNiveau, const ParamEquation& paramVolume, 
          const std::vector<ParamEquation>& paramDebitInt, const std::vector<InfoEvacuationExt>& paramDebitExt);
  //! Constructeur barrage type 3.
  Barrage(int idCPAval, const std::vector<float>& debitsMesures);

  ~Barrage();

  //! Getter idCP.
  int idCP() const;
  //! Getter idCPAval.
  int idCPAval() const;
  //! Getter type barrage
  TypeBarrage typeBarrage() const;
  //! Getter volume.
  float volume() const;
  //! Setter volume.
  void volume(float valeur);
  // Getter debit
  const std::vector<float>& debitsMesures() const;

  //! Calcul de la hauteur selon le volume du barrage.
  float calculerNiveauAvecVolume() const;
  //! Calcul de la hauteur avec un volume.
  float calculerNiveauAvecVolume(float volume) const;
  //! Calcul du debit selon le volume du barrage.
  float calculerDebitAvecVolume() const;
  //! Calcul du debit selon le volume du barrage en specifiant les parametres d'equation.
  float calculerDebitAvecVolume(const ParamEquation& paramDebit) const;
  //! Calcul du debit avec un volume en specifiant les parametres d'equation.
  float calculerDebitAvecVolume(float volume, const ParamEquation& paramDebit) const;
  //! Calcul du volume provenant du barrage sur le CP en aval.
  float calculerVolumeCpAval(float volumeEntrant, float deltaTemps, float volumeInitialCpAval, const DateChrono& datePasDeTemps,
                             std::vector<float>& volumesSortants); 

private:
  //! Type de barrage.
  TypeBarrage typeBarrage_;
  //! Id du carreau partiel sur lequel est situe le barrage.
  int idCP_;
  //! Id du carreau partiel directement en aval du barrage.
  int idCPAval_;
  //! Volume dans le barrage;
  float volume_;

  /*! 
    Contient les parametres de la relation entre le niveau d�eau dans le 
    reservoir (m) et le volume emmagasin� (millions de m3).
  */
  ParamEquation paramNiveau_;

  /*!
    Contient les parametres de la relation entre le debit (m3/s) a evacuer du
    barrage et le volume emmagasine (millions de m3).
  */
  ParamEquation paramDebit_;

  /*!
    Contient les parametres de la relation entre le le volume emmagasin� (millions de m3),
    et la hauteur d'eau dans le reservoir (m).
  */
  ParamEquation paramVolume_;

  /*!
    Contient les parametres de la relation entre le debit (m3/s) a evacuer du
    barrage et le volume emmagasine (millions de m3). 
    X fois le nombre de relations necessaires pour calculer les evacuations
    du reservoir vers au autre carreau � l'interieur du bassin versant.
  */
  std::vector<ParamEquation> paramDebitEvacuationInt_;
  
  /*!
    Contient les parametres de la relation entre le debit (m3/s) a evacuer du
    barrage et le volume emmagasine (millions de m3). 
    X fois le nombre de relations necessaires pour calculer les evacuations
    du reservoir vers un carreau a l'exterieur du bassin versant. Chaque relation
    possede un intervalle de date d'application.
  */
  std::vector<InfoEvacuationExt> paramDebitEvacuationExt_;

  //! Les debits observes.
  std::vector<float> debitsMesures_;

  //! Transformation du resultat de CalculerDebitEvacue en volume.
  float calculerVolumeEvacue(float volumeEntrant, float deltaTemps, const ParamEquation& paramDebit); 
  //! Calcul du debit evacue selon la methode de Goodrich.
  float calculerDebitEvacue(float volumeEntrant, float deltaTemps, const ParamEquation& paramDebit) const;
  float resoudreGoodrich(float volume, float deltaTemps, float constante, const ParamEquation& paramDebit) const;
  //! Trouver pour les evacuations a l'exterieur du bassin versant, les parametres d'equation selon un periode.
  const ParamEquation* const trouverParamDebitExt(const DateChrono& jour) const;
};
