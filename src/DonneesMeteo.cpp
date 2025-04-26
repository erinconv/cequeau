//****************************************************************************
// Fichier: DonneesMeteo.cpp
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
#include "DonneesMeteo.h"

//------------------------------------------------------------------
DonneesMeteo::DonneesMeteo()
  : nbCarreauxEntiers_(0), nbPasDeTemps_(0), estPtot_(false)
{
  FILE_LOG(logDEBUG) << "DonneesMeteo::DonneesMeteo()";
}
  //------------------------------------------------------------------
DonneesMeteo::DonneesMeteo(int nbCarreauxEntiers, int nbPasDeTemps)
  : nbCarreauxEntiers_(nbCarreauxEntiers), nbPasDeTemps_(nbPasDeTemps), estPtot_(false)
{
  FILE_LOG(logDEBUG) << "DonneesMeteo::DonneesMeteo(int nbCarreauxEntiers, int nbPasDeTemps)";
  valeurs_.reserve(nbPasDeTemps);
}

//------------------------------------------------------------------
DonneesMeteo::~DonneesMeteo()
{
  FILE_LOG(logDEBUG) << "DonneesMeteo::~DonneesMeteo()";
}

//------------------------------------------------------------------
int DonneesMeteo::nbCarreauxEntiers() const
{
  FILE_LOG(logDEBUG) << "DonneesMeteo::nbCarreauxEntiers()";
  return nbCarreauxEntiers_;
}

//------------------------------------------------------------------
int DonneesMeteo::nbPasDeTemps() const
{
  FILE_LOG(logDEBUG) << "DonneesMeteo::nbPasDeTemps()";
  return nbPasDeTemps_;
}

//------------------------------------------------------------------
bool DonneesMeteo::estPtot() const
{
  FILE_LOG(logDEBUG) << "DonneesMeteo::estPtot()";
  return estPtot_;
}

//------------------------------------------------------------------
const std::vector<MeteoGrille>& DonneesMeteo::valeurs() const
{
  FILE_LOG(logDEBUG) << "DonneesMeteo::valeurs()";
  return valeurs_;
}

//------------------------------------------------------------------
#ifdef ENV_MATLAB
// Initialisation a partir de fichiers Matlab.
void DonneesMeteo::initialiserFichier(const std::string fichierEntree)
{
  FILE_LOG(logDEBUG) << "DonneesMeteo::initialiser(const std::string fichierEntree)";

  MATFile* pFichierMeteo = MexHelper::mhMatOpen(fichierEntree, "r");
  mxArray* donneesMeteo = MexHelper::mhMatGetStructVariable(pFichierMeteo, "meteo");

  // Pour valider le type des donnees (double ou simple precision).
  // ATTENTION: on valide seulement sur tMin et tMax mais TOUTES les donnes 
  // meteo doivent etre du meme type.
  mxArray* tMin = MexHelper::mhMxGetField(donneesMeteo, 0, "tMin");
  mxArray* tMax = MexHelper::mhMxGetField(donneesMeteo, 0, "tMax");

  std::vector<std::string> dummy;
  if (mxIsDouble(tMin) && mxIsDouble(tMax)) {
    initialiser<double>(donneesMeteo, dummy, dummy, dummy);
  }
  else if (mxIsSingle(tMin) && mxIsSingle(tMax)) {
    initialiser<float>(donneesMeteo, dummy, dummy, dummy);
  }
  else {
    std::string erreur = "Type de donnees meteo non-supporte.";
    throw std::runtime_error(erreur);
  }
  
  matClose(pFichierMeteo);
  mxDestroyArray(donneesMeteo);
}
#endif

//------------------------------------------------------------------
void DonneesMeteo::initialiser(const mxArray* meteo)
{
  std::vector<std::string> dummy;
  initialiser(meteo, dummy, dummy, dummy);
}

//------------------------------------------------------------------
// Methode publique d'initialisation
void DonneesMeteo::initialiser(const mxArray* meteo, const std::vector<std::string>& champsFonte, 
                               const std::vector<std::string>& champsEvapo, const std::vector<std::string>& champsAutre) 
{
  FILE_LOG(logDEBUG) << "DonneesMeteo::initialiser(const mxArray* meteo)";

  // Pour valider le type des donnees (double ou simple precision).
  // ATTENTION: on valide seulement sur tMin et tMax mais TOUTES les donnes 
  // meteo doivent etre du meme type.
  mxArray* tMin = MexHelper::mhMxGetField(meteo, 0, "tMin");
  mxArray* tMax = MexHelper::mhMxGetField(meteo, 0, "tMax");
  mxArray *pTot = mxGetField(meteo, 0, std::string("pTot").c_str()); 
  mxArray *pluie, *neige;

  if (pTot == NULL) {
    pluie = MexHelper::mhMxGetField(meteo, 0, "pluie"); 
    neige = MexHelper::mhMxGetField(meteo, 0, "neige"); 
  }
  else {
    pluie = pTot;
    neige = pTot;
  }

  bool estTypeDouble = (mxIsDouble(tMin) && mxIsDouble(tMax) && mxIsDouble(pluie) && mxIsDouble(neige));
  bool estTypeSingle = (mxIsSingle(tMin) && mxIsSingle(tMax) && mxIsSingle(pluie) && mxIsSingle(neige));

  if (estTypeDouble) {
    initialiser<double>(meteo, champsFonte, champsEvapo, champsAutre);
  }
  else if (estTypeSingle) {
    initialiser<float>(meteo, champsFonte, champsEvapo, champsAutre);
  }
  else {
    std::string erreur = "Type de donnees meteo incorrect.";
    throw std::runtime_error(erreur);
  }
}
 
//------------------------------------------------------------------
template <typename Type>
void DonneesMeteo::initialiser(const mxArray* meteo, const std::vector<std::string>& champsFonte, 
                               const std::vector<std::string>& champsEvapo, const std::vector<std::string>& champsAutre)
{
  FILE_LOG(logDEBUG) << "DonneesMeteo::initialiser<>(const mxArray* meteo)";

  mxArray* tMin = MexHelper::mhMxGetField(meteo, 0, "tMin");
  mxArray* tMax = MexHelper::mhMxGetField(meteo, 0, "tMax");
  
  std::string nomVariable = "pTot";
  mxArray *pTot = mxGetField(meteo, 0, nomVariable.c_str()); 

  mxArray *pluie;
  mxArray *neige;
  estPtot_ = false;

  if (pTot == NULL) {
    pluie = MexHelper::mhMxGetField(meteo, 0, "pluie"); 
    neige = MexHelper::mhMxGetField(meteo, 0, "neige"); 
  }
  else {
    pluie = pTot;
    neige = pTot;
    estPtot_ = true;
  }

  // Pas de temps
  size_t nbPasDeTemps = mxGetM(tMin);
  // Donnees par CE
  size_t nbCE = mxGetN(tMin);

  // Dans le cas ou le constructeur par defaut a ete utilise, on prend les quantite trouve 
  // dans l'intrant
  if (nbCarreauxEntiers_ == 0 && nbPasDeTemps_ == 0) {
    nbCarreauxEntiers_ = (int)nbCE; 
    nbPasDeTemps_ = (int)nbPasDeTemps;
    valeurs_.reserve(nbPasDeTemps);
  }

  Type* dataMin = MexHelper::mhMxGetData<Type>(tMin, "tMin");
  Type* dataMax = MexHelper::mhMxGetData<Type>(tMax, "tMax");
  Type* dataPluie = MexHelper::mhMxGetData<Type>(pluie, "pluie");
  Type* dataNeige = MexHelper::mhMxGetData<Type>(neige, "neige");

  std::vector<mxArray*> listeDataFonte;
  std::vector<Type*> dataFonte;
  std::vector<mxArray*> listeDataEvapo;
  std::vector<Type*> dataEvapo;
  std::vector<mxArray*> listeDataAutre;
  std::vector<Type*> dataAutre;
  std::vector<std::string>::const_iterator champsIter;
  typename std::vector<Type*>::const_iterator dataIter;
  int i = 0;

  // Champs meteo specifiques au module de fonte
  if (champsFonte.size() > 0) {
    for (champsIter = champsFonte.begin(); champsIter != champsFonte.end(); champsIter++) {
      listeDataFonte.push_back(MexHelper::mhMxGetField(meteo, 0, *champsIter));
      dataFonte.push_back(MexHelper::mhMxGetData<Type>(listeDataFonte.back(), *champsIter));
    }
  }
  // Champs meteo specifiques au module de d'evapotranspiration
  if (champsEvapo.size() > 0) {
    for (champsIter = champsEvapo.begin(); champsIter != champsEvapo.end(); champsIter++) {
      listeDataEvapo.push_back(MexHelper::mhMxGetField(meteo, 0, *champsIter));
      dataEvapo.push_back(MexHelper::mhMxGetData<Type>(listeDataEvapo.back(), *champsIter));
    }
  }
  // Autres champs meteo (exemple: qualite)
  if (champsAutre.size() > 0) {
    mwSize numRows;
    mwSize numCols;

    for (champsIter = champsAutre.begin(); champsIter != champsAutre.end(); champsIter++) {
        if (MexHelper::hasField(meteo, 0, *champsIter)) {
            listeDataAutre.push_back(MexHelper::mhMxGetField(meteo, 0, *champsIter));
            
            const mwSize* dims = mxGetDimensions(listeDataAutre.back());
            numRows = dims[0];
            numCols = dims[1];

            dataAutre.push_back(MexHelper::mhMxGetData<Type>(listeDataAutre.back(), *champsIter));
        } else {
            mwSize dims[2] = { numRows, numCols };  // You can adjust the dimensions based on your needs
            mxArray* emptyArray = mxCreateNumericArray(2, dims, mxDOUBLE_CLASS, mxREAL);  // Adjust the type if needed
            listeDataAutre.push_back(emptyArray);
            dataAutre.push_back(MexHelper::mhMxGetData<Type>(listeDataAutre.back(), *champsIter));
        }

    }
  }

  float valeurMin, valeurMax, valeurPluie, valeurNeige;
  std::vector<float> valeursFonte, valeursEvapo, valeursAutre;
  for (size_t i = 0; i < nbPasDeTemps; i++) {
    MeteoGrille meteoGrille;
    meteoGrille.reserve(nbCarreauxEntiers_);

    for (size_t j = 0; j < nbCE; j++) {
      valeurMin = (float)dataMin[i + j * nbPasDeTemps];
      valeurMax = (float)dataMax[i + j * nbPasDeTemps];
      valeurPluie = (float)dataPluie[i + j * nbPasDeTemps];

      if (estPtot_) {
        valeurNeige = 0.0f;
      }
      else {
        valeurNeige = (float)dataNeige[i + j * nbPasDeTemps];
      }

      MeteoPtr meteoPtr(new Meteo(valeurMin, valeurMax, valeurPluie, valeurNeige));
      
      valeursFonte.clear();
      for (dataIter = dataFonte.begin(); dataIter != dataFonte.end(); dataIter++) {
        valeursFonte.push_back((float)(*dataIter)[i + j * nbPasDeTemps]);
      }
  
      valeursEvapo.clear();
      for (dataIter = dataEvapo.begin(); dataIter != dataEvapo.end(); dataIter++) {
        valeursEvapo.push_back((float)(*dataIter)[i + j * nbPasDeTemps]);
      }

      valeursAutre.clear();
      for (dataIter = dataAutre.begin(); dataIter != dataAutre.end(); dataIter++) {
        valeursAutre.push_back((float)(*dataIter)[i + j * nbPasDeTemps]);
      }
      meteoPtr->meteoFonte(valeursFonte);
      meteoPtr->meteoEvapo(valeursEvapo);
      meteoPtr->meteoAutre(valeursAutre);

      meteoGrille.push_back(meteoPtr);
    }
    valeurs_.push_back(meteoGrille);
  }

  if (validerDonneesMeteo() != 0) {
    std::stringstream erreur;
    
    erreur << "Nombre incoherent de donnees meteo.\n";
    erreur << "  Nb pas de temps meteo: " << valeurs_.size() << ", Nb pas de temps simul: " << nbPasDeTemps_ << std::endl;
    erreur << "  Nb CE meteo: " << valeurs_.back().size() << ", Nb CE simul: " << nbCarreauxEntiers_ << std::endl;
    throw std::runtime_error(erreur.str());
  }
}

//------------------------------------------------------------------
int DonneesMeteo::validerDonneesMeteo() 
{
    FILE_LOG(logDEBUG) << "DonneesMeteo::validerDonneesMeteo()";
    int retVal = 0;

    if (valeurs_.size() != nbPasDeTemps_) {
        FILE_LOG(logERROR) << "Error: valeurs_.size() != nbPasDeTemps_";
        retVal = -1;
    }

    if (valeurs_.back().size() != nbCarreauxEntiers_) {
        FILE_LOG(logERROR) << "Error: valeurs_.back().size() != nbCarreauxEntiers_";
        retVal = -1;
    }

    return retVal;

}

//------------------------------------------------------------------
bool TrieurCEGrille::operator ()(const CarreauEntierPtr carreauEntier1, const CarreauEntierPtr carreauEntier2)
{
  FILE_LOG(logDEBUG) << "TrieurCEGrille::operator ()";
  // Comparaison des lignes
  if (carreauEntier1->j() < carreauEntier2->j())
    return true;
  // Si sur la meme ligne, comparaison des colonnes.
  else if (carreauEntier1->j() == carreauEntier2->j() && carreauEntier1->i() < carreauEntier2->i()) 
    return true;
  else 
    return false;
};

//------------------------------------------------------------------
bool TrieurMeteoId::operator ()(const MeteoPtr meteo1, const MeteoPtr meteor2)
{
  FILE_LOG(logDEBUG) << "TrieurMeteoId::operator ()";
  // Comparaison des lignes
  if (meteo1->idCarreauEntier() < meteor2->idCarreauEntier())
    return true;
  else 
    return false;
};

//------------------------------------------------------------------
void DonneesMeteo::trierParId(const std::vector<CarreauEntierPtr>& carreauxEntiers)
{
  FILE_LOG(logDEBUG) << "DonneesMeteo::trierParId(const std::vector<CarreauEntierPtr>& carreauxEntiers)";
  std::vector<CarreauEntierPtr> carreauxEntiersTemporaire;
  carreauxEntiersTemporaire.reserve(carreauxEntiers.size());
  
  // Copie de travail des carreaux entiers.
  std::vector<CarreauEntierPtr>::const_iterator carreauxEntiersIter = carreauxEntiers.begin();
  for (; carreauxEntiersIter!= carreauxEntiers.end(); carreauxEntiersIter++) {
    carreauxEntiersTemporaire.push_back(CarreauEntierPtr(new CarreauEntier(
      carreauxEntiersIter->get()->id(),
      carreauxEntiersIter->get()->i(),
      carreauxEntiersIter->get()->j())));
  }
   
  TrieurCEGrille trieurCEGrille;
  TrieurMeteoId trieurMeteoId;
  // Tri des carreaux entiers selon grille i,j. Meme ordre que les donnees meteo.
  std::sort(carreauxEntiersTemporaire.begin(), carreauxEntiersTemporaire.end(), trieurCEGrille);  

  // On parcourt chaque donnee meteo pour lui assigner l'id du
  // carreau entier correspondant. Le vecteur de donnees meteo est 
  // ensuite trie par id. Ceci pour chaque pas de temps.
  std::vector<MeteoGrille>::iterator valeursIter = valeurs_.begin();
  MeteoGrille::iterator meteoGrilleIter; 
  // Boucle sur l'ensemble des donnees meteo.
  for (; valeursIter != valeurs_.end(); valeursIter++) {
    meteoGrilleIter = valeursIter->begin();
    // Boucle sur les donnees pour un pas de temps.
    for (size_t i = 0; i < valeursIter->size() ; i++) {
      meteoGrilleIter[i]->idCarreauEntier(carreauxEntiersTemporaire[i]->id());
    }
    std::sort(valeursIter->begin(), valeursIter->end(), trieurMeteoId);
  }
}
