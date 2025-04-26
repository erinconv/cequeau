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
//#include <boost/math/special_functions/fpclassify.hpp>

//------------------------------------------------------------------
#ifdef ENV_MATLAB
void Simulation::ecrireResultats() {
  FILE_LOG(logDEBUG) << "Simulation::ecrireResultats()";
  MATFile* pfichierResultats = MexHelper::mhMatOpen(fichierResultats_, "w");

  // Anciennes sorties. Conserve en commentaire. Peut etre utile pour tester.
  // Les memes resultats structures differemment se retrouve dans les resultats 
  // plus loin ci-dessous.
  /*****
  FILE_LOG(logINFO) << "Ecriture des bilans";
  ecrireResultatBilans(pfichierResultats);

  FILE_LOG(logINFO) << "Ecriture des debits";
  ecrireResultatDebits(pfichierResultats);

  FILE_LOG(logINFO) << "Ecriture des positions Carreaux Entiers";
  ecrirePositionsCE(pfichierResultats);

  FILE_LOG(logINFO) << "Ecriture des positions Carreaux Partiels";
  ecrirePositionsCP(pfichierResultats);
  *****/
  //FIN. Anciennes sorties. Conserve en commentaire. Peut etre utile pour tester.

  FILE_LOG(logINFO) << "Ecriture des etats des Carreaux Entiers";
  ecrireEtatsCE(pfichierResultats);

  FILE_LOG(logINFO) << "Ecriture des etats des Carreaux Partiels";
  ecrireEtatsCP(pfichierResultats);

  FILE_LOG(logINFO) << "Ecriture des etats des Barrages";
  ecrireEtatsBarrage(pfichierResultats);

  FILE_LOG(logINFO) << "Ecriture des pas de temps";
  ecrirePasDeTemps(pfichierResultats);

  matClose(pfichierResultats);
}
#endif

//------------------------------------------------------------------
#ifdef ENV_MATLAB
matError Simulation::ecrireEtatsCE(MATFile* pfichierMatlab)
{
  FILE_LOG(logDEBUG) << "Simulation::ecrireEtatsCE(MATFile* pfichierMatlab)";
  
  mxArray* etatsCE = obtenirEtatsCE();
  matError retVal = matPutVariable(pfichierMatlab, "etatsCE", etatsCE);
  mxDestroyArray(etatsCE);

  return retVal;
}
#endif

//------------------------------------------------------------------
mxArray* Simulation::obtenirEtatsCE()
{
  FILE_LOG(logDEBUG) << "Simulation::obtenirEtatsCE()";
  return obtenirEtatsCE(bilans_);
}

//------------------------------------------------------------------
mxArray* Simulation::obtenirEtatsCE(const std::vector<EtatsCarreauxEntiers>& etatsPasDeTemps)
{
  FILE_LOG(logDEBUG) << "Simulation::obtenirEtatsCE(const std::vector<EtatsCarreauxEntiers>& etatsPasDeTemps)";
  
  const size_t m = etatsPasDeTemps.size();
  // Nombre de CE qu'on desire en sortie
  size_t n = (size_t)std::count(parametres_.resultatsIdCE().begin(), parametres_.resultatsIdCE().end(), true);
  size_t nbCE = (size_t)etatsPasDeTemps.back().size();

  // Si on a moins de carreaux dans la liste des etats passee en parametres
  // que le filtre, on prend tous les carreaux du parametre.
  n = (nbCE < n ? nbCE : n);

  // Allocation de la memoire. 
  mxArray** id                    = (mxArray**)mxMalloc(m * sizeof(mxArray*)); 
  mxArray** iCarreauEntier        = (mxArray**)mxMalloc(m * sizeof(mxArray*)); 
  mxArray** jCarreauEntier        = (mxArray**)mxMalloc(m * sizeof(mxArray*)); 
  mxArray** niveauEauSol          = (mxArray**)mxMalloc(m * sizeof(mxArray*)); 
  mxArray** niveauEauNappe        = (mxArray**)mxMalloc(m * sizeof(mxArray*)); 
  mxArray** niveauEauLacsMarais   = (mxArray**)mxMalloc(m * sizeof(mxArray*)); 
  mxArray** evapoPotJour          = (mxArray**)mxMalloc(m * sizeof(mxArray*)); 
  mxArray** production            = (mxArray**)mxMalloc(m * sizeof(mxArray*)); 
  mxArray** recharge            = (mxArray**)mxMalloc(m * sizeof(mxArray*)); 

  for (unsigned int i = 0; i < m; i++) {
    id[i]                    = mxCreateDoubleMatrix(1, n, mxREAL);
    iCarreauEntier[i]        = mxCreateDoubleMatrix(1, n, mxREAL);
    jCarreauEntier[i]        = mxCreateDoubleMatrix(1, n, mxREAL);
    niveauEauSol[i]          = mxCreateDoubleMatrix(1, n, mxREAL);
    niveauEauNappe[i]        = mxCreateDoubleMatrix(1, n, mxREAL);
    niveauEauLacsMarais[i]   = mxCreateDoubleMatrix(1, n, mxREAL);
    evapoPotJour[i]          = mxCreateDoubleMatrix(1, n, mxREAL);
    production[i]            = mxCreateDoubleMatrix(1, n, mxREAL);
    recharge[i]            = mxCreateDoubleMatrix(1, n, mxREAL);
  }

  double idCE, iCE, jCE;
  double niveauSol, niveauNappe, niveauLacsMarais, evapoPot, prod, rech;
  int indexCE;
  std::vector<EtatsCarreauxEntiers>::const_iterator tempsIter = etatsPasDeTemps.begin();
  EtatsCarreauxEntiers::const_iterator etatsIter;
  size_t i = 0, j;
  // Parcourt des pas de temps
  for (; tempsIter != etatsPasDeTemps.end(); tempsIter++) {
    j = 0;
    // Parcourt des carreaux entiers
    for (etatsIter = tempsIter->begin();  etatsIter != tempsIter->end(); etatsIter++) {
      idCE             = (double)etatsIter->idCarreauEntier;
      iCE              = (double)etatsIter->iCarreauEntier;
      jCE              = (double)etatsIter->jCarreauEntier;
      niveauSol        = (double)etatsIter->niveauEauSol;
      niveauNappe      = (double)etatsIter->niveauEauNappe;
      niveauLacsMarais = (double)etatsIter->niveauEauLacsMarais;
      evapoPot         = (double)etatsIter->evapoPotJour;
      prod             = (double)etatsIter->production;
      rech             = (double)etatsIter->recharge;

      // Selection des CE qu'on desire en sortie
      indexCE = (int)idCE - 1;
      if (parametres_.resultatsIdCE()[indexCE]) {
        memcpy((void*)(mxGetPr(id[i]) + j),                    (void*)&idCE,             sizeof(double));
        memcpy((void*)(mxGetPr(iCarreauEntier[i]) + j),        (void*)&iCE,              sizeof(double));
        memcpy((void*)(mxGetPr(jCarreauEntier[i]) + j),        (void*)&jCE,              sizeof(double));
        memcpy((void*)(mxGetPr(niveauEauSol[i]) + j),          (void*)&niveauSol,        sizeof(double));
        memcpy((void*)(mxGetPr(niveauEauNappe[i]) + j),        (void*)&niveauNappe,      sizeof(double));
        memcpy((void*)(mxGetPr(niveauEauLacsMarais[i]) + j),   (void*)&niveauLacsMarais, sizeof(double));
        memcpy((void*)(mxGetPr(evapoPotJour[i]) + j),          (void*)&evapoPot,         sizeof(double));
        memcpy((void*)(mxGetPr(production[i]) + j),            (void*)&prod,             sizeof(double));
        memcpy((void*)(mxGetPr(recharge[i]) + j),            (void*)&rech,             sizeof(double));
        j++;
      }
    }
    i++;
  }

  const char *nomChamps[] = {"id", "iCarreauEntier", "jCarreauEntier", "niveauEauSol", "niveauEauNappe", 
                             "niveauEauLacsMarais", "evapoPotJour", "production", "recharge"}; 
  int nbChamps = sizeof(nomChamps) / sizeof(char*);
  mxArray* etatsCE = mxCreateStructMatrix(1, m, nbChamps, nomChamps);

  for (unsigned int i = 0; i < m; i++) {
    mxSetField(etatsCE, i, "id",                    id[i]);
    mxSetField(etatsCE, i, "iCarreauEntier",        iCarreauEntier[i]);
    mxSetField(etatsCE, i, "jCarreauEntier",        jCarreauEntier[i]);
    mxSetField(etatsCE, i, "niveauEauSol",          niveauEauSol[i]);
    mxSetField(etatsCE, i, "niveauEauNappe",        niveauEauNappe[i]);
    mxSetField(etatsCE, i, "niveauEauLacsMarais",   niveauEauLacsMarais[i]);
    mxSetField(etatsCE, i, "evapoPotJour",          evapoPotJour[i]);
    mxSetField(etatsCE, i, "production",            production[i]);
    mxSetField(etatsCE, i, "recharge",              recharge[i]);
  }

  return etatsCE;
}

//------------------------------------------------------------------
#ifdef ENV_MATLAB
matError Simulation::ecrireEtatsCP(MATFile* pfichierMatlab)
{
  FILE_LOG(logDEBUG) << "Simulation::ecrireEtatsCP(MATFile* pfichierMatlab)";

  mxArray* etatsCP = obtenirEtatsCP();
  matError status = matPutVariable(pfichierMatlab, "etatsCP", etatsCP);
  mxDestroyArray(etatsCP);

  return status;
}
#endif

//------------------------------------------------------------------
mxArray* Simulation::obtenirEtatsCP() 
{
  FILE_LOG(logDEBUG) << "Simulation::obtenirEtatsCP()";
  return obtenirEtatsCP(debits_);
}

//------------------------------------------------------------------
mxArray* Simulation::obtenirEtatsCP(const std::vector<EtatsCarreauxPartiels>& etatsPasDeTemps)
{
  FILE_LOG(logDEBUG) << "Simulation::obtenirEtatsCP(const std::vector<EtatsCarreauxPartiels>& etatsPasDeTemps)";
  const size_t m = etatsPasDeTemps.size();
  // Nombre de CP qu'on desire en sortie
  size_t n = (size_t)std::count(parametres_.resultatsIdCP().begin(), parametres_.resultatsIdCP().end(), true);
  size_t nbCP = (size_t)etatsPasDeTemps.back().size();

  // Si on a moins de carreaux dans la liste des etats passee en parametres
  // que le filtre, on prend tous les carreaux du parametre.
  n = (nbCP < n ? nbCP : n);

  // Allocation de la memoire.
  mxArray** id      = (mxArray**)mxMalloc(m * sizeof(mxArray*)); 
  mxArray** apport  = (mxArray**)mxMalloc(m * sizeof(mxArray*)); 
  mxArray** volume  = (mxArray**)mxMalloc(m * sizeof(mxArray*));
  mxArray** debit   = (mxArray**)mxMalloc(m * sizeof(mxArray*));

  for (unsigned int i = 0; i < m; i++) {
    id[i]     = mxCreateDoubleMatrix(1, n, mxREAL);
    apport[i] = mxCreateDoubleMatrix(1, n, mxREAL);
    volume[i] = mxCreateDoubleMatrix(1, n, mxREAL);
    debit[i]  = mxCreateDoubleMatrix(1, n, mxREAL);
  }

  double apportCP, volumeCP, debitCP, idCP;
  int indexCP;
  std::vector<EtatsCarreauxPartiels>::const_iterator tempsIter = etatsPasDeTemps.begin();
  EtatsCarreauxPartiels::const_iterator etatsIter;
  size_t i = 0, j;
  // Parcourt des pas de temps
  for (; tempsIter != etatsPasDeTemps.end(); tempsIter++) {
    j = 0;
    // Parcourt des carreaux partiels
    for (etatsIter = tempsIter->begin();  etatsIter != tempsIter->end(); etatsIter++) {
      idCP     = (double)etatsIter->idCarreauPartiel;
      apportCP = (double)etatsIter->apport;
      volumeCP = (double)etatsIter->volume;
      debitCP  = (double)etatsIter->debit;

      // Selection des CE qu'on desire en sortie
      indexCP = (int)idCP - 1;
      if (parametres_.resultatsIdCP()[indexCP]) {
        memcpy((void*)(mxGetPr(id[i]) + j),     (void*)&idCP,     sizeof(double));
        memcpy((void*)(mxGetPr(apport[i]) + j), (void*)&apportCP, sizeof(double));
        memcpy((void*)(mxGetPr(volume[i]) + j), (void*)&volumeCP, sizeof(double));
        memcpy((void*)(mxGetPr(debit[i]) + j),  (void*)&debitCP,  sizeof(double));
        j++;
      }
    }
    i++;
  }

  const char *nomChamps[] = {"id" ,"apport", "volume", "debit"}; 
  int nbChamps = sizeof(nomChamps) / sizeof(char*);
  mxArray* etatsCP = mxCreateStructMatrix(1, m, nbChamps, nomChamps);

  for (unsigned int i = 0; i < m; i++) {
    mxSetField(etatsCP, i, "id",     id[i]);
    mxSetField(etatsCP, i, "apport", apport[i]);
    mxSetField(etatsCP, i, "volume", volume[i]);
    mxSetField(etatsCP, i, "debit",  debit[i]);
  }

  return etatsCP;
}

//------------------------------------------------------------------
#ifdef ENV_MATLAB
matError Simulation::ecrirePositionsCP(MATFile* pfichierMatlab)
{
  FILE_LOG(logDEBUG) << "Simulation::ecrirePositionsCP(MATFile* pfichierMatlab)";
  int nbCarreauxPartiels = bassinVersant_.nbCarreauxPartiels();
  // 3 = i, j, (a,b,c ou d)
  mxArray* positionsCP = mxCreateDoubleMatrix(nbCarreauxPartiels, 3, mxREAL);

  std::vector<CarreauPartielPtr>::const_iterator carreauxPartielsIterator = bassinVersant_.carreauxPartiels().begin();

  int i = 0, j, index;
  double absI, ordJ, codeCarreauPartiel;
  for (; carreauxPartielsIterator != bassinVersant_.carreauxPartiels().end(); carreauxPartielsIterator++) {
    absI = (double)carreauxPartielsIterator->get()->iCarreauEntier();
    ordJ = (double)carreauxPartielsIterator->get()->jCarreauEntier();
    codeCarreauPartiel = (double)carreauxPartielsIterator->get()->codeCarreauPartiel();
    j = 0;
    index = i + j * nbCarreauxPartiels;
    memcpy((void*)(mxGetPr(positionsCP) + index), (void*)&absI, sizeof(double));
    j = 1;
    index = i + j * nbCarreauxPartiels;
    memcpy((void*)(mxGetPr(positionsCP) + index), (void*)&ordJ, sizeof(double));
    j = 2;
    index = i + j * nbCarreauxPartiels;
    memcpy((void*)(mxGetPr(positionsCP) + index), (void*)&codeCarreauPartiel, sizeof(double));
    i++;
  }
  
  matError status = matPutVariable(pfichierMatlab, "positionsCP", positionsCP);
  mxDestroyArray(positionsCP);

  return status;
}
#endif

//------------------------------------------------------------------
#ifdef ENV_MATLAB
matError Simulation::ecrireEtatsBarrage(MATFile* pfichierMatlab)
{
  FILE_LOG(logDEBUG) << "Simulation::ecrireEtatsBarrage(MATFile* pfichierMatlab)";

  mxArray* etatsBarrages = obtenirEtatsBarrage();
  matError status = matPutVariable(pfichierMatlab, "etatsBarrage", etatsBarrages);
  mxDestroyArray(etatsBarrages);

  return status;
}
#endif

//------------------------------------------------------------------
mxArray* Simulation::obtenirEtatsBarrage()
{
  FILE_LOG(logDEBUG) << "Simulation::obtenirEtatsBarrage()";
  return obtenirEtatsBarrage(barrages_);
}

//------------------------------------------------------------------
mxArray* Simulation::obtenirEtatsBarrage(const std::vector<EtatsBarrages>& etatsPasDeTemps)
{
  FILE_LOG(logDEBUG) << "Simulation::obtenirEtatsBarrage(const std::vector<EtatsBarrages>& etatsPasDeTemps)";
  const size_t m = etatsPasDeTemps.size();

  if (m == 0) {
	  const char *nomChamps[] = {""};
	  mxArray* etatsBarrages = mxCreateStructMatrix(1, 0, 0, nomChamps);
    return etatsBarrages;
  }
  size_t n = (size_t)etatsPasDeTemps.back().size();
  size_t nbBar = (size_t)etatsPasDeTemps.back().size();
  size_t nDebitsOut = (size_t)etatsPasDeTemps.back().back().debitsSortants.size();
  // Si on a moins de carreaux dans la liste des etats passee en parametres
  // que le filtre, on prend tous les carreaux du parametre.
  n = (nbBar < n ? nbBar : n);

  // Allocation de la memoire.
  mxArray** idCP      = (mxArray**)mxMalloc(m * sizeof(mxArray*));
  mxArray** volume    = (mxArray**)mxMalloc(m * sizeof(mxArray*));
  mxArray** niveau    = (mxArray**)mxMalloc(m * sizeof(mxArray*));
  mxArray** debitIn   = (mxArray**)mxMalloc(m * sizeof(mxArray*));
  mxArray** debitsOut = (mxArray**)mxMalloc(m * sizeof(mxArray*));
  for (unsigned int i = 0; i < m; i++) {
    idCP[i]      = mxCreateDoubleMatrix(1, n, mxREAL);
    volume[i]    = mxCreateDoubleMatrix(1, n, mxREAL);
    niveau[i]    = mxCreateDoubleMatrix(1, n, mxREAL);
    debitIn[i]   = mxCreateDoubleMatrix(1, n, mxREAL);
    debitsOut[i] = mxCreateDoubleMatrix(nDebitsOut, n, mxREAL);
  }

  double volumeBarrage, niveauBarrage, id, debitInBarrage, debitsOutBarrage;
  std::vector<EtatsBarrages>::const_iterator tempsIter = etatsPasDeTemps.begin();
  EtatsBarrages::const_iterator etatsIter;
  size_t i = 0, j;
  // Parcourt des pas de temps
  for (; tempsIter != etatsPasDeTemps.end(); tempsIter++) {
    j = 0;
    // Parcourt des barrages
    for (etatsIter = tempsIter->begin();  etatsIter != tempsIter->end(); etatsIter++) {
      id = etatsIter->idCarreauPartiel;
      volumeBarrage = etatsIter->volume;
      niveauBarrage = etatsIter->niveau;
      debitInBarrage = etatsIter->debitEntrant;

      memcpy((void*)(mxGetPr(idCP[i]) + j),      (void*)&id,             sizeof(double));
      memcpy((void*)(mxGetPr(volume[i]) + j),    (void*)&volumeBarrage,  sizeof(double));
      memcpy((void*)(mxGetPr(niveau[i]) + j),    (void*)&niveauBarrage,  sizeof(double));
      memcpy((void*)(mxGetPr(debitIn[i]) + j),   (void*)&debitInBarrage, sizeof(double));
      for (unsigned int k = 0; k < etatsIter->debitsSortants.size(); k++) {
        debitsOutBarrage = (double)etatsIter->debitsSortants[k];
        memcpy((void*)(mxGetPr(debitsOut[i]) + k + j * nDebitsOut), (void*)&debitsOutBarrage, sizeof(double));
      }
      j++;
    }
    i++;
  }

  const char *nomChamps[] = {"idCarreauPartiel", "volume", "niveau", "debitEntrant", "debitsSortants"}; 
  int nbChamps = sizeof(nomChamps) / sizeof(char*);
  mxArray* etatsBarrages = mxCreateStructMatrix(1, m, nbChamps, nomChamps);

  for (unsigned int i = 0; i < m; i++) {
    mxSetField(etatsBarrages, i, "idCarreauPartiel", idCP[i]);
    mxSetField(etatsBarrages, i, "volume", volume[i]);
    mxSetField(etatsBarrages, i, "niveau", niveau[i]);
    mxSetField(etatsBarrages, i, "debitEntrant", debitIn[i]);
    mxSetField(etatsBarrages, i, "debitsSortants", debitsOut[i]);
  }

  return etatsBarrages;
}

//------------------------------------------------------------------
#ifdef ENV_MATLAB
matError Simulation::ecrirePasDeTemps(MATFile* pfichierMatlab)
{
  FILE_LOG(logDEBUG) << "Simulation::ecrirePasDeTemps(MATFile* pfichierMatlab)";

  mxArray* pasDeTemps = obtenirPasDeTemps();
  matError status = matPutVariable(pfichierMatlab, "pasDeTemps", pasDeTemps);
  mxDestroyArray(pasDeTemps);

  return status;
}
#endif
//------------------------------------------------------------------
mxArray* Simulation::obtenirPasDeTemps()
{
  FILE_LOG(logDEBUG) << "Simulation::obtenirPasDeTemps()";
  int nbPasDeTemps = parametres_.nbPasSimulation() + 1; // Incluant l'initialisation
  mxArray* pasDeTemps = mxCreateDoubleMatrix(1, nbPasDeTemps, mxREAL);
  // Duree en format Matlab datenum d'un pas de temps.
  // Pour un datenum, la partie decimale est une fraction de jour.
  // Autrement dit les heures, min et sec sont la partie decimale.
  double dureePasDeTemps = (double)parametres_.dureeHeuresPasSimulation() / 24.0; // 24 heures
  //double dateDebut = parametres_.dateDebut().day_count().as_number() - MexHelper::OFFSET_DATENUM; 
  double dateDebut = parametres_.dateDebut().toMatlabDatenum(); 
  double datePasDeTemps;

  for (int i = 0; i < nbPasDeTemps; i++) {
    datePasDeTemps = dateDebut + (i * dureePasDeTemps);
    memcpy((void*)(mxGetPr(pasDeTemps) + i), (void*)&datePasDeTemps, sizeof(double));
  }
  
  return pasDeTemps;
}

//------------------------------------------------------------------
mxArray* Simulation::obtenirEtatsAvantAssimilations()
{
  FILE_LOG(logDEBUG) << "Simulation::obtenirEtatsAvantAssimilations()";
  std::vector<DateChrono> datesdistinctes;

  std::map<DateChrono, EtatsCarreauxEntiers>::const_iterator iterCE;
  std::map<DateChrono, EtatsCarreauxPartiels>::const_iterator iterCP;
  std::map<DateChrono, EtatsBarrages>::const_iterator iterBarrage;
  
  for (iterCE = avantAssimilationsCE_.begin(); iterCE != avantAssimilationsCE_.end(); iterCE++) {
    datesdistinctes.push_back(iterCE->first);
  }

  for (iterCP = avantAssimilationsCP_.begin(); iterCP != avantAssimilationsCP_.end(); iterCP++) {
    // Est-ce que cette date existe deja dans la liste?
    if (std::find(datesdistinctes.begin(), datesdistinctes.end(), iterCP->first) == datesdistinctes.end()) {
      datesdistinctes.push_back(iterCP->first);
    }
  }
  
  iterBarrage = avantAssimilationsBarrages_.begin();
  for (; iterBarrage != avantAssimilationsBarrages_.end(); iterBarrage++) {
    // Est-ce que cette date existe deja dans la liste?
    if (std::find(datesdistinctes.begin(), datesdistinctes.end(), iterBarrage->first) == datesdistinctes.end()) {
      datesdistinctes.push_back(iterBarrage->first);
    }
  }

  // Tri en ordre ascendant
  std::sort(datesdistinctes.begin(), datesdistinctes.end());
  const size_t nbPasDeTemps = datesdistinctes.size();

  const char *nomChamps[] = {"pasDeTemps", "etatsCE", "etatsCP", "etatsBarrage"}; 
  int nbChamps = sizeof(nomChamps) / sizeof(char*);
  mxArray* etatsAvant = mxCreateStructMatrix(1, nbPasDeTemps, nbChamps, nomChamps);
  
  mxArray *pasDetemps, *etatsCE, *etatsCP, *etatsBarrage;

  // Variable de travail pour l'appel a "obtenirEtatsCE()"
  std::vector<EtatsCarreauxEntiers> listeEtatsCarreauxEntiers;
  listeEtatsCarreauxEntiers.reserve(1);

  // Variable de travail pour l'appel a "obtenirEtatsCP()"
  std::vector<EtatsCarreauxPartiels> listeEtatsCarreauxPartiels;
  listeEtatsCarreauxPartiels.reserve(1);

  // Variable de travail pour l'appel a "obtenirEtatsBarrage()"
  std::vector<EtatsBarrages> listeEtatsBarrages;
  listeEtatsBarrages.reserve(1);

  for (unsigned int i = 0; i < nbPasDeTemps; i++) {
    pasDetemps = mxCreateDoubleMatrix(1,1,mxREAL);
    *mxGetPr(pasDetemps) = MexHelper::dateToDatenum(datesdistinctes[i]);
    mxSetField(etatsAvant, i, "pasDeTemps", pasDetemps);
    
    // Des etatsCE sauvegardes pour cette date?
    if (assimilationsCE_.count(datesdistinctes[i]) > 0) {
      listeEtatsCarreauxEntiers.push_back(avantAssimilationsCE_[datesdistinctes[i]]);
      etatsCE = obtenirEtatsCE(listeEtatsCarreauxEntiers);
      listeEtatsCarreauxEntiers.clear();
    }
    else {
      etatsCE = NULL;
    }
    mxSetField(etatsAvant, i, "etatsCE", etatsCE);

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

    // Des etatsBarrage sauvegardes pour cette date?
    if (assimilationsBarrages_.count(datesdistinctes[i]) > 0) {
      listeEtatsBarrages.push_back(avantAssimilationsBarrages_[datesdistinctes[i]]);
      etatsBarrage = obtenirEtatsBarrage(listeEtatsBarrages);
      listeEtatsBarrages.clear();
    }
    else {
      etatsBarrage = NULL;
    }
    mxSetField(etatsAvant, i, "etatsBarrage", etatsBarrage);
  }

  return etatsAvant;

}

//------------------------------------------------------------------

matError Simulation::chargerEtatsCE(EtatsCarreauxEntiers& etatsCarreauxEntiers)
{
  FILE_LOG(logDEBUG) << "Simulation::chargerEtatsCE(EtatsCarreauxEntiers& etatsCarreauxEntiers)";
  int retCode = 0;

  // Pas d'etats precedents
  if (etatsPrecedents_ == NULL) {
    return retCode;
  }

  const mxArray *etats = etatsPrecedents_; 

  mxArray *etatsCE =  MexHelper::mhMxGetField(etats, 0, "etatsCE");
  
  mxArray *iCarreauEntier        = MexHelper::mhMxGetField(etatsCE, 0, "iCarreauEntier");
  mxArray *jCarreauEntier        = MexHelper::mhMxGetField(etatsCE, 0, "jCarreauEntier");
  mxArray *niveauEauSol          = MexHelper::mhMxGetField(etatsCE, 0, "niveauEauSol");
  mxArray *niveauEauNappe        = MexHelper::mhMxGetField(etatsCE, 0, "niveauEauNappe");
  mxArray *niveauEauLacsMarais   = MexHelper::mhMxGetField(etatsCE, 0, "niveauEauLacsMarais");
  mxArray *evapoPotJour          = MexHelper::mhMxGetField(etatsCE, 0, "evapoPotJour");
  mxArray *production            = MexHelper::mhMxGetField(etatsCE, 0, "production");

  if (iCarreauEntier != NULL) {
    // Donnees par CE
    int nbCE = (int)mxGetN(iCarreauEntier);
  
    double* iCarreauEntierData        = MexHelper::mhMxGetPr(iCarreauEntier, "iCarreauEntier");
    double* jCarreauEntierData        = MexHelper::mhMxGetPr(jCarreauEntier, "jCarreauEntier");
    double* niveauEauSolData          = MexHelper::mhMxGetPr(niveauEauSol, "niveauEauSol");
    double* niveauEauNappeData        = MexHelper::mhMxGetPr(niveauEauNappe, "niveauEauNappe");
    double* niveauEauLacsMaraisData   = MexHelper::mhMxGetPr(niveauEauLacsMarais, "niveauEauLacsMarais");
    double* evapoPotJourData          = MexHelper::mhMxGetPr(evapoPotJour, "evapoPotJour");
    double* productionData            = MexHelper::mhMxGetPr(production, "production");

    EtatCarreauEntier etatCarreauEntier;
    for (int i = 0; i < nbCE; i++) {
      etatCarreauEntier.idCarreauEntier             = i + 1;
      etatCarreauEntier.iCarreauEntier              = (int)iCarreauEntierData[i];
      etatCarreauEntier.jCarreauEntier              = (int)jCarreauEntierData[i];
      etatCarreauEntier.niveauEauSol                = (float)niveauEauSolData[i];
      etatCarreauEntier.niveauEauNappe              = (float)niveauEauNappeData[i];
      etatCarreauEntier.niveauEauLacsMarais         = (float)niveauEauLacsMaraisData[i];
      etatCarreauEntier.evapoPotJour                = (float)evapoPotJourData[i];
      etatCarreauEntier.production                  = (float)productionData[i];

      etatsCarreauxEntiers.push_back(etatCarreauEntier);
    }
    retCode = (int)nbCE;
  }

  return retCode;
}
//------------------------------------------------------------------

matError Simulation::chargerEtatsCP(EtatsCarreauxPartiels& etatsCarreauxPartiels)
{
  FILE_LOG(logDEBUG) << "Simulation::chargerEtatsCP(EtatsCarreauxPartiels& EtatsCarreauxPartiels)";
  int retCode = 0;

  // Pas d'etats precedents
  if (etatsPrecedents_ == NULL) {
    return retCode;
  }

  const mxArray *etats = etatsPrecedents_; 

  mxArray *etatsCP =  MexHelper::mhMxGetField(etats, 0, "etatsCP");
  
  mxArray *apport = MexHelper::mhMxGetField(etatsCP, 0, "apport");
  mxArray *volume = MexHelper::mhMxGetField(etatsCP, 0, "volume");
  mxArray *debit  = MexHelper::mhMxGetField(etatsCP, 0, "debit");

  if (apport != NULL) {
    // Donnees par CP
    int nbCP = (int)mxGetN(apport);
  
    double* apportData = MexHelper::mhMxGetPr(apport, "apport");
    double* volumeData = MexHelper::mhMxGetPr(volume, "volume");
    double* debitData  = MexHelper::mhMxGetPr(debit, "debit");

    EtatCarreauPartiel etatCarreauPartiel;
    for (int i = 0; i < nbCP; i++) {
      etatCarreauPartiel.idCarreauPartiel  = i + 1;
      etatCarreauPartiel.apport = (float)apportData[i];
      etatCarreauPartiel.volume = (float)volumeData[i];
      etatCarreauPartiel.debit  = (float)debitData[i];

      etatsCarreauxPartiels.push_back(etatCarreauPartiel);
    }

    retCode = nbCP;
  }

  return retCode;
}

//------------------------------------------------------------------

matError Simulation::chargerEtatsBarrages()
{
  FILE_LOG(logDEBUG) << "Simulation::chargerEtatsBarrages()";
  int retCode = 0;

  if (!bassinVersant_.possedeBarrage()) {
    return -1;
  }
  // Pas d'etats precedents
  if (etatsPrecedents_ == NULL) {
    return retCode;
  }

  const mxArray *etats = etatsPrecedents_; 

  mxArray *etatsBarrage =  MexHelper::mhMxGetField(etats, 0, "etatsBarrage");
  
  mxArray *idCarreauPartiel = MexHelper::mhMxGetField(etatsBarrage, 0, "idCarreauPartiel");
  mxArray *volume           = MexHelper::mhMxGetField(etatsBarrage, 0, "volume");

  if (idCarreauPartiel != NULL) {
    // Donnees par barrage
    size_t nbBarrages = mxGetN(idCarreauPartiel);
    double* idCarreauPartielData = MexHelper::mhMxGetPr(idCarreauPartiel, "idCarreauPartiel");
    double* volumeData           = MexHelper::mhMxGetPr(volume, "volume");

    for (size_t i = 0; i < nbBarrages; i++) {
      BarragePtr barrage = bassinVersant_.trouverBarrageParIdCP((int)idCarreauPartielData[i]);

      if (barrage) {
        barrage->volume((float)volumeData[i]);
        retCode++;
      }
    }
  }

  return retCode;
}

//------------------------------------------------------------------
int Simulation::initialiserAssimilations(const mxArray* assimilations)
{
  FILE_LOG(logDEBUG) << "Simulation::initialiserAssimilations(const mxArray* assimilations)";
  int retCode = 0;

  if (assimilations == NULL) {
    return retCode;
  }

  
  size_t nbDonnees, nbDonneesCE, nbDonneesCP, nbDonneesBarrage;

  mxArray *etatsCE, *idCE;
  mxArray *niveauEauSol, *niveauEauNappe, *niveauEauLacsMarais, *evapoPotJour, *production;
  mxArray *etatsCP, *idCP, *apport, *volume, *debit;
  mxArray *etatsBarrage, *niveau;

  double pasDeTempsData, *idCEData;
  double *niveauEauSolData, *niveauEauNappeData, *niveauEauLacsMaraisData;
  double *evapoPotJourData, *productionData, *idCPData  , *apportData, *volumeData, *debitData, *niveauData;
  DateChrono datePasDeTemps;
     
  EtatCarreauEntierAssim etatCarreauEntierAssim;
  EtatsCarreauxEntiersAssim etatsCarreauxEntiersAssim;
  EtatCarreauPartielAssim etatCarreauPartielAssim;
  EtatsCarreauxPartielsAssim etatsCarreauxPartielsAssim;
  EtatBarrageAssim etatBarrageAssim;
  EtatsBarragesAssim etatsBarragesAssim;

  // Nombre de donnees d'assimilation (pas de temps distincts)
  nbDonnees = mxGetNumberOfElements(assimilations);
  for (size_t i = 0; i < nbDonnees; i++) {
    MexHelper::chargerValeurs(assimilations, "pasDeTemps", pasDeTempsData, i);
    //datePasDeTemps = MexHelper::datenumToDate(&pasDeTempsData);
    datePasDeTemps = DateChrono::fromMatlabDatenum(pasDeTempsData);
    etatsCE = MexHelper::mhMxGetField(assimilations, i, "etatsCE");
    
    /***** Carreaux Entiers *****/
    // Donnees d'assimilation relatives aux CE pour ce pas de temps d'assimilation
    if (mxGetNumberOfElements(etatsCE) > 0) {
      idCE = MexHelper::mhMxGetField(etatsCE, 0, "id");
      idCEData = MexHelper::mhMxGetPr(idCE, "id");

      // Obtention des pointeurs de donnees
      // On utilise mxGetPr plutot que MexHelper::mhMxGetPr pour la possibilite 
      // d'avoir un pointeur null
      niveauEauSol = MexHelper::mhMxGetField(etatsCE, 0, "niveauEauSol");
      etatCarreauEntierAssim.niveauEauSolType = AssimilationHelper::obtenirTypeAssim(niveauEauSol);
      niveauEauSolData = mxGetPr(niveauEauSol);

      niveauEauNappe = MexHelper::mhMxGetField(etatsCE, 0, "niveauEauNappe");
      etatCarreauEntierAssim.niveauEauNappeType = AssimilationHelper::obtenirTypeAssim(niveauEauNappe);
      niveauEauNappeData = mxGetPr(niveauEauNappe);

      niveauEauLacsMarais = MexHelper::mhMxGetField(etatsCE, 0, "niveauEauLacsMarais");
      etatCarreauEntierAssim.niveauEauLacsMaraisType = AssimilationHelper::obtenirTypeAssim(niveauEauLacsMarais);
      niveauEauLacsMaraisData = mxGetPr(niveauEauLacsMarais);

      evapoPotJour = MexHelper::mhMxGetField(etatsCE, 0, "evapoPotJour");
      etatCarreauEntierAssim.evapoPotJourType = AssimilationHelper::obtenirTypeAssim(evapoPotJour);
      evapoPotJourData = mxGetPr(evapoPotJour);

      production = MexHelper::mhMxGetField(etatsCE, 0, "production");
      etatCarreauEntierAssim.productionType = AssimilationHelper::obtenirTypeAssim(production);
      productionData = mxGetPr(production);

      // Nombre de CE vises.
      nbDonneesCE = mxGetNumberOfElements(idCE);
      etatsCarreauxEntiersAssim.clear();
      for (size_t j = 0; j < nbDonneesCE; j++) {
        // Pas utilises pour l'assimilation
        etatCarreauEntierAssim.iCarreauEntier = -1;
        etatCarreauEntierAssim.jCarreauEntier = -1;

        etatCarreauEntierAssim.idCarreauEntier = (int)idCEData[j];

        AssimilationHelper::obtenirValeursAssim(niveauEauSolData, etatCarreauEntierAssim.niveauEauSolType, 
                            j, etatCarreauEntierAssim.niveauEauSol);

        AssimilationHelper::obtenirValeursAssim(niveauEauNappeData, etatCarreauEntierAssim.niveauEauNappeType, 
                            j, etatCarreauEntierAssim.niveauEauNappe);

        AssimilationHelper::obtenirValeursAssim(niveauEauLacsMaraisData, etatCarreauEntierAssim.niveauEauLacsMaraisType, 
                            j, etatCarreauEntierAssim.niveauEauLacsMarais);

        AssimilationHelper::obtenirValeursAssim(evapoPotJourData, etatCarreauEntierAssim.evapoPotJourType, 
                            j, etatCarreauEntierAssim.evapoPotJour);

        AssimilationHelper::obtenirValeursAssim(productionData, etatCarreauEntierAssim.productionType, 
                            j, etatCarreauEntierAssim.production);

        etatsCarreauxEntiersAssim.push_back(etatCarreauEntierAssim);
      }

      assimilationsCE_.insert(std::make_pair(datePasDeTemps, etatsCarreauxEntiersAssim));
    }

    /***** Carreaux Partiels *****/
    // Donnees d'assimilation relatives aux CP pour ce pas de temps d'assimilation.
    etatsCP = MexHelper::mhMxGetField(assimilations, i, "etatsCP");
    if (mxGetNumberOfElements(etatsCP) > 0) {
      idCP = MexHelper::mhMxGetField(etatsCP, 0, "id");
      idCPData = MexHelper::mhMxGetPr(idCP, "id");

      // Obtention des pointeurs de donnees
      apport = MexHelper::mhMxGetField(etatsCP, 0, "apport");
      etatCarreauPartielAssim.apportType = AssimilationHelper::obtenirTypeAssim(apport);
      apportData = mxGetPr(apport);

      volume = MexHelper::mhMxGetField(etatsCP, 0, "volume");
      etatCarreauPartielAssim.volumeType = AssimilationHelper::obtenirTypeAssim(volume);
      volumeData = mxGetPr(volume);

      debit = MexHelper::mhMxGetField(etatsCP, 0, "debit");
      etatCarreauPartielAssim.debitType = AssimilationHelper::obtenirTypeAssim(debit);
      debitData = mxGetPr(debit);

       // Nombre de CP vises.
      nbDonneesCP = mxGetNumberOfElements(idCP);
      etatsCarreauxPartielsAssim.clear();
      for (size_t k = 0; k < nbDonneesCP; k++) {

        etatCarreauPartielAssim.idCarreauPartiel = (int)idCPData[k];

        AssimilationHelper::obtenirValeursAssim(apportData, etatCarreauPartielAssim.apportType, 
                            k, etatCarreauPartielAssim.apport);

        AssimilationHelper::obtenirValeursAssim(volumeData, etatCarreauPartielAssim.volumeType, 
                            k, etatCarreauPartielAssim.volume);

        AssimilationHelper::obtenirValeursAssim(debitData, etatCarreauPartielAssim.debitType, 
                            k, etatCarreauPartielAssim.debit);

        etatsCarreauxPartielsAssim.push_back(etatCarreauPartielAssim);
      }

       assimilationsCP_.insert(std::make_pair(datePasDeTemps, etatsCarreauxPartielsAssim));
    }
    
    /***** Barrages *****/
    // Donnees d'assimilation relatives aux barrages pour ce pas de temps d'assimilation.
    etatsBarrage = MexHelper::mhMxGetField(assimilations, i, "etatsBarrage");
    if (mxGetNumberOfElements(etatsBarrage) > 0) {
      idCP = MexHelper::mhMxGetField(etatsBarrage, 0, "idCarreauPartiel");
      idCPData = MexHelper::mhMxGetPr(idCP, "idCarreauPartiel");

      // Obtention des pointeurs de donnees
      volume = MexHelper::mhMxGetField(etatsBarrage, 0, "volume");
      etatBarrageAssim.volumeType = AssimilationHelper::obtenirTypeAssim(volume);
      volumeData = mxGetPr(volume);

      niveau = MexHelper::mhMxGetField(etatsBarrage, 0, "niveau");
      etatBarrageAssim.niveauType = AssimilationHelper::obtenirTypeAssim(niveau);
      niveauData = mxGetPr(niveau);

       // Nombre de Barrages vises.
      nbDonneesBarrage = mxGetNumberOfElements(idCP);
      etatsBarragesAssim.clear();
      for (size_t l = 0; l < nbDonneesBarrage; l++) {

        etatBarrageAssim.idCarreauPartiel = (int)idCPData[l];

        AssimilationHelper::obtenirValeursAssim(volumeData, etatBarrageAssim.volumeType, l, etatBarrageAssim.volume);
        AssimilationHelper::obtenirValeursAssim(niveauData, etatBarrageAssim.niveauType, l, etatBarrageAssim.niveau);

        etatsBarragesAssim.push_back(etatBarrageAssim);
      }

      assimilationsBarrages_.insert(std::make_pair(datePasDeTemps, etatsBarragesAssim));
    }
  }

  return retCode;
}
