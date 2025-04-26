//****************************************************************************
// Fichier: ParametresInterpolation.h
//
// Date creation: 2013-05-21
// Auteur: 
//                Rio Tinto Alcan                     
//                Energie electrique                  
//                1954 Davis, Saguenay arr. Jonquiere,
//                G7S 4R7, QC, Canada                 
//
//****************************************************************************
#pragma once

//! Classe des parametres de la simulation
class ParametresInterpolation
{
public:
  //! Constructeur. 
  ParametresInterpolation();
  //! Destructeur. 
  ~ParametresInterpolation();
   
  //! Getter
  int type() const;
  //! Getter
  float coep() const;
  //! Getter
  float coet() const;

   //! Initialisation.
  void initialiser(const mxArray* paramInterpolation);

private:
  int   type_;
  float coep_;
  float coet_;
};

