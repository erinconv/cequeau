//****************************************************************************
// Fichier: Simulation.h
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

#include "BassinVersant.h"
#include "Parametres.h"
#include "DonneesMeteo.h"
#include "Fonte.h"
#include "Evapo.h"
// #include "PompageEau.h"
#include "DateChrono.h"

typedef std::shared_ptr<Fonte> FontePtr;
typedef std::shared_ptr<Evapo> EvapoPtr;

// Les modules de fonte disponibles
// TODO New module: Your module here
namespace ModuleFonte {
  enum ModuleFonte {CEQUEAU = 1, CEMA_NEIGE = 2, UEB = 3, FONTE_TEMPLATE = 3};
}
namespace ModuleEvapo {
  enum ModuleEvapo {CEQUEAU = 1, KPENMAN = 2, PRIESTLEYTAYLOR = 3, MCGUINNESS = 4, PENMONT = 5, MORTON = 6};
}

//! Classe de donnees de l'etat d'un carreau entier.
/*!
  Utilise pour conserver l'etat d'un carreau entier apres
  la fonction de production.
*/
class EtatCarreauEntier
{
public:
  int   idCarreauEntier;
  int   iCarreauEntier;
  int   jCarreauEntier;
  float niveauEauSol;
  float niveauEauNappe;
  float niveauEauLacsMarais;
  float evapoPotJour;
  float production;
  float recharge;

  // QUALITE
  // Donnees necessaires pour les calculs de qualite
  class Qualite 
  {
  public:
    float RUISST;
    float SOLT;
    float SONAPT;
    float SLAMAT;
    float PPTTOT;
    // evapotranspirationSol 
    float ETRSOL; 
    // evapotranspirationNappe,
    float ETRNAP;
    //evaporationLac, 
    float ETRLAC;
  } Qualite;
};

//! Classe de donnees pour l'assimilation des etats d'un carreau entier.
/*!
  Utilise pour assimiler des etats aux carreaux entiers.
  Une valeur: on substitue cette valeur. 
  Deux valeurs: La premiere est un facteur multiplicatif 
  et la seconde une valeur a additionner.
*/
class EtatCarreauEntierAssim
{
public:
  int   idCarreauEntier;
  int   iCarreauEntier;
  int   jCarreauEntier;
  TypeAssim niveauEauSolType;
  float niveauEauSol[2];
  TypeAssim niveauEauNappeType;
  float niveauEauNappe[2];
  TypeAssim niveauEauLacsMaraisType;
  float niveauEauLacsMarais[2];
  TypeAssim evapoPotJourType;
  float evapoPotJour[2];
  TypeAssim productionType;
  float production[2];
};

//! Classe de donnees de l'etat d'un carreau partiel.
/*!
  Utilise pour conserver l'etat d'un carreau partiel apres
  la fonction de transfert.
*/
class EtatCarreauPartiel
{
public:
  int   idCarreauPartiel;
  float apport;
  float volume;
  float debit;
  // QUALITE
  // Donnees necessaires pour les calculs de qualite
  class Qualite
  {
  public:
    float volumeSortant;
  } Qualite;
};

//! Classe de donnees pour l'assimilation des etats d'un carreau partiel.
/*!
  Utilise pour assimiler des etats aux carreaux partiels.
  Une valeur: on substitue cette valeur. 
  Deux valeurs: La premiere est un facteur multiplicatif 
  et la seconde une valeur a additionner.
*/
class EtatCarreauPartielAssim
{
public:
  int   idCarreauPartiel;
  TypeAssim apportType;
  float apport[2];
  TypeAssim volumeType;
  float volume[2];
  TypeAssim debitType;
  float debit[2];
};

//! Classe de donnees de l'etat d'un barrage
class EtatBarrage
{
public:
  int   idCarreauPartiel;
  float volume;
  float niveau;
  // Debit entrant au barrage
  float debitEntrant;
  // Debits sortants du barrage. 
  // Contient une valeur sauf dans le cas d'un barrage type 5.
  // Dans ce cas les valeurs sont ordonnees par regles d'evacuations internes 
  // suivi de l'evacuation externe
  std::vector<float> debitsSortants;
};

//! Classe de donnees pour l'assimilation des etats d'un barrage.
/*!
  Utilise pour assimiler des etats aux barrage.
  Une valeur: on substitue cette valeur. 
  Deux valeurs: La premiere est un facteur multiplicatif 
  et la seconde une valeur a additionner.
*/
class EtatBarrageAssim
{
public:
  int   idCarreauPartiel;
  TypeAssim volumeType;
  float volume[2];
  TypeAssim niveauType;
  float niveau[2];
};

// TODO: Releves neige
class ReleveNeige
{
public:
  int no;
  int idCE;
  int i;
  int j;
  float valeur;
  float correction;
  float distance;
}; 
typedef std::vector<ReleveNeige> RelevesNeige;
bool sortByDistance(const ReleveNeige &lhs, const ReleveNeige &rhs); 

//! Liste des etats des carreaux entiers a chaque pas de temps.
typedef std::vector<EtatCarreauEntier>  EtatsCarreauxEntiers;
//! Liste des etats des carreaux partiels a chaque pas de temps.
typedef std::vector<EtatCarreauPartiel> EtatsCarreauxPartiels;
//! Liste des etats des barrages a chaque pas de temps.
typedef std::vector<EtatBarrage>        EtatsBarrages;

//! Liste des assimilations des carreaux entiers a chaque pas de temps.
typedef std::vector<EtatCarreauEntierAssim>  EtatsCarreauxEntiersAssim;
//! Liste des assimilations des carreaux partiels a chaque pas de temps.
typedef std::vector<EtatCarreauPartielAssim> EtatsCarreauxPartielsAssim;
//! Liste des assimilations des barrages a chaque pas de temps.
typedef std::vector<EtatBarrageAssim>        EtatsBarragesAssim;

//! Classe simulation
/*!
  Classe principale de l'application. 
*/
class Simulation
{
public:
  friend class SimulationQualite;
  //! Construteur
  /*!
    \param bv Reference sur un objet BassinVersant.
    \param param Reference sur un objet Parametres.
    \param meteo Reference sur un objet DonneesMeteo.
    \param fichierEtats Repertoire et nom du fichier Matlab des etats precedents.
    \param fichierResultats Repertoire et nom du fichier Matlab des resultats.
  */
  Simulation(BassinVersant& bv, const Parametres& param, const DonneesMeteo& meteo,
             const mxArray* etatsPrecedents, const std::string& fichierResultats, FontePtr moduleFonte,
             EvapoPtr moduleEvapo, const mxArray* assimilations = NULL);
  ~Simulation();

  // TODO: Releves neige
  bool hasNeigeAjustee() const;

  //! Execution de la simulation.
  int executer(bool estMexFile = false);
  //! Etats CE en mxArray
  mxArray* obtenirEtatsCE();
  //! Etats CP en mxArray
  mxArray* obtenirEtatsCP();
  //! Etats barrages en mxArray
  mxArray* obtenirEtatsBarrage();
  //! Pas de temps en mxArray
  mxArray* obtenirPasDeTemps();
  //! Pas de temps en mxArray
  mxArray* obtenirEtatsAvantAssimilations();

private:
  // TODO: Releves neige
  bool hasNeigeAjustee_;

  BassinVersant& bassinVersant_;
  const Parametres& parametres_;
  const DonneesMeteo& donneesMeteo_;
  FontePtr fonte_;
  EvapoPtr evapo_;

  //! Etats precedents.
  const mxArray* etatsPrecedents_;
  //! Fichier des resultats, incluant le repertoire.
  std::string fichierResultats_;
  // Repertoire ou sont produit les resultats.
  std::string repertoireOut_;

  //! Donnees a assimiler aux etats des carreaux entiers.
  std::map<DateChrono, EtatsCarreauxEntiersAssim> assimilationsCE_;
  //! Donnees a assimiler aux etats des carreaux partiels.
  std::map<DateChrono, EtatsCarreauxPartielsAssim> assimilationsCP_;
  //! Donnees a assimiler aux etats des barrages.
  std::map<DateChrono, EtatsBarragesAssim> assimilationsBarrages_;
  //! Etats des carreaux entiers, avant assimilation.
  std::map<DateChrono, EtatsCarreauxEntiers> avantAssimilationsCE_;
  //! Etats des carreaux partiels, avant assimilation.
  std::map<DateChrono, EtatsCarreauxPartiels> avantAssimilationsCP_;
  //! Etats des barrages, avant assimilation.
  std::map<DateChrono, EtatsBarrages> avantAssimilationsBarrages_;

  //! Resultat de la production.
  std::vector<EtatsCarreauxEntiers>  bilans_;
  //! Resultat du transfert.
  std::vector<EtatsCarreauxPartiels> debits_;
  //! Suivi du volume aux barrages.
  std::vector<EtatsBarrages> barrages_;

  //! Traitement des parametres facultatifs.
  void traiterParamFacultatifs();

  //! Fonction de production
  /*!
    \param datePasDeTemps Date du pas de temps.
    \param noPasDeTemps No du du pas de temps. 
  */
  int  executerProduction(const DateChrono& datePasDeTemps, const int& noPasDeTemps);

  //! Initialisation des donnees d'assimilation 
  int initialiserAssimilations(const mxArray* assimilations);
  //! Assimilation donnees CE
  int assimiler(const DateChrono& datePasDeTemps, EtatsCarreauxEntiers& etatsCarreauxEntiers);
  //! Assimilation donnees CP
  int assimiler(const DateChrono& datePasDeTemps, EtatsCarreauxPartiels& etatsCarreauxPartiels);
  //! Assimilation donnees Barrages
  int assimiler(const DateChrono& datePasDeTemps, EtatsBarrages& etatsBarrages);
  
  //! Fonction de transfert
  /*!
    \param nbTransfertsParJour Nombre de transfert par jour.
    \param datePasDeTemps Date du pas de temps.
    \param noPasDeTemps No du du pas de temps. 
  */
  int  executerTransfert(const int& nbTransfertsParJour, const DateChrono& datePasDeTemps,
                         const int& noPasDeTemps, const float& duree);

  int calculerTemperatureEau(const DateChrono& datePasDeTemps);

  //! Initialisation des coefficient de transfert de chaque carreau. partiel.
  void  initialiserCoeffTransfertCP(int nbTransfertParJour);

  //! Initialisation des etats initiaux.
  /*!
    L'initialisation se fait a partir des parametres ou bien d'etats precedents, s'ils sont
    presents dans les intrants.
  */
  void  initialiserEtats();

  //! Calcul de la fonte sur un carreau entier pour un pas de temps.
  /*!
    \param[in] surfaceForetCE Surface occupee par la foret.
    \param[in] etatPrecCarreauEntier Etat du carreau entier pour le pas de temps precedent.
    \param[out] etatCarreauEntier Reference sur l'etat du carreau entier du pas de temps courant.
    \param[in] PJE_pluie Quantite de pluie sur le carreau entier pour le pas de temps.
    \param[in] PJN_neige Quantite de neige sur le carreau entier pour le pas de temps.
    \param[in] TJE_tempMoy Temperature moyenne sur le carreau entier pour le pas de temps.
    \param[in] facteurModulSoleilNeige Facteur de modulation solaire pour la fonte pour le pas de temps.
    \param[out] fonteDisponible Eau disponible resultant de la fonte sur le carreau entier pour le pas de temps.
    \param[out] eauDisponible Eau totale disponible sur le carreau entier pour le pas de temps.
    \return 0 ok, -1 sinon
  */
  int calculerFonte(float surfaceForetCE, const EtatCarreauEntier& etatPrecCarreauEntier,
                EtatCarreauEntier& etatCarreauEntier, float PJE_pluie, float PJN_neige, float TJE_tempMoy, 
                float facteurModulSoleilNeige, float& eauDisponible);

  //! Calcul de la fonte sur un carreau entier pour un pas de temps. 
  /*!
    Version avec enonces goto. Conservee a titre de reference. 
  */
  /***
  int calculerFonteAvecGoto(float surfaceForetCE, const EtatCarreauEntier& etatPrecCarreauEntier,
                EtatCarreauEntier& etatCarreauEntier, float PJE_pluie, float PJN_neige, float TJE_tempMoy, 
                float facteurModulSoleilNeige, float& fonteDisponible, float& eauDisponible);
  ***/

  // TODO: Releves neige
  int ajusterNeigeZone(const DateChrono& datePasDeTemps, std::ofstream& fOut);

  // TODO: Releves neige
  int ajusterNeige3Releves(const DateChrono& datePasDeTemps, std::ofstream& fOut);

  //! Calcul du bilan d'un carreau entier.
  /*!
    \param[in] carreauEntier Reference sur le carreau entier.
    \param[in] etatPrecCarreauEntier Etat du carreau entier pour le pas de temps precedent.
    \param[out] etatCarreauEntier Reference sur l'etat du carreau entier du pas de temps courant.
    \param[in] TJE_tempMoy Temperature moyenne sur le carreau entier pour le pas de temps.
    \param[in] PLUFON_eauDisponible Eau totale disponible sur le carreau entier pour le pas de temps.
    \param[in] facteurModulSoleilEvapo Facteur de modulation solaire pour l'evapotranspiration pour le pas de temps.
    \return 0 ok, -1 sinon
  */
  // int calculerBilanReservoirs(int noJour, CarreauEntier& carreauEntier, const EtatCarreauEntier& etatPrecCarreauEntier,
  //                             EtatCarreauEntier& etatCarreauEntier, const Meteo& meteo, float PLUFON_eauDisponible, double donneePompage);
    int calculerBilanReservoirs(int noJour, CarreauEntier& carreauEntier, const EtatCarreauEntier& etatPrecCarreauEntier,
                              EtatCarreauEntier& etatCarreauEntier, const Meteo& meteo, float PLUFON_eauDisponible, const std::vector<PuitsPtr>& listePuits, int indexPasDeTemps);

  //! Calcul de la position des intrants meteo et debit
  int calculerIndexMeteo(const DateChrono& datePasDeTemps, const int& noPasDeTemps);

  int calculerIndexPompage(const DateChrono& datePasDeTemps, const int& noPasDeTemps);
  int convertirMetreCubeEnHauteur(const DateChrono& datePasDeTemps, const int& noPasDeTemps);

  //! Ecriture des resultats de la simulation.
  void ecrireResultats();
  
  //! Etats CE en mxArray
  mxArray* obtenirEtatsCE(const std::vector<EtatsCarreauxEntiers>& etatsPasDeTemps);
 
  //! Etats CP en mxArray
  mxArray* obtenirEtatsCP(const std::vector<EtatsCarreauxPartiels>& etatsPasDeTemps);

  //! Etats CP en mxArray
  mxArray* obtenirEtatsBarrage(const std::vector<EtatsBarrages>& etatsPasDeTemps);


#ifdef ENV_MATLAB
  //! Sauvegarde de l'etat de chaque carreau entier en format Matlab.
  /*!
    Ecriture de l'etat de chaque carreau entier a chaque pas de temps
    en format Matlab.
  */
  matError ecrireEtatsCE(MATFile* pfichierMatlab);
  
  //! Sauvegarde de l'etat de chaque carreau partiel en format Matlab.
  /*!
    Ecriture de l'etat de chaque carreau partiel a chaque pas de temps
    en format Matlab.
  */
  matError ecrireEtatsCP(MATFile* pfichierMatlab);

  //! Ecriture de la position i-j de chaque carreau partiel en format Matlab.
  matError ecrirePositionsCP(MATFile* pfichierMatlab);

  //! Ecriture de l'etat des barrages en format Matlab.
  matError ecrireEtatsBarrage(MATFile* pfichierMatlab);

  //! Ecriture des pas de temps (format datenum) en format Matlab.
  matError ecrirePasDeTemps(MATFile* pfichierMatlab);

#endif

  //! Initialisation des etats des carreaux entiers a partir d'etats precedents en format Matlab.
  int chargerEtatsCE(EtatsCarreauxEntiers& etatsCarreauxEntiers);

  //! Initialisation des etats des carreaux partiels a partir d'etats precedents en format Matlab.
  int chargerEtatsCP(EtatsCarreauxPartiels& etatsCarreauxPartiels);

  //! Chargements etats des barrages a partir d'etats precedents en format Matlab.
  int chargerEtatsBarrages();
};
