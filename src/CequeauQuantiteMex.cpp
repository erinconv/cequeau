//****************************************************************************
// Fichier: CequeauQuantite.cpp
//
// Date creation: 2012-10-01
// Auteur:
//                Rio Tinto Alcan
//                Energie electrique
//                1954 Davis, Saguenay arr. Jonquiere,
//                G7S 4R7, QC, Canada
//
//****************************************************************************

// CequeauQuantiteMec.cpp : Point d'entree de l'application Cequeau Quantite en format mexfile.
//
#include "stdafx.h"
#include "BassinVersant.h"
#include "Simulation.h"
#include "Parametres.h"
#include "DonneesMeteo.h"
#include "FonteCequeau.h"
#include "EvapoCequeau.h"
#include "ParametresQualite.h"
#include "SimulationQualite.h"
#include "DLI.h"

// TODO New module: Your header file here.
#include "FonteTemplate.h"
#include "FonteUEB.h"
#include "EvapoTemplate.h"
#include "EvapoKPenman.h"
#include "EvapoPriestleyTaylor.h"
#include "EvapoPenmanMont.h"
#include "EvapoMcGuinness.h"
#include "FonteCemaNeige.h"
#include "EvapoMorton.h"

// Ajout Interpolation
#include "InterpolateurPolygoneThiessen.h"
#include "InterpolateurPonderationStations.h"
#include "ParametresInterpolation.h"

const std::string idMex = "Cequeau Quantite v4.6.1 et Qualite v2.0.0";

//typedef boost::shared_ptr<Interpolateur> InterpolateurPtr;
typedef std::shared_ptr<Interpolateur> InterpolateurPtr;

void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
	try {
#ifndef SANS_LOG
		mexPrintf((idMex + ": DEBUT DE LA SIMULATION\n").c_str());
#endif
		//FILELog::ReportingLevel() = logWARNING;
		FILELog::ReportingLevel() = logINFO;
		//FILELog::ReportingLevel() = logDEBUG;

		if (nrhs < 7 || nrhs > 8 || nlhs < 9) {
			FILE_LOG(logERROR) << "Nombre de parametres incorrect.";
			mexPrintf("\nNombre de parametres incorrect.\n");
			mexPrintf("Usage: [etatsCE, etatsCP, etatsFonte, etatsEvapo, etatsBarrage, pasDeTemps, avantAssimilationsCE, avantAssimilationsFonte, avantAssimilationsEvapo, etatsQualCP, avAssimQual] = \n");
			mexPrintf("       CequeauQuantiteMex(execution, parametres, bassinVersant, meteo, etatsPrecedents, assimilations, stations)\n");
			mexPrintf("Note: Si on ne desire pas utiliser etatsPrecedents et/ou assimilations, donner une matrice vide [] \n");
			mexPrintf("      Pour executer la qualite en plus de la quantite, parametres.option.calculQualite = 1 \n");
			return;
		}

		// Variables d'entree
		const mxArray* structExecution = prhs[0];
		const mxArray* structParamSimulation = prhs[1];
		const mxArray* structBassinVersant = prhs[2];
		const mxArray* structMeteo = prhs[3];
		const mxArray* structEtatsPrecedents = prhs[4];
		const mxArray* structAssimilations = prhs[5];
		const mxArray* structStations = prhs[6];

		const mxArray* structEtatsPrecedentsQuantite = NULL;
		const mxArray* structEtatsPrecedentsQualite = NULL;
		const mxArray* structAssimilationsQuantite = NULL;
		const mxArray* structAssimilationsQualite = NULL;

		if (mxIsStruct(structEtatsPrecedents)) {
			structEtatsPrecedentsQuantite = mxGetField(structEtatsPrecedents, 0, "quantite");
			structEtatsPrecedentsQualite = mxGetField(structEtatsPrecedents, 0, "qualite");
		}

		if (mxIsStruct(structAssimilations)) {
			structAssimilationsQuantite = mxGetField(structAssimilations, 0, "quantite");
			structAssimilationsQualite = mxGetField(structAssimilations, 0, "qualite");
		}

		char currentPath[FILENAME_MAX];

		if (GetCurrentDir(currentPath, sizeof(currentPath)) == nullptr) {
			FILE_LOG(logERROR) << "Error getting current working directory";
			return;
		}

		std::string pathRepertoireLog(currentPath);
		std::string fichierLog;
		std::string mex_file_name;

		#ifdef octave_mex_h
			mex_file_name = mexFunctionName();
			fichierLog = (pathRepertoireLog + "/" + mex_file_name + ".log");
		#else
			// FILE_LOG(logINFO) << ("MATLAB\n");
			const char *name = mexFunctionName();
			mex_file_name = name;
			
			fichierLog = (pathRepertoireLog + "/" + mex_file_name + ".log");
		#endif
		
#ifndef SANS_LOG
		FILE* log_fd = fopen(fichierLog.c_str(), "w");
		Output2FILE::Stream() = log_fd;
#endif
		FILE_LOG(logINFO) << "pathRepertoireLog " << pathRepertoireLog;
		FILE_LOG(logINFO) << "fichierLog " << fichierLog;
		FILE_LOG(logINFO) << "mexfilename " << mex_file_name << "\n";

		FILE_LOG(logINFO) << " ---------- testing ---------- ";
		FILE_LOG(logINFO) << "pathRepertoireLog " << pathRepertoireLog;
		FILE_LOG(logINFO) << "fichierLog " << fichierLog;

		// Si on veut afficher ce message peut importe le ReportingLevel
		FILE_LOG(logINFO) << "Debut de la simulation Quantite";

		BassinVersant bassinVersant;
		Parametres parametres;

		FILE_LOG(logINFO) << "Initialisation du bassin versant";
		bassinVersant.initialiser(structBassinVersant);

		FILE_LOG(logINFO) << "Initialisation des parametres";
		parametres.initialiser(structExecution, structParamSimulation, bassinVersant.nbCarreauxEntiers(), bassinVersant.nbCarreauxPartiels());
		
		if (parametres.option().modulePompage > 0) {
			mexPrintf("Attention: modulePompage est encore en phase de test. Utilisation avec précaution.\n");
		}
		
		if (parametres.option().moduleOmbrage > 0) {
			mexPrintf("Attention: moduleOmbrage est encore en phase de test. Utilisation avec précaution.\n");
		}

		if (parametres.nbPasParJourSimulation() > 1 && structAssimilations != NULL) {
			mexPrintf("Attention: L'assimilation n'est pas supportee pour les pas de temps < 24h.\n");
			structAssimilations = NULL;
		}

		// TODO New module: Add a test for your module
		// Module de fonte
		FontePtr fonte;
		mxArray* paramsFonte = MexHelper::mhMxGetField(structParamSimulation, 0, "fonte");
		mxArray* paramsFonteModele;
		switch (parametres.option().moduleFonte) {
		case 0:
		case ModuleFonte::CEQUEAU:
			fonte = FontePtr(new FonteCequeau((int)parametres.sol().latitudeMoyenneBV, bassinVersant.nbCarreauxEntiers()));
			paramsFonteModele = MexHelper::mhMxGetField(paramsFonte, 0, "cequeau");
			FILE_LOG(logINFO) << "CEQUEAU";
			// mexPrintf("ModuleFonte::CEQUEAU\n");
			break;
		case ModuleFonte::CEMA_NEIGE:
			fonte = FontePtr(new FonteCemaNeige((int)parametres.sol().latitudeMoyenneBV, bassinVersant.nbCarreauxEntiers()));
			paramsFonteModele = MexHelper::mhMxGetField(paramsFonte, 0, "cemaNeige");
			FILE_LOG(logINFO) << "CEMA_NEIGE";
			// mexPrintf("ModuleFonte::CEMA_NEIGE\n");
			break;
		case ModuleFonte::UEB:
			fonte = FontePtr(new FonteUEB((int)parametres.sol().latitudeMoyenneBV, bassinVersant.nbCarreauxEntiers()));
			paramsFonteModele = MexHelper::mhMxGetField(paramsFonte, 0, "UEB");
			FILE_LOG(logINFO) << "Fonte UEB";
			break;
		default:
			mexPrintf("Module de fonte inexistant.");
			mexPrintf("\nparametres.option.moduleFonte: %d\n", parametres.option().moduleFonte);
			return;
		}
		fonte->lireParametres(paramsFonteModele);
		fonte->initialiserAssimilations(structAssimilationsQuantite);

		// AJOUT_MODULE_EVAPO: Ajouter la condition pour votre module d'evapotranspiration
		// Module de fonte
		EvapoPtr evapo;
		mxArray* paramsEvapo = MexHelper::mhMxGetField(structParamSimulation, 0, "evapo");
		mxArray* paramsEvapoModele;
		switch (parametres.option().moduleEvapo) {
		case 0:
		case ModuleEvapo::CEQUEAU:
			evapo = EvapoPtr(new EvapoCequeau((int)parametres.sol().latitudeMoyenneBV, bassinVersant.nbCarreauxEntiers(),
				parametres.nbPasParJourSimulation()));
			paramsEvapoModele = MexHelper::mhMxGetField(paramsEvapo, 0, "cequeau");
			FILE_LOG(logINFO) << "Thornthwaite";
			// mexPrintf("ModuleEvapo::CEQUEAU\n");
			break;
		case ModuleEvapo::KPENMAN:
			evapo = EvapoPtr(new EvapoKPenman((int)parametres.sol().latitudeMoyenneBV, bassinVersant.nbCarreauxEntiers(),
				parametres.nbPasParJourSimulation()));
			paramsEvapoModele = MexHelper::mhMxGetField(paramsEvapo, 0, "KPenman");
			FILE_LOG(logINFO) << "Kimberley-Penman";
			// mexPrintf("ModuleEvapo::KPENMAN\n");
			break;
		case ModuleEvapo::PRIESTLEYTAYLOR:
			evapo = EvapoPtr(new EvapoPriestleyTaylor((int)parametres.sol().latitudeMoyenneBV, bassinVersant.nbCarreauxEntiers(),
				parametres.nbPasParJourSimulation()));
			paramsEvapoModele = MexHelper::mhMxGetField(paramsEvapo, 0, "PriestleyTaylor");
			FILE_LOG(logINFO) << "Priestley-Taylor";
			// mexPrintf("ModuleEvapo::PRIESTLEYTAYLOR\n");
			break;

		case ModuleEvapo::MCGUINNESS:
			evapo = EvapoPtr(new EvapoMcGuinness((int)parametres.sol().latitudeMoyenneBV, bassinVersant.nbCarreauxEntiers(),
				parametres.nbPasParJourSimulation()));
			paramsEvapoModele = MexHelper::mhMxGetField(paramsEvapo, 0, "McGuinness");
			FILE_LOG(logINFO) << "McGuinness";
			// mexPrintf("ModuleEvapo::MCGUINNESS\n");
			break;

		case ModuleEvapo::PENMONT:
			evapo = EvapoPtr(new EvapoPenmanMont((int)parametres.sol().latitudeMoyenneBV, bassinVersant.nbCarreauxEntiers(),
				parametres.nbPasParJourSimulation()));
			paramsEvapoModele = MexHelper::mhMxGetField(paramsEvapo, 0, "PenmanMonteith");
			FILE_LOG(logINFO) << "Penman-Monteith";
			// mexPrintf("ModuleEvapo::PENMONT\n");
			break;

		case ModuleEvapo::MORTON:
			evapo = EvapoPtr(new EvapoMorton((int)parametres.sol().latitudeMoyenneBV, bassinVersant.nbCarreauxEntiers(),
				parametres.nbPasParJourSimulation()));
			paramsEvapoModele = MexHelper::mhMxGetField(paramsEvapo, 0, "Morton");
			FILE_LOG(logINFO) << "Morton";
			// mexPrintf("ModuleEvapo::MORTON\n");
			break;
		default:
			mexPrintf("Module d'evapotranspiration inexistant.");
			mexPrintf("\nparametres.option.moduleEvapo: %d\n", parametres.option().moduleEvapo);
			return;
		}
		evapo->lireParametres(paramsEvapoModele);
		evapo->initialiserAssimilations(structAssimilationsQuantite);

		DonneesMeteo donneesMeteo(bassinVersant.nbCarreauxEntiers(), parametres.nbPasSimulation());

		std::vector<std::string> autresChampsMeteo;
		// Champs meteo necessaires au calcul de la qualite
		autresChampsMeteo.push_back("nebulosite");
		autresChampsMeteo.push_back("pression");
		autresChampsMeteo.push_back("rayonnement");
		autresChampsMeteo.push_back("vitesseVent");
		autresChampsMeteo.push_back("longwaveRad"); // Keep the consistency with the UEB model names
	    autresChampsMeteo.push_back("lai");
        autresChampsMeteo.push_back("lai_norm");


		const mxArray* structDonneesMeteo = structMeteo;

		// Les stations sont presentes?
		int nbStations = (int)mxGetNumberOfElements(structStations);
		if (nbStations != 0) {

			FILE_LOG(logINFO) << "Initialisation des stations";
			ListeStation listeStation;
			listeStation.initialiser(structStations);

			mxArray* tMin = MexHelper::mhMxGetField(structMeteo, 0, "tMin");
			size_t nbPoints = mxGetN(tMin);

			if (nbPoints == listeStation.quantiteStations()) {

				FILE_LOG(logINFO) << "Initialisation des parametres d'interpolation";
				ParametresInterpolation paramInterpolation;
				paramInterpolation.initialiser(structParamSimulation);

				FILE_LOG(logINFO) << "Initialisation des donnees meteo des stations";
				DonneesMeteo meteoStations(listeStation.quantiteStations(), parametres.nbPasSimulation());
				// Autres champs meteo?
				std::vector<std::string> dummy, nomsChampsMeteo;
				std::string nomChampMeteo;
				int nbChampsMeteo = mxGetNumberOfFields(structMeteo);
				int nbStationsChamp;
				mxArray* champMeteo;
				for (int i = 0; i < nbChampsMeteo; i++) {
					nomChampMeteo = mxGetFieldNameByNumber(structMeteo, i);
					if (nomChampMeteo != "tMin" && nomChampMeteo != "tMax" &&
						nomChampMeteo != "pluie" && nomChampMeteo != "neige" &&
						nomChampMeteo != "pTot") {

						// Est-ce que la meteo a interpoler contient toutes les stations?
						// Si non on interpole pas
						champMeteo = mxGetField(structMeteo, 0, nomChampMeteo.c_str());
						nbStationsChamp = (int)mxGetN(champMeteo);
						if (nbStationsChamp == listeStation.quantiteStations()) {
							nomsChampsMeteo.push_back(nomChampMeteo);
						}
					}
				}

				meteoStations.initialiser(structMeteo, dummy, dummy, nomsChampsMeteo);

				InterpolateurPtr interpolateur;
				// Choix de la methode d'interpolation
				switch (paramInterpolation.type()) {
				case 1:
				{
					FILE_LOG(logINFO) << "Interpolateur choisi: InterpolateurPolygoneThiessen";
					interpolateur = InterpolateurPtr(new InterpolateurPolygoneThiessen(bassinVersant, listeStation, parametres, paramInterpolation, meteoStations, nomsChampsMeteo));
					break;
				}
				case 3:
				{
					FILE_LOG(logINFO) << "Interpolateur choisi: InterpolateurPonderationStations";
					interpolateur = InterpolateurPtr(new InterpolateurPonderationStations(bassinVersant, listeStation, parametres, paramInterpolation, meteoStations, nomsChampsMeteo));
					break;
				}
				default:
					throw std::runtime_error("Type d'interpolation non supportee.");
					break;
				}

				// Variables de sortie
				FILE_LOG(logINFO) << "Execution de l'interpolation";
				interpolateur->executer();

				FILE_LOG(logINFO) << "Ecriture de la meteo interpolee";
				structDonneesMeteo = interpolateur->obtenirMeteoInterpolee();

				FILELog().Get(logINFO) << "Fin de l'interpolation";
			}
		}

		FILE_LOG(logINFO) << "Initialisation des donnees meteo";
		donneesMeteo.initialiser(structDonneesMeteo, fonte->nomsChampsMeteo(), evapo->nomsChampsMeteo(), autresChampsMeteo);


		FILE_LOG(logINFO) << "Initialisation pompage eau";
		Simulation simulation(bassinVersant, parametres, donneesMeteo, structEtatsPrecedentsQuantite, "", fonte, evapo, structAssimilationsQuantite);

		FILE_LOG(logINFO) << "Execution de la simulation";
		simulation.executer(true);

		// Variables de sortie
		plhs[0] = simulation.obtenirEtatsCE();
		plhs[1] = simulation.obtenirEtatsCP();
		plhs[2] = fonte->obtenirEtats(parametres.resultatsIdCE());
		plhs[3] = evapo->obtenirEtats(parametres.resultatsIdCE());
		plhs[4] = simulation.obtenirEtatsBarrage();
		plhs[5] = simulation.obtenirPasDeTemps();
		plhs[6] = simulation.obtenirEtatsAvantAssimilations();
		plhs[7] = fonte->obtenirEtatsAvantAssimilations(parametres.resultatsIdCE());
		plhs[8] = evapo->obtenirEtatsAvantAssimilations(parametres.resultatsIdCE());

		// Execution de la qualite
		if (parametres.option().calculQualite) {
			TODO: 
			if (parametres.dureeHeuresPasSimulation() < 24) {
				mexPrintf("Attention: calculQualite avec pas de temps < 24h est encore en phase de test. Utilisation avec précaution.\n");
			}
			// QUALITE
			ParametresQualite parametresQualite;
			DLI dli;

			const mxArray* structParamQualite = MexHelper::mhMxGetField(structParamSimulation, 0, "qualite");
			const mxArray* structParamDLI = MexHelper::mhMxGetField(structParamSimulation, 0, "dli");
			const mxArray* structParamQualiteCequeau = MexHelper::mhMxGetField(structParamQualite, 0, "cequeau");

			parametresQualite.initialiser(structParamQualiteCequeau);
			dli.initialiser(parametres.option().moduleDLI, structParamDLI);

			if (parametres.option().moduleOmbrage != 0) {
				parametresQualite.initialiserOmbrage(structParamQualiteCequeau);
			}

			SimulationQualite simulationQualite(simulation, parametresQualite, dli, structEtatsPrecedentsQualite, structAssimilationsQualite);

			simulationQualite.executer(true);

			plhs[9] = simulationQualite.obtenirEtatsCP();
			plhs[10] = simulationQualite.obtenirEtatsAvantAssimilations();
		} else {
			plhs[9] = mxCreateStructMatrix(1, 0, 0, 0);
			plhs[10] = mxCreateStructMatrix(1, 0, 0, 0);
		}

		if (simulation.hasNeigeAjustee()) {
			std::string message = "Neige Ajustee\n";
			mexPrintf(message.c_str());
		}

		// Si on veut afficher ce message peut importe le ReportingLevel
		//FILELog().Get(logINFO) << "Fin de la simulation Quantite";
		FILE_LOG(logINFO) << "Fin de la simulation Quantite";
#ifndef SANS_LOG
		mexPrintf((idMex + ": FIN DE LA SIMULATION\n").c_str());
#endif

		return;
	} catch (const std::exception& ex) {
		FILE_LOG(logERROR) << ex.what();
		FILELog().Get(logINFO) << "Fin de la simulation Quantite";
		mexPrintf("%s\n", ex.what());
		mexPrintf("Fin de la simulation Quantite\n");
		return;
	} catch (...) {
		FILELog().Get(logINFO) << "Fin de la simulation Quantite";
		mexPrintf("Erreur. Fin du programme.\n");
		mexPrintf("Fin de la simulation Quantite\n");
		return;
	}
}

