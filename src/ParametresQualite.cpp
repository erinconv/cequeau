//****************************************************************************
// Fichier: ParametresQualite.cpp
//
// Date creation: 2013-04-11
// Auteur: 
//                Rio Tinto Alcan                     
//                Energie electrique                  
//                1954 Davis, Saguenay arr. Jonquiere,
//                G7S 4R7, QC, Canada                 
//
//****************************************************************************
#include "stdafx.h"
#include "ParametresQualite.h"
#include "mex.h"

//------------------------------------------------------------------
ParametresQualite::ParametresQualite()
{ 
  FILE_LOG(logDEBUG) << "ParametresQualite::ParametresQualite()";
}


//------------------------------------------------------------------
ParametresQualite::~ParametresQualite()
{
  FILE_LOG(logDEBUG) << "ParametresQualite::~ParametresQualite()";
}

//------------------------------------------------------------------
float ParametresQualite::coprom() const
{
  FILE_LOG(logDEBUG) << "ParametresQualite::COPROM()";
  return coprom_;
}

//------------------------------------------------------------------
float ParametresQualite::colarg() const
{
  FILE_LOG(logDEBUG) << "ParametresQualite::COLARG()";
  return colarg_;
}
   
//------------------------------------------------------------------
const ParamTemperature& ParametresQualite::temperature() const
{
  FILE_LOG(logDEBUG) << "ParametresQualite::temperature()";
  
  return temperature_;
}

//------------------------------------------------------------------
void ParametresQualite::initialiser()
{
  coprom_ = 1.0f;
  colarg_ = 1.0f;
  
  temperature_.crayso = 1.0f;
  temperature_.crayin = 1.0f;
  temperature_.cevapo = 1.0f;
  temperature_.cconve = 1.0f;
  temperature_.crigel = 1.0f; 
  temperature_.tnap = 5.0f; 
  temperature_.panap = 1.0f;
  temperature_.tinit = 2.0f;
  temperature_.bassol = 20.0f;
  temperature_.corsol = 0.20f;
}

void ParametresQualite::initialiser(const mxArray* paramQualite) 
{
  MexHelper::chargerValeurs(paramQualite, "coprom", coprom_);
  MexHelper::chargerValeurs(paramQualite, "colarg", colarg_);

  mxArray* temperat = MexHelper::mhMxGetField(paramQualite, 0, "temperat"); 
  MexHelper::chargerValeurs(temperat, "crayso", temperature_.crayso);
  MexHelper::chargerValeurs(temperat, "crayin", temperature_.crayin);
  MexHelper::chargerValeurs(temperat, "cevapo", temperature_.cevapo);
  MexHelper::chargerValeurs(temperat, "cconve", temperature_.cconve);
  MexHelper::chargerValeurs(temperat, "crigel", temperature_.crigel);
  MexHelper::chargerValeurs(temperat, "tnap",   temperature_.tnap);
  MexHelper::chargerValeurs(temperat, "panap",  temperature_.panap);
  MexHelper::chargerValeurs(temperat, "tinit",  temperature_.tinit);
  MexHelper::chargerValeurs(temperat, "bassol", temperature_.bassol);
  MexHelper::chargerValeurs(temperat, "corsol", temperature_.corsol);
}

void ParametresQualite::initialiserOmbrage(const mxArray* paramQualite) {
	// Fonction pour allez chercher le champs de tableaux 'temperat' 
	mxArray* temperat = MexHelper::mhMxGetField(paramQualite, 0, "temperat");
	// coefficient de la canopee
	if (!MexHelper::hasField(temperat, 0, "ccanope")) {
		temperature_.ccanope = 1;
	} else {
		MexHelper::chargerValeurs(temperat, "ccanope", temperature_.ccanope);
	}

	// heure on on mesure l'ombre lorsque les valeurs sont journalieres. Si l'utilisateur ne met aucune valeur, l'heure moyenne durant la journee sera pris (14 heures)
	if (!MexHelper::hasField(temperat, 0, "hombre")) {
		mexPrintf("Aucun parametre \'hombre\' trouve, une valeur par defaut sera defini.\n");
		temperature_.hombre = 14.0f;
	} else MexHelper::chargerValeurs(temperat, "hombre", temperature_.hombre);
	if (temperature_.hombre - 8 < 0 || temperature_.hombre - 10 > 8) {
		std::stringstream erreur;
		erreur << "La valeur du parametre \'hombre\' est invalide, celle-ci doit ï¿½tre entre 8 et 18" << std::endl << "Vous avez entre : " << temperature_.hombre << std::endl;
		throw std::range_error(erreur.str());
	}
}