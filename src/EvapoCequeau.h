//****************************************************************************
// Fichier: EvapoCequeau.h
//
// Creation date: 2014-04-09
// Created by: 
//                Rio Tinto Alcan                     
//                Energie electrique                  
//                1954 Davis, Saguenay arr. Jonquiere,
//                G7S 4R7, QC, Canada                 
// 
// Usage: Use this template to create you own evapotranspiration module:
//        1- Copy and paste EvapoCequeau.h and EvapoCequeau.cpp
//        2- Rename the new files after your module name (please keep "Evapo" at the beginning). 
//           Example: EvapoSomething.h and EvapoSomething.cpp
//        3- Search for "// TODO New module" in the project. These are placeholders for you code.  
//           All methods MUST be implemented (see interface Evapo.h).
//
//****************************************************************************
#pragma once

#include "Evapo.h"

class EvapoCequeau : public Evapo
{
public:
	//! Constructeur.
	EvapoCequeau();
	EvapoCequeau(int latitudeMoyenneBV, int nbCE, int pasParJour);
	~EvapoCequeau();

	int calculerEvapo(
		// IN
		const int& noJour,
		const Meteo& meteo,
		const CarreauEntier& carreauEntier,
		const float niveauEauNappe,
		// OUT
		float& evapotranspirationSol,
		float& evapotranspirationNappe,
		float& evaporationLac,
		float& evapotranspirationPotentielle
	);

	int assimiler(const DateChrono& datePasDeTemps);

	int initialiserAssimilations(const mxArray* assimilations);

	void initialiserEtats(const mxArray* etatsInitiaux);

	void lireParametres(const mxArray* paramSimul);

	mxArray* obtenirEtats(const std::vector<bool> filtreCE);

	mxArray* obtenirEtatsAvantAssimilations(const std::vector<bool> filtreCE);

	void preserverEtatsPasDeTemps();

	// Etat de l'evapo pour un carreau entier
	// Est publique uniquement pour les besoins de Cequeau Qualite.
	// TODO: Devrait etre prive. A voir selon les autres modeles d'evapo si besoin.
	// State variables
	class EtatEvapoCE
	{
	public:
		// TODO New module: Add your state variables
		// Type MUST BE float
		/*** Example:****/
		float stateSol;
		float stateNappe;
		float stateEauLibre;
		float statePot;
	};

	// Pour le calcul de la temperature de l'eau de Cequeau qualite
	// TODO: Generaliser selon les autres modeles de fonte, si besoin.
	const std::vector<std::vector<EtatEvapoCE> >& etatsEvapo() const { return etatsEvapo_; };

private:
	//! Constante de modulation solaire (TAPHI).
	/*!
	  Initialisee par le constructeur. Varie en fonction de la latitude
	  moyenne du basin versant.
	*/
	float constModulationSoleil_;
	//! Nombre de pas de temps par jour
	int pasParJour_;



	class Params
	{
	public:
		// TODO New module: Add your parameters
		int jourSoleilMaxEvapo;
		//! Fraction de l'évapotranspiration prise dans le réservoir NAPPE (de 0.0 a 1.0);
		// EVNAP
		float fractionEvapoNappe;
		//! Exposant de la formule de Thornthwaite;
		// XAA
		float exposantThornthwaite;
		//! Valeur de l'index thermique de Thornthwaite;
		// XIT
		float indexThermiqueThornthwaite;
		// Use a vector for parameter different for each CE 
	   /*** Example:
	   std::vector<float> param2;
	   ***/
	} params_;

	// Donnees a assimiler
	class EtatEvapoAssimCE
	{
	public:
		int idCarreauEntier;
		// TODO New module: Add your variables
		/*** Example
		TypeAssim stateVar1Type;
		float stateSol;
		TypeAssim stateVar2Type;
		float stateVar2[2];
		***/
	};

	//! Noms des variable d'etat dans les intrants
	std::vector<std::string> nomChamps_;
	//! Etat pour un carreau entier
	EtatEvapoCE etatEvapoCE_;
	//! Etats des carreaux entiers pour un pas de temps
	std::vector<EtatEvapoCE> etatsEvapoCE_;
	//! Etats des carreaux entiers pour tous les pas de temps
	std::vector<std::vector<EtatEvapoCE>> etatsEvapo_;

	// Donnees d'assimilation
	std::map<DateChrono, std::vector<EtatEvapoAssimCE>> assimilationsCE_;
	// conservation des etats avant assimilation
	std::map<DateChrono, std::vector<EtatEvapoCE>> avantAssimilationsCE_;

	double calculerFacteurModulationSoleil(int noJour) const;
	double calculerEthorn(int noJour, float tempMoy) const;

};
