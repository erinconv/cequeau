//****************************************************************************
// Fichier: BassinVersant.cpp
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
#include "BassinVersant.h"

//------------------------------------------------------------------
BassinVersant::BassinVersant() {
	FILE_LOG(logDEBUG) << "BassinVersant::BassinVersant()";
}

//------------------------------------------------------------------
BassinVersant::~BassinVersant() {
	FILE_LOG(logDEBUG) << "BassinVersant::~BassinVersant()";
}

//------------------------------------------------------------------
std::string BassinVersant::nomBassinVersant() const {
	FILE_LOG(logDEBUG) << "BassinVersant::nomBassinVersant()";
	return nomBassinVersant_;
}

//------------------------------------------------------------------
int BassinVersant::nbCarreauxEntiers() const {
	FILE_LOG(logDEBUG) << "BassinVersant::nbCarreauxEntiers()";
	return nbCarreauxEntiers_;
}

//------------------------------------------------------------------
int BassinVersant::nbCarreauxPartiels() const {
	FILE_LOG(logDEBUG) << "BassinVersant::nbCarreauxPartiels()";
	return nbCarreauxPartiels_;
}

//------------------------------------------------------------------
int BassinVersant::nbCarreauxPartielsCheminLong() const {
	FILE_LOG(logDEBUG) << "BassinVersant::nbCarreauxPartiels()";
	return nbCarreauxPartielsCheminLong_;
}

//------------------------------------------------------------------
int BassinVersant::nbBarrages() const {
	FILE_LOG(logDEBUG) << "BassinVersant::nbBarrages()";
	return (int)barrages_.size();
}

//------------------------------------------------------------------
float BassinVersant::superficieCarreauEntier() const {
	FILE_LOG(logDEBUG) << "BassinVersant::superficieCarreauEntier()";
	return superficieCarreauEntier_;
}

//------------------------------------------------------------------
const std::vector<CarreauEntierPtr>& BassinVersant::carreauxEntiers() const {
	FILE_LOG(logDEBUG) << "BassinVersant::carreauxEntiers()";
	return carreauxEntiers_;
}

//------------------------------------------------------------------
const std::vector<CarreauPartielPtr>& BassinVersant::carreauxPartiels() const {
	FILE_LOG(logDEBUG) << "BassinVersant::carreauxPartiels()";
	return carreauxPartiels_;
}

//------------------------------------------------------------------
std::vector<BarragePtr>& BassinVersant::barrages() {
	FILE_LOG(logDEBUG) << "BassinVersant::barrages()";
	return barrages_;
}

//------------------------------------------------------------------
bool BassinVersant::possedeBarrage() const {
	FILE_LOG(logDEBUG) << "BassinVersant::possedeBarrage()";
	if (barrages_.size() > 0)
		return true;
	else
		return false;
}

//------------------------------------------------------------------
bool BassinVersant::possedeBarrage(int idCP) const {
	FILE_LOG(logDEBUG) << "BassinVersant::possedeBarrage(int idCP)";

	if (listeCpBarrage_.count(idCP) > 0)
		return true;
	else
		return false;
}

//------------------------------------------------------------------
const BarragePtr BassinVersant::trouverBarrageParIdCpAval(int idCP) const {
	FILE_LOG(logDEBUG) << "BassinVersant::trouverBarrageParIdCpAval(int idCP)";

	std::vector<BarragePtr>::const_iterator barragesIter = barrages_.begin();
	for (; barragesIter != barrages_.end(); barragesIter++) {
		if (barragesIter->get()->idCPAval() == idCP) {
			return *barragesIter;
		}
	}

	// Pas trouve. Retour pointeur nul.
	return BarragePtr();
}

//------------------------------------------------------------------
const BarragePtr BassinVersant::trouverBarrageParIdCP(int idCP) const {
	FILE_LOG(logDEBUG) << "BassinVersant::trouverBarrageParIdCP(int idCP)";

	std::vector<BarragePtr>::const_iterator barragesIter = barrages_.begin();
	for (; barragesIter != barrages_.end(); barragesIter++) {
		if (barragesIter->get()->idCP() == idCP) {
			return *barragesIter;
		}
	}

	// Pas trouve. Retour pointeur nul.
	return BarragePtr();
}

//------------------------------------------------------------------
const CarreauEntierPtr BassinVersant::trouverCarreauEntierParId(int id) const {
	FILE_LOG(logDEBUG) << "BassinVersant::trouverCarreauEntierParId(int id)";

	/* Lent. De toute maniere les carreaux sont tries pas ID.
	std::vector<CarreauEntierPtr>::const_iterator carreauxEntiersIterator = carreauxEntiers_.begin();
	for (; carreauxEntiersIterator != carreauxEntiers_.end(); carreauxEntiersIterator++) {
	  if (carreauxEntiersIterator->get()->id() == id)
		break;
	}

	return *carreauxEntiersIterator;
	*/

	return carreauxEntiers_[id - 1];
}

//------------------------------------------------------------------
const std::map<int, std::vector<PuitsPtr>>& BassinVersant::puits() const {
  	return puits_;
}

//------------------------------------------------------------------
const std::vector<PuitsPtr>& BassinVersant::puitsParIdCE(int idCE) const {
    auto it = puits_.find(idCE);
    if (it != puits_.end()) {
        return it->second;
    } else {
	static const std::vector<PuitsPtr> empty_vector;
        return empty_vector; // Return a reference to a static empty vector
    }
}

//------------------------------------------------------------------
void BassinVersant::initialiserCarreauxPartiels(const mxArray* bassinVersant) {
	FILE_LOG(logDEBUG) << "initialiserCarreauxPartiels(mxArray* bassinVersant)";

	char  codeCarreauPartiel;
	int   pctSurface, iCarreauEntier, jCarreauEntier, idCarreauPartielAval;
	int   idCarreauxPartielsAmont[NB_CP_AMONT];
	int   idCarreauEntier, pctEau, pctForet, pctMarais, pctSolNu, altitudeMoyenne;
	int   profondeurMinimum, longueurCoursEauPrincipal, largeurCoursEauPrincipal;
	int   penteRiviere, cumulPctSuperficieCPAmont, cumulPctSuperficieLacsAmont;
	int   cumulPctSuperficieMaraisAmont, cumulPctSuperficieForetAmont;
	int   pctFermetureFutaie;
	float indiceFoliaire, custom_1, custom_2;
	int lat = 0;
	int lon = 0;
	int pctFeuillue, pctConifer;
	double hautMoyenneArbre;
	int azimutCoursEau;



	mxArray* carreauPartiel = MexHelper::mhMxGetField(bassinVersant, 0, "carreauxPartiels");
	int nbCP = (int)mxGetNumberOfElements(carreauPartiel);

	nbCarreauxPartiels_ = nbCP;
	carreauxPartiels_.reserve(nbCarreauxPartiels_);
	// TODO: Here, the names of the field lat-lon must be changed to Latitude, Longitude. 
	// TODO: Also, the type of latitude and longitude must be float
	bool possedeLat = MexHelper::hasField(carreauPartiel,0,"lat");
	bool possedeLon = MexHelper::hasField(carreauPartiel,0,"lon");
	for (int k = 0; k < nbCP; k++) {
		MexHelper::chargerValeurs(carreauPartiel, "i", iCarreauEntier, k);
		MexHelper::chargerValeurs(carreauPartiel, "j", jCarreauEntier, k);
		MexHelper::chargerValeurs(carreauPartiel, "code", codeCarreauPartiel, k);
		MexHelper::chargerValeurs(carreauPartiel, "pctSurface", pctSurface, k);
		MexHelper::chargerValeurs(carreauPartiel, "idCPAval", idCarreauPartielAval, k);
		MexHelper::chargerValeurs(carreauPartiel, "idCPsAmont", idCarreauxPartielsAmont, NB_CP_AMONT, k);
		MexHelper::chargerValeurs(carreauPartiel, "idCE", idCarreauEntier, k);
		MexHelper::chargerValeurs(carreauPartiel, "pctEau", pctEau, k);
		MexHelper::chargerValeurs(carreauPartiel, "pctForet", pctForet, k);
		MexHelper::chargerValeurs(carreauPartiel, "pctMarais", pctMarais, k);
		MexHelper::chargerValeurs(carreauPartiel, "pctSolNu", pctSolNu, k);
		MexHelper::chargerValeurs(carreauPartiel, "altitudeMoy", altitudeMoyenne, k);
		MexHelper::chargerValeurs(carreauPartiel, "profondeurMin", profondeurMinimum, k);
		MexHelper::chargerValeurs(carreauPartiel, "longueurCoursEauPrincipal", longueurCoursEauPrincipal, k);
		MexHelper::chargerValeurs(carreauPartiel, "largeurCoursEauPrincipal", largeurCoursEauPrincipal, k);
		MexHelper::chargerValeurs(carreauPartiel, "penteRiviere", penteRiviere, k);
		MexHelper::chargerValeurs(carreauPartiel, "cumulPctSuperficieCPAmont", cumulPctSuperficieCPAmont, k);
		MexHelper::chargerValeurs(carreauPartiel, "cumulPctSuperficieLacsAmont", cumulPctSuperficieLacsAmont, k);
		MexHelper::chargerValeurs(carreauPartiel, "cumulPctSuperficieMaraisAmont", cumulPctSuperficieMaraisAmont, k);
		MexHelper::chargerValeurs(carreauPartiel, "cumulPctSuperficieForetAmont", cumulPctSuperficieForetAmont, k);

		MexHelper::chargerValeurs(carreauPartiel, "pctFermetureFutaie", pctFermetureFutaie, -1, k);
		MexHelper::chargerValeurs(carreauPartiel, "indiceFoliaire", indiceFoliaire, -1.0f, k);
		MexHelper::chargerValeurs(carreauPartiel, "custom_1", custom_1, -1.0f, k);
		MexHelper::chargerValeurs(carreauPartiel, "custom_2", custom_2, -1.0f, k);
		
		// This was added to keep the compatibility with old structures
		if (possedeLat)
		{
			MexHelper::chargerValeurs(carreauPartiel, "lat", lat, k);
		}
		if (possedeLon)
		{
			MexHelper::chargerValeurs(carreauPartiel, "lon", lon, k);
		}
		if (MexHelper::hasField(carreauPartiel, 0, "pctFeuillue")) MexHelper::chargerValeurs(carreauPartiel, "pctFeuillue", pctFeuillue, k); // inutile
		if (MexHelper::hasField(carreauPartiel, 0, "pctConifer")) MexHelper::chargerValeurs(carreauPartiel, "pctConifer", pctConifer, k); // inutile
		if (MexHelper::hasField(carreauPartiel, 0, "hautMoyenneArbre")) MexHelper::chargerValeurs(carreauPartiel, "hautMoyenneArbre", hautMoyenneArbre, k);
		if (MexHelper::hasField(carreauPartiel, 0, "azimutCoursEau")) MexHelper::chargerValeurs(carreauPartiel, "azimutCoursEau", azimutCoursEau, k);


		carreauxPartiels_.push_back(CarreauPartielPtr(
			new CarreauPartiel(k + 1,
				codeCarreauPartiel,
				pctSurface,
				iCarreauEntier,
				jCarreauEntier,
				idCarreauPartielAval,
				idCarreauxPartielsAmont,
				idCarreauEntier,
				pctEau,
				pctForet,
				pctMarais,
				pctSolNu,
				altitudeMoyenne,
				profondeurMinimum,
				longueurCoursEauPrincipal,
				largeurCoursEauPrincipal,
				penteRiviere,
				cumulPctSuperficieCPAmont,
				cumulPctSuperficieLacsAmont,
				cumulPctSuperficieMaraisAmont,
				cumulPctSuperficieForetAmont,
				superficieCarreauEntier_,
				pctFermetureFutaie,
				indiceFoliaire,
				custom_1,
				custom_2,
				lat,
				lon,
				pctFeuillue,
				pctConifer,
				hautMoyenneArbre,
				azimutCoursEau))); 
	
	}

	
}

//------------------------------------------------------------------
void BassinVersant::initialiserCarreauxEntiers(const mxArray* bassinVersant) {
	FILE_LOG(logDEBUG) << "BassinVersant::initialiserCarreauxEntiers(mxArray* bassinVersant)";

	int i, j, pctLacRiviere, pctForet, pctMarais;
	float latitude = -999.0f;
	float longitude = -999.0f;
	float altitude = -1.0f;
	float meanSlope = 0.0f;

	mxArray* carreauEntier = MexHelper::mhMxGetField(bassinVersant, 0, "carreauxEntiers");
	int nbCE = (int)mxGetNumberOfElements(carreauEntier);

	nbCarreauxEntiers_ = nbCE;
	carreauxEntiers_.reserve(nbCarreauxEntiers_);

	// L'altitude est optionnelle
	bool possedeAltitude = MexHelper::hasField(carreauEntier, 0, "altitude");
	// Check if there is latitute-longitude
	bool possedeLat = MexHelper::hasField(carreauEntier,0,"Latitude");
	bool possedeLon = MexHelper::hasField(carreauEntier,0,"Longitude");
	bool possedeSlope = MexHelper::hasField(carreauEntier,0,"meanSlope");
	for (int k = 0; k < nbCE; k++) {
		MexHelper::chargerValeurs(carreauEntier, "i", i, k);
		MexHelper::chargerValeurs(carreauEntier, "j", j, k);
		MexHelper::chargerValeurs(carreauEntier, "pctLacRiviere", pctLacRiviere, k);
		MexHelper::chargerValeurs(carreauEntier, "pctForet", pctForet, k);
		MexHelper::chargerValeurs(carreauEntier, "pctMarais", pctMarais, k);

		if (possedeAltitude) {
			MexHelper::chargerValeurs(carreauEntier, "altitude", altitude, k);
		}
		if (possedeLat) {
			MexHelper::chargerValeurs(carreauEntier, "Latitude", latitude, k);
		}
		if (possedeLon) {
			MexHelper::chargerValeurs(carreauEntier, "Longitude", longitude, k);
		}
		if (possedeSlope) {
			MexHelper::chargerValeurs(carreauEntier, "meanSlope", meanSlope, k);
		}
		

		carreauxEntiers_.push_back(CarreauEntierPtr(new CarreauEntier(k + 1, i, j, pctLacRiviere, pctForet, pctMarais, 0, altitude, latitude, longitude, meanSlope)));
	}
}

//------------------------------------------------------------------
void BassinVersant::initialiserBarrage(int idCP, int idCPAval, float volumeInitial, ParamEquation& coeffNiveau, ParamEquation& coeffDebit) {
	FILE_LOG(logDEBUG) << "initialiserBarrages_Type2";
	barrages_.push_back(BarragePtr(new Barrage(idCP, idCPAval, volumeInitial, coeffNiveau, coeffDebit)));
	listeCpBarrage_.insert(std::make_pair(idCP, true));
}

//------------------------------------------------------------------
void BassinVersant::initialiserBarrage(int idCP, int idCPAval, float volumeInitial, ParamEquation& coeffNiveau, ParamEquation& coeffVolume,
	std::vector<ParamEquation>& coeffDebitInt, std::vector<InfoEvacuationExt>& coeffDebitExt) {
	FILE_LOG(logDEBUG) << "initialiserBarrages_Type5";
	barrages_.push_back(BarragePtr(new Barrage(idCP, idCPAval, volumeInitial, coeffNiveau, coeffVolume, coeffDebitInt, coeffDebitExt)));
	listeCpBarrage_.insert(std::make_pair(idCP, true));
}

//------------------------------------------------------------------
void BassinVersant::initialiserBarrage(int idCPAval, std::vector<float>& debitsExternesMesures) {
	FILE_LOG(logDEBUG) << "initialiserBarrages_Type3";
	barrages_.push_back(BarragePtr(new Barrage(idCPAval, debitsExternesMesures)));
	// Pas besoin de la ligne suivante car le barrage est hors du bassin versant.
	// listeCpBarrage_.insert(std::make_pair(0, true));
}

//------------------------------------------------------------------
#ifdef ENV_MATLAB
void BassinVersant::initialiserFichier(const std::string nomFichierintrants) {
	FILE_LOG(logDEBUG) << "BassinVersant::initialiserFichier(const std::string nomFichierintrants)";

	MATFile* pFichierParam = MexHelper::mhMatOpen(nomFichierintrants, "r");
	mxArray* bv = MexHelper::mhMatGetStructVariable(pFichierParam, "bassinVersant");

	initialiser(bv);

	matClose(pFichierParam);
	mxDestroyArray(bv);
}
#endif

//------------------------------------------------------------------
void BassinVersant::initialiser(const mxArray* bassinVersant) {
	FILE_LOG(logDEBUG) << "BassinVersant::initialiser(const mxArray* bassinVersant)";

	MexHelper::chargerValeurs(bassinVersant, "nbCpCheminLong", nbCarreauxPartielsCheminLong_);
	MexHelper::chargerValeurs(bassinVersant, "superficieCE", superficieCarreauEntier_);

	MexHelper::chargerValeurs(bassinVersant, "nomBassinVersant", nomBassinVersant_);

	// Chargement du ou des barrages s'il existe.
	mxArray* barrage = mxGetField(bassinVersant, 0, "barrage");
	if (barrage != NULL) {
		if (mxIsStruct(barrage)) {
			int idCP, idCPAval, typeBarrage;
			float volumeInitial = 0.0f;
			ParamEquation niveau, debit, volume;
			std::vector<ParamEquation> debitsInterne;
			std::vector<InfoEvacuationExt> debitsExterne;
			std::vector<float> debitsMesures;
			int nbBarrages = (int)mxGetNumberOfElements(barrage);

			for (int i = 0; i < nbBarrages; i++) {
				MexHelper::chargerValeurs(barrage, "idCP", idCP, i);
				MexHelper::chargerValeurs(barrage, "idCPAval", idCPAval, i);
				MexHelper::chargerValeurs(barrage, "type", typeBarrage, i);
				MexHelper::chargerValeurs(barrage, "volumeInitial", volumeInitial, i);

				std::stringstream avertissement;
				switch (typeBarrage) {
				case EVACUATION_MESUREE_INT:
					avertissement << "Type de barrage non-supporte." << std::endl;
					mexPrintf(avertissement.str().c_str());
					FILE_LOG(logWARNING) << avertissement.str();
					break;
				case EVACUATION_MESUREE_EXT:
					MexHelper::chargerValeurs(barrage, "debit", debitsMesures, i);
					initialiserBarrage(idCPAval, debitsMesures);
					break;
				case EVACUATION_CALCULEE:
					MexHelper::chargerValeurs(barrage, "niveau", niveau, i);
					MexHelper::chargerValeurs(barrage, "debit", debit, i);
					initialiserBarrage(idCP, idCPAval, volumeInitial, niveau, debit);
					break;
				case EVACUATION_CALCULEE_SPECIAL:
					MexHelper::chargerValeurs(barrage, "niveau", niveau, i);
					MexHelper::chargerValeurs(barrage, "volume", volume, i);
					MexHelper::chargerValeurs(barrage, "debitsInterne", debitsInterne, i);
					MexHelper::chargerValeurs(barrage, "debitsExterne", debitsExterne, i);
					initialiserBarrage(idCP, idCPAval, volumeInitial, niveau, volume, debitsInterne, debitsExterne);
					break;
				default:
					avertissement << "Type de barrage non-supporte." << std::endl;
					mexPrintf(avertissement.str().c_str());
					FILE_LOG(logWARNING) << avertissement.str();
					break;
				}
			}
		}
	}

	// Chargement du ou des barrages s'il existe.
	mxArray* puits = mxGetField(bassinVersant, 0, "puits");
	if (puits != NULL) {
		if (mxIsStruct(puits)) {
			int idCE;
			int active;
			double distanceRiviere;
			double h0;
			double rayonP = 1.0f;;
			std::vector<double> niveauxPuits;
			std::vector<double> debitPompage;

			int nbPuits  = (int)mxGetNumberOfElements(puits);
			for (int i = 0; i < nbPuits; i++) {

				MexHelper::chargerValeurs(puits, "idCE", idCE, i);
				MexHelper::chargerValeurs(puits, "active", active, i);
				MexHelper::chargerValeurs(puits, "distanceRiviere", distanceRiviere, i);
				MexHelper::chargerValeurs(puits, "h0", h0, i);

				
				MexHelper::chargerValeurs(puits, "niveauxPuits", niveauxPuits, i);
				MexHelper::chargerValeurs(puits, "debitPompage", debitPompage, i);

				if (MexHelper::hasField(puits,0,"rayonP")) {
					MexHelper::chargerValeurs(puits, "rayonP", rayonP, i);
				}

				puits_[idCE].push_back(PuitsPtr(new Puits(idCE, active, distanceRiviere, h0, rayonP, niveauxPuits, debitPompage)));
			}
		}
	}

	initialiserCarreauxEntiers(bassinVersant);
	initialiserCarreauxPartiels(bassinVersant);
}
