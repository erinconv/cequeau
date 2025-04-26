//****************************************************************************
// Fichier: Meteo.cpp
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
#include "Meteo.h"

//------------------------------------------------------------------
Meteo::Meteo(float tMin, float tMax, float pluie, float neige)
	: tMin_(tMin), tMax_(tMax), pluie_(pluie), neige_(neige) {
	FILE_LOG(logDEBUG) << "Meteo::Meteo(float tMin, float tMax, float pluie, float neige)";
}

//------------------------------------------------------------------
Meteo::Meteo(int tMin, int tMax, int pluie, int neige) {
	FILE_LOG(logDEBUG) << "Meteo::Meteo(int tMin, int tMax, int pluie, int neige)";
	float tMinF, tMaxF, pluieF, neigeF;

	tMinF = (float)(tMin) / 10.0f;
	tMaxF = (float)(tMax) / 10.0f;
	pluieF = (float)(pluie) / 10.0f;
	neigeF = (float)(neige) / 10.0f;

	tMin_ = tMinF;
	tMax_ = tMaxF;
	pluie_ = pluieF;
	neige_ = neigeF;
}

//------------------------------------------------------------------
Meteo::Meteo() {
	FILE_LOG(logDEBUG) << "Meteo::Meteo()";
	tMin_ = -1.0;
	tMax_ = 1.0;
	pluie_ = 1.0;
	neige_ = 1.0;
}

//------------------------------------------------------------------
Meteo::~Meteo() {
	FILE_LOG(logDEBUG) << "Meteo::~Meteo()";
}

//------------------------------------------------------------------
float Meteo::tMin() const {
	FILE_LOG(logDEBUG) << "Meteo::tMin()";
	return tMin_;
}

//------------------------------------------------------------------
float Meteo::tMax() const {
	FILE_LOG(logDEBUG) << "Meteo::tMax()";
	return tMax_;
}

//------------------------------------------------------------------
float Meteo::pluie() const {
	FILE_LOG(logDEBUG) << "Meteo::pluie()";
	return pluie_;
}

//------------------------------------------------------------------
float Meteo::neige() const {
	FILE_LOG(logDEBUG) << "Meteo::neige()";
	return neige_;
}

//------------------------------------------------------------------
const std::vector<float>& Meteo::meteoFonte() const {
	return meteoFonte_;
}

//------------------------------------------------------------------
const std::vector<float>& Meteo::meteoEvapo() const {
	return meteoEvapo_;
}

//------------------------------------------------------------------
const std::vector<float>& Meteo::meteoAutre() const {
	return meteoAutre_;
}

//------------------------------------------------------------------
void Meteo::meteoFonte(const std::vector<float>& donneesMeteo) {
	meteoFonte_ = donneesMeteo;
}

//------------------------------------------------------------------
void Meteo::meteoEvapo(const std::vector<float>& donneesMeteo) {
	meteoEvapo_ = donneesMeteo;
}

//------------------------------------------------------------------
void Meteo::meteoAutre(const std::vector<float>& donneesMeteo) {
	meteoAutre_ = donneesMeteo;
}

//------------------------------------------------------------------
float Meteo::calculerTempMoy() const {
	FILE_LOG(logDEBUG) << "Meteo::calculerTempMoy()";
	return (tMax_ + tMin_) / 2.0f;
}

//------------------------------------------------------------------
int Meteo::idCarreauEntier() const {
	FILE_LOG(logDEBUG) << "Meteo::idCarreauEntier()";
	return idCarreauEntier_;
}

//------------------------------------------------------------------
void Meteo::idCarreauEntier(int idCarreauEntier) {
	FILE_LOG(logDEBUG) << "Meteo::idCarreauEntier(const int idCarreauEntier)";
	idCarreauEntier_ = idCarreauEntier;
}
