//****************************************************************************
// Fichier: ModuleBase.hpp
//
// Date creation: 2014-04-01
// Auteur: 
//                Rio Tinto Alcan                     
//                Energie electrique                  
//                1954 Davis, Saguenay arr. Jonquiere,
//                G7S 4R7, QC, Canada                 
//
//****************************************************************************
#pragma once

#include "AssimilationHelper.hpp"
#include "DateChrono.h"

//! Classe de base pour les modules de fonte et d'evapotranspiration
class ModuleBase
{
public:
  //! Constructeur.
  ModuleBase();
  ModuleBase(int nbCE, std::string nomModele);
  virtual ~ModuleBase();
  
  //! Nom des intrants meteo propre au modele
  std::vector<std::string>& nomsChampsMeteo() {return nomsChampsMeteo_;};
  // ! Nom du modele
  std::string nomModele();

protected:
  //! Nombre de carreaux entiers
  int nbCE_;

  //! Helper pour la lecture d'un parametre spatialisable
  void lireParametresHelper(const mxArray* paramFonte, const std::string& nomChamp, std::vector<float>& champ);

  //! Helper generique pour la lecture d'un parametre
  template <typename Type>
  void lireParametresHelper(const mxArray* paramFonte, const std::string& nomChamp, Type& champ);

  //! Helper generique pour l'initialisation des etats
  template <typename Type>
  void initialiserEtatsHelper(const mxArray* etats, std::map<std::string, float*>& champs, 
                              Type& etatCE, std::vector<Type>& etatsCE);

  //! Helper generique pour la sortie des etats en mxArray
  template <typename Type>
  mxArray* obtenirEtatsHelper(const std::vector<std::string>& nomsChamps, const std::vector<std::vector<Type> >& etats,
                              const std::vector<bool>& filtreCE);

  //! Helper generique pour la sortie des sauvegarde de etats avant assimilation en mxArray
  template <typename Type>
  mxArray* obtenirEtatsAvantAssimHelper(const std::vector<bool>& filtreCE, const std::vector<std::string>& nomChamps,
                                        const std::map<DateChrono, std::vector<Type> >& etatsAvantAssim)
  {
    const size_t nbPasDeTemps = etatsAvantAssim.size();
    const char *champs[] = {"pasDeTemps", "etatsFonte"}; 
    int nbChamps = sizeof(champs) / sizeof(char*);
    mxArray* etatsAvant = mxCreateStructMatrix(1, nbPasDeTemps, nbChamps, champs);
  
    mxArray *pasDetemps, *etatsFonte;

    // Variable de travail pour l'appel a "obtenirEtats()"
    std::vector<std::vector<Type> > listeEtatsCarreauxEntiers;
    listeEtatsCarreauxEntiers.reserve(1);

    int i = 0;
    DateChrono dateAssim;
    typename std::map<DateChrono, std::vector<Type> >::const_iterator etatsAvantAssimIter = etatsAvantAssim.begin();
    for (; etatsAvantAssimIter != etatsAvantAssim.end(); etatsAvantAssimIter++) {
      pasDetemps = mxCreateDoubleMatrix(1,1,mxREAL);
      dateAssim = etatsAvantAssimIter->first;
      //*mxGetPr(pasDetemps) = MexHelper::dateToDatenum(dateAssim);
      *mxGetPr(pasDetemps) = dateAssim.toMatlabDatenum();
      mxSetField(etatsAvant, i, "pasDeTemps", pasDetemps);
    
      listeEtatsCarreauxEntiers.push_back(etatsAvantAssimIter->second);
      etatsFonte = obtenirEtatsHelper(nomChamps, listeEtatsCarreauxEntiers, filtreCE);
      listeEtatsCarreauxEntiers.clear();
      mxSetField(etatsAvant, i, "etatsFonte", etatsFonte);
      i++;
    }

    return etatsAvant;
  }

private:
  std::string nomModele_;
  std::vector<std::string> nomsChampsMeteo_;
  //! Validation d'un parametre spatialisable. Si une seule valeur, on l'assigne a tous les carreaux entiers.
  void validerParametreSpatial(const std::string& nomChamp, std::vector<float>& champ);
};

//------------------------------------------------------------------
//                      Definitions
//------------------------------------------------------------------

//------------------------------------------------------------------
inline ModuleBase::ModuleBase()
{
}

//------------------------------------------------------------------
inline ModuleBase::ModuleBase(int nbCE, std::string nomModele)
  : nbCE_(nbCE), nomModele_(nomModele)
{
}

//------------------------------------------------------------------
inline ModuleBase::~ModuleBase()
{
}

//------------------------------------------------------------------
inline std::string ModuleBase::nomModele()
{
  return nomModele_;
}

//------------------------------------------------------------------
inline void ModuleBase::lireParametresHelper(const mxArray* paramFonte, const std::string& nomChamp, std::vector<float>& champ)
{
  MexHelper::chargerValeurs(paramFonte, nomChamp, champ);
  validerParametreSpatial(nomChamp, champ);
}

//------------------------------------------------------------------
template <typename Type>
inline void ModuleBase::lireParametresHelper(const mxArray* paramFonte, const std::string& nomChamp, Type& champ)
{
  MexHelper::chargerValeurs(paramFonte, nomChamp, champ);
}

//------------------------------------------------------------------
template <typename Type>
inline void ModuleBase::initialiserEtatsHelper(const mxArray* etats, std::map<std::string, float*>& champs, 
                                               Type& etatCE, std::vector<Type>& etatsCE)
{
  // Liste des mxArray contenant les valeurs d'un etat pour tous les CE
  std::vector<mxArray*> listeDonnees;
  // Valeurs d'un etat pour tous les CE
  std::vector<double*> donnees;
  std::map<std::string, float*>::iterator champsIter;

  // Obtention des mxArray pour chaque champ
  for (champsIter = champs.begin(); champsIter!= champs.end(); champsIter++) {
    listeDonnees.push_back(MexHelper::mhMxGetField(etats, 0, champsIter->first));
  }

  // Validation sur le nombre de donnees
  int nbDonnees = (int)mxGetN(listeDonnees.back());
  if (nbDonnees != nbCE_) {
    std::stringstream erreur;
    erreur << "Le nombre de CE des etats initiaux: " << nbDonnees << std::endl;
    erreur << "n'est pas le meme que pour le bassin versant: " << nbCE_;
    throw std::runtime_error(erreur.str());
  }

  // Obtention de la liste des valeurs de chaque CE pour chaque champ
  int i = 0;
  for (champsIter = champs.begin(); champsIter!= champs.end(); champsIter++) {
    donnees.push_back(MexHelper::mhMxGetPr(listeDonnees.at(i), champsIter->first));
    i++;
  }

  // Assignation des valeurs
  int j;
  for (int i = 0; i < nbCE_; i++) {
    j = 0;
    for (champsIter = champs.begin(); champsIter!= champs.end(); champsIter++) {
      *(champsIter->second) = (float)donnees[j][i];
      j++;
    }

    // Comme chaque item du map est une reference sur un champ de l'etatCE
    // donc etatCE est modifie par la boucle ci-dessus
    etatsCE.push_back(etatCE);
  }
}

//------------------------------------------------------------------
template <typename Type>
inline mxArray* ModuleBase::obtenirEtatsHelper(const std::vector<std::string>& champs, const std::vector<std::vector<Type> >& etats, const std::vector<bool>& filtreCE)
{
  const size_t nbPasDeTemps = etats.size();

  // Pour un modele sans etat
  if (nbPasDeTemps == 0) {
    return  mxCreateStructMatrix(1, 0, 0, 0);
  }

  // Nombre de CE qu'on desire en sortie
  size_t nbCE = (size_t)std::count(filtreCE.begin(), filtreCE.end(), true);
  size_t nbTotalCE = (size_t)etats.back().size();

  // Si on a moins de carreaux dans la liste des etats passee en parametres
  // que le filtre, on prend tous les carreaux du parametre.
  nbCE = (nbTotalCE < nbCE ? nbTotalCE : nbCE);

  std::vector<mxArray**> donnesCompletes;
  // Allocation de la memoire pour chaque champ. 
  for (int i = 0; i < champs.size(); i++) {
    donnesCompletes.push_back((mxArray**)mxMalloc(nbPasDeTemps * sizeof(mxArray*)));
    // Allocation de la memoire pour chaque pas de temps de chaqie champ. 
    for (unsigned int i = 0; i < nbPasDeTemps; i++) {
      donnesCompletes.back()[i] = mxCreateDoubleMatrix(1, nbCE, mxREAL);
    }
  }

  typename std::vector<std::vector<Type> >::const_iterator tempsIter; 
  typename std::vector<Type>::const_iterator etatsIter;
  std::vector<std::string>::const_iterator champsIter;

  // Valeur pour chaque champ
  std::vector<double> donnees;
  int indexCE;
  float* unEtat;
  size_t cntPasTemps = 0, cntCE, cntChamp;

  // Parcourt des pas de temps
  for (tempsIter = etats.begin(); tempsIter != etats.end(); tempsIter++) {
    cntCE = 0;
    // Parcourt des carreaux entiers
    for (etatsIter = tempsIter->begin();  etatsIter != tempsIter->end(); etatsIter++) {
      donnees.clear();
      // Les variables d'etat du CE doivent de type float sinon le resusltat sera imprevisible...
      unEtat = (float*)&(*etatsIter);
      cntChamp = 0;
      for (champsIter = champs.begin(); champsIter != champs.end(); champsIter++) {
        // Donnee d'un champ pour ce CE
        donnees.push_back((double)unEtat[cntChamp]);
        //FILE_LOG(logINFO) << "cntPasTemps," << cntPasTemps << ",ce," << cntCE << ",donnee," << donnees.back();
        cntChamp++;
      }

      // Selection des CE qu'on desire en sortie
      indexCE = (int)cntCE;
      if (filtreCE[indexCE]) {
        cntChamp = 0;
        for (champsIter = champs.begin(); champsIter != champs.end(); champsIter++) {
          // copie d'une donnee du CE dans le bon mxArray
          // l'alignement en memoire de Matlab est colonne/range
          memcpy((void*)(mxGetPr(donnesCompletes.at(cntChamp)[cntPasTemps]) + cntCE), (void*)&donnees.at(cntChamp), sizeof(double));
          cntChamp++;
        }
        cntCE++;
      }
    }
    cntPasTemps++;
  }

  // Assignation des nom de champs
  const char **nomChamps = new const char*[champs.size()]; 
  cntChamp = 0;
  for (champsIter = champs.begin(); champsIter != champs.end(); champsIter++) {
    nomChamps[cntChamp] = (*champsIter).c_str();
    cntChamp++;
  }

  // allocation de la memoire pour la structure de sortie
  int nbChamps = (int)champs.size();
  mxArray* etatsOut = mxCreateStructMatrix(1, nbPasDeTemps, nbChamps, nomChamps);

  // Assignation des listes de donnees a la structure de sortie
  for (unsigned int i = 0; i < nbPasDeTemps; i++) {
    cntChamp = 0;
    for (champsIter = champs.begin(); champsIter != champs.end(); champsIter++) {
      mxSetField(etatsOut, i, (*champsIter).c_str(), donnesCompletes.at(cntChamp)[i]);
      cntChamp++;
    }
  }

  return etatsOut;
}

// TODO Probleme au link: mis dans la declaration
//------------------------------------------------------------------
//template <typename Type>
//inline mxArray* obtenirEtatsAvantAssimHelper(const std::vector<bool>& filtreCE, const std::vector<std::string>& nomChamps,
//                                             const std::map<date, std::vector<Type>>& etatsAvantAssim)
//{
//  const size_t nbPasDeTemps = etatsAvantAssim.size();
//  const char *champs[] = {"pasDeTemps", "etatsFonte"}; 
//  int nbChamps = sizeof(champs) / sizeof(char*);
//  mxArray* etatsAvant = mxCreateStructMatrix(1, nbPasDeTemps, nbChamps, champs);
//  
//  mxArray *pasDetemps, *etatsFonte;
//
//  // Variable de travail pour l'appel a "obtenirEtats()"
//  std::vector<std::vector<Type>> listeEtatsCarreauxEntiers;
//  listeEtatsCarreauxEntiers.reserve(1);
//
//  int i = 0;
//  date dateAssim;
//  std::map<date, std::vector<Type>>::const_iterator etatsAvantAssimIter = etatsAvantAssim.begin();
//  for (; etatsAvantAssimIter != etatsAvantAssim.end(); etatsAvantAssimIter++) {
//    pasDetemps = mxCreateDoubleMatrix(1,1,mxREAL);
//    dateAssim = etatsAvantAssimIter->first;
//    *mxGetPr(pasDetemps) = MexHelper::dateToDatenum(dateAssim);
//    mxSetField(etatsAvant, i, "pasDeTemps", pasDetemps);
//    
//    listeEtatsCarreauxEntiers.push_back(etatsAvantAssimIter->second);
//    etatsFonte = obtenirEtatsHelper(nomChamps, listeEtatsCarreauxEntiers, filtreCE);
//    listeEtatsCarreauxEntiers.clear();
//    mxSetField(etatsAvant, i, "etatsFonte", etatsFonte);
//    i++;
//  }
//
//  return etatsAvant;
//}

//------------------------------------------------------------------
inline void ModuleBase::validerParametreSpatial(const std::string& nomChamp, std::vector<float>& champ) 
{
  int nbValeur = (int)champ.size(); 

  // Si plus d'une valeur, on doit avoir un nombre de valeurs egal au nombre de CE.
  if (nbValeur > 1 && nbValeur != nbCE_) {
    std::stringstream erreur;
    erreur << "Le nombre de valeurs de " << nomChamp << " est incoherent: " << nbValeur;
    erreur << "Doit egaler 1 ou bien nombre de carreaux: " << nbCE_;
    throw std::runtime_error(erreur.str());
  }

  // Si une seule valeur, on l'assigne a tous les carreaux entiers
  if (nbValeur == 1) {
    float valeur = champ.front();
    champ.assign(nbCE_, valeur);
  }
}
