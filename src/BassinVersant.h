//****************************************************************************
// Fichier:  BassinVersant.h
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

#include "CarreauEntier.h" 
#include "CarreauPartiel.h"
#include "Barrage.h"
#include "Puits.h"

//! "Smart pointer" de carreau entier. 
typedef std::shared_ptr<CarreauEntier> CarreauEntierPtr;
//! "Smart pointer" de carreau partiel. 
typedef std::shared_ptr<CarreauPartiel> CarreauPartielPtr;
//! "Smart pointer" de barrage. 
typedef std::shared_ptr<Barrage> BarragePtr;
//! "Smart pointer" de puits
typedef std::shared_ptr<Puits> PuitsPtr;

//! Definition du bassin versant.
/*! 
 *  Definition du bassin versant.
 *  NOTE: Il s'agit de la representation
 *  des donnees physiographiques PREPAREES.
 */
class BassinVersant
{
public:
  //! Constructeur.
  BassinVersant();
  ~BassinVersant();

  //! Initialisation du bassin versant a partir d'un fichier Matlab.
  void initialiserFichier(const std::string nomFichierintrants);
  //! Initialisation du bassin versant a partir d'un variable Matlab (mxArray)
  void initialiser(const mxArray* bassinVersant);

  //! Getter nomBassinVersant
  std::string nomBassinVersant() const;
  //! Getter nbCarreauxEntiers
  int nbCarreauxEntiers() const; 
  //! Getter nbCarreauxPartiels
  int nbCarreauxPartiels() const;
  //! Getter nbCarreauxPartielsCheminLong
  int nbCarreauxPartielsCheminLong() const;
  //! Getter nbBarrages
  int nbBarrages() const;
  //! Getter superficieCarreauEntier
  float superficieCarreauEntier() const;
  //! Getter liste des carreaux entiers
  const std::vector<CarreauEntierPtr>& carreauxEntiers() const;
  //! Getter liste des carreaux partiels
  const std::vector<CarreauPartielPtr>& carreauxPartiels() const;
  //! Getter barrage
  std::vector<BarragePtr>& barrages();
  //! Initialisation barrage type 2
  void initialiserBarrage(int idCP, int idCPAval, float volumeInitial, ParamEquation& coeffNiveau, ParamEquation& coeffDebit);
  //! Initialisation barrage type 5
  void initialiserBarrage(int idCP, int idCPAval, float volumeInitial, ParamEquation& coeffNiveau, ParamEquation& coeffVolume,
                          std::vector<ParamEquation>& coeffDebitInt, std::vector<InfoEvacuationExt>& coeffDebitExt);
  //! Initialisation barrage type 3
  void initialiserBarrage(int idCPAval, std::vector<float>& debitsExternesMesures);
  //! Le bassin possede des barrages?
  bool possedeBarrage() const;
  //! Le CP possede un barrage?
  bool possedeBarrage(int idCP) const;
  //! Recherche d'un barrage ayant un CP particuler en aval
  const BarragePtr trouverBarrageParIdCpAval(int idCP) const;
  //! Recherche d'un barrage situe sur ce CP
  const BarragePtr trouverBarrageParIdCP(int idCP) const;

  //! Obtention d'un carreau entier selon son id.
  const CarreauEntierPtr trouverCarreauEntierParId(int id) const;

  //! Getter liste des puits
  const std::map<int, std::vector<PuitsPtr>>& puits() const;

  const std::vector<PuitsPtr>& puitsParIdCE(int idCE) const;

private:
  int nbCarreauxEntiers_; 
  int nbCarreauxPartiels_;
  std::string nomBassinVersant_;
  /*!
   *  Nombre de carreaux partiels sur le chemin le plus long,
   *  c'est-a-dire de l'exutoire a la partie la plus eloignee du
   *  bassin versant
   */
  int nbCarreauxPartielsCheminLong_;
  //! Superficie des carreaux entier (km**2)
  float superficieCarreauEntier_;
  
  //! Liste des carreaux entiers du bassin versant.
  std::vector<CarreauEntierPtr> carreauxEntiers_;
  //! Liste des carreaux partiels du bassin versant.
  std::vector<CarreauPartielPtr> carreauxPartiels_;
  //! Liste des barrages du bassin versant.
  std::vector<BarragePtr> barrages_;
  //! Liste des CP ayant un barrage.
  std::map<int, bool> listeCpBarrage_;
  //! Liste des puits
  std::map<int, std::vector<PuitsPtr>> puits_;
  
  //! Initialisation des carreaux entiers a partir d'une structure Matlab.
  void initialiserCarreauxEntiers(const mxArray* bassinVersant);
  //! Initialisation des carreaux partiels a partir d'une structure Matlab.
  void initialiserCarreauxPartiels(const mxArray* bassinVersant);
};

