//****************************************************************************
// Fichier: CarreauPartiel.h
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

//! Nombre maximal de carreaux partiels affluants.
const int NB_CP_AMONT = 5;

//! Definition d'un carreau partiel.
/*! 
 *  Definition d'un carreau partiel
 *  selon les donnees physiographiques PREPAREES.
 */
class CarreauPartiel
{ 
public:
  //! Constructeur.
  CarreauPartiel(
    int   id,
    char  codeCarreauPartiel,
    int   pctSurface,
    int   iCarreauEntier,
    int   jCarreauEntier,
    int   idCarreauPartielAval,
    int   idCarreauxPartielsAmont[NB_CP_AMONT],
    int   idCarreauEntier,
    int   pctEau,
    int   pctForet,
    int   pctMarais,
    int   pctSolNu,
    int   altitudeMoyenne,
    int   profondeurMinimum,
    int   longueurCoursEauPrincipal,
    int   largeurCoursEauPrincipal,
    int   penteRiviere,
    int   cumulPctSuperficieCPAmont,
    int   cumulPctSuperficieLacsAmont,
    int   cumulPctSuperficieMaraisAmont,
    int   cumulPctSuperficieForetAmont,
    float superficieCarreauEntier,
    // Ajout pour qualite: 2015-09
    int   pctFermetureFutaie,
    float indiceFoliaire,
    float custom_1,
    float custom_2,
    // Ajout pour ombre: 2018-08
    int lat,
    int lon,
    int pctFeuillu,
    int pctConifer,
    double hautMoyenneArbre,
    int azimutCoursEau
  );
  
  ~CarreauPartiel();
  
  // Getters/Setters
  //! Getter Id du carreau partiel.
  int     id() const;
  //! Getter Code du carreau partiel (A - B - C ou D).
  char    codeCarreauPartiel() const;
  //! Getter Pourcentage du carreau partiel par rapport au carreau entier (0 a 1).
  float   pctSurface() const;
  //! Getter Abscisse I du carreau entier auquel appartient le carreau partiel.
  int     iCarreauEntier() const;
  //! Getter Ordonnï¿½e J du carreau entier auquel appartient le carreau partiel.
  int     jCarreauEntier() const;
  //! Getter Id du carreau partiel dans lequel il se jette.
  int     idCarreauPartielAval() const;
  //! Getter Liste des id des cinq carreaux affluents possible.
  const int*  idCarreauxPartielsAmont();
  //! Getter Id du carreau entier auquel il appartient.
  int     idCarreauEntier() const;
  //! Getter Poucentage d'eau sur le carreau partiel.
  float   pctEau() const;
  //! Getter Poucentage de foret sur le carreau partiel.
  float   pctForet() const;
  //! Getter Poucentage de marais sur le carreau partiel.
  float   pctMarais() const;
  //! Getter Poucentage de sol deboise sur le carreau partiel.
  float   pctSolNu() const;
  //! Getter altitude moyenne (metres).
  int     altitudeMoyenne() const;
  //! Getter Profondeur minimum (1/100 de metres).
  int     profondeurMinimum() const;
  //! Getter Longueur du cours d'eau principal sur la parcelle (1/10 de km).
  int     longueurCoursEauPrincipal() const;
  //! Getter Largeur du cours d'eau principal sur la parcelle (1/10 de metres).
  int     largeurCoursEauPrincipal() const;
  //! Getter Pente de la riviere sur le carreau partiel (1/1000 de metres/km).
  int     penteRiviere() const;
  /*! Getter Cumul des pourcentages de superficie des carreaux partiels en
      amont, calcule ï¿½ la sortie du carreau partiel considere. */
  int     cumulPctSuperficieCPAmont() const;
  //! Setter superficieCPAmontCorrige.
  void    superficieCPAmontCorrige(float valeur);
  /*! Getter Cumul des pourcentages de superficie des lacs en amont, calcule
      a la sortie du carreau partiel considere. */
  int     cumulPctSuperficieLacsAmont() const;
  /*! Getter Cumul des pourcentages de superficie des marais en amont, calcule
      a la sortie du carreau partiel considere. */
  int     cumulPctSuperficieMaraisAmont() const;
  /*! Getter Cumul des pourcentages de superficie de foret en amont, calcule
      a la sortie du carreau partiel considere. */
  int     cumulPctSuperficieForetAmont() const;
  //! Getter Coefficient de transfert pour le pas de temps d'une journee.
  double  coeffTransfert() const;
  //! Getter Volume d'eau initial.
  float   volumeInitial() const;
  //! Getter Pourcentage fermeture futaie.
  int     pctFermetureFutaie() const;
  //! Getter Indice foliaire.
  float   indiceFoliaire() const;
  //! Getter Custom_1.
  float   custom_1() const;
  //! Getter Custom_2.
  float   custom_2() const;
  //! Setter coefficient de transfert pour le pas de temps d'une journee.
  void    coeffTransfert(double valeur); 
  //! Setter Volume d'eau initial.
  void    volumeInitial(float valeur);

  //! Getter latitude 
  int latitude() const;
  //! Getter longitude
  int longitude() const;
  //! Getter pctFeuillu
  int pctFeuillu() const;
  //! Getter pctConifer
  int pctConifer() const;
  //! Getter hautMoy
  double hautMoyenneArbre() const;
  //! Getter azimutCoursEau
  int azimutCoursEau() const;

  /*! Facteur de correction du debit calcule. 
      = cumulPctSuperficieCPAmontCorrige / cumulPctSuperficieCPAmont
  */
  float calculerCorrectionDebit() const;

private:
  //! Id du carreau partiel. 1 a nbCarreauxPartielss du bassin versant.
  int  id_;
  char codeCarreauPartiel_;
  int  pctSurface_;
  int  iCarreauEntier_;
  int  jCarreauEntier_;
  int  idCarreauPartielAval_;
  int  idCarreauxPartielsAmont_[NB_CP_AMONT];
  int  idCarreauEntier_;
  int  pctEau_;
  int  pctForet_;
  int  pctMarais_;
  int  pctSolNu_;
  int  altitudeMoyenne_;
  int  profondeurMinimum_;
  int  longueurCoursEauPrincipal_;
  int  largeurCoursEauPrincipal_;
  int  penteRiviere_;
  int  cumulPctSuperficieCPAmont_;
  float superficieCPAmont_;
  float superficieCPAmontCorrige_;
  int  cumulPctSuperficieLacsAmont_;
  int  cumulPctSuperficieMaraisAmont_;
  int  cumulPctSuperficieForetAmont_;
  double coeffTransfert_;
  float volumeInitial_;
  int   pctFermetureFutaie_;
  float indiceFoliaire_;
  float custom_1_;
  float custom_2_;
  int latitude_;
  int longitude_;
  int pctFeuillu_;
  int pctConifer_;
  double hautMoyenneArbre_;
  int azimutCoursEau_;
};
