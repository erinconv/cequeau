//****************************************************************************
// Fichier:  AssimilationHelper.hpp
//
// Date creation: 2013-04-04
// Auteur: 
//                Rio Tinto Alcan                     
//                Energie electrique                  
//                1954 Davis, Saguenay arr. Jonquiere,
//                G7S 4R7, QC, Canada                 
//
//****************************************************************************
#pragma once

enum TypeAssim {AUCUNE, VALEUR, FACTEURS};
const float ASSIMILATION_NO_DATA = -1.0f;

//! Fonctions utilitaires pour l'assimilation.
namespace AssimilationHelper {
  //! Remplacement d'une valeur par une donnee d'assimilation
  void assimilerValeur(const float valeurAssim[2], TypeAssim typeAssim, float& valeur);
  //! Trouver le type d'assimilation pour une variable d'assimilation
  TypeAssim obtenirTypeAssim(const mxArray* varAssimilation);
  //! Trouver les valeurs d'assimilation pour une variable d'assimilation
  void obtenirValeursAssim(const double* valeursAssim, TypeAssim typeAssim, size_t indexCol, float valeurs[2]);

  //------------------------------------------------------------------
  //                      Definitions
  //------------------------------------------------------------------
  
  //------------------------------------------------------------------
  // assimilerValeur(assimilationsIter->temperature, assimilationsIter->temperatureType, iterCP->temperature);
  //------------------------------------------------------------------
  inline void assimilerValeur(const float valeurAssim[2], TypeAssim typeAssim, float& valeur)
  {
    switch(typeAssim) {
    case VALEUR:
      if (valeurAssim[0] != ASSIMILATION_NO_DATA) {
          valeur = valeurAssim[0];
      }
      break;
    case FACTEURS:
      if (valeurAssim[0] != ASSIMILATION_NO_DATA && valeurAssim[1] != ASSIMILATION_NO_DATA) {
        valeur = valeur * valeurAssim[0] + valeurAssim[1];
      }
      break;
    default:
      break;
    }
  }

  //------------------------------------------------------------------
  inline TypeAssim obtenirTypeAssim(const mxArray* varAssimilation)
  {
    FILE_LOG(logDEBUG) << "Simulation::obtenirTypeAssim(const mxArray* varAssimilation)";

    TypeAssim typeAssim;
  
    // Aucune ligne = pas de donnees a assimiler
    // Une ligne = Assimilation par valeurs
    // Deux lignes = Ligne #1 facteurs multiplicatifs
    //               Ligne #2 Valeur a additionner
    switch ((int)mxGetM(varAssimilation)) {
    case 0:
      typeAssim = AUCUNE;
      break;
    case 1:
      typeAssim = VALEUR;
      break;
    case 2:
      typeAssim = FACTEURS;
      break;
    default:
      typeAssim = AUCUNE;
      break;
    }

    return typeAssim;
  }

  //------------------------------------------------------------------
  inline void obtenirValeursAssim(const double* valeursAssim, TypeAssim typeAssim, size_t indexCol, float valeurs[2])
  {
    FILE_LOG(logDEBUG) << "Simulation::obtenirValeursAssim(...)";
    float floatTmp;
    size_t index;

    switch(typeAssim) {
    case AUCUNE:
      valeurs[0] = ASSIMILATION_NO_DATA;
      valeurs[1] = ASSIMILATION_NO_DATA;
      break;
    case VALEUR:
      floatTmp = (valeursAssim == NULL ? ASSIMILATION_NO_DATA : (float)valeursAssim[indexCol]);
      valeurs[0] = ((std::isnan(floatTmp) ? ASSIMILATION_NO_DATA : floatTmp));
      valeurs[1] = ASSIMILATION_NO_DATA;
      break;
    case FACTEURS:
      // Les matrice Matlab sont ordonnees col / row. index = noLigne + noCol * nbLignes
      index = indexCol * 2;
      floatTmp = (valeursAssim == NULL ? ASSIMILATION_NO_DATA : (float)valeursAssim[index]);
      valeurs[0] = ((std::isnan(floatTmp) ? ASSIMILATION_NO_DATA : floatTmp));
  
      if (valeurs[0] != ASSIMILATION_NO_DATA) {
        index = 1 + indexCol * 2;
        floatTmp = (float)valeursAssim[index];
        valeurs[1] = ((std::isnan(floatTmp) ? ASSIMILATION_NO_DATA : floatTmp));
      }
      else {
        valeurs[1] = ASSIMILATION_NO_DATA;
      }
      break;
    default:
      valeurs[0] = ASSIMILATION_NO_DATA;
      valeurs[1] = ASSIMILATION_NO_DATA;
      break;
    }
  }


};
