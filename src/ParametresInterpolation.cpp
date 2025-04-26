//****************************************************************************
// Fichier: ParametresInterpolation.cpp
//
// Date creation: 2013-05-21
// Auteur: 
//                Rio Tinto Alcan                     
//                Energie electrique                  
//                1954 Davis, Saguenay arr. Jonquiere,
//                G7S 4R7, QC, Canada                 
//
//****************************************************************************
#include "stdafx.h"
#include "ParametresInterpolation.h"

//------------------------------------------------------------------
ParametresInterpolation::ParametresInterpolation()
{ 
}


//------------------------------------------------------------------
ParametresInterpolation::~ParametresInterpolation()
{
}

//------------------------------------------------------------------
int ParametresInterpolation::type() const
{
  return type_;
}

//------------------------------------------------------------------
float ParametresInterpolation::coep() const
{
  return coep_;
}

//------------------------------------------------------------------
float ParametresInterpolation::coet() const
{
  return coet_;
}
   
//------------------------------------------------------------------
void ParametresInterpolation::initialiser(const mxArray* paramInterpolation) 
{
  mxArray* interpolation = MexHelper::mhMxGetField(paramInterpolation, 0, "interpolation"); 
  MexHelper::chargerValeurs(interpolation, "type", type_);
  MexHelper::chargerValeurs(interpolation, "coep", coep_);
  MexHelper::chargerValeurs(interpolation, "coet", coet_);
}
