//****************************************************************************
// Fichier: CarreauPartiel.cpp
//
// Date creation: 2012-10-01
// Auteur: 
//                Rio Tinto Alcan                     
//                Energie electrique                  
//                1954 Davis, Saguenay arr. Jonquiere,
//                G7S 4R7, QC, Canada                 
//
//****************************************************************************
#include "stdafx.h"
#include "CarreauPartiel.h"

//------------------------------------------------------------------
CarreauPartiel::CarreauPartiel(
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
    int   pctFermetureFutaie,
    float indiceFoliaire,
    float custom_1,
    float custom_2,
    //! --- ombre -----
    int latitude,
    int longitude,
    int pctFeuillu,
    int pctConifer,
    double hautMoyenneArbre,
    int azimutCoursEau
    )
    :
    id_(id),
    codeCarreauPartiel_(codeCarreauPartiel),
    pctSurface_(pctSurface),
    iCarreauEntier_(iCarreauEntier),
    jCarreauEntier_(jCarreauEntier),
    idCarreauPartielAval_(idCarreauPartielAval),
    idCarreauEntier_(idCarreauEntier),
    pctEau_(pctEau),
    pctForet_(pctForet),
    pctMarais_(pctMarais),
    pctSolNu_(pctSolNu),
    altitudeMoyenne_(altitudeMoyenne),
    profondeurMinimum_(profondeurMinimum),
    longueurCoursEauPrincipal_(longueurCoursEauPrincipal),
    largeurCoursEauPrincipal_(largeurCoursEauPrincipal),
    penteRiviere_(penteRiviere),
    cumulPctSuperficieCPAmont_(cumulPctSuperficieCPAmont),
    cumulPctSuperficieLacsAmont_(cumulPctSuperficieLacsAmont),
    cumulPctSuperficieMaraisAmont_(cumulPctSuperficieMaraisAmont),
    cumulPctSuperficieForetAmont_(cumulPctSuperficieForetAmont),
    volumeInitial_(0.0f),
    pctFermetureFutaie_(pctFermetureFutaie),
    indiceFoliaire_(indiceFoliaire),
    custom_1_(custom_1),
    custom_2_(custom_2),
    latitude_(latitude),
    longitude_(longitude),
    pctFeuillu_(pctFeuillu),
    pctConifer_(pctConifer),
    hautMoyenneArbre_(hautMoyenneArbre),
    azimutCoursEau_(azimutCoursEau)
{
  FILE_LOG(logDEBUG) << "CarreauPartiel::CarreauPartiel";
  // Liste des CP en amont.
  std::copy(idCarreauxPartielsAmont, idCarreauxPartielsAmont + NB_CP_AMONT, idCarreauxPartielsAmont_);
  superficieCPAmont_ = (float)cumulPctSuperficieCPAmont / 100.0f * superficieCarreauEntier;
  // On met la meme pour l'instant, si besoin sera modifie plus tard. 
  superficieCPAmontCorrige_ = superficieCPAmont_;
}

//------------------------------------------------------------------
CarreauPartiel::~CarreauPartiel()
{
  FILE_LOG(logDEBUG) << "CarreauPartiel::~CarreauPartiel()";
}

//------------------------------------------------------------------
int CarreauPartiel::id() const
{ 
  FILE_LOG(logDEBUG) << "CarreauPartiel::id()";
  return id_;
}

//------------------------------------------------------------------
char CarreauPartiel::codeCarreauPartiel() const
{
  FILE_LOG(logDEBUG) << "CarreauPartiel::codeCarreauPartiel()";
  return codeCarreauPartiel_;
}

//------------------------------------------------------------------
float CarreauPartiel::pctSurface() const
{
  FILE_LOG(logDEBUG) << "CarreauPartiel::pctSurface()";
  return (float)pctSurface_ / 100.0f;
}

//------------------------------------------------------------------
int CarreauPartiel::iCarreauEntier() const
{
  FILE_LOG(logDEBUG) << "CarreauPartiel::iCarreauEntier()";
  return iCarreauEntier_;
}

//------------------------------------------------------------------
int CarreauPartiel::jCarreauEntier() const
{
  FILE_LOG(logDEBUG) << "CarreauPartiel::jCarreauEntier()";
  return jCarreauEntier_;
}

//------------------------------------------------------------------
int CarreauPartiel::idCarreauPartielAval() const
{
  FILE_LOG(logDEBUG) << "CarreauPartiel::idCarreauPartielAval()";
  return idCarreauPartielAval_;
}

//------------------------------------------------------------------
const int* CarreauPartiel::idCarreauxPartielsAmont()
{
  FILE_LOG(logDEBUG) << "CarreauPartiel::idCarreauxPartielsAmont()";
  return idCarreauxPartielsAmont_;
}

//------------------------------------------------------------------
int CarreauPartiel::idCarreauEntier() const
{
  FILE_LOG(logDEBUG) << "CarreauPartiel::idCarreauEntier()";
  return idCarreauEntier_;
}

//------------------------------------------------------------------
float CarreauPartiel::pctEau() const
{
  FILE_LOG(logDEBUG) << "CarreauPartiel::pctEau()";
  return (float)pctEau_ / 100.0f;
}

//------------------------------------------------------------------
float CarreauPartiel::pctForet() const
{
  FILE_LOG(logDEBUG) << "CarreauPartiel::pctForet()";
  return (float)pctForet_ / 100.0f;
}

//------------------------------------------------------------------
float CarreauPartiel::pctMarais() const
{
  FILE_LOG(logDEBUG) << "CarreauPartiel::pctMarais()";
  return (float)pctMarais_ / 100.0f;
}

//------------------------------------------------------------------
float CarreauPartiel::pctSolNu() const
{
  FILE_LOG(logDEBUG) << "CarreauPartiel::pctSolNu()";
  return (float)pctSolNu_ / 100.0f;
}

//------------------------------------------------------------------
int CarreauPartiel::altitudeMoyenne() const
{
  FILE_LOG(logDEBUG) << "CarreauPartiel::pctSolNu()";
  return altitudeMoyenne_;
}

//------------------------------------------------------------------
int CarreauPartiel::profondeurMinimum() const
{
  FILE_LOG(logDEBUG) << "CarreauPartiel::profondeurMinimum()";
  return profondeurMinimum_;
}

//------------------------------------------------------------------
int CarreauPartiel::longueurCoursEauPrincipal() const
{
  FILE_LOG(logDEBUG) << "CarreauPartiel::longueurCoursEauPrincipal()";
  return longueurCoursEauPrincipal_;
}

//------------------------------------------------------------------
int CarreauPartiel::largeurCoursEauPrincipal() const
{
  FILE_LOG(logDEBUG) << "CarreauPartiel::largeurCoursEauPrincipal()";
  return largeurCoursEauPrincipal_;
}

//------------------------------------------------------------------
int CarreauPartiel::penteRiviere() const
{
  FILE_LOG(logDEBUG) << "CarreauPartiel::penteRiviere()";
  return penteRiviere_;
}

//------------------------------------------------------------------
int CarreauPartiel::cumulPctSuperficieCPAmont() const
{
  FILE_LOG(logDEBUG) << "CarreauPartiel::cumulPctSuperficieCPAmont()";
  return cumulPctSuperficieCPAmont_;
}

//------------------------------------------------------------------
void CarreauPartiel::superficieCPAmontCorrige(float valeur)
{
  FILE_LOG(logDEBUG) << "CarreauPartiel::cumulPctSuperficieCPAmontCorrige(int valeur)";
  superficieCPAmontCorrige_ = valeur;
}

//------------------------------------------------------------------
int CarreauPartiel::cumulPctSuperficieLacsAmont() const
{
  FILE_LOG(logDEBUG) << "CarreauPartiel::cumulPctSuperficieLacsAmont()";
  return cumulPctSuperficieLacsAmont_;
}

//------------------------------------------------------------------
int CarreauPartiel::cumulPctSuperficieMaraisAmont() const
{
  FILE_LOG(logDEBUG) << "CarreauPartiel::cumulPctSuperficieMaraisAmont()";
  return cumulPctSuperficieMaraisAmont_;
}

//------------------------------------------------------------------
int CarreauPartiel::cumulPctSuperficieForetAmont() const
{
  FILE_LOG(logDEBUG) << "CarreauPartiel::cumulPctSuperficieForetAmont()";
  return cumulPctSuperficieForetAmont_;
}

//------------------------------------------------------------------
double CarreauPartiel::coeffTransfert() const
{
  FILE_LOG(logDEBUG) << "CarreauPartiel::coeffTransfert()";
  return coeffTransfert_;
}  

//------------------------------------------------------------------
float CarreauPartiel::volumeInitial() const
{
  FILE_LOG(logDEBUG) << "CarreauPartiel::volumeInitial()";
  return volumeInitial_;
}

//------------------------------------------------------------------
int CarreauPartiel::pctFermetureFutaie() const
{
  return pctFermetureFutaie_;
}
//------------------------------------------------------------------
float CarreauPartiel::indiceFoliaire() const
{
  return indiceFoliaire_;
}
//------------------------------------------------------------------
float CarreauPartiel::custom_1() const
{
  return custom_1_;
}

//------------------------------------------------------------------
float CarreauPartiel::custom_2() const
{
  return custom_2_;
}

//------------------------------------------------------------------
int CarreauPartiel::latitude() const {
    FILE_LOG(logDEBUG) << "CarreauPartiel::latitude()";

    return latitude_;
}

//------------------------------------------------------------------
int CarreauPartiel::longitude() const {
    FILE_LOG(logDEBUG) << "CarreauPartiel::longitude()";

    return longitude_;
}

//------------------------------------------------------------------
int CarreauPartiel::pctFeuillu() const {
    FILE_LOG(logDEBUG) << "CarreauPartiel::pctFeuillu()";

    return pctFeuillu_;
}

//------------------------------------------------------------------
int CarreauPartiel::pctConifer() const {
    FILE_LOG(logDEBUG) << "CarreauPartiel::pctConifer()";

    return pctConifer_;
}

//------------------------------------------------------------------
double CarreauPartiel::hautMoyenneArbre() const {
    FILE_LOG(logDEBUG) << "CarreauPartiel::hautMoyenneArbre";

    return hautMoyenneArbre_;
}

//------------------------------------------------------------------
int CarreauPartiel::azimutCoursEau() const {
    FILE_LOG(logDEBUG) << "CarreauPartiel::azimutCoursEau()";

    return azimutCoursEau_;
}

//------------------------------------------------------------------
void CarreauPartiel::coeffTransfert(double valeur) 
{
  FILE_LOG(logDEBUG) << "CarreauPartiel::coeffTransfert(double valeur) ";
  coeffTransfert_ = valeur;
}  

//------------------------------------------------------------------
void CarreauPartiel::volumeInitial(float valeur)
{
  FILE_LOG(logDEBUG) << "CarreauPartiel::volumeInitial(float valeur)";
  volumeInitial_ = valeur;
}

//------------------------------------------------------------------
float CarreauPartiel::calculerCorrectionDebit() const
{
  FILE_LOG(logDEBUG) << "CarreauPartiel::calculerCorrectionDebit()";
  
  return superficieCPAmontCorrige_ / superficieCPAmont_;
}