//****************************************************************************
// Fichier: Simulation.cpp
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
#include "Simulation.h" 

//------------------------------------------------------------------
int Simulation::executerTransfert(const int& nbTransfertsParJour, const DateChrono& datePasDeTemps,
	const int& noPasDeTemps, const float& duree) {
	FILE_LOG(logDEBUG) << "Simulation::executerTransfert(...)";
	const int nbCP = bassinVersant_.nbCarreauxPartiels();
	EtatsCarreauxPartiels etatsPasDeTemps;
	etatsPasDeTemps.reserve(nbCP);

	// Extraction des apports des carreaux entiers pour le pas de temps.
	int indexCE, idCP;
	BarragePtr barrage;
	float apportProd;
	std::vector<float> apportsProd;
	apportsProd.reserve(nbCP);
	EtatsCarreauxEntiers etatsCarreauxEntiers = bilans_.back();
	// Accumulateur du volume au CP.
	std::vector<float> volumesCP;
	volumesCP.reserve(nbCP);
	EtatsCarreauxPartiels& etatsPrecedentsCP = debits_.back();
	// Accumulateur du volume evacue des barrages.
	std::map<int, float> volumeEntrantBarrages;
	std::map<int, float> volumeEvacueBarrages;
	// Details du volume evacue dans le cas ou il y aurait plus d'une regle 
	// d'evacuation (barrage type 5)
	std::map<int, std::vector<float> > volumesEvacuesBarrages;

	// Initialisation des apports et volumes
	for (int i = 0; i < nbCP; i++) {
		indexCE = bassinVersant_.carreauxPartiels()[i]->idCarreauEntier() - 1;

		apportProd = etatsCarreauxEntiers[indexCE].production *
			bassinVersant_.carreauxPartiels()[i]->pctSurface() *
			(1.0f / nbTransfertsParJour);

		// Traitement des barrages externes, si presents.
		// Initialisation des accumulateurs pour les barrages
		if (bassinVersant_.possedeBarrage()) {
			// Un barrage dont les evacuation sont sur ce CP?
			idCP = i + 1;
			barrage = bassinVersant_.trouverBarrageParIdCpAval(idCP);
			if (barrage) {
				if (barrage->typeBarrage() == EVACUATION_MESUREE_EXT) {
					int indexDebit = calculerIndexMeteo(datePasDeTemps, noPasDeTemps);
					float debit = barrage->debitsMesures()[indexDebit];
					// Ajout de l'apport du barrage externe
					apportProd += (debit * duree);
				}
			}
			barrage = bassinVersant_.trouverBarrageParIdCP(idCP);
			if (barrage) {
				volumeEvacueBarrages.insert(std::make_pair(idCP, 0.0f));
				volumeEntrantBarrages.insert(std::make_pair(idCP, 0.0f));
				volumesEvacuesBarrages.insert(std::make_pair(idCP, 0));
			}
		}

		apportsProd.push_back(apportProd);
		volumesCP.push_back(etatsPrecedentsCP[i].volume);
	}

	// Variables de travail
	float volumeCP, volumeAvantEvacuation, apportAmont, apportMoyen;
	float apportTotal; // !!!!!
	std::vector<float> volumesSortantsBarrages; // Volumes sortants du barrage
	// Cumul des volumes pour le nombre de transferts du pas de temps.
	// Volume final du CP pour le pas de temps
	std::vector<float> cumulsVolumesCP;
	cumulsVolumesCP.reserve(nbCP);
	cumulsVolumesCP.assign(nbCP, 0.0f);
	double coeffTransfert;
	int idCPAmont;
	float secondesPasTemps = ((float)parametres_.dureeHeuresPasSimulation() * 3600.0f);

	// Boucle sur le nombre de transferts par jour
	for (int i = 0; i < nbTransfertsParJour; i++) {

		// Cumul des volumes
		for (int l = 0; l < nbCP; l++) {
			cumulsVolumesCP[l] += (volumesCP[l] * (float)bassinVersant_.carreauxPartiels()[l]->coeffTransfert());
		}

		// Boucle sur les carreaux partiels
		for (int j = 0; j < nbCP; j++) {
			volumeCP = volumesCP[j];
			coeffTransfert = bassinVersant_.carreauxPartiels()[j]->coeffTransfert();
			// Volume restant sur le carreau partiel
			volumeCP = volumeCP * (float)(1.0 - coeffTransfert);
			apportTotal = 0;

			// Boucle sur les carreaux partiels en amont
			for (int k = 0; k < NB_CP_AMONT; k++) {
				idCPAmont = bassinVersant_.carreauxPartiels()[j]->idCarreauxPartielsAmont()[k];

				// Un 0 indique la fin de la liste des CP en amont.
				if (idCPAmont == 0) {
					break;
				} else {
					apportAmont = volumesCP[idCPAmont - 1] *
						(float)bassinVersant_.carreauxPartiels()[idCPAmont - 1]->coeffTransfert();

					// Barrage sur le CP en amont?
					if (bassinVersant_.possedeBarrage(idCPAmont)) {
						barrage = bassinVersant_.trouverBarrageParIdCP(idCPAmont);
						apportMoyen = apportAmont * bassinVersant_.carreauxPartiels()[idCPAmont - 1]->calculerCorrectionDebit();
						volumeAvantEvacuation = volumeCP;
						volumeCP = barrage->calculerVolumeCpAval(apportMoyen, duree, volumeCP, datePasDeTemps, volumesSortantsBarrages);
						volumeEvacueBarrages[idCPAmont] += (volumeCP - volumeAvantEvacuation);

						// Pour conserver le volume entrant et les volumes sortants aux barrages
						volumeEntrantBarrages[idCPAmont] += apportMoyen;
						for (unsigned int i = 0; i < volumesSortantsBarrages.size(); i++) {
							// Initialisation au premier passage
							if (volumesEvacuesBarrages[idCPAmont].size() < volumesSortantsBarrages.size()) {
								volumesEvacuesBarrages[idCPAmont].assign(volumesSortantsBarrages.size(), 0.0f);
							}
							volumesEvacuesBarrages[idCPAmont][i] += volumesSortantsBarrages[i];
						}
					} else {
						// Pas de barrage
						volumeCP += apportAmont;
					}

				}
			} // Fin Boucle sur les carreaux partiels en amont.

			volumesCP[j] = maxf(0.0, volumeCP + apportsProd[j]);

		} // Fin Boucle sur les carreaux partiels.
	}  // Fin Boucle sur le nombre de transferts par jour.

	// Sauvegarde des etats des CP pour le pas de temps.
	for (int i = 0; i < nbCP; i++) {
		EtatCarreauPartiel etatCarreauPartiel;

		etatCarreauPartiel.idCarreauPartiel = i + 1;
		etatCarreauPartiel.apport = apportsProd[i];
		etatCarreauPartiel.volume = volumesCP[i];
		// QUALITE
		etatCarreauPartiel.Qualite.volumeSortant = cumulsVolumesCP[i];

		if (bassinVersant_.possedeBarrage(i + 1)) {
			etatCarreauPartiel.debit = volumeEvacueBarrages[i + 1] / secondesPasTemps;
		} else {
			etatCarreauPartiel.debit = cumulsVolumesCP[i] * bassinVersant_.carreauxPartiels()[i]->calculerCorrectionDebit()
				/ secondesPasTemps;
		}
		etatsPasDeTemps.push_back(etatCarreauPartiel);
	}

	debits_.push_back(etatsPasDeTemps);

	// Sauvegarde de l'etat des barrages
	if (bassinVersant_.possedeBarrage()) {
		EtatsBarrages etatsBarrages;

		int nbBarrages = bassinVersant_.nbBarrages();
		for (int i = 0; i < nbBarrages; i++) {
			int idCP;
			float debit;
			// Pas d'etat a conserver pour un barrage externe.
			if (bassinVersant_.barrages()[i]->typeBarrage() != EVACUATION_MESUREE_EXT) {
				EtatBarrage etatBarrage;
				idCP = bassinVersant_.barrages()[i]->idCP();
				etatBarrage.idCarreauPartiel = idCP;
				etatBarrage.volume = bassinVersant_.barrages()[i]->volume();
				etatBarrage.niveau = bassinVersant_.barrages()[i]->calculerNiveauAvecVolume();
				etatBarrage.debitEntrant = volumeEntrantBarrages[idCP] / secondesPasTemps;

				for (unsigned int i = 0; i < volumesEvacuesBarrages[idCP].size(); i++) {
					debit = volumesEvacuesBarrages[idCP][i] / secondesPasTemps;
					etatBarrage.debitsSortants.push_back(debit);
				}
				etatsBarrages.push_back(etatBarrage);
			}
		}

		if (etatsBarrages.size() > 0) {
			barrages_.push_back(etatsBarrages);
		}
	}

	return 0;
}

//------------------------------------------------------------------
void  Simulation::initialiserCoeffTransfertCP(int nbTransfertParJour) {
	FILE_LOG(logDEBUG) << "Simulation::initialiserCoeffTransfertCP(int nbTransfertParJour)";
	// Pour faciliter la comprehension, les variables ont le meme nom que les variables
	// telles que definies dans le manuel Cequeau.
	double SPA1 = bassinVersant_.carreauxPartiels()[0]->cumulPctSuperficieCPAmont();
	double AMONCP, SL, /* SOLACT,*/ SLAC, RMA3, VOLCP;
	double EXXKT = parametres_.transfert().paramCalculCoeffTransfertCP;
	double QO = parametres_.solInitial().debitInitialExutoire;
	// DUREE=PASSIM/FLOAT(NPJO)
	double DUREE = (3600.0 * (double)parametres_.dureeHeuresPasSimulation()) / (double)nbTransfertParJour;
	double CEKM2 = bassinVersant_.superficieCarreauEntier();
	double XXKTCP, TT1;
	int nbCP = bassinVersant_.nbCarreauxPartiels();
	int idCE, idCP, idCPAval;
	std::vector<double> SOLACT;
	SOLACT.reserve(nbCP);
	CarreauEntierPtr carreauEntierPtr;

	double workSOLACT;
	for (int i = 0; i < nbCP; i++) {
		idCE = bassinVersant_.carreauxPartiels()[i]->idCarreauEntier();
		carreauEntierPtr = bassinVersant_.trouverCarreauEntierParId(idCE);
		workSOLACT = carreauEntierPtr->pctLacRiviere() * bassinVersant_.carreauxPartiels()[i]->pctSurface();
		SOLACT.push_back(workSOLACT);
	}

	int codeLac;
	float pctSuperficie;
	for (int i = nbCP - 1; i >= 0; i--) {
		idCP = i + 1;
		if (parametres_.facultatifs().lacs.count(idCP) > 0) {
			codeLac = parametres_.facultatifs().lacs.find(idCP)->second;
			switch (codeLac) {
			case 0:
				continue;
				break;
			case 1:
				idCPAval = bassinVersant_.carreauxPartiels()[i]->idCarreauPartielAval();
				SOLACT[idCPAval - 1] = SOLACT[idCPAval - 1] + SOLACT[i];
			case 2:
				SOLACT[i] = 0.0f;
				break;
				// codeLac > 2
			default:
				pctSuperficie = ((float)codeLac - 2.0f) / 10.0f;
				SOLACT[i] = SOLACT[i] * (1.0f - pctSuperficie);
				break;
			}
		}
	}

	for (int i = 0; i < nbCP; i++) {
		idCP = i + 1;
		AMONCP = (double)bassinVersant_.carreauxPartiels()[i]->cumulPctSuperficieCPAmont();

		idCE = bassinVersant_.carreauxPartiels()[i]->idCarreauEntier();
		carreauEntierPtr = bassinVersant_.trouverCarreauEntierParId(idCE);
		// Si le carreau partiel appartient a un carreau entier faisant partie d'un zone lac exutoire,
		// le transfert est direct.
		if (carreauEntierPtr->pctLacExutoire() > 0.01f) {
			XXKTCP = 1.0;
		}
		// XXKTCP: Soit est calcule ou bien possibilite d'une valeur deja calculee dans vecteur facultatif CTP.
		else if (parametres_.facultatifs().coeffTxParticuliers.count(idCP) > 0) {
			XXKTCP = parametres_.facultatifs().coeffTxParticuliers.find(idCP)->second;
		} else {
			SL = (1.0 + std::sqrt(AMONCP)) / 50.0;
			SLAC = SOLACT[i] * 100.0;
			RMA3 = AMONCP + 200.0;

			// Formule differente pour pas de temps plus petit que 24h
			// 2012-02-27 Pas utilise. Resultats a valider.
			//if (parametres_.dureeHeuresPasSimulation() < 24) {
			if (parametres_.dureeHeuresPasSimulation() < 0) {
				SL *= 0.01;
				SLAC *= 0.01;
				RMA3 *= 0.01;
				TT1 = EXXKT * RMA3 / (std::max<double>(SL, SLAC) + bassinVersant_.carreauxPartiels()[i]->pctSurface());
				TT1 = std::sqrt(TT1);
			} else {
				TT1 = EXXKT * RMA3 / std::max<double>(SL, SLAC) * 100.0 / CEKM2;
			}

			XXKTCP = 1.0 - std::exp(-std::min<double>(36.0, TT1));
			XXKTCP = 1.0 - std::pow(1.0 - XXKTCP, 1.0 / (double)nbTransfertParJour);
		}

		VOLCP = QO * DUREE * AMONCP / (SPA1 * XXKTCP);

		// Y'a-t-il un barrage en amont?
		BarragePtr barrage = bassinVersant_.trouverBarrageParIdCpAval(i + 1);
		if (barrage != NULL) {
			if (barrage->typeBarrage() == EVACUATION_CALCULEE ||
				barrage->typeBarrage() == EVACUATION_CALCULEE_SPECIAL) {
				// Volume provenant du barrage en amont
				VOLCP = barrage->calculerDebitAvecVolume() * DUREE;
			}
		}

		bassinVersant_.carreauxPartiels()[i]->coeffTransfert(XXKTCP);
		bassinVersant_.carreauxPartiels()[i]->volumeInitial((float)VOLCP);
	}
}


