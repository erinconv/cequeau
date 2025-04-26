//****************************************************************************
// Fichier: FonteCequeau.h
//
// Date creation: 2014-03-11
// Auteur: 
//                Rio Tinto Alcan                     
//                Energie electrique                  
//                1954 Davis, Saguenay arr. Jonquiere,
//                G7S 4R7, QC, Canada                 
//
//****************************************************************************
#pragma once

#include "Fonte.h"

 //! Definition d'une classe de calcul de la fonte
class FonteCequeau: public Fonte
{
public:
  // TODO: Releves neige
  friend class Simulation;

  //! Constructeur.
  FonteCequeau();
  FonteCequeau(int latitudeMoyenneBV, int nbCE);
  ~FonteCequeau();

  int calculerFonte(
  // IN
  const DateChrono& datePasDeTemps,
  const Meteo& meteo,
  const CarreauEntier& carreauEntier,
  // OUT
  float& precipationsTotales,
  float& eauDisponible
  );

  int assimiler(const DateChrono& datePasDeTemps);

  int initialiserAssimilations(const mxArray* assimilations);

  void initialiserEtats(const mxArray* etatsInitiaux);

  void lireParametres(const mxArray* paramSimul);

  mxArray* obtenirEtats(const std::vector<bool> filtreCE);

  mxArray* obtenirEtatsAvantAssimilations(const std::vector<bool> filtreCE);

  void preserverEtatsPasDeTemps();

  // Etat de la fonte pour un carreau entier
  // Est publique uniquement pour les besoins de Cequeau Qualite.
  // TODO: Devrait etre prive. A voir selon les autres modeles de fonte si besoin.
  class EtatFonteCE
  {
    public:
      float SNC_stockNeigeForet;
      float SND_stockNeigeClairiere;
      float QNUI3_indexMurissementNeige;
      float QNUI4_indexTempNeige;
      float eauDisponible;
  };

  // Pour le calcul de la temperature de l'eau de Cequeau qualite
  // TODO: Generaliser selon les autres modeles de fonte, si besoin.
  const std::vector<std::vector<EtatFonteCE> >& etatsFonte() const {return etatsFonte_;};

private:
  //! Constante de modulation solaire (TAPHI).
  /*!
    Initialisee par le constructeur. Varie en fonction de la latitude
    moyenne du basin versant.
  */
  float constModulationSoleil_;

  //! Parametres necessaires au module de fonte    
  class Params
  {
   public:
    //! Parametre qui permet de decaler la date d'insolation maximale pour le
    //! calcul de la fonte de la neige. 
    //! Si 80, la durï¿½e d'ensoleillement potentiel est maximale le 21 juin.
    // JONEI
    int jourSoleilMaxFonteNeige;
    //! Valeur initiale de indexMurissementNeige
    // TMUR
    float indexMurissementNeigeIni;
    //! Valeur initiale de indexTempNeige
    // TSTOCK
    float indexTempNeigeIni;
    //! Seuil de transformation pluie-neige (degC).
    // STRNE
    std::vector<float> seuilTranformationPluieNeige; 
    //! Taux potentiel de fonte en forï¿½t (mm/degC/jour).
    // TFC
    std::vector<float> tauxPotentielFonteForet;
    //! Taux potentiel de fonte en clairiï¿½re (mm/degC/jour).
    // TFD
    std::vector<float> tauxPotentielFonteClairiere;
    //! Seuil de tempï¿½rature de fonte en foret (degC).
    // TSC
    std::vector<float> seuilTempFonteForet;
    //! Seuil de tempï¿½rature de fonte en clairiere (degC).
    // TSD
    std::vector<float> seuilTempFonteClairiere;
    //! Coefficient de deficit calorifique.
    // TTD
    float coeffDeficitCalorique;
    //! Temperature du murissement du stock de neige (degC).
    // TTS
    std::vector<float> tempMurissementNeige;
  } params_;
  
  //! Noms des variable d'etat dans les intrants
  std::vector<std::string> nomChamps_;
  //! Etat pour un carreau entier
  EtatFonteCE etatFonteCE_;
  //! Etats des carreaux entiers pour un pas de temps
  std::vector<EtatFonteCE> etatsFonteCE_;
  //! Etats des carreaux entiers pour tous les pas de temps
  std::vector<std::vector<EtatFonteCE> > etatsFonte_;

  // Donnees a assimiler
  class EtatFonteAssimCE
  {
  public:
    int idCarreauEntier;
    TypeAssim SNC_stockNeigeForetType;
    float SNC_stockNeigeForet[2];
    TypeAssim SND_stockNeigeClairiereType;
    float SND_stockNeigeClairiere[2];
    TypeAssim QNUI3_indexMurissementNeigeType;
    float QNUI3_indexMurissementNeige[2];
    TypeAssim QNUI4_indexTempNeigeType;
    float QNUI4_indexTempNeige[2];
  };

  std::map<DateChrono, std::vector<EtatFonteAssimCE> > assimilationsCE_;
  // conservation des etats avant assimilation
  std::map<DateChrono, std::vector<EtatFonteCE> > avantAssimilationsCE_;

  double calculerFacteurModulationSoleil(int noJour) const;
};
