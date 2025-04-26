#pragma once
#include "ModuleBase.hpp"

/*
* Downward Longwave Irradiance
*/
class DLI: public ModuleBase {

public:
	DLI();
	
	// Initialisation
	void initialiser(int moduleDLI, const mxArray* structParamDLI);

	// Lire les parametres pour l'equation
	void lireParametres(const mxArray* paramSimul, int numParams);
	
	// Sï¿½lectionnez la fonction de calcul correcte
	double calculeRadiation(double sigma, double ENNUAG, double TKR, double PREVAP, double TKO, double LAI, double DLI);

	// Calcule 
	double calculeM1(double B);
	double calculeM2(double B, double TKR);
	double calculeM3(double B, double TKR);
	double calculeM4(double B, double ea);
	double calculeM5(double B, double TKR, double ea);
	double calculeM6(double B, double TKR, double ea);
	double calculeM7(double B, double TKR, double ea);
	double calculeM8(double B, double TKR, double ea);
    double calculeM9(double B, double ea, double LAI);

	// Get
	int moduleDLI();

private:
	// Equation selectionee pour DLI
	int moduleDLI_;

	// Classe de paramatres
	class Params {
	public:
		double u;
		double v;
		double a;
		double b;
		double c;
		double d;
	} params_;
};

