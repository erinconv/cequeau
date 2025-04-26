//****************************************************************************
// Fichier: FonteCequeau.cpp
//
// Date creation: 2014-03-11
// Auteur: 
//                Rio Tinto Alcan                     
//                Energie electrique                  
//                1954 Davis, Saguenay arr. Jonquiere,
//                G7S 4R7, QC, Canada                 
//
//****************************************************************************
#include "stdafx.h"
#include "FonteCequeau.h"

//------------------------------------------------------------------
FonteCequeau::FonteCequeau() {
}

//------------------------------------------------------------------
FonteCequeau::FonteCequeau(int latitudeMoyenneBV, int nbCE)
	: Fonte(nbCE, "FonteCequeau") {
	int degresEnMinutes = (latitudeMoyenneBV / 100) * 60;
	int minutes = latitudeMoyenneBV % 100;

	constModulationSoleil_ = tan((float)(degresEnMinutes + minutes) * PI / 10800.0f);

	// Nom de champs d'un etat CE
	nomChamps_.push_back("stockNeigeForet");
	nomChamps_.push_back("stockNeigeClairiere");
	nomChamps_.push_back("indexMurissementNeige");
	nomChamps_.push_back("indexTempNeige");
	nomChamps_.push_back("eauDisponible");
}

//------------------------------------------------------------------
FonteCequeau::~FonteCequeau() {
}

//------------------------------------------------------------------
int FonteCequeau::calculerFonte(
	// IN
	const DateChrono& datePasDeTemps,
	const Meteo& meteo,
	const CarreauEntier& carreauEntier,
	// OUT
	float& precipationsTotales,
	float& eauDisponible
) {

	// Pour faciliter la comprehension, les variables sont prefixees du nom des variables
	// telles que definies dans le manuel Cequeau.

	const int indexCE = carreauEntier.id() - 1;
	// Nouveau pas de temps
	if (indexCE == 0) {
		etatsFonteCE_.clear();
		etatsFonteCE_.reserve(nbCE_);
	}

	EtatFonteCE& etatFonteCE = etatsFonte_.back().at(indexCE);

	float PJE_pluie, PJN_neige;
	float TJE_tempMoy = meteo.calculerTempMoy();
	float seuilTranformationPluieNeige = params_.seuilTranformationPluieNeige.at(indexCE);
	float neigeMeteo = meteo.neige(), pluieMeteo = meteo.pluie();
	// Si la temperature moyenne est inferieure au seuil de transformation - 2 deg,
	// la pluie est transformee en neige.
	if (TJE_tempMoy <= seuilTranformationPluieNeige - 2.0) {
		PJE_pluie = 0.0;
		PJN_neige = neigeMeteo + pluieMeteo;
	}
	// Si la temperature moyenne est comprise entre le seuil de transformation +/- 2 deg,
	// la pluie est transformee partiellement en neige.
	else if (TJE_tempMoy <= seuilTranformationPluieNeige + 2.0) {
		float facteurTransformation = std::abs(TJE_tempMoy - seuilTranformationPluieNeige - 2.0f) / 4.0f;
		PJE_pluie = pluieMeteo * (1.0f - facteurTransformation);
		PJN_neige = pluieMeteo * facteurTransformation + neigeMeteo;
	} else {
		PJE_pluie = pluieMeteo;
		PJN_neige = neigeMeteo;
	}

	float QNUI3_indexMurissementNeige = etatFonteCE.QNUI3_indexMurissementNeige;
	float QNUI4_indexTempNeige = etatFonteCE.QNUI4_indexTempNeige;
	float SNC_stockNeigeForet = etatFonteCE.SNC_stockNeigeForet + PJN_neige;
	float SND_stockNeigeClairiere = etatFonteCE.SND_stockNeigeClairiere + PJN_neige;

	float SNDDD_stockNeigeClairiereDebut = SND_stockNeigeClairiere;
	float PJD_pluieClairiere = PJE_pluie;
	float PJC_pluieForet = PJE_pluie;
	int noJour = datePasDeTemps.getDayOfYear();
	float facteurModulationSoleilNeige = static_cast<float>(calculerFacteurModulationSoleil(noJour));
	float TFDS_tauxPotentielFonteClairiere = params_.tauxPotentielFonteClairiere.at(indexCE);;
	float TSDS_seuilTempFonteClairiere = params_.seuilTempFonteClairiere.at(indexCE);;
	float TFCS_tauxPotentielFonteForet = params_.tauxPotentielFonteForet.at(indexCE);;
	float TSCS_seuilTempFonteForet = params_.seuilTempFonteForet.at(indexCE);;
	float TTSS_tempMurissementNeige = params_.tempMurissementNeige.at(indexCE);;
	float TTD_coeffDeficitCalorique = params_.coeffDeficitCalorique;

	// Fonte potentielle
	float TED_fonteClairiere = TFDS_tauxPotentielFonteClairiere *
		std::max<float>(0.0f, (TJE_tempMoy - TSDS_seuilTempFonteClairiere)) *
		facteurModulationSoleilNeige;

	// Fonte potentielle
	float TEC_fonteForet = TFCS_tauxPotentielFonteForet *
		std::max<float>(0.0f, (TJE_tempMoy - TSCS_seuilTempFonteForet)) *
		facteurModulationSoleilNeige;

	QNUI3_indexMurissementNeige += std::max<float>(0.0f, (TJE_tempMoy - TTSS_tempMurissementNeige));

	QNUI4_indexTempNeige = (QNUI4_indexTempNeige * TTD_coeffDeficitCalorique) +
		(TJE_tempMoy * (1.0f - TTD_coeffDeficitCalorique));

	if (SND_stockNeigeClairiere < 10.0) {
		if (SNC_stockNeigeForet <= 0.254f) {
			QNUI3_indexMurissementNeige = 0.0f;
		}
		QNUI4_indexTempNeige = TSDS_seuilTempFonteClairiere;
	} else if (QNUI4_indexTempNeige > TSDS_seuilTempFonteClairiere || PJE_pluie <= 0.00f) {
		if ((QNUI3_indexMurissementNeige * TFDS_tauxPotentielFonteClairiere) > SNC_stockNeigeForet) {
			if (SNC_stockNeigeForet <= 0.254f) {
				QNUI3_indexMurissementNeige = 0.0f;
			}
			QNUI4_indexTempNeige = TSDS_seuilTempFonteClairiere;
		} else {
			TED_fonteClairiere = TED_fonteClairiere *
				std::min<float>(1.0f, (QNUI3_indexMurissementNeige * TFDS_tauxPotentielFonteClairiere) / (SND_stockNeigeClairiere + 1.0f));
			TEC_fonteForet = TEC_fonteForet *
				std::min<float>(1.0f, (QNUI3_indexMurissementNeige * TFCS_tauxPotentielFonteForet) / (SNC_stockNeigeForet + 1.0f));
		}
	} else {
		float TND_pluieEnNeigeClairiere = (TSDS_seuilTempFonteClairiere - QNUI4_indexTempNeige) * SND_stockNeigeClairiere / 88.888f;
		float TNC_pluieEnNeigeForet = (TSDS_seuilTempFonteClairiere - QNUI4_indexTempNeige) * SNC_stockNeigeForet / 88.888f;

		if (TND_pluieEnNeigeClairiere >= PJE_pluie) {
			SND_stockNeigeClairiere = SND_stockNeigeClairiere + PJE_pluie;
			PJD_pluieClairiere = 0.0f;
		} else {
			PJD_pluieClairiere = PJE_pluie - TND_pluieEnNeigeClairiere;
			SND_stockNeigeClairiere = SND_stockNeigeClairiere + TND_pluieEnNeigeClairiere;
		}

		if (TNC_pluieEnNeigeForet >= PJE_pluie) {
			SNC_stockNeigeForet = SNC_stockNeigeForet + PJE_pluie;
			PJC_pluieForet = 0.0f;
		} else {
			PJC_pluieForet = PJE_pluie - TNC_pluieEnNeigeForet;
			SNC_stockNeigeForet = SNC_stockNeigeForet + TNC_pluieEnNeigeForet;
		}

		if ((QNUI3_indexMurissementNeige * TFDS_tauxPotentielFonteClairiere) > SNC_stockNeigeForet) {
			if (SNC_stockNeigeForet <= 0.254f) {
				QNUI3_indexMurissementNeige = 0.0f;
			}
			QNUI4_indexTempNeige = TSDS_seuilTempFonteClairiere;
		} else {
			TED_fonteClairiere = TED_fonteClairiere *
				std::min<float>(1.0f, (QNUI3_indexMurissementNeige * TFDS_tauxPotentielFonteClairiere) / (SND_stockNeigeClairiere + 1.0f));
			TEC_fonteForet = TEC_fonteForet *
				std::min<float>(1.0f, (QNUI3_indexMurissementNeige * TFCS_tauxPotentielFonteForet) / (SNC_stockNeigeForet + 1.0f));
		}
	}

	TEC_fonteForet = std::min<float>(TEC_fonteForet, SNC_stockNeigeForet);
	TED_fonteClairiere = std::min<float>(TED_fonteClairiere, SND_stockNeigeClairiere);

	float stockNeigeForet = SNC_stockNeigeForet - TEC_fonteForet;
	if (stockNeigeForet < 0.0f)
		stockNeigeForet = 0.0f;

	float stockNeigeClairiere = SND_stockNeigeClairiere - TED_fonteClairiere;
	if (stockNeigeClairiere < 0.0f)
		stockNeigeClairiere = 0.0f;

	QNUI3_indexMurissementNeige = QNUI3_indexMurissementNeige *
		std::min<float>(1.0f, SND_stockNeigeClairiere / (SNDDD_stockNeigeClairiereDebut + 1.0f));

	// Reuslats du calcul de la fonte:
	// 1- Preservation des nouveaux etats
	etatFonteCE_.SNC_stockNeigeForet = stockNeigeForet;
	etatFonteCE_.SND_stockNeigeClairiere = stockNeigeClairiere;
	etatFonteCE_.QNUI3_indexMurissementNeige = QNUI3_indexMurissementNeige;
	etatFonteCE_.QNUI4_indexTempNeige = QNUI4_indexTempNeige;


	float surfaceTotale = 1.0f;
	float surfaceClairiere = surfaceTotale - carreauEntier.pctForet();
	float fonteDisponible = (TED_fonteClairiere * surfaceClairiere +
		TEC_fonteForet * carreauEntier.pctForet()) / surfaceTotale;

	// 2- Eau disponible sur le CE
	eauDisponible = fonteDisponible +
		((PJD_pluieClairiere * surfaceClairiere +
			PJC_pluieForet * carreauEntier.pctForet()) / surfaceTotale);
	etatFonteCE_.eauDisponible = eauDisponible;

	// 3- Precipitations totales calculees par le modele 
	precipationsTotales = PJE_pluie + PJN_neige;

	etatsFonteCE_.push_back(etatFonteCE_);

	return 0;
}

//------------------------------------------------------------------
int FonteCequeau::assimiler(const DateChrono& datePasDeTemps) {
	if (assimilationsCE_.count(datePasDeTemps) > 0) {
		std::vector<EtatFonteCE> etatsSimules;
		std::vector<EtatFonteCE>::iterator iterCE;
		// Toutes les assimilations CE pour ce pas de temps
		std::vector<EtatFonteAssimCE> assimilations = assimilationsCE_.find(datePasDeTemps)->second;

		int idCE;
		// Parcourt des assimilations
		std::vector<EtatFonteAssimCE>::const_iterator assimilationsIter = assimilations.begin();
		for (; assimilationsIter != assimilations.end(); assimilationsIter++) {
			// Parcourt des etats
			idCE = 0;
			for (iterCE = etatsFonteCE_.begin(); iterCE != etatsFonteCE_.end(); iterCE++) {
				// Si id egaux, on fait la mise a jour
				idCE++;
				if (idCE == assimilationsIter->idCarreauEntier) {

					// Sauvegarde de l'etat avant l'assimilation
					etatsSimules.push_back(*iterCE);
					AssimilationHelper::assimilerValeur(assimilationsIter->SNC_stockNeigeForet,
						assimilationsIter->SNC_stockNeigeForetType, iterCE->SNC_stockNeigeForet);

					AssimilationHelper::assimilerValeur(assimilationsIter->SND_stockNeigeClairiere,
						assimilationsIter->SND_stockNeigeClairiereType, iterCE->SND_stockNeigeClairiere);

					AssimilationHelper::assimilerValeur(assimilationsIter->QNUI3_indexMurissementNeige,
						assimilationsIter->QNUI3_indexMurissementNeigeType, iterCE->QNUI3_indexMurissementNeige);

					AssimilationHelper::assimilerValeur(assimilationsIter->QNUI4_indexTempNeige,
						assimilationsIter->QNUI4_indexTempNeigeType, iterCE->QNUI4_indexTempNeige);

					break;
				}
			} // for
		} // for

		// Conservation des etats simules pour comparaison avec etats assimiles
		if (etatsSimules.size() > 0) {
			avantAssimilationsCE_.insert(std::make_pair(datePasDeTemps, etatsSimules));
		}

	} // if (assimilationsCE_.count(datePasDeTemps) > 0)

	return 0;
}

//------------------------------------------------------------------
int FonteCequeau::initialiserAssimilations(const mxArray* assimilations) {
	int retCode = 0;

	if (assimilations == NULL) {
		return retCode;
	}


	size_t nbDonnees, nbDonneesCE;

	mxArray* etatsFonte, * idCE, * stockNeigeForet, * stockNeigeClairiere, * indexMurissementNeige, * indexTempNeige;

	double pasDeTempsData, * idCEData, * stockNeigeForetData, * stockNeigeClairiereData, * indexMurissementNeigeData;
	double* indexTempNeigeData;
	DateChrono datePasDeTemps;

	EtatFonteAssimCE etatFonteAssimCE;
	std::vector<EtatFonteAssimCE> etatsFonteAssimCE;

	// Nombre de donnees d'assimilation (pas de temps distincts)
	nbDonnees = mxGetNumberOfElements(assimilations);
	for (size_t i = 0; i < nbDonnees; i++) {
		MexHelper::chargerValeurs(assimilations, "pasDeTemps", pasDeTempsData, i);
		//datePasDeTemps = MexHelper::datenumToDate(&pasDeTempsData);
		datePasDeTemps = DateChrono::fromMatlabDatenum(pasDeTempsData);
		etatsFonte = MexHelper::mhMxGetField(assimilations, i, "etatsFonte");

		/***** Carreaux Entiers *****/
		// Donnees d'assimilation relatives aux CE pour ce pas de temps d'assimilation
		if (mxGetNumberOfElements(etatsFonte) > 0) {
			idCE = MexHelper::mhMxGetField(etatsFonte, 0, "id");
			idCEData = MexHelper::mhMxGetPr(idCE, "id");

			// Obtention des pointeurs de donnees
			// On utilise mxGetPr plutot que MexHelper::mhMxGetPr pour la possibilite 
			// d'avoir un pointeur null
			stockNeigeForet = MexHelper::mhMxGetField(etatsFonte, 0, "stockNeigeForet");
			etatFonteAssimCE.SNC_stockNeigeForetType = AssimilationHelper::obtenirTypeAssim(stockNeigeForet);
			stockNeigeForetData = mxGetPr(stockNeigeForet);

			stockNeigeClairiere = MexHelper::mhMxGetField(etatsFonte, 0, "stockNeigeClairiere");
			etatFonteAssimCE.SND_stockNeigeClairiereType = AssimilationHelper::obtenirTypeAssim(stockNeigeClairiere);
			stockNeigeClairiereData = mxGetPr(stockNeigeClairiere);

			indexMurissementNeige = MexHelper::mhMxGetField(etatsFonte, 0, "indexMurissementNeige");
			etatFonteAssimCE.QNUI3_indexMurissementNeigeType = AssimilationHelper::obtenirTypeAssim(indexMurissementNeige);
			indexMurissementNeigeData = mxGetPr(indexMurissementNeige);

			indexTempNeige = MexHelper::mhMxGetField(etatsFonte, 0, "indexTempNeige");
			etatFonteAssimCE.QNUI4_indexTempNeigeType = AssimilationHelper::obtenirTypeAssim(indexTempNeige);
			indexTempNeigeData = mxGetPr(indexTempNeige);

			// Nombre de CE vises.
			nbDonneesCE = mxGetNumberOfElements(idCE);
			etatsFonteAssimCE.clear();
			for (size_t j = 0; j < nbDonneesCE; j++) {
				etatFonteAssimCE.idCarreauEntier = (int)idCEData[j];

				AssimilationHelper::obtenirValeursAssim(stockNeigeForetData, etatFonteAssimCE.SNC_stockNeigeForetType,
					j, etatFonteAssimCE.SNC_stockNeigeForet);

				AssimilationHelper::obtenirValeursAssim(stockNeigeClairiereData, etatFonteAssimCE.SND_stockNeigeClairiereType,
					j, etatFonteAssimCE.SND_stockNeigeClairiere);

				AssimilationHelper::obtenirValeursAssim(indexMurissementNeigeData, etatFonteAssimCE.QNUI3_indexMurissementNeigeType,
					j, etatFonteAssimCE.QNUI3_indexMurissementNeige);

				AssimilationHelper::obtenirValeursAssim(indexTempNeigeData, etatFonteAssimCE.QNUI4_indexTempNeigeType,
					j, etatFonteAssimCE.QNUI4_indexTempNeige);

				etatsFonteAssimCE.push_back(etatFonteAssimCE);
			}

			assimilationsCE_.insert(std::make_pair(datePasDeTemps, etatsFonteAssimCE));
		}
	}

	return retCode;
}

//------------------------------------------------------------------
void FonteCequeau::initialiserEtats(const mxArray* etatsInitiaux) {
	etatsFonteCE_.reserve(nbCE_);

	// Pas d'etats precedents, on initialise avec les valeurs par defaut des parametres
	if (etatsInitiaux == NULL) {

		for (int i = 0; i < nbCE_; i++) {
			etatFonteCE_.QNUI3_indexMurissementNeige = params_.indexMurissementNeigeIni;
			etatFonteCE_.QNUI4_indexTempNeige = params_.indexTempNeigeIni;
			etatFonteCE_.SNC_stockNeigeForet = 0.0f;
			etatFonteCE_.SND_stockNeigeClairiere = 0.0f;
			etatFonteCE_.eauDisponible = 0.0f;
			etatsFonteCE_.push_back(etatFonteCE_);
		}

		etatsFonte_.push_back(etatsFonteCE_);
	} else {
		// Association entre le nom du champ des intrants et une reference sur la variable correspondante
		std::map<std::string, float*> mapChamps;
		mapChamps.insert(std::make_pair("stockNeigeForet", &etatFonteCE_.SNC_stockNeigeForet));
		mapChamps.insert(std::make_pair("stockNeigeClairiere", &etatFonteCE_.SND_stockNeigeClairiere));
		mapChamps.insert(std::make_pair("indexMurissementNeige", &etatFonteCE_.QNUI3_indexMurissementNeige));
		mapChamps.insert(std::make_pair("indexTempNeige", &etatFonteCE_.QNUI4_indexTempNeige));
		mapChamps.insert(std::make_pair("eauDisponible", &etatFonteCE_.eauDisponible));

		initialiserEtatsHelper(etatsInitiaux, mapChamps, etatFonteCE_, etatsFonteCE_);

		etatsFonte_.push_back(etatsFonteCE_);
	}
}

//------------------------------------------------------------------
void FonteCequeau::lireParametres(const mxArray* paramsFonte) {
	lireParametresHelper(paramsFonte, "jonei", params_.jourSoleilMaxFonteNeige);
	lireParametresHelper(paramsFonte, "strne_s", params_.seuilTranformationPluieNeige);
	lireParametresHelper(paramsFonte, "tfc_s", params_.tauxPotentielFonteForet);
	lireParametresHelper(paramsFonte, "tfd_s", params_.tauxPotentielFonteClairiere);
	lireParametresHelper(paramsFonte, "tsc_s", params_.seuilTempFonteForet);
	lireParametresHelper(paramsFonte, "tsd_s", params_.seuilTempFonteClairiere);
	lireParametresHelper(paramsFonte, "ttd", params_.coeffDeficitCalorique);
	lireParametresHelper(paramsFonte, "tts_s", params_.tempMurissementNeige);
	lireParametresHelper(paramsFonte, "tmur", params_.indexMurissementNeigeIni);
	lireParametresHelper(paramsFonte, "tstock", params_.indexTempNeigeIni);
}

//------------------------------------------------------------------
mxArray* FonteCequeau::obtenirEtatsAvantAssimilations(const std::vector<bool> filtreCE) {
	return obtenirEtatsAvantAssimHelper(filtreCE, nomChamps_, avantAssimilationsCE_);
}

//------------------------------------------------------------------
mxArray* FonteCequeau::obtenirEtats(const std::vector<bool> filtreCE) {
	return obtenirEtatsHelper(nomChamps_, etatsFonte_, filtreCE);
}

//------------------------------------------------------------------
void FonteCequeau::preserverEtatsPasDeTemps() {
	etatsFonte_.push_back(etatsFonteCE_);
}

//------------------------------------------------------------------
double FonteCequeau::calculerFacteurModulationSoleil(int noJour) const {
	// DUREEE(X)=ACOS(-TAN(ASIN(.409425*SIN(.0172*X)))*TAPHI)/1.5708
	double diff = (double)(noJour - params_.jourSoleilMaxFonteNeige);
	double calcul = acos(-tan(asin(0.409425f * sin(0.0172f * diff))) * (double)constModulationSoleil_) / 1.5708f;

	return calcul;
}
