#pragma once

#include "mex.h"
#include <string>
#include <vector>

#include "log.h"
#include "DateChrono.h"

#include "Defs.h"

// seconds (24 hr x 60 min x 60s)
const unsigned int SECONDES = 86400;

#include "DateChrono.h"

//const unsigned int OFFSET_DATENUM = 719529;


//struct date_chrono {
//    std::chrono::system_clock::time_point time_point;
//
//    date_chrono(unsigned int datenum) {
//        if (datenum > OFFSET_DATENUM) {
//            auto seconds_since_epoch = static_cast<long long>(datenum - OFFSET_DATENUM) * 86400;
//            time_point = std::chrono::system_clock::from_time_t(seconds_since_epoch);
//        } else {
//            time_point = std::chrono::system_clock::time_point();
//        }
//    }
//};

//! Fonctions utilitaires pour faciliter l'usage de l'API Matlab.
namespace MexHelper {

  //! Equivalent d'un matOpen mais avec gestion d'erreur.
  // MATFile* mhMatOpen(const std::string& filename, const std::string& mode);
  // //! Obtention d'un variable avec validation sur type struct et gestion d'erreur.
  // mxArray* mhMatGetStructVariable(MATFile* pMF, const std::string& name);
  //! Equivalent d'un mxGetField mais avec gestion d'erreur. 
  mxArray* mhMxGetField(const mxArray* pm, mwIndex index, const std::string& fieldname);
  //! Est-ce que le champ existe dans la struct?
  bool hasField(const mxArray* pm, mwIndex index, const std::string& fieldname);
  //! Equivalent d'un mxGetPr mais avec gestion d'erreur.
  double*  mhMxGetPr(const mxArray* pm, const std::string& fieldname);
  //! mxGetData generique avec gestion d'erreur. 
  template <typename Type>
  Type*  mhMxGetData(const mxArray* pm, const std::string& fieldname);
  //! Chargement de valeurs dans un vecteur 
  template <typename Type>
  void chargerValeurs(const mxArray* pStruct, const std::string& nomChamp, std::vector<Type>& param, mwIndex index = 0);
  //! Chargement d'une valeur dans un type de base
  template <typename Type>
  void chargerValeurs(const mxArray* pStruct, const std::string& nomChamp, Type& param, mwIndex index = 0);
  //! Chargement d'une valeur dans un type de base avec une valeur par defaut si le champ est absent
  template <typename Type>
  void chargerValeurs(const mxArray* pStruct, const std::string& nomChamp, Type& param, Type& valDefaut, mwIndex index);
  //! Chargement des valeurs dans un array de int
  void chargerValeurs(const mxArray* pStruct, const std::string& nomChamp, int* param, unsigned int nbItems, mwIndex index = 0);
  //! Chargement d'une valeur dans un string
  void chargerValeurs(const mxArray* pStruct, const std::string& nomChamp, std::string& param, mwIndex index = 0);
  //! Chargement des valeurs dans un ParamEquation
  void chargerValeurs(const mxArray* pStruct, const std::string& nomChamp, ParamEquation& param, mwIndex index = 0);
  //! Chargement des valeurs dans un MoyenneMois
  void chargerValeurs(const mxArray* pStruct, const std::string& nomChamp, MoyenneMois& param, mwIndex index = 0);
  //! Chargement des valeurs dans un vecteur de ParamEquation
  void chargerValeurs(const mxArray* pStruct, const std::string& nomChamp, std::vector<ParamEquation>& param, mwIndex index = 0);
  //! Chargement des valeurs dans un vecteur de InfoEvacuationExt
  void chargerValeurs(const mxArray* pStruct, const std::string& nomChamp, std::vector<InfoEvacuationExt>& param, mwIndex index = 0);

  //date_chrono convertDatenumToDate(double* aDatenum);
  //double convertDateToDatenum(const date_chrono& aDate);

  ////! Conversion d'un date Matlab de format datenum en date 
  //date datenumToDate(double* aDatenum);
  ////! Conversion d'une date en format Matlab datenum 
  //double dateToDatenum(date& aDate);

  //! Conversion d'un date Matlab de format datenum en date 
  DateChrono datenumToDate(double* aDatenum);
  //! Conversion d'une date en format Matlab datenum 
  double dateToDatenum(DateChrono& aDate);

  //------------------------------------------------------------------
  //                      Definitions
  //------------------------------------------------------------------
  
  //------------------------------------------------------------------
  #ifdef ENV_MATLAB
  //------------------------------------------------------------------
  inline MATFile* mhMatOpen(const std::string& filename, const std::string& mode)
  {
    FILE_LOG(logDEBUG) << "mhMatOpen(const std::string& filename, const std::string& mode)";

    MATFile* pMF = matOpen(filename.c_str(), mode.c_str());

    if (pMF == NULL) {
      std::string erreur = "Erreur ï¿½ l'ouverture du fichier: " + filename;
      throw std::runtime_error(erreur);
    }

    return pMF;
  }

  //------------------------------------------------------------------
  inline mxArray* mhMatGetStructVariable(MATFile* pMF, const std::string& name)
  {
    FILE_LOG(logDEBUG) << "MexHelper::mhMatGetStructVariable(MATFile* pMF, const std::string& name)";

    mxArray *pMxArray = matGetVariable(pMF, name.c_str()); 

    if (pMxArray == NULL) {
      std::string erreur;
      erreur = "La variable " + name + " n'existe pas.";
      throw std::runtime_error(erreur);
    }

    if (!mxIsStruct(pMxArray)) {
      std::string erreur;
      erreur = "La variable " + name + " n'est pas une structure.";
      throw std::runtime_error(erreur);
    }

    return pMxArray;
  }

  #endif

  //------------------------------------------------------------------
  inline mxArray* mhMxGetField(const mxArray* pm, mwIndex index, const std::string& fieldname) {
    FILE_LOG(logDEBUG) << "MexHelper::mhMxGetField(const mxArray* pm, mwIndex index, const std::string& fieldname)";
    mxArray *pMxArray = mxGetField(pm, index, fieldname.c_str());
    if (pMxArray == NULL) {
      std::string erreur;
      erreur = "Le champ " + fieldname + " n'existe pas.";
      throw std::runtime_error(erreur);
    }

    return pMxArray;
  }

  //------------------------------------------------------------------
  inline bool hasField(const mxArray* pm, mwIndex index, const std::string& fieldname) {
    FILE_LOG(logDEBUG) << "MexHelper::hasField(const mxArray* pm, mwIndex index, const std::string& fieldname)";
    bool retVal = true;
    mxArray *pMxArray = mxGetField(pm, index, fieldname.c_str());
    if (pMxArray == NULL) {
      retVal = false;
    }
    return retVal;
  }

  //------------------------------------------------------------------
  inline double*  mhMxGetPr(const mxArray* pm, const std::string& fieldname) {
    FILE_LOG(logDEBUG) << "MexHelper::mhMxGetPr(const mxArray* pm, const std::string& fieldname)";

    double* valeur = mxGetPr(pm);

    if (valeur == NULL) {
      std::string erreur;
      erreur = "La valeur du champ " + fieldname + " est introuvable.";
      throw std::runtime_error(erreur);
    }

    return valeur;
  }

  //------------------------------------------------------------------
  // Attention, le resultat est indetermine pour un type autre que 
  // numerique (float, double, int)
  template <typename Type>
  inline Type*  mhMxGetData(const mxArray* pm, const std::string& fieldname) {
    FILE_LOG(logDEBUG) << "MexHelper::mhMxGetData(const mxArray* pm, const std::string& fieldname)";

    Type* valeur = (Type*)mxGetData(pm);

    if (valeur == NULL) {
      std::string erreur;
      erreur = "La valeur du champ " + fieldname + " est introuvable.";
      throw std::runtime_error(erreur);
    }

    return valeur;
  }

  //------------------------------------------------------------------
  template <typename Type>
  inline void chargerValeurs(const mxArray* pStruct, const std::string& nomChamp, std::vector<Type>& param, mwIndex index ) {
    FILE_LOG(logDEBUG) << "MexHelper::chargerValeurs(..., std::vector<Type>& param)";

    mxArray* pValeur = MexHelper::mhMxGetField(pStruct, index, nomChamp);
    double* valeur   = MexHelper::mhMxGetPr(pValeur, nomChamp);
    size_t nbValeurs = mxGetNumberOfElements(pValeur);

    param.clear();
    for (unsigned int i = 0; i < nbValeurs; i++) {
      param.push_back((Type)valeur[i]);
    }
  }
#include <typeinfo> // for typeid
  //------------------------------------------------------------------
  template <typename Type>
  inline void chargerValeurs(const mxArray* pStruct, const std::string& nomChamp, Type& param, mwIndex index) {
    FILE_LOG(logDEBUG) << "MexHelper::chargerValeurs(..., Type& param)";

    mxArray* pValeur = MexHelper::mhMxGetField(pStruct, index, nomChamp);
    double* valeur   = MexHelper::mhMxGetPr(pValeur, nomChamp);
    param = (Type)*valeur;

    if (nomChamp == "altitude") {
      param = static_cast<Type>(mxGetScalar(pValeur));
    }

  }
  
  //------------------------------------------------------------------
  template <typename Type>
  inline void chargerValeurs(const mxArray* pStruct, const std::string& nomChamp, Type& param, Type valDefaut, mwIndex index) {
    if (MexHelper::hasField(pStruct, index, nomChamp)) {
      MexHelper::chargerValeurs(pStruct, nomChamp, param, index);
    } else {
      param = valDefaut;
    }

  }

  //------------------------------------------------------------------
  inline void chargerValeurs(const mxArray* pStruct, const std::string& nomChamp, int* param, unsigned int nbItems, mwIndex index ) {
    FILE_LOG(logDEBUG) << "MexHelper::chargerValeurs(..., int* param)";

    mxArray* pValeur = MexHelper::mhMxGetField(pStruct, index, nomChamp);
    double* valeur   = MexHelper::mhMxGetPr(pValeur, nomChamp);
    size_t nbValeurs = mxGetNumberOfElements(pValeur);

    for (unsigned int i = 0; i < nbItems; i++) {
      if (nbValeurs <= i) {
        param[i] = 0;
      } else {
        param[i] = (int)valeur[i];
      }
    }
  }

  //------------------------------------------------------------------
  inline void chargerValeurs(const mxArray* pStruct, const std::string& nomChamp, std::string& param, mwIndex index) {
    FILE_LOG(logDEBUG) << "MexHelper::chargerValeurs(..., std::string& param)";

    char* buf;
    size_t buflen;

    mxArray* pValeur = MexHelper::mhMxGetField(pStruct, index, nomChamp);

    buflen = mxGetN(pValeur) * sizeof(mxChar) + 1;
    buf = (char*)mxMalloc(buflen);
    mxGetString(pValeur, buf, (mwSize)buflen);

    param = std::string(buf);

    mxFree(buf);
  }

  //------------------------------------------------------------------
  inline void chargerValeurs(const mxArray* pStruct, const std::string& nomChamp, ParamEquation& param, mwIndex index) {
    FILE_LOG(logDEBUG) << "MexHelper::chargerValeurs(..., ParamEquation& param)";

    // Pre-fill the entire param array with zeros
    std::fill(param.begin(), param.end(), 0.0f);

    mxArray* pValeur = MexHelper::mhMxGetField(pStruct, index, nomChamp);
    double* valeur   = MexHelper::mhMxGetPr(pValeur, nomChamp);
    size_t nbValeurs = mxGetNumberOfElements(pValeur);

    for (unsigned int i = 0; i < nbValeurs && i < param.size(); i++) {
      param[i] = (float)valeur[i];
    }
  }

  //------------------------------------------------------------------
  inline void chargerValeurs(const mxArray* pStruct, const std::string& nomChamp, MoyenneMois& param, mwIndex index) {
    FILE_LOG(logDEBUG) << "MexHelper::chargerValeurs(..., ParamEquation& param)";

    mxArray* pValeur = MexHelper::mhMxGetField(pStruct, index, nomChamp);
    double* valeur   = MexHelper::mhMxGetPr(pValeur, nomChamp);
    size_t nbValeurs = mxGetNumberOfElements(pValeur);

    for (unsigned int i = 0; i < nbValeurs && i < param.size(); i++) {
      param[i] = (float)valeur[i];
    }
  }

  //------------------------------------------------------------------
  inline void chargerValeurs(const mxArray* pStruct, const std::string& nomChamp, std::vector<ParamEquation>& param, mwIndex index) {
    FILE_LOG(logDEBUG) << "MexHelper::chargerValeurs(..., std::vector<ParamEquation>& param)";
    ParamEquation paramEquation;

    mxArray* pValeur = MexHelper::mhMxGetField(pStruct, index, nomChamp);
    double* valeur   = MexHelper::mhMxGetPr(pValeur, nomChamp);

    // Nombre de relations necessaires pour calculer les evacuations du reservoir
    // vers un autre carreau a l'interieur du bassin versant 
    size_t nbRelations = mxGetM(pValeur);
    // Nombre de valeurs par relation
    size_t nbValeurs = mxGetN(pValeur);

    for (unsigned int i = 0; i < nbRelations; i++) {
      paramEquation.fill(0.0f);
      for (unsigned int j = 0; j < nbValeurs && j < paramEquation.size(); j++) {
        // Le stockage interne d'un mxArray est colonne/ligne
        paramEquation[j] = (float)valeur[i + j * nbRelations];
      }
      param.push_back(paramEquation);
    }
  }

  //------------------------------------------------------------------
  inline void chargerValeurs(const mxArray* pStruct, const std::string& nomChamp, std::vector<InfoEvacuationExt>& param, mwIndex index) {
    FILE_LOG(logDEBUG) << "MexHelper::chargerValeurs(..., std::vector<InfoEvacuationExt>& param)";
    InfoEvacuationExt infoEvacuationExt;
    int intDateTemp;
    std::string strDateDebut, strDateFin;

    mxArray* pValeur = MexHelper::mhMxGetField(pStruct, index, nomChamp);
    // Nombre de relations necessaires pour calculer les evacuations du reservoir
    // vers un autre carreau a l'exterieur du bassin versant 
    size_t nbRelations = mxGetNumberOfElements(pValeur);
    // Nombre de valeurs par relation
    size_t nbValeurs = mxGetN(pValeur);

    for (unsigned int i = 0; i < nbRelations; i++) {
      chargerValeurs(pValeur, "dateDebut", intDateTemp, i);
      infoEvacuationExt.dateDebut = DateChrono::fromMatlabDatenum((double)intDateTemp);

      chargerValeurs(pValeur, "dateFin", intDateTemp, i);
      infoEvacuationExt.dateDebut = DateChrono::fromMatlabDatenum((double)intDateTemp);

      chargerValeurs(pValeur, "parametres", infoEvacuationExt.paramDebit, i);
      param.push_back(infoEvacuationExt);
    }
  }

  //inline date_chrono convertDatenumToDate(double* aDatenum) {
  //    FILE_LOG(logDEBUG) << "MatlabHelper::datenumToDate(double* aDatenum)";

  //    return date_chrono(static_cast<unsigned int>(*aDatenum));
  //}

  //inline double dateToDatenum(const date_chrono& aDate) {
  //    FILE_LOG(logDEBUG) << "MatlabHelper::dateToDatenum(const date_chrono& aDate)";

  //    auto seconds_since_epoch = std::chrono::duration_cast<std::chrono::seconds>(aDate.time_point.time_since_epoch()).count();
  //    double retDatenum = static_cast<double>(seconds_since_epoch) / 86400 + OFFSET_DATENUM;
  //    return retDatenum;
  //}

    // date_chrono(unsigned int datenum) {
    //     if (datenum > OFFSET_DATENUM) {
    //         auto seconds_since_epoch = static_cast<long long>(datenum - OFFSET_DATENUM) * 86400;
    //         time_point = std::chrono::system_clock::from_time_t(seconds_since_epoch);
    //     } else {
    //         time_point = std::chrono::system_clock::time_point();
    //     }
    // }


  //------------------------------------------------------------------
  inline DateChrono datenumToDate(double* aDatenum) {
    FILE_LOG(logDEBUG) << "MexHelper::datenumToDate(double* aDatenum)";
    return DateChrono::fromMatlabDatenum(*aDatenum);
  }

  //------------------------------------------------------------------
  inline double dateToDatenum(DateChrono& aDate) {
    FILE_LOG(logDEBUG) << "MexHelper::dateToDatenum(date& aDate)";
    double retDatenum = aDate.toMatlabDatenum();
    return retDatenum;
  }
};
