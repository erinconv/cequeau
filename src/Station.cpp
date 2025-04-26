//****************************************************************************
// Fichier:  Station.cpp
//
// Date creation: 2013-05-13
// Auteur: 
//                Rio Tinto Alcan                     
//                Energie electrique                  
//                1954 Davis, Saguenay arr. Jonquiere,
//                G7S 4R7, QC, Canada                 
//
//****************************************************************************
#include "stdafx.h"
#include "Station.h"

//------------------------------------------------------------------
Station::Station(std::string no, std::string nom, int id, int i, int j, float moyAnPrecipitation, float altitude)
  : no_(no), nom_(nom), id_(id), i_(i), j_(j), moyAnPrecipitation_(moyAnPrecipitation), altitude_(altitude)
{
}

//------------------------------------------------------------------
Station::~Station() 
{
}


//------------------------------------------------------------------
int Station::i() const
{
  return i_;
}

//------------------------------------------------------------------
int Station::j() const
{
  return j_;
}

//------------------------------------------------------------------
int Station::id() const
{
  return id_;
}

//------------------------------------------------------------------
float Station::moyAnPrecipitation() const
{
  return moyAnPrecipitation_;
}

//------------------------------------------------------------------
float Station::altitude() const
{
  return altitude_;
}



