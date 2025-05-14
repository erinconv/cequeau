//****************************************************************************
// Fichier: ParametresQualite.h
//
// Date creation: 2013-04-11
// Auteur: 
//                Rio Tinto Alcan                     
//                Energie electrique                  
//                1954 Davis, Saguenay arr. Jonquiere,
//                G7S 4R7, QC, Canada                 
//
//****************************************************************************
#pragma once

//#include <boost/array.hpp>

//typedef boost::array<float, 12> MoyenneMois;
typedef std::array<float, 12> MoyenneMois;

//! Classe de donnees des parametres qualite relatifs a la temperature de l'eau.
class ParamTemperature
{
public:
  //! Coefficient rayonnement solaire.
  /*! Coefficient permettant d'augmenter ou de diminuer
    l'importance du rayonnement solaire dans le bilan d'energie. */
  float crayso;
  //! Coefficient rayonnement infrarouge.
  /*! Coefficient permettant d'augmenter ou de diminuer
    l'importance du rayonnement infrarouge dans le bilan d'energie. */
  float crayin;
  //! Coefficient evaporation.
  /*! Coefficient permettant d'augmenter ou de diminuer
    l'importance de l'ï¿½vaporation dans le bilan d'ï¿½nergie */
  float cevapo;
  //! Coefficient convection.
  /*! Coefficient permettant d'augmenter ou de diminuer
      l'importance de la convection dans le bilan d'ï¿½nergie */
  float cconve;
  //! Critere de gel.
  float crigel; 
  //! Temperature moyenne annuelle des eaux souterraines sur le bassin versant.
  float tnap; 
  //! Parametre permettant d'ajuster la temperature de l'ecoulement souterrain arrivant en riviere.
  float panap;
  //! Temperature moyenne de l'eau sur l'ensemble du bassin versant estimee au premier jour des simulations
  float tinit;
  //! Lame de precipitation totale permettant de detecter les jours de faible rayonnement solaire.
  float bassol;
  //! Correction du rayonnement solaire moyen.
  /*! Correction du rayonnement solaire moyen (RSM) pour les
      jours sans pluie (RSM x (1+CORSOL)) et les jours de fortes
      pluies (RSM x (1-CORSOL)) (entre 0.0 et 1.0). */
  float corsol;

  //! couverture du sol par la canopï¿½ de la forï¿½t (entre 0.0 et 1.0)
  float ccanope;
  float hombre;
};

//! Classe des parametres de la simulation
class ParametresQualite
{
public:
  //! Constructeur. 
  ParametresQualite();
  //! Destructeur. 
  ~ParametresQualite();
   
  //! Getter
  float coprom() const;
  //! Getter
  float colarg() const;
  //! Getter
  const ParamTemperature& temperature() const;

   //! Initialisation avec valeurs fixes pour fin de tests.
  void initialiser();
   //! Initialisation.
  void initialiser(const mxArray* paramQualite);
  void initialiser(const mxArray* paramQualite, const mxArray* paramDLI);
  void initialiserOmbrage(const mxArray* paramQualite);

private:
  float coprom_;
  float colarg_;
  ParamTemperature temperature_;

};

