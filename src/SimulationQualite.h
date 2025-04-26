//****************************************************************************
// Fichier: SimulationQualite.h
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

#include "Simulation.h"
#include "FonteCequeau.h"
#include "FonteCemaNeige.h"
#include "FonteUEB.h"
#include "EvapoPriestleyTaylor.h"
#include "EvapoCequeau.h"
#include "ParametresQualite.h"
#include "DLI.h"
#include "DateChrono.h"

namespace Qualite {
  //! Etats de la qualite a chaque CP
  class EtatCarreauPartiel
  {
  public:
	int   idCarreauPartiel;
	float temperature;
	float qruiss; // energie ruisseau
	float qnappe; // energie nappe
	float qhypo;  // energie hypodermique
	float qlacma; // energie lac & marais
	float qradso; // energie radiation courte longueur d'onde  du soleil
	float qradin; // energie radiation longue longueur d'onde du soleil
	float qevap;  // energie evaporation
	float qconv;  // energie convection
	float enerlo; // energie local 
	float eneram; // energie amont
	float eneres; // energie reseau
    EtatCarreauPartiel() :idCarreauPartiel(0), temperature(0.0f), qruiss(0.0f), qnappe(0.0f), 
      qhypo(0.0f), qlacma(0.0f), qradso(0.0f), qradin(0.0f), qevap(0.0f), qconv(0.0f), enerlo(0.0f), eneram(0.0f), eneres(0.0f) {}
  };

  //! Classe de donnees pour l'assimilation des etats d'un carreau partiel.
  /*!
    Utilise pour assimiler des etats aux carreaux partiels.
    Une valeur: on substitue cette valeur. 
    Deux valeurs: La premiere est un facteur multiplicatif 
    et la seconde une valeur a additionner.
  */
  class EtatCarreauPartielAssim
  {
  public:
    int   idCarreauPartiel;
    TypeAssim temperatureType;
    float temperature[2];
  };

  //! Liste des etats des carreaux partiels a chaque pas de temps.
  typedef std::vector<EtatCarreauPartiel> EtatsCarreauxPartiels;
  //! Liste des assimilations des carreaux partiels a chaque pas de temps.
  typedef std::vector<EtatCarreauPartielAssim> EtatsCarreauxPartielsAssim;
	
  typedef struct {
	/* tire de l'algorithme SPA */
	// 

	const DateChrono pasDeTemps;	// time step
	float heureLocal;			// local time
	double latitude;
	double longitude;
	double elevation;
	float fuseauHoraire;		// timezone offset in hours

	//---------------valeur intermediaire---------------------
	double jj; // jour julien
	double jd; // jour julien incluant les heures
	double jce; // siecle julien ephemeris
	double jme; // millenaire julien ephemeris

	//------------------------valeur final--------------------

	double azimuth;
	double altitude;

	} spa_data;

}


//! Class pour calcul des positions solaires
class PositionSolaire {
public:
	PositionSolaire();
	~PositionSolaire();
	void calcul_altitude_et_azimut_du_soleil(double jd, double lat, double lon,
		double* azimut, double* altitude, double elevation = 0);
	void calcul_altitude_et_azimut_du_soleil(Qualite::spa_data& spa);
private:
	enum { TERM_A, TERM_B, TERM_C, TERM_COUNT };
	enum { TERM_X0, TERM_X1, TERM_X2, TERM_X3, TERM_X4, TERM_X_COUNT };
	enum { TERM_PSI_A, TERM_PSI_B, TERM_EPS_C, TERM_EPS_D, TERM_PE_COUNT };
	enum { JD_MINUS, JD_ZERO, JD_PLUS, JD_COUNT };
	enum { SUN_TRANSIT, SUN_RISE, SUN_SET, SUN_COUNT };

	const int l_subcount[6] = { 64,34,20,7,3,1 };
	const int b_subcount[2] = { 5,2 };
	const int r_subcount[5] = { 40,10,6,2,1 };

	const long double PI = 3.141592653589793;
	const double L_TERMS[6][64][TERM_COUNT];

	const double B_TERMS[2][5][TERM_COUNT];

	const double R_TERMS[5][40][TERM_COUNT];

	//! ----------------------------------------------------
	const int Y_TERMS[63][TERM_X_COUNT];
	const double PE_TERMS[63][TERM_PE_COUNT];

	double limit_degrees(double degrees);
	double deg2rad(double degrees);
	double rad2deg(double radians);
	double third_order_polynomial(double a, double b, double c, double d, double x);
	double earth_periodic_term_summation(const double terms[][TERM_COUNT], int count, double jme);
	double earth_values(double term_sum[], int count, double jme);
	double earth_heliocentric_longitude(double jme);
	double earth_heliocentric_latitude(double jme);
	double earth_radius_vector(double jme);
	double mean_elongation_moon_sun(double jce);
	double anomalie_moyenne_du_soleil(int jd);
	double mean_anomaly_sun(double jce);
	double mean_anomaly_moon(double jce);
	double argument_latitude_moon(double jce);
	double ascending_longitude_moon(double jce);
	double xy_term_summation(int i, double x[TERM_X_COUNT]);
	void nutation_longitude_and_obliquity(double jce, double x[TERM_X_COUNT], double* del_psi,
		double* del_epsilon);
	double ecliptic_mean_obliquity(double jme);
	double ecliptic_true_obliquity(double delta_epsilon, double epsilon0);
	double aberration_correction(double r);
	double apparent_sun_longitude(double theta, double delta_psi, double delta_tau);
	double greenwich_mean_sidereal_time(double jd);
	double greenwich_sidereal_time(double nu0, double delta_psi, double epsilon);
	double geocentric_right_ascension(double lamda, double epsilon, double beta);
	double geocentric_declination(double beta, double epsilon, double lamda);
	double observer_hour_angle(double nu, double longitude, double alpha_deg);
	double sun_equatorial_horizontal_parallax(double r);
	void right_ascension_parallax_and_topocentric_dec(double latitude, double elevation,
		double xi, double h, double delta, double* delta_alpha, double* delta_prime);
	double topocentric_right_ascension(double alpha_deg, double delta_alpha);
	double topocentric_local_hour_angle(double h, double delta_alpha);
	double topocentric_elevation_angle(double latitude, double delta_prime, double h_prime);
	double topocentric_zenith_angle(double e);
	double topocentric_azimuth_angle_astro(double h_prime, double latitude, double delta_prime);
	double topocentric_azimuth_angle(double azimuth_astro);
};

//! Classe SimulationQualite
class SimulationQualite
{
public:
  //! Construteur
  SimulationQualite(Simulation& simulation, ParametresQualite& param, DLI& dli,
                    const mxArray* etatsPrecedents, const mxArray* assimilations = NULL);
  ~SimulationQualite();

  //! Execution de la simulation.
  int executer(bool estMexFile = false);

  //! Etats CP Qualite en mxArray
  mxArray* obtenirEtatsCP();
  mxArray* obtenirEtatsAvantAssimilations();

private:
  const Simulation& simulation_;
  const BassinVersant& bassinVersant_;
  const ParametresQualite& parametresQualite_;
  const DonneesMeteo& donneesMeteo_;
  DLI& dli_;

  //! Etats precedents.
  const mxArray* etatsPrecedents_;

  //! Initialisation des etats initiaux.
  /*!
    L'initialisation se fait a partir des parametres ou bien d'etats precedents, s'ils sont
    presents dans les intrants.
  */
  void  initialiserEtats();

  //! Initialisation des etats des carreaux partiels a partir d'etats precedents en format Matlab.
  int chargerEtatsCP(Qualite::EtatsCarreauxPartiels& etatsCarreauxPartiels);

  //! Donnees a assimiler aux etats des carreaux partiels.
  std::map<DateChrono, Qualite::EtatsCarreauxPartielsAssim> assimilationsCP_;
  //! Etats des carreaux partiels, avant assimilation.
  std::map<DateChrono, Qualite::EtatsCarreauxPartiels> avantAssimilationsCP_;
  //! Initialisation des donnees d'assimilation 
  int initialiserAssimilations(const mxArray* assimilations);
  //! Assimilation donnees CP
  int assimiler(const DateChrono& datePasDeTemps, Qualite::EtatsCarreauxPartiels& etatsCarreauxPartiels);

  //! Resultat du transfert.
  std::vector<Qualite::EtatsCarreauxPartiels> qualite_;

  int calculerTemperatureEau(const DateChrono& datePasDeTemps, const int& noPasDeTemps);
//   int calculerTemperatureEau(const DateChrono& datePasDeTemps);

  //! Etats CP en mxArray
  mxArray* obtenirEtatsCP(const std::vector<Qualite::EtatsCarreauxPartiels>& etatsPasDeTemps);

	int calculerIndexPasDeTemps(const DateChrono& datePasDeTemps, const int& noPasDeTemps);


  //! Module ombrage
  float calculerRatioOmbre(const DateChrono& datePasDeTemps, PositionSolaire& pos, const CarreauPartielPtr& carreauPartielPtr) const;
  double calculerRatioOmbre(double altitudeSolaire, double azimutSolaire, double azimutRiviere, double largeurRiviere, double hauteurArbre) const;
  
  double calculerCanopeLAI(double LAI);
  double calculerCanopeTypeArbre(double pctConifer, double pctFeuillu);


};
