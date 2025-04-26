#include "stdafx.h"
#include "DLI.h"

// Constructeur. 
DLI::DLI() {
	FILE_LOG(logDEBUG) << "DLI(int moduleDLI, const mxArray* structDonneesMeteo)";
	moduleDLI_ = 1;

	params_.u = 0;
	params_.v = 0;
	params_.a = 0;
	params_.b = 0;
	params_.c = 0;
	params_.d = 0;
}

// Get
int DLI::moduleDLI() {
	return moduleDLI_;
}

// Sï¿½lectionnez la fonction de calcul correcte
double DLI::calculeRadiation(double sigma, double ENNUAG, double TKR, double PREVAP, double TKO, double LAI, double DLI) {
	double B = ENNUAG;
	double ea = PREVAP;
	double RADIN = 0;
	double ATMOS = 0;

	switch (moduleDLI_) {
		case 0:
            ATMOS = calculeM4(B, ea);
			break;
		case 1:
			ATMOS = calculeM1(B);
			break;
		case 2:
			ATMOS = calculeM2(B, TKR);
			break;
		case 3:
			ATMOS = calculeM3(B, TKR);
			break;
		case 4:
			ATMOS = calculeM4(B, ea);
			break;
		case 5:
			ATMOS = calculeM5(B, TKR, ea);
			break;
		case 6:
			ATMOS = calculeM6(B, TKR, ea);
			break;
		case 7:
			ATMOS = calculeM7(B, TKR, ea);
			break;
		case 8:
			ATMOS = calculeM8(B, TKR, ea);
			break;
        case 9:
			ATMOS = calculeM9(B, ea, LAI);
            break;
        case 10:
            RADIN = DLI - 0.97f * sigma * pow(TKO, 4.0f);
            return RADIN;
            break;
		default:
			break;
	}
    
    RADIN = sigma * ATMOS * pow(TKR, 4.0f) - 0.97f * sigma * pow(TKO, 4.0f);

	return RADIN;
}

double DLI::calculeM1(double B) {
	return (1.0f + params_.u * pow(B, params_.v)) * params_.a;
}

double DLI::calculeM2(double B, double TKR) {
	return (1.f + params_.u * pow(B, params_.v)) * (params_.a * pow(TKR, 2.0f));
}

double DLI::calculeM3(double B, double TKR) {
	return (1.0f + params_.u * pow(B, params_.v)) * (1.0f - params_.a * exp(params_.b * pow(273.0f - TKR, 2.0f)));
}

double DLI::calculeM4(double B, double ea) {
	return 0.97f * (0.74f + 0.0065f * ea) * (1.0f + 0.17f * pow(B, 2.0f));
}

double DLI::calculeM5(double B, double TKR, double ea) {
	return (1.0f + params_.u * pow(B, params_.v)) * (params_.a * pow(1.3332239f * ea / TKR, params_.b));
}

double DLI::calculeM6(double B, double TKR, double ea) {
	return (1.0f + params_.u * pow(B, params_.v)) * (params_.a * (1.0f - exp(-1.0f * pow(1.3332239f * ea, TKR / params_.b)))); 
}

double DLI::calculeM7(double B, double TKR, double ea) {
	double temp = (-1.0f) * pow(params_.b + params_.c * params_.a * (1.3332239f * ea / TKR), params_.d);
	return (1.0f + params_.u * pow(B, params_.v)) * (1.0f - (1.0f + params_.a * (1.3332239f * ea / TKR)) * exp(temp));
}

double DLI::calculeM8(double B, double TKR, double ea) {
	if (1.3332239f * ea >= 2.0f) {
		return (1.0f + params_.u * pow(B, params_.v)) * (params_.a + params_.b * (1.3332239f * ea - 2.0f));
	}
	else {
 		return (1.0f + params_.u * pow(B, params_.v)) * (params_.a - params_.c * (1.3332239f * ea - 2.0f));
	}
}

double DLI::calculeM9(double B, double ea, double LAI) {
	return static_cast<double>((1.0f + 0.17f * pow(B, 2.0f)) * exp(-1 * params_.a * LAI) * 0.97f * (0.74f + 0.0065f * ea) + 0.98 * (1 - exp(-1 * params_.a * LAI)));
}


// Initialisation
void DLI::initialiser(int moduleDLI, const mxArray* structParamDLI) {
	mxArray* m;

	moduleDLI_ = moduleDLI;

	switch (moduleDLI_) {
	case 0:
		m = MexHelper::mhMxGetField(structParamDLI, 0, "m4");
		lireParametres(m, 4);
		break;
	case 1:
		m = MexHelper::mhMxGetField(structParamDLI, 0, "m1");
		lireParametres(m, 3);
		break;
	case 2:
		m = MexHelper::mhMxGetField(structParamDLI, 0, "m2");
		lireParametres(m, 3);
		break;
	case 3:
		m = MexHelper::mhMxGetField(structParamDLI, 0, "m3");
		lireParametres(m, 4);
		break;
	case 4:
		m = MexHelper::mhMxGetField(structParamDLI, 0, "m4");
		lireParametres(m, 4);
		break;
	case 5:
		m = MexHelper::mhMxGetField(structParamDLI, 0, "m5");
		lireParametres(m, 4);
		break;
	case 6:
		m = MexHelper::mhMxGetField(structParamDLI, 0, "m6");
		lireParametres(m, 4);
		break;
	case 7:
		m = MexHelper::mhMxGetField(structParamDLI, 0, "m7");
		lireParametres(m, 6);
		break;
	case 8:
		m = MexHelper::mhMxGetField(structParamDLI, 0, "m8");
		lireParametres(m, 5);
		break;
	case 9:
		m = MexHelper::mhMxGetField(structParamDLI, 0, "m4");
		lireParametres(m, 4);
	default:
		break;
	}

}

// Lire les parametres pour l'equation
void DLI::lireParametres(const mxArray* paramsDLI, int numParams) {
	// FILE_LOG(logINFO) << "numParams " << numParams;

	if (numParams >= 1) {
		lireParametresHelper(paramsDLI, "u", params_.u);
	}
	// FILE_LOG(logINFO) << "params_.u " << params_.u;

	if (numParams >= 2) {
		lireParametresHelper(paramsDLI, "v", params_.v);
	}
	// FILE_LOG(logINFO) << "params_.v " << params_.v;

	if (numParams >= 3) {
		lireParametresHelper(paramsDLI, "a", params_.a);
	}
	// FILE_LOG(logINFO) << "params_.a " << params_.a;

	if (numParams >= 4) {
		lireParametresHelper(paramsDLI, "b", params_.b);
	}
	// FILE_LOG(logINFO) << "params_.b " << params_.b;

	if (numParams >= 5) {
		lireParametresHelper(paramsDLI, "c", params_.c);
	}
	// FILE_LOG(logINFO) << "params_.c " << params_.c;

	if (numParams >= 6) {
		lireParametresHelper(paramsDLI, "d", params_.d);
	}
	// FILE_LOG(logINFO) << "params_.d " << params_.d;;

}
