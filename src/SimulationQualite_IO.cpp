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
#include "SimulationQualite.h"

//------------------------------------------------------------------
mxArray* SimulationQualite::obtenirEtatsCP() 
{
  FILE_LOG(logDEBUG) << "SimulationQualite::obtenirEtatsQualiteCP()";
  return obtenirEtatsCP(qualite_);
}

//------------------------------------------------------------------
mxArray* SimulationQualite::obtenirEtatsCP(const std::vector<Qualite::EtatsCarreauxPartiels>& etatsPasDeTemps)
{
  FILE_LOG(logDEBUG) << "SimulationQualite::obtenirEtatsQualiteCP(const std::vector<Qualite::EtatsCarreauxPartiels>& etatsPasDeTemps)";
  const size_t m = etatsPasDeTemps.size();
  // Nombre de CP
  const size_t n = etatsPasDeTemps.back().size();

  // Allocation de la memoire.
  mxArray** id = (mxArray**)mxMalloc(m * sizeof(mxArray*)); 
  mxArray** temperature = (mxArray**)mxMalloc(m * sizeof(mxArray*));
  mxArray** qruiss = (mxArray**)mxMalloc(m * sizeof(mxArray*));
  mxArray** qnappe = (mxArray**)mxMalloc(m * sizeof(mxArray*));
  mxArray** qhypo  = (mxArray**)mxMalloc(m * sizeof(mxArray*));
  mxArray** qlacma = (mxArray**)mxMalloc(m * sizeof(mxArray*));
  mxArray** qradso = (mxArray**)mxMalloc(m * sizeof(mxArray*));
  mxArray** qradin = (mxArray**)mxMalloc(m * sizeof(mxArray*));
  mxArray** qevap  = (mxArray**)mxMalloc(m * sizeof(mxArray*));
  mxArray** qconv  = (mxArray**)mxMalloc(m * sizeof(mxArray*));
  mxArray** enerlo  = (mxArray**)mxMalloc(m * sizeof(mxArray*));
  mxArray** eneram  = (mxArray**)mxMalloc(m * sizeof(mxArray*));
  mxArray** eneres  = (mxArray**)mxMalloc(m * sizeof(mxArray*));

  for (unsigned int i = 0; i < m; i++) {
    id[i] = mxCreateDoubleMatrix(1, n, mxREAL);
    temperature[i] = mxCreateDoubleMatrix(1, n, mxREAL);
    qruiss[i] = mxCreateDoubleMatrix(1, n, mxREAL);
    qnappe[i] = mxCreateDoubleMatrix(1, n, mxREAL);
    qhypo[i]  = mxCreateDoubleMatrix(1, n, mxREAL);
    qlacma[i] = mxCreateDoubleMatrix(1, n, mxREAL);
    qradso[i] = mxCreateDoubleMatrix(1, n, mxREAL);
    qradin[i] = mxCreateDoubleMatrix(1, n, mxREAL);
    qevap[i]  = mxCreateDoubleMatrix(1, n, mxREAL);
    qconv[i]  = mxCreateDoubleMatrix(1, n, mxREAL);
	enerlo[i]  = mxCreateDoubleMatrix(1, n, mxREAL);
	eneram[i]  = mxCreateDoubleMatrix(1, n, mxREAL);
	eneres[i]  = mxCreateDoubleMatrix(1, n, mxREAL);
  }

  double temperatureCP, idCP, qruissCP, qnappeCP, qhypoCP, qlacmaCP, qradsoCP, qradinCP, qevapCP, qconvCP, enerloCP, eneramCP, eneresCP;
  std::vector<Qualite::EtatsCarreauxPartiels>::const_iterator tempsIter = etatsPasDeTemps.begin();
  Qualite::EtatsCarreauxPartiels::const_iterator etatsIter;
  size_t i = 0, j;
  // Parcourt des pas de temps
  for (; tempsIter != etatsPasDeTemps.end(); tempsIter++) {
    j = 0;
    // Parcourt des carreaux entiers
    for (etatsIter = tempsIter->begin();  etatsIter != tempsIter->end(); etatsIter++) {
      idCP     = (double)etatsIter->idCarreauPartiel;
      temperatureCP  = (double)etatsIter->temperature;
      qruissCP = (double)etatsIter->qruiss;
      qnappeCP = (double)etatsIter->qnappe;
      qhypoCP  = (double)etatsIter->qhypo;
      qlacmaCP = (double)etatsIter->qlacma;
      qradsoCP = (double)etatsIter->qradso;
      qradinCP = (double)etatsIter->qradin;
      qevapCP  = (double)etatsIter->qevap;
      qconvCP  = (double)etatsIter->qconv;
	  enerloCP  = (double)etatsIter->enerlo;
	  eneramCP  = (double)etatsIter->eneram;
	  eneresCP  = (double)etatsIter->eneres;

      memcpy((void*)(mxGetPr(id[i]) + j), (void*)&idCP, sizeof(double));
      memcpy((void*)(mxGetPr(temperature[i]) + j), (void*)&temperatureCP, sizeof(double));
      memcpy((void*)(mxGetPr(qruiss[i]) + j), (void*)&qruissCP, sizeof(double));
      memcpy((void*)(mxGetPr(qnappe[i]) + j), (void*)&qnappeCP, sizeof(double));
      memcpy((void*)(mxGetPr(qhypo[i]) + j),  (void*)&qhypoCP,  sizeof(double));
      memcpy((void*)(mxGetPr(qlacma[i]) + j), (void*)&qlacmaCP, sizeof(double));
      memcpy((void*)(mxGetPr(qradso[i]) + j), (void*)&qradsoCP, sizeof(double));
      memcpy((void*)(mxGetPr(qradin[i]) + j), (void*)&qradinCP, sizeof(double));
      memcpy((void*)(mxGetPr(qevap[i]) + j),  (void*)&qevapCP,  sizeof(double));
      memcpy((void*)(mxGetPr(qconv[i]) + j),  (void*)&qconvCP,  sizeof(double));
	  memcpy((void*)(mxGetPr(enerlo[i]) + j),  (void*)&enerloCP,  sizeof(double));
	  memcpy((void*)(mxGetPr(eneram[i]) + j),  (void*)&eneramCP,  sizeof(double));
	  memcpy((void*)(mxGetPr(eneres[i]) + j),  (void*)&eneresCP,  sizeof(double));
      j++;
    }
    i++;
  }

  const char *nomChamps[] = {"id", "temperature", "qruiss", "qnappe", "qhypo",
                             "qlacma", "qradso", "qradin", "qevap", "qconv", "enerlo", "eneram", "eneres"}; 
  int nbChamps = sizeof(nomChamps) / sizeof(char*);
  mxArray* etatsQualiteCP = mxCreateStructMatrix(1, m, nbChamps, nomChamps);

  for (unsigned int i = 0; i < m; i++) {
    mxSetField(etatsQualiteCP, i, "id", id[i]);
    mxSetField(etatsQualiteCP, i, "temperature", temperature[i]);
    mxSetField(etatsQualiteCP, i, "qruiss", qruiss[i]);
    mxSetField(etatsQualiteCP, i, "qnappe", qnappe[i]);
    mxSetField(etatsQualiteCP, i, "qhypo",  qhypo[i]);
    mxSetField(etatsQualiteCP, i, "qlacma", qlacma[i]);
    mxSetField(etatsQualiteCP, i, "qradso", qradso[i]);
    mxSetField(etatsQualiteCP, i, "qradin", qradin[i]);
    mxSetField(etatsQualiteCP, i, "qevap" , qevap[i]);
    mxSetField(etatsQualiteCP, i, "qconv" , qconv[i]);
	mxSetField(etatsQualiteCP, i, "enerlo" , enerlo[i]);
	mxSetField(etatsQualiteCP, i, "eneram" , eneram[i]);
	mxSetField(etatsQualiteCP, i, "eneres" , eneres[i]);
  }

  return etatsQualiteCP;
}

//------------------------------------------------------------------

matError SimulationQualite::chargerEtatsCP(Qualite::EtatsCarreauxPartiels& etatsCarreauxPartiels)
{
  FILE_LOG(logDEBUG) << "SimulationQualite::chargerEtatsCP(EtatsCarreauxPartiels& EtatsCarreauxPartiels)";
  int retCode = 0;

  // Pas d'etats precedents
  if (etatsPrecedents_ == NULL) {
    return retCode;
  }

  const mxArray *etats = etatsPrecedents_; 

  mxArray *etatsCP =  MexHelper::mhMxGetField(etats, 0, "etatsCP");
  // Seule variable d'etat ayant un impact sur les calculs
  mxArray *temperature = MexHelper::mhMxGetField(etatsCP, 0, "temperature");

  if (temperature != NULL) {
    // Donnees par CP
    int nbCP = (int)mxGetN(temperature);
  
    double* temperatureData = MexHelper::mhMxGetPr(temperature, "temperature");

    Qualite::EtatCarreauPartiel etatCarreauPartiel;
    for (int i = 0; i < nbCP; i++) {
      etatCarreauPartiel.idCarreauPartiel  = i + 1;
      etatCarreauPartiel.temperature = (float)temperatureData[i];

      etatsCarreauxPartiels.push_back(etatCarreauPartiel);
    }

    retCode = nbCP;
  }

  return retCode;
}

//------------------------------------------------------------------
int SimulationQualite::initialiserAssimilations(const mxArray* assimilations)
{
  FILE_LOG(logDEBUG) << "SimulationQualite::initialiserAssimilations(const mxArray* assimilations)";
  int retCode = 0;

  if (assimilations == NULL) {
    return retCode;
  }

  
  size_t nbDonnees, nbDonneesCP;
  mxArray *etatsCP, *idCP, *temperature;
  double pasDeTempsData, *idCPData, *temperatureData;
  DateChrono datePasDeTemps;
     
  Qualite::EtatCarreauPartielAssim etatCarreauPartielAssim;
  Qualite::EtatsCarreauxPartielsAssim etatsCarreauxPartielsAssim;

  // Nombre de donnees d'assimilation (pas de temps distincts)
  nbDonnees = mxGetNumberOfElements(assimilations);
  for (size_t i = 0; i < nbDonnees; i++) {
    MexHelper::chargerValeurs(assimilations, "pasDeTemps", pasDeTempsData, i);
    //datePasDeTemps = MexHelper::datenumToDate(&pasDeTempsData);
    datePasDeTemps = DateChrono::fromMatlabDatenum(pasDeTempsData);
    
    // Donnees d'assimilation relatives aux CP pour ce pas de temps d'assimilation.
    etatsCP = MexHelper::mhMxGetField(assimilations, i, "etatsCP");
    if (mxGetNumberOfElements(etatsCP) > 0) {
      idCP = MexHelper::mhMxGetField(etatsCP, 0, "id");
      idCPData = MexHelper::mhMxGetPr(idCP, "id");

      // Obtention des pointeurs de donnees
      temperature = MexHelper::mhMxGetField(etatsCP, 0, "temperature");
      etatCarreauPartielAssim.temperatureType = AssimilationHelper::obtenirTypeAssim(temperature);
      temperatureData = mxGetPr(temperature);

       // Nombre de CP vises.
      nbDonneesCP = mxGetNumberOfElements(idCP);
      etatsCarreauxPartielsAssim.clear();
      for (size_t k = 0; k < nbDonneesCP; k++) {

        etatCarreauPartielAssim.idCarreauPartiel = (int)idCPData[k];

        AssimilationHelper::obtenirValeursAssim(temperatureData, etatCarreauPartielAssim.temperatureType, k, etatCarreauPartielAssim.temperature);

        etatsCarreauxPartielsAssim.push_back(etatCarreauPartielAssim);
      }

       assimilationsCP_.insert(std::make_pair(datePasDeTemps, etatsCarreauxPartielsAssim));
    }
  }

  return retCode;
}

//------------------------------------------------------------------
mxArray* SimulationQualite::obtenirEtatsAvantAssimilations()
{
  FILE_LOG(logDEBUG) << "SimulationQualite::obtenirEtatsAvantAssimilations()";
  std::vector<DateChrono> datesdistinctes;

  std::map<DateChrono, Qualite::EtatsCarreauxPartiels>::const_iterator iterCP;
  
  for (iterCP = avantAssimilationsCP_.begin(); iterCP != avantAssimilationsCP_.end(); iterCP++) {
    // Est-ce que cette date existe deja dans la liste?
    if (std::find(datesdistinctes.begin(), datesdistinctes.end(), iterCP->first) == datesdistinctes.end()) {
      datesdistinctes.push_back(iterCP->first);
    }
  }

  // Tri en ordre ascendant
  std::sort(datesdistinctes.begin(), datesdistinctes.end());
  const size_t nbPasDeTemps = datesdistinctes.size();

  const char *nomChamps[] = {"pasDeTemps", "etatsCP"}; 
  int nbChamps = sizeof(nomChamps) / sizeof(char*);
  mxArray* etatsAvant = mxCreateStructMatrix(1, nbPasDeTemps, nbChamps, nomChamps);
  
  mxArray *pasDetemps, *etatsCP;

  // Variable de travail pour l'appel a "obtenirEtatsCP()"
  std::vector<Qualite::EtatsCarreauxPartiels> listeEtatsCarreauxPartiels;
  listeEtatsCarreauxPartiels.reserve(1);

  for (unsigned int i = 0; i < nbPasDeTemps; i++) {
    pasDetemps = mxCreateDoubleMatrix(1,1,mxREAL);
    *mxGetPr(pasDetemps) = MexHelper::dateToDatenum(datesdistinctes[i]);
    mxSetField(etatsAvant, i, "pasDeTemps", pasDetemps);
    
    // Des etatsCP sauvegardes pour cette date?
    if (assimilationsCP_.count(datesdistinctes[i]) > 0) {
      listeEtatsCarreauxPartiels.push_back(avantAssimilationsCP_[datesdistinctes[i]]);
      etatsCP = obtenirEtatsCP(listeEtatsCarreauxPartiels);
      listeEtatsCarreauxPartiels.clear();
    }
    else {
      etatsCP = NULL;
    }
    mxSetField(etatsAvant, i, "etatsCP", etatsCP);
  }

  return etatsAvant;

}
