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

// CequeauQuantiteMex.cpp : Point d'entree de l'application Cequeau Quantite en format mexfile.
//
#include "stdafx.h"
#include "InterpolateurPolygoneThiessen.h"
#include "InterpolateurPonderationStations.h"
#include "ParametresInterpolation.h"
#include "mex.h"
const std::string idMex = "Cequeau Interpolation v1.0.1";

typedef std::shared_ptr<Interpolateur> InterpolateurPtr;

void mexFunction(int nlhs, mxArray *plhs[],int nrhs, const mxArray *prhs[])
{
  
  try {
#ifndef SANS_LOG
    mexPrintf((idMex + ": DEBUT DE L'INTERPOLATION\n").c_str());
#endif
    FILELog::ReportingLevel() = logWARNING; 

    if (nrhs == 1) {
      mexPrintf((idMex + "\n").c_str());
      return;
    }

    if (nrhs != 5 || nlhs != 1) {
      FILE_LOG(logERROR) << "Nombre de parametres incorrect.";
      mexPrintf("\nNombre de parametres incorrect.\n");
      mexPrintf("Usage: meteoInterpolee = cequeauInterpolationMex(execution, parametres, bassinVersant, stations, meteoStations)\n");
      return;
    }

    // Variables d'entree
    const mxArray* structExecution = prhs[0];
    const mxArray* structParamSimulation = prhs[1];
    const mxArray* structBassinVersant = prhs[2];
    const mxArray* structStations = prhs[3];
    const mxArray* structMeteo = prhs[4];


    char currentPath[FILENAME_MAX];

		if (GetCurrentDir(currentPath, sizeof(currentPath)) == nullptr) {
			FILE_LOG(logERROR) << "Error getting current working directory";
			return;
		}

		std::string pathRepertoireLog(currentPath);
		std::string fichierLog;
		std::string fichierCSV;
		std::string mex_file_name;

		#ifdef octave_mex_h
			mex_file_name = mexFunctionName();
			fichierLog = (pathRepertoireLog + "/" + mex_file_name + ".log");
		#else
			const char *name = mexFunctionName();
			mex_file_name = name;
			fichierLog = (pathRepertoireLog + "/" + mex_file_name + ".log");
		#endif

#ifndef SANS_LOG
    FILE* log_fd = fopen(fichierLog.c_str(), "w");
    Output2FILE::Stream() = log_fd;
#endif


//
//    // Fichier de journalisation.
//    boost::filesystem::path pathRepertoireLog = boost::filesystem::initial_path();
//    std::string fichierLog(pathRepertoireLog.string() + "\\CequeauInterpolation.log");
//#ifndef SANS_LOG
//    FILE* log_fd = fopen(fichierLog.c_str(), "w");
//    Output2FILE::Stream() = log_fd;
//#endif

    FILE_LOG(logINFO) << "Debut de l'interpolation";
    FILE_LOG(logINFO) << "Initialisation du bassin versant";
    BassinVersant bassinVersant;
    bassinVersant.initialiser(structBassinVersant);

    FILE_LOG(logINFO) << "Initialisation des parametres";
    Parametres parametres;
    parametres.initialiser(structExecution, structParamSimulation, bassinVersant.nbCarreauxEntiers(), bassinVersant.nbCarreauxPartiels());
    ParametresInterpolation paramInterpolation;
    paramInterpolation.initialiser(structParamSimulation);

    FILE_LOG(logINFO) << "Initialisation des stations";
    ListeStation listeStation;
    listeStation.initialiser(structStations);

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
    plhs[0] = interpolateur->obtenirMeteoInterpolee();
    
    FILE_LOG(logINFO) << "Fin de l'interpolation";

#ifndef SANS_LOG
    mexPrintf((idMex + ": FIN DE L'INTERPOLATION\n").c_str());
#endif

    return;
  }
  catch (const std::exception& ex) {
    FILE_LOG(logERROR) << ex.what();
    FILELog().Get(logINFO) << "Fin de l'interpolation";
    mexPrintf("%s\n", ex.what());
    mexPrintf("Fin de l'interpolation\n");
    return;
  }
  catch (...) {
    FILELog().Get(logINFO) << "Fin de l'interpolation";
    mexPrintf("Erreur. Fin du programme.\n");
    mexPrintf("Fin de l'interpolation\n");
    return;
  }
}

