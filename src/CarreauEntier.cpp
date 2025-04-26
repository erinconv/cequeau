//****************************************************************************
// Fichier: CarreauEntier.cpp
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
#include "CarreauEntier.h"

//------------------------------------------------------------------
CarreauEntier::CarreauEntier(int id, int pctLacRiviere, int pctForet, int pctMarais, int pctLacExutoire)
	: id_(id), pctLacRiviere_(pctLacRiviere), pctForet_(pctForet), pctMarais_(pctMarais), pctLacExutoire_(pctLacExutoire) {
	FILE_LOG(logDEBUG) << "CarreauEntier::CarreauEntier(int id, int pctLacRiviere, int pctForet, int pctMarais)";
}

//------------------------------------------------------------------
CarreauEntier::CarreauEntier(int id, int i, int j)
	: id_(id), i_(i), j_(j) {
	FILE_LOG(logDEBUG) << "CarreauEntier::CarreauEntier(int id, int i, int j)";
}

//------------------------------------------------------------------
CarreauEntier::CarreauEntier(int id, int i, int j, int pctLacRiviere, int pctForet, int pctMarais, int pctLacExutoire, float altitude)
	: id_(id), i_(i), j_(j), pctLacRiviere_(pctLacRiviere), pctForet_(pctForet), pctMarais_(pctMarais), pctLacExutoire_(pctLacExutoire), altitude_(altitude) {
	FILE_LOG(logDEBUG) << "CarreauEntier::CarreauEntier(int id, int i, int j, int pctLacRiviere, int pctForet, int pctMarais, float altitude)";
}

//------------------------------------------------------------------
CarreauEntier::CarreauEntier(int id, int i, int j, int pctLacRiviere, int pctForet, int pctMarais, int pctLacExutoire, float altitude, float latitude, float longitude)
	: id_(id), i_(i), j_(j), pctLacRiviere_(pctLacRiviere), pctForet_(pctForet), pctMarais_(pctMarais), pctLacExutoire_(pctLacExutoire), altitude_(altitude), latitude_(latitude), longitude_(longitude) {
	FILE_LOG(logDEBUG) << "CarreauEntier::CarreauEntier(int id, int i, int j, int pctLacRiviere, int pctForet, int pctMarais, float altitude, float latitude, float longitude)";
}

//------------------------------------------------------------------
CarreauEntier::CarreauEntier(int id, int i, int j, int pctLacRiviere, int pctForet, int pctMarais, int pctLacExutoire, float altitude, float latitude, float longitude, float meanSlope)
	: id_(id), i_(i), j_(j), pctLacRiviere_(pctLacRiviere), pctForet_(pctForet), pctMarais_(pctMarais), pctLacExutoire_(pctLacExutoire), altitude_(altitude), latitude_(latitude), longitude_(longitude), meanSlope_(meanSlope) {
	FILE_LOG(logDEBUG) << "CarreauEntier::CarreauEntier(int id, int i, int j, int pctLacRiviere, int pctForet, int pctMarais, float altitude, float latitude, float longitude, float meanSlope)";
}

//------------------------------------------------------------------
CarreauEntier::~CarreauEntier() {
	FILE_LOG(logDEBUG) << "CarreauEntier::~CarreauEntier()";
}

//------------------------------------------------------------------
int CarreauEntier::id() const {
	FILE_LOG(logDEBUG) << "CarreauEntier::id()";
	return id_;
}

//------------------------------------------------------------------
int CarreauEntier::i() const {
	FILE_LOG(logDEBUG) << "CarreauEntier::i()";
	return i_;
}

//------------------------------------------------------------------
int CarreauEntier::j() const {
	FILE_LOG(logDEBUG) << "CarreauEntier::j()";
	return j_;
}

//------------------------------------------------------------------
void CarreauEntier::i(int val) {
	FILE_LOG(logDEBUG) << "CarreauEntier::i(int val)";
	i_ = val;
}

//------------------------------------------------------------------
void CarreauEntier::j(int val) {
	FILE_LOG(logDEBUG) << "CarreauEntier::j(int val)";
	j_ = val;
}

//------------------------------------------------------------------
float CarreauEntier::pctLacRiviere() const {
	FILE_LOG(logDEBUG) << "CarreauEntier::pctLacRiviere()";
	float pct = (float)pctLacRiviere_ / 100.0f;
	return pct;
}

//------------------------------------------------------------------
float CarreauEntier::pctForet() const {
	FILE_LOG(logDEBUG) << "CarreauEntier::pctForet()";
	float pct = (float)pctForet_ / 100.0f;
	return pct;
}

//------------------------------------------------------------------
float CarreauEntier::pctMarais() const {
	FILE_LOG(logDEBUG) << "CarreauEntier::pctMarais()";
	float pct = (float)pctMarais_ / 100.0f;
	return pct;
}

//------------------------------------------------------------------
float CarreauEntier::pctLacExutoire() const {
	FILE_LOG(logDEBUG) << "CarreauEntier::pctLacExutoire()";
	float pct = (float)pctLacExutoire_ / 100.0f;
	return pct;
}

//------------------------------------------------------------------
void CarreauEntier::pctLacRiviere(int pctLacRiviere) {
	pctLacRiviere_ = pctLacRiviere;
}

//------------------------------------------------------------------
void CarreauEntier::pctForet(int pctForet) {
	pctForet_ = pctForet;
}

//------------------------------------------------------------------
void CarreauEntier::pctMarais(int pctMarais) {
	pctMarais_ = pctMarais;
}

//------------------------------------------------------------------
void CarreauEntier::pctLacExutoire(int pctLacExutoire) {
	pctLacExutoire_ = pctLacExutoire;
}

//------------------------------------------------------------------
float CarreauEntier::pctEau() const {
	FILE_LOG(logDEBUG) << "CarreauEntier::pctEau()";
	return pctLacRiviere() + pctMarais() + pctLacExutoire();
}

//------------------------------------------------------------------
float CarreauEntier::pctSol() const {
	FILE_LOG(logDEBUG) << "CarreauEntier::pctSol()";
	return 1.0f - pctEau();
}

//------------------------------------------------------------------
float CarreauEntier::calculerCoeffPonderation() const {
	FILE_LOG(logDEBUG) << "CarreauEntier::calculerCoeffPonderation()";
	float pctSurfaceCE = 1.0f;
	float pctEauForet = pctEau() + pctForet();

	float calcul = (4.0f + (pctSurfaceCE * pctEauForet)) / (4.0f + pctSurfaceCE);

	return calcul;
}

//------------------------------------------------------------------
ParamCE& CarreauEntier::param() {
	FILE_LOG(logDEBUG) << "CarreauEntier::param()";
	return param_;
}

//------------------------------------------------------------------
const ParamCE& CarreauEntier::param() const {
	FILE_LOG(logDEBUG) << "CarreauEntier::param()";
	return param_;
}

//------------------------------------------------------------------
void CarreauEntier::altitude(float val) {
	altitude_ = val;
}

//------------------------------------------------------------------
float CarreauEntier::altitude() const {
	return altitude_;
}

//------------------------------------------------------------------
float CarreauEntier::latitude() const {
    FILE_LOG(logDEBUG) << "CarreauEntier::latitude()";

    return latitude_;
}

//------------------------------------------------------------------
float CarreauEntier::longitude() const {
    FILE_LOG(logDEBUG) << "CarreauEntier::longitude()";

    return longitude_;
}

//------------------------------------------------------------------
float CarreauEntier::meanSlope() const {
    FILE_LOG(logDEBUG) << "CarreauEntier::meanSlope()";

    return meanSlope_;
}
