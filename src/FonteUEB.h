

//****************************************************************************
// Fichier: FonteCequeau.h
//
// Date creation: 2014-03-11
// Auteur:
//                Rio Tinto Alcan
//                Energie electrique
//                1954 Davis, Saguenay arr. Jonquiere,
//                G7S 4R7, QC, Canada
//
//****************************************************************************
#pragma once

#include "Fonte.h"

//! Definition d'une classe de calcul de la fonte
class FonteUEB : public Fonte
{
public:
    FonteUEB();
    FonteUEB(int latitudeMoyenneBV, int nbCE);
    ~FonteUEB();

    int calculerFonte(
        // IN
        const DateChrono &datePasDeTemps,
        const Meteo &meteo,
        const CarreauEntier &carreauEntier,
        // OUT
        float &precipationsTotales,
        float &eauDisponible);

    int assimiler(const DateChrono &datePasDeTemps);

    int initialiserAssimilations(const mxArray *assimilations);

    void initialiserEtats(const mxArray *etatsInitiaux);

    void lireParametres(const mxArray *paramSimul);

    mxArray *obtenirEtats(const std::vector<bool> filtreCE);

    mxArray *obtenirEtatsAvantAssimilations(const std::vector<bool> filtreCE);

    void preserverEtatsPasDeTemps();

    // Define first the state variables for this process
    class EtatFonteCE
    {
    public:
        // Snow water equivalent (m)
        float w;
        // Internal energy [MJ m-2]
        float ub;
        float Qh;
        float Qe;
        // water equivalence depth of sublimation
        float E;
        // Melt outflow rate
        float Mr;
        float Qm;
        // energy balance [MJ m-2 d-1]
        float Q;
        // mass balance [m/d]
        float Fm;
        // Snow temperature (K)
        float tsurf;
        float albedo;
        // Depth average temperature
        float tave;
        // Snow age
        float tausn;
        // Cumulates values
        // float cump;
        // float cume;
        // float cummr;

        // Legacy variables
        float SNC_stockNeigeForet = 0.0f;
        float SND_stockNeigeClairiere = 0.0f;
    };
    const std::vector<std::vector<EtatFonteCE>> &etatsFonte() const { return etatsFonte_; };

private:
    // Define the parameters for the model
    class parameters
    {
    public:
        // Thereshold  air temperature above which all precipitation is snow (°C)
        float seuilTranformationPluieNeige;
        // Albedo extinction depth (m)
        float aep;
        // Parameter to compute atmospheric transmission [0 - 0.02]
        float b;
        //  Surface aerodynamic roughness [m],
        float z0;
        // Wind/air temperature measurement height (m),
        float z;
        //  Stability correction control parameter 0 = no corrections, 1 = full corrections
        int fstab = 1;
        // Drift factor (a number that precipitation in the form of snow is multiplied by to account for drift accumulations)
        float dF;
        // The capillary retention as a fraction of the solid matrix water equivalence (0.05)
        float Lc;
        // Grain diameter (m)
        float D;
        // Snow Saturated hydraulic conductivity 20 m/hr is 480 m/day
        float K_sat;
        // Snow Surface thermal conductance (m/hr)
        float K_s;
        // the albedo for visible radiation incident on new snow with solar zenith angle less than
        float avo;
        // Melt fraction
        float melt_frac;
        // the albedo of new snow for near-infrared solar radiation with solar zenith angle less than 60°
        float airo;
        // Choose between two methods for computing the snow surface tempetature
        int snow_temp_method;
        // hours for albedo
        float hours;
        // Initial conditions
        // Snow water equivalent (m)
        float w;
        // Internal energy [MJ m-2]
        float ub;
        // water equivalence depth of sublimation
        float E;
        // Melt outflow rate
        float Mr;
        // Snow age
        float tausn;
        // Snow temperature (K)
        float tsurf;
        // Depth average temperature
        float tave;
        // Albedo
        float albedo;
        // Thermally active depth of soil (0.4 m)
        float de;
        // Wind decay coefficient for the forest
        float Wcoeff = 0.0f;
        // Density of soil layer (kg m-3)
        float rho_g = 1700.0f;
        // Snow density (450.0 kg m-3)
        float rho_s;
        // Emissivity of canopy
        float emc = 0.98f;
        //  thermal emissivity of snow nominally 0.99
        float ems = 0.99f;
        // Temperature of freezing (0 C)
        float T0 = 0.0f;
        // Temperature to convert C to K (273.15)
        float Tk = 273.15f;
        // Density of air (this is calculated based on the air pressure each time step).
        // float RHOa;
    } params_;

    class constants
    {
    public:
        // Factor converting s-1 to day-1
        float tf = 86400.0f;
        // Factor converting the temperature to kelvin (273.15 K)
        float tk = 273.15f;
        // Freezing point (0 deg C)
        float t0 = 0.0f;
        // Gravity
        float g = 9.81f;
        // Dynamic viscosity of water at 0 [kg m-1 s-1]
        float mu = 0.001792f;
        // kinematic viscosity of water at 0°C [m2 s-1]
        float nu = 1.79e-6f;
        /*
        Stefan boltzman constant (2.0747e-7 KJ/m^2/hr/K)
        float stefanBoltzman = 2.0747e-7;
        Stefan-Boltzmann constant 5.67e-8 W m-2 K-4 to MJ / m^2 / K^4
        */
        float stefanBoltzman = 4.89888e-09f;
        // Density of water (kg m-3)
        float rho_w = 1000.0f;
        // Density of ice (kg m-3)
        float rho_i = 917.0f;
        // Snow density (kg m-3)
        float rho_s = 450.0f;
        // Density of soil layer (kg m-3)
        float rho_g = 1700.0f;
        // Capilary retention fraction
        // Liquid holding capacity of snow (0.05)
        float L_c = 0.05f;
        // Wind/air temperature measurement height (m)
        float z = 2.0f;
        // Thermally active depth of soil (0.4 m)
        // Soil effective depth
        float D_e = 0.4f;
        // Bare ground albedo
        float A_bg = 0.25f;
        // Albedo extinction depth (m)
        float h = 0.1f;
        // Ideal Gas constant for dry air (287 J= kg= K)
        float Ra_g = 287.0f;
        // Heat of fusion (333.5 KJ / kg)
        float H_f = 333.5f;
        // Heat of Vaporization (Ice to Vapor, 2834 KJ / kg)
        float Hne_u = 2834.0f;
        // Water Heat Capacity (4.191 KJ kg-1 K-1)
        float C_w = 4.191f;
        // Ice heat capacity (2.09 KJ kg-1 K-1) (2.102)
        float C_s = 2.09f;
        // Air Heat Capacity (1.005 KJ kg-1 K-1)
        float C_p = 1.005f;
        // Ground heat capacitiy (2.09 kJ kg-1 K-1)
        float C_g = 2.09f;
        // Von Karman constant
        float K_vc = 0.4f;
        // Snow Saturated hydraulic conductivity 0.02 m/hr is 0.48 m/day
        float K_s = 0.48f;

    } const_;

    // Model routines
    std::tuple<float, float, float, float, float, float, float, float, float, float> PREDICORR(float dt, float FC, float ub, float w, float a, float Ta, float prain, float ps, float Ws, float Ea, float Pr, float Qsi, float Qli, float rkn, float Fm, float tsurf);
    std::tuple<float, float, float, float, float, float, float, float, float> QFM(float FC, float ub, float w, float A, float Ta, float prain, float ps, float ws, float Ea, float Pr, float Qsi, float Qli, float rkn, float Fm);
    std::tuple<float, float, float> TURBFLUX(float Pr, float Ta, float Ts, float rkn, float ws, float Ea);
    std::tuple<float, float, float, float, float, float> PREHELP(float w1, float w, float dt, float Fm1, float fac, float ps, float prain, float E, float Q, float Qm, float Mr, float Qe);

    float QPF(float prain, float ta, float ps);
    float FMELT(float ub, float w, float prain);
    float TAVG(float ub, float w);
    float AGESN(float tausn, float dt, float ps, float tsurf);
    float SRFTMPO(float Qsi, float A, float Qli, float Qpin, float Ea, float Ta, float Tave, float Pr, float w, float ws, float rkn, float FC);
    float SNOTMP(float Qsi, float A, float Qli, float Qpin, float Ea, float Ta, float Tave, float Pr, float w, float ws, float rkn, float FC, float Tstar);
    float SRFTMP(float Qsi, float A, float Qli, float Qpin, float Ea, float Ta, float Tave, float Pr, float w, float ws, float rkn, float FC);
    float surfeb(float Ts, float rkn, float ws, float Tak, float Qp, float densa, float Pr, float Ea, float Tavek, float Qsn, float Qli, float FC);
    float RKINST(float rkn, float ws, float ta, float ts);

    // Support functions
    float albedo(float tausn, float coszen, float d);
    std::tuple<float, float> split_precip_cequeau(Meteo meteo);
    float svpw(float T);
    float svpi(float T);
    float svp(float T);
    float Delta(float T);
    //! Noms des variable d'etat dans les intrants
    std::vector<std::string> nomChamps_;
    //! Etat pour un carreau entier
    EtatFonteCE etatFonteCE_;
    //! Etats des carreaux entiers pour un pas de temps
    std::vector<EtatFonteCE> etatsFonteCE_;
    //! Etats des carreaux entiers pour tous les pas de temps
    std::vector<std::vector<EtatFonteCE>> etatsFonte_;

    // Variable to store the zenith angle
    float zenitAngle_;

    // Donnees a assimiler
    class EtatFonteAssimCE
    {
    public:
        int idCarreauEntier;
        TypeAssim SNC_stockNeigeForetType;
        float SNC_stockNeigeForet[2];
        TypeAssim SND_stockNeigeClairiereType;
        float SND_stockNeigeClairiere[2];
        TypeAssim QNUI3_indexMurissementNeigeType;
        float QNUI3_indexMurissementNeige[2];
        TypeAssim QNUI4_indexTempNeigeType;
        float QNUI4_indexTempNeige[2];
    };

    std::map<DateChrono, std::vector<EtatFonteAssimCE>> assimilationsCE_;
    // conservation des etats avant assimilation
    std::map<DateChrono, std::vector<EtatFonteCE>> avantAssimilationsCE_;
};
