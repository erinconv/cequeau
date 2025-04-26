

#include "stdafx.h"
#include <cmath>
#include "FonteUEB.h"
#include "SolarFunctions.h"
// #include "FonteUEB.hpp"

FonteUEB::FonteUEB()
{
}

//------------------------------------------------------------------
FonteUEB::FonteUEB(int latitudeMoyenneBV, int nbCE)
    : Fonte(nbCE, "FonteUEB")
{
    int degresEnMinutes = (latitudeMoyenneBV / 100) * 60;
    int minutes = latitudeMoyenneBV % 100;

    // float constModulationSoleil_ = tan((float)(degresEnMinutes + minutes) * PI / 10800.0f);

    // Nom de champs d'un etat CE
    nomChamps_.push_back("w");
    nomChamps_.push_back("ub");
    nomChamps_.push_back("Qh");
    nomChamps_.push_back("Qe");
    nomChamps_.push_back("E");
    nomChamps_.push_back("Mr");
    nomChamps_.push_back("Qm");
    nomChamps_.push_back("Q");
    nomChamps_.push_back("Fm");
    nomChamps_.push_back("tsurf");
    nomChamps_.push_back("albedo");
    nomChamps_.push_back("tave");
    nomChamps_.push_back("tausn");
    // nomChamps_.push_back("stockNeigeForet");
    // nomChamps_.push_back("stockNeigeClairiere");

    // Send the required meteo values
    // Incoming shortwave radiation
    nomsChampsMeteo().push_back("rayonnement");
    // Incoming longwave radiation
    nomsChampsMeteo().push_back("longwaveRad");
    // Wind speed
    nomsChampsMeteo().push_back("vitesseVent");
    // Surface pressure
    nomsChampsMeteo().push_back("surfacePressure");
    // Saturated vapor pressure
    nomsChampsMeteo().push_back("pression");

    // Enlever si jamais utilisï¿½
    // nomsChampsMeteo().push_back("humidity");
}

//------------------------------------------------------------------
FonteUEB::~FonteUEB()
{
}

//------------------------------------------------------------------
int FonteUEB::calculerFonte( // IN
    const DateChrono &datePasDeTemps,
    const Meteo &meteo,
    const CarreauEntier &carreauEntier,
    // OUT
    float &precipationsTotales,
    float &eauDisponible)
{

    const int indexCE = carreauEntier.id() - 1;
    // Nouveau pas de temps
    if (indexCE == 0)
    {
        etatsFonteCE_.clear();
        etatsFonteCE_.reserve(nbCE_);
    }

    EtatFonteCE &etatFonteCE = etatsFonte_.back().at(indexCE);
    // state variables
    float w;     // Snow Energy Content  (KJ/m^2)
    float ub;    // Snow Water Equivalent (m) relative to T = 0 C solid phase
    float tsurf; // Snow surface temperature
    float tausn; // Snowage
    float A;     // Albedo
    // Mass terms
    // float cump, cume, cummr;
    float E, Mr, Fm;
    // Energy terms
    float Qh, Qe, Qm, Q;

    float FC = carreauEntier.pctForet(); // Fraction of forest (0-1)
    SolarFunctions solarFunc(datePasDeTemps, carreauEntier);
    solarFunc.computeZenithAngle(params_.hours);
    // Check if the lat-lon fields were provided as input
    float coszen;
    if (carreauEntier.longitude() != -999.0f)
    {
        const float angle_deg = solarFunc.getZenithAngle(); // Cos(angle between direct sunlight and surface normal).
        coszen = cos(rad * angle_deg);
    }
    else
    {
        coszen = 0.5;
    }
    // const float angle_deg = solarFunc.getZenithAngle(); // Cos(angle between direct sunlight and surface normal).
    // float coszen = cos(rad * angle_deg);
    // // float coszen = 0.1;                  // Cos(angle between direct sunlight and surface normal).

    // Initialize variables
    w = etatFonteCE.w;
    ub = etatFonteCE.ub;
    tausn = etatFonteCE.tausn;

    float cd_up = std::pow(const_.K_vc, 2.0f) * const_.tf;
    float cd_den = std::log(params_.z / params_.z0);
    float cd = cd_up / std::pow(cd_den, 2.0f) * (1.0f - 0.8f * FC);

    float RRHOI = const_.rho_i / const_.rho_w;
    float RRHO = params_.rho_s / const_.rho_w;
    float RID = 1.0f / RRHO - 1.0f / RRHOI;

    // Variables
    float Qsi = meteo.meteoFonte().at(0);           // Incoming shortwave radiation (MJ m-2 day-1)
    float Qli = meteo.meteoFonte().at(1);           // Incoming longwave radiation (MJ m-2 day-1)
    float ws = 0.277778f * meteo.meteoFonte().at(2); // Wind Speed form km/h to m/s
    float Pr = meteo.meteoFonte().at(3);            // Atmospheric pressure [Pa]
    float Ea = 133.322f * meteo.meteoFonte().at(4);  // vapor pressure from mmHg to Pa
    float Ta = meteo.calculerTempMoy();             // Air temperature input (Degrees C)

    // Split the presipitation into snowfall and rain fall
    float ps, prain;
    std::tie(ps, prain) = split_precip_cequeau(meteo);

    // Convert prain from mm to m
    prain = prain / 1000.0f;
    ps = ps / 1000.0f;

    A = albedo(tausn, coszen, w / RRHO);

    float qsi = Qsi * (1.0f - FC);
    float qli = Qli * (1.0f - FC);
    float dt = 1.0f;
    float rkn = cd * ws; // Calculate neutral mass transfer coefficient

    std::tie(ub, w, Qh, Qe, E, Mr, Qm, Q, Fm, tsurf) = PREDICORR(dt, FC, ub, w, A, Ta, prain, ps, ws, Ea, Pr, qsi, qli, rkn, Fm, tsurf);

    tausn = AGESN(tausn, dt, ps, tsurf);
    float tave = TAVG(ub, w);

    etatFonteCE_.w = w;
    etatFonteCE_.ub = ub;
    etatFonteCE_.E = E;
    etatFonteCE_.Mr = Mr;
    etatFonteCE_.tausn = tausn;
    etatFonteCE_.tsurf = tsurf;
    etatFonteCE_.tave = tave;
    etatFonteCE_.Q = Q;
    etatFonteCE_.Fm = Fm;
    etatFonteCE_.albedo = A;
    etatFonteCE_.Qh = Qh;
    etatFonteCE_.Qe = Qe;
    etatFonteCE_.Qm = Qm;

    // etatFonteCE_ = etatFonteCE;
    // Output variables
    // etatFonteCE_.SNC_stockNeigeForet = etatFonteCE_.SNC_stockNeigeForet + meteo.neige() - etatFonteCE.Mr;
    // etatFonteCE_.SND_stockNeigeClairiere = etatFonteCE_.SND_stockNeigeClairiere + meteo.neige() - etatFonteCE.Mr;
    etatsFonteCE_.push_back(etatFonteCE_);

    // eauDisponible = etatFonteCE.Mr * 1000 + part_rain;
    eauDisponible = (Mr) * 1000.0f;
    // eauDisponible = 0;
    precipationsTotales = (ps + prain) * 1000.0f;
    // eauDisponible = 0;
    // precipationsTotales = totalPrecip * 1000;

    return 0;
}

std::tuple<float, float, float, float, float, float, float, float, float, float> FonteUEB::PREDICORR(float dt, float FC, float ub, float w, float a, float Ta, float prain, float ps, float Ws, float Ea, float Pr, float Qsi, float Qli, float rkn, float Fm, float tsurf)
{
    float Qh, Qe, E, Mr, Qm, Q, Qnet;

    // Tolerances
    float wtol = 0.025f;
    float utol = 2000.0f;

    std::tie(Fm, Q, Mr, Qe, E, tsurf, Qh, Qnet, Qm) = QFM(FC, ub, w, a, Ta, prain, ps, Ws, Ea, Pr, Qsi, Qli, rkn, Fm);
    float w1 = w + dt * Fm;

    if (w1 < 0.0f)
    {
        w1 = 0.0f;
        std::tie(Q, Qm, Mr, Qe, E, Fm) = PREHELP(w1, w, dt, 0, 1, ps, prain, E, Q, Qm, Mr, Qe);
    }
    float ub1 = ub + dt * Q;

    float Q1 = Q;
    float Fm1 = Fm;
    float Qh1 = Qh;
    float Qe1 = Qe;
    float E1 = E;
    float Mr1 = Mr;
    float Qm1 = Qm;
    float tsurf1 = tsurf;
    float Qnet1 = Qnet;

    std::tie(Fm, Q, Mr, Qe, E, tsurf, Qh, Qnet, Qm) = QFM(FC, ub1, w1, a, Ta, prain, ps, Ws, Ea, Pr, Qsi, Qli, rkn, Fm);

    float w2 = w + dt / 2.0f * (Fm1 + Fm);

    if (w2 < 0.0f)
    {
        w2 = 0.0f;
        // PREHELP(w2, w, dt, Fm, Fm1, 2, ps, prain, E, Q, Qm, Mr, Qe);
        // Fm = PREHELP(w1, w, dt, Fm1, 2, ps, prain, E, Q, Qm, Mr, Qe);
        std::tie(Q, Qm, Mr, Qe, E, Fm) = PREHELP(w2, w, dt, Fm1, 2, ps, prain, E, Q, Qm, Mr, Qe);
    }

    float ub2 = ub + dt / 2.0f * (Q1 + Q);

    int niter = 1;
    int imax = 5;

    while ((std::abs(w2 - w1) > wtol || std::abs(ub2 - ub1) > utol) && (niter < imax))
    {
        w1 = w2;
        ub1 = ub2;
        std::tie(Fm, Q, Mr, Qe, E, tsurf, Qh, Qnet, Qm) = QFM(FC, ub1, w1, a, Ta, prain, ps, Ws, Ea, Pr, Qsi, Qli, rkn, Fm);
        w2 = w + dt / 2.0f * (Fm1 + Fm);
        if (w2 < 0.0f)
        {
            w2 = 0.0f;
            std::tie(Q, Qm, Mr, Qe, E, Fm) = PREHELP(w2, w, dt, Fm1, 2.0f, ps, prain, E, Q, Qm, Mr, Qe);
        }
        ub2 = ub + dt / 2.0f * (Q1 + Q);
        niter = niter + 1;
        if (niter >= imax)
        {
            float ae = (Q1 + Q + Qm1 + Qm) * 0.5f;
            if (ub > 0.0f && ae > 0.0f && w > 0.0f)
            {
                float E2 = (E + E1) * 0.5f;
                float rlf = (ub + ae) / (const_.rho_w * w * const_.H_f);
                if (rlf > 1.0f)
                {
                    Mr = w / dt + (ps + prain - E2);
                    if (Mr < 0.0f)
                    {
                        Mr = 0.0f;
                    }
                    Qm = Mr * const_.rho_w * const_.H_f;
                    w2 = 0.0f;
                    ub2 = ub + ae - Qm * dt;
                }
                else
                {
                    float r = w / ub;
                    ub2 = (const_.rho_w * const_.H_f * (w + (ps + prain - E2) * dt) - ae - ub) / (const_.rho_w * const_.H_f * r - 1.0f);
                    w2 = r * ub2;
                    Qm = (ae + ub - ub2) / dt;
                    Mr = Qm / (const_.rho_w * const_.H_f);
                    Q = ae / dt - Qm;
                    Qm1 = Qm;
                    Mr1 = Mr;
                    Q1 = Q;
                }
            }
        }
    }
    w = w2;
    ub = ub2;
    Qh = (Qh + Qh1) * 0.5f;
    Qe = (Qe + Qe1) * 0.5f;
    E = (E + E1) * 0.5f;
    Mr = (Mr + Mr1) * 0.5f;
    Qm = (Qm + Qm1) * 0.5f;
    tsurf = (tsurf + tsurf1) * 0.5f;
    Qnet = (Qnet + Qnet1) * 0.5f;
    Q = (Q + Q1) * 0.5f;
    Fm = (Fm + Fm1) * 0.5f;

    return std::make_tuple(ub, w, Qh, Qe, E, Mr, Qm, Q, Fm, tsurf);
}

std::tuple<float, float, float, float, float, float> FonteUEB::PREHELP(float w1, float w, float dt, float Fm1, float fac, float ps, float prain, float E, float Q, float Qm, float Mr, float Qe)
{
    float Fm_other = (w1 - w) / dt * fac - Fm1;
    Mr = std::max<float>(0.0f, ps + prain - Fm_other - E);
    E = ps + prain - Fm_other - Mr;
    float Qother = Q + Qm - Qe;
    Qm = Mr * const_.rho_w * const_.H_f;
    Qe = -E * const_.rho_w * const_.Hne_u;
    Q = Qother - Qm + Qe;
    return std::make_tuple(Q, Qm, Mr, Qe, E, Fm_other);
}

std::tuple<float, float, float, float, float, float, float, float, float> FonteUEB::QFM(float FC, float ub, float w, float A, float Ta, float prain, float ps, float ws, float Ea, float Pr, float Qsi, float Qli, float rkn, float Fm)

{
    float Q, Mr, Qe, E, Qh, Qnet, Qm;
    float Qp = QPF(prain, Ta, ps);
    float Qg = 0.0f;
    float tave = TAVG(ub, w);
    float tsurf;
    // if (params_.snow_temp_method == 1)
    // {
    //     tsurf = SRFTMP(Qsi, A, Qli, Qp, Ea, Ta, tave, Pr, w, ws, rkn, FC);
    // }
    // else if (params_.snow_temp_method == 2)
    // {
    //     tsurf = SRFTMPO(Qsi, A, Qli, Qp, Ea, Ta, tave, Pr, w, ws, rkn, FC);
    // }
    tsurf = SRFTMPO(Qsi, A, Qli, Qp, Ea, Ta, tave, Pr, w, ws, rkn, FC);

    // float tsurf = SRFTMPO(Qsi, A, Qli, Qp, Ea, Ta, tave, Pr, w, ws, rkn, FC);
    float Qle = (1.0f - FC) * params_.ems * const_.stefanBoltzman * std::pow(tsurf + const_.tk, 4); // [MJ m-2 day-1]
    float Qlnet = Qli - Qle;
    Qnet = (1.0f - A) * Qsi + Qlnet;
    std::tie(Qh, Qe, E) = TURBFLUX(Pr, Ta, tsurf, rkn, ws, Ea);
    Mr = FMELT(ub, w, prain);
    Qm = Mr * const_.rho_w * (const_.H_f + (tave - const_.t0) * const_.C_w);
    Q = Qnet * 1000.0f + Qp + Qg + Qh + Qe - Qm;
    Fm = prain + ps - Mr - E;
    return std::make_tuple(Fm, Q, Mr, Qe, E, tsurf, Qh, Qnet, Qm);
}

std::tuple<float, float, float> FonteUEB::TURBFLUX(float Pr, float Ta, float Ts, float rkn, float ws, float Ea)
{
    float Qh, Qe, E;
    float Tak = Ta + const_.tk;
    float Tsk = Ts + const_.tk;
    float RKIN = RKINST(rkn, ws, Tak, Tsk);
    float RHOA = Pr / (const_.Ra_g * Tak);
    float Es = svp(Ts);
    Qh = RHOA * (Ta - Ts) * const_.C_p * RKIN;
    Qe = 0.622 * const_.Hne_u / (const_.Ra_g * Tak) * (RKIN * (Ea - Es));
    E = -Qe / (const_.rho_w * const_.Hne_u);
    return std::make_tuple(Qh, Qe, E);
}

float FonteUEB::QPF(float prain, float ta, float ps)
{
    // Split the equation in two terms
    float term1, term2;
    // Snow and rain temperatures
    float Train, Tsnow;
    if (ta > const_.t0)
    {
        Train = ta;
        Tsnow = const_.t0;
    }
    else
    {
        Tsnow = ta;
        Train = const_.t0;
    }

    term1 = prain * const_.rho_w * (const_.H_f + const_.C_w * (Train - const_.t0));
    term2 = ps * const_.rho_w * const_.C_s * (Tsnow - const_.t0);
    float Qp = term1 + term2; // [KJ m-2 day-1]
    return Qp;
}

float FonteUEB::FMELT(float ub, float w, float prain)
{
    float UU = 0.0;
    float melt;
    float SS = 0.0;

    float RRHOI = const_.rho_i / const_.rho_w;
    float RRHO = params_.rho_s / const_.rho_w;
    float RID = 1.0 / RRHO - 1.0 / RRHOI;

    if (ub < 0.0f)
    {
        melt = 0.0f;
    }
    else if (w <= 0.0f)
    {
        melt = prain;
        if (prain <= 0.0f)
        {
            melt = 0.0f;
        }
    }
    else
    {
        UU = ub / (const_.rho_w * w * const_.H_f);
        if (UU > params_.melt_frac)
        {
            UU = params_.melt_frac;
        }

        if ((UU / (1 - UU)) <= params_.Lc)
        {
            SS = 0.0f;
        }
        else
        {
            // SS = (UU / ((1 - UU) * RID) - params_.Lc / RID) / (1 - params_.Lc / RID);
            SS = (UU / (1 - UU) - params_.Lc) / (RID - params_.Lc);
        }
        float K_sat = 3.0f * std::pow(params_.D / 2, 2) * std::exp(-0.013 * params_.rho_s) * (const_.g * const_.rho_w / const_.mu);
        K_sat = K_sat * const_.tf;
        // std::cout << K_sat << std::endl;
        // melt = K_sat * std::pow(SS, 3);
        melt = params_.K_sat * std::pow(SS, 3);
    }

    return melt;
}

float FonteUEB::AGESN(float tausn, float dt, float ps, float tsurf)
{
    float Tsk = tsurf + const_.tk;
    float R1 = std::exp(5000.0 * (1 / const_.tk - 1 / Tsk));
    float R2 = std::pow(R1, 10.0);
    //  // TODO: This need to be check and put as input
    // float dt = 1;
    if (R2 > 1.0f)
    {
        R2 = 1.0f;
    }
    // R3 = 0.01 for antartica. R3 - 0.3 elsewhere
    float R3 = 0.3f;
    // Originally 10**6 sec. Converted to days
    float t0 = 11.5740741f;
    float delta_tau = 1.0f / t0 * (R1 + R2 + R3) * dt;
    tausn = (tausn + delta_tau) * (1.0f - 100.0f * ps);
    tausn = std::max<float>(0.0f, tausn);
    return tausn;
}

float FonteUEB::TAVG(float ub, float w)
{
    // heat required to melt all the snow water equivalence at 0 Â°C [KJ m-2]
    float heat_to_melt = const_.rho_w * w * const_.H_f;
    // # heat capacity of the soil layer [KJ C-1]
    float heat_soil = const_.rho_g * params_.de * const_.C_g;
    // # Heat capacity of snow [KJ K-1 m-2]
    float heat_snow = const_.rho_w * w * const_.C_s;
    // # Heat capacity of liquid water [KJ K-1 m-2]
    float heat_water = const_.rho_w * w * const_.C_w;
    // Average temperature
    float tave;
    // All solid phase
    if (ub <= 0.0f)
    {
        tave = ub / (heat_snow + heat_soil);
    }
    else
    {
        float AL = ub/const_.rho_w * const_.H_f;
        if (w > AL)
        {
            tave = 0;
        }
        else
        {
            tave = (ub - heat_to_melt) / (heat_soil + heat_water);
        }
        
    }
    return tave;
}

float FonteUEB::SRFTMPO(float Qsi, float A, float Qli, float Qpin, float Ea, float Ta, float Tave, float Pr, float w, float ws, float rkn, float FC)
{
    int ncall = 0;
    int niter = 20;
    float tol = 0.05f;
    float Tstar = Ta;
    // float Err = 1.0f;
    float qp = Qpin;
    if (w < 0.0f && qp > 0.0f)
    {
        qp = 0.0f;
    }
    float tsurf = SNOTMP(Qsi, A, Qli, qp, Ea, Ta, Tave, Pr, w, ws, rkn, FC, Tstar);

    // Define upper and lower boundaries to start the process
    float tlb, tub;
    float thereshold = 30.0f;
    if (tsurf > Ta)
    {
        tlb = Ta;
        tub = Ta + thereshold; // Max upper bound 30 C warmer than surface
        if (tsurf > tub)
        {
            tsurf = (tlb + tub) / 2.0f;
        }
    }
    else
    {
        tlb = Ta - thereshold;
        tub = Ta;
        if (tsurf < tlb)
        {
            tsurf = (tlb + tub) / 2.0f;
        }
    }
    Tstar = tsurf;

    for (size_t i = 0; i < niter; i++)
    {
        float tsurf = SNOTMP(Qsi, A, Qli, qp, Ea, Ta, Tave, Pr, w, ws, rkn, FC, Tstar);

        if (tlb >= tsurf && tsurf <= tub)
        {
            if (tsurf > Tstar)
            {
                tlb = Tstar;
            }
            else
            {
                tub = Tstar;
            }
        }
        else if (tsurf > tub)
        {
            tlb = Tstar;
            tsurf = (tlb + tub) / 2.0f;
        }
        else
        {
            tub = Tstar;
            tsurf = (tlb + tub) / 2.0f;
        }

        // Check convergence
        if (std::abs(tsurf - Tstar) < tol)
        {
            // Break and exit the looop
            // std::cout << "Si converge" << std::endl;
            break;
        }
        else
        {
            Tstar = tsurf;
        }
    }

    if (w > 0.0f && tsurf > 0.0f)
    {
        tsurf = 0.0f;
    }

    return tsurf;
}

float FonteUEB::SNOTMP(float Qsi, float A, float Qli, float Qpin, float Ea, float Ta, float Tave, float Pr, float w, float ws, float rkn, float FC, float Tstar)
{
    float Tak = Ta + const_.tk;
    float Tstark = Tstar + const_.tk;
    float Tavek = Tave + const_.tk;

    float RHOA = Pr / (const_.Ra_g * Tak);
    float RKIN = RKINST(rkn, ws, Tak, Tstark);

    // Depict the equation 22 in TARBOTON and CHOWDHURY, 1995
    float term1, term2, term3, term4, term5, term6, term7, term8, term9;
    float Qsn = (1.0f - A) * Qsi;
    term1 = (Qsn + Qli) * 1000.0f + Qpin;
    term2 = RKIN * Tak * RHOA * const_.C_p;
    float e_tstar = svp(Tstar);
    float delta = Delta(Tstar);
    term3 = 0.622 * RKIN * const_.Hne_u * RHOA * (e_tstar - Ea - Tstark * delta) / Pr;
    term4 = 3 * (1.0f - FC) * params_.ems * const_.stefanBoltzman * std::pow(Tstark, 4.0f) * 1000.0f;
    term5 = params_.rho_s * const_.C_s * Tavek * params_.K_s;

    term6 = params_.rho_s * const_.C_s * params_.K_s;
    term7 = RKIN * RHOA * const_.C_p;
    term8 = 0.622f * delta * RKIN * const_.Hne_u * RHOA / Pr;
    term9 = 4.0f * (1.0f - FC) * params_.ems * const_.stefanBoltzman * std::pow(Tstark, 3.0f) * 1000.0f;

    float tsurf = (term1 + term2 - term3 + term4 + term5) / (term6 + term7 + term8 + term9);
    return tsurf - const_.tk;
}

float FonteUEB::SRFTMP(float Qsi, float A, float Qli, float Qpin, float Ea, float Ta, float Tave, float Pr, float w, float ws, float rkn, float FC)
{
    float fff = 0.999;
    float tol = 0.0001;
    float Qsn = (1.0f - A) * Qsi;
    float Tak = Ta + const_.tk;
    float Tavek = Tave + const_.tk;
    float densa = Pr / (const_.Ra_g * Tak);
    float dens = params_.rho_s;
    float qp = Qpin;
    if (w < 0.0f && qp > 0.0f)
    {
        qp = 0.0f;
    }
    // First approximation
    float Ts = Tak;
    float ER = 1.0f;
    int niter = 0;
    float Tsalast;
    float dTs = 273.15f;
    while (ER > tol && niter < 10)
    {
        Tsalast = Ts;
        float Ts1 = Ts - dTs;
        float Ts2 = Ts + dTs;
        float F1 = surfeb(Ts1, rkn, ws, Tak, qp, densa, Pr, Ea, Tavek, Qsn, Qli, FC);
        float F2 = surfeb(Ts2, rkn, ws, Tak, qp, densa, Pr, Ea, Tavek, Qsn, Qli, FC);
        float F = surfeb(Ts, rkn, ws, Tak, qp, densa, Pr, Ea, Tavek, Qsn, Qli, FC);
        if (std::abs(F2 - F1) < 1e-7)
        {
            Ts = Tak;
            break;
        }
        else
        {
            float dF = (F2 - F1)/(Ts2 - Ts1);
            Ts = Ts - F/dF;
        }

        // Ts = Ts - ((1.0f - fff) * Ts * F1) / (F1 - F2);
        ER = std::abs(Ts - Tsalast);
        niter = niter + 1;
    }
    Ts = Ts - const_.tk;
    if (w > 0.0f && Ts > 0.0f)
    {
        Ts = 0.0f;
    }

    return Ts;
}

float FonteUEB::surfeb(float Ts, float rkn, float ws, float Tak, float Qp, float densa, float Pr, float Ea, float Tavek, float Qsn, float Qli, float FC)
{
    float Ta = Tak - const_.tk;
    float RKIN = RKINST(rkn, ws, Tak, Ts);
    float RHOA = Pr / (const_.Ra_g * Tak);
    float term1 = RKIN * densa * const_.C_p * (Tak - Ts);
    float Es = svp(Ts - const_.tk);
    float term2 = (const_.Hne_u * densa * 0.622 * RKIN) / Pr * (Ea - Es);
    float term3 = params_.rho_s * const_.C_s * params_.K_s * (Ts - Tavek);
    // float surf_eb = Qp + RKIN * densa * const_.C_p * (Tak - ts) + (const_.Hne_u * densa * 0.622 * RKIN) / Pr * (Ea - svp(Ta) - densa * const_.C_s * params_.Rs * (ts - Tavek)); // [KJ m-2 day-1]
    float surf_eb = Qp + term1 + term2 + term3;                                                   // [KJ m-2 day-1]
    float Q = (Qsn + Qli - (1.0f - FC) * params_.ems * const_.stefanBoltzman * std::pow(Ts, 4)); // [MJ m-2 day-1]
    surf_eb = surf_eb + Q * 1000.0f;
    return surf_eb;
}

float FonteUEB::RKINST(float rkn, float ws, float ta, float ts)
{
    float rkinst;
    if (ws <= 0)
    {
        rkinst = 0;
    }
    else
    {
        float rich = const_.g * (ta - ts) * params_.z / (ws * ws * ta);
        if (rich > 0.0f)
        {
            rkinst = rkn / (1.0f + 10.0f * rich);
        }
        else
        {
            rkinst = rkn / (1.0f - 10.0f * rich);
        }
    }
    rkinst = rkn + params_.fstab * (rkinst - rkn);
    return rkinst;
}

float FonteUEB::albedo(float tausn, float coszen, float d)
{
    // Values extracted from Dickinson et al. pg 21.
    float B = 2.0f; // Can be adjusted
    float CS = 0.2f;
    float CN = 0.5f;

    float FZEN = 0.0f; // Avoid using else in the if statement below

    float FAGE = tausn / (1.0f + tausn);

    if (coszen < 0.5f)
    {
        FZEN = 1.0f / B * ((B + 1.0f) / (1.0f + 2.0f * B * coszen) - 1.0f);
    }

    float AVD = (1.0f - CS * FAGE) * params_.avo;
    float AVIS = AVD + 0.4f * FZEN * (1.0f - AVD);
    float AIRD = (1.0f - CN * FAGE) * params_.airo;
    float ANIR = AIRD + 0.4f * FZEN * (1.0f - AIRD);
    float ALBEDO = (AVIS + ANIR) / 2.0f;

    if (d < params_.aep)
    {
        float rr = (1.0f - d / params_.aep) * std::exp(-d * 0.5f / params_.aep);
        ALBEDO = rr * const_.A_bg + (1.0f - rr) * ALBEDO;
    }

    return ALBEDO;
}

float FonteUEB::svp(float T)
{
    float svp_;
    if (T >= 0.0f)
    {
        svp_ = svpw(T);
    }
    else
    {
        svp_ = svpi(T);
    }
    return svp_;
}

float FonteUEB::Delta(float T)
{
    // Lowe (1977) polynomial formulation
    float a0, a1, a2, a3, a4, a5, a6;
    a0 = 5.030305237e-1f;
    a1 = 3.773255020e-2f;
    a2 = 1.267995369e-3f;
    a3 = 2.477563108e-5f;
    a4 = 3.005693132e-7f;
    a5 = 2.158542548e-9f;
    a6 = 7.131097725e-12f;
    float delta = a0 + T * (a1 + T * (a2 + T * (a3 + T * (a4 + T * (a5 + a6 * T)))));
    // Convert mb to Pa
    return 100 * delta;
}

float FonteUEB::svpw(float T)
{
    // Temperatures in degC
    float a0, a1, a2, a3, a4, a5, a6;
    a0 = 6.107799961f;
    a1 = 0.4436518521f;
    a2 = 0.01428945805f;
    a3 = 0.0002650648471f;
    a4 = 3.031240936e-6f;
    a5 = 2.034080948e-8f;
    a6 = 6.136820929e-11f;
    float svp_ = a0 + T * (a1 + T * (a2 + T * (a3 + T * (a4 + T * (a5 + a6 * T)))));
    // float delta2 = a1 + 2*a2*T + 3*a3*std::pow(T,2) + 4*a4*std::pow(T,3) + 5*a5*std::pow(T,4) + 6*a6*std::pow(T,5);
    // Convert mb to Pa
    return 100 * svp_;
}

float FonteUEB::svpi(float T)
{
    // Temperatures in degC
    float a0, a1, a2, a3, a4, a5, a6;
    a0 = 6.109177956f;
    a1 = 0.503469897f;
    a2 = 0.01886013408f;
    a3 = 0.0004176223716f;
    a4 = 5.82472028e-6f;
    a5 = 4.838803174e-8f;
    a6 = 1.838826904e-10f;
    // This nested way of computation is faster than the expanded version
    float svp_ = a0 + T * (a1 + T * (a2 + T * (a3 + T * (a4 + T * (a5 + a6 * T)))));
    // float delta2 = a1 + 2*a2*T + 3*a3*std::pow(T,2) + 4*a4*std::pow(T,3) + 5*a5*std::pow(T,4) + 6*a6*std::pow(T,5);
    // Convert mb to Pa
    return 100 * svp_;
}

std::tuple<float, float> FonteUEB::split_precip_cequeau(Meteo meteo)
{
    float PJE_pluie, PJN_neige;
    float TJE_tempMoy = meteo.calculerTempMoy();
    float seuilTranformationPluieNeige = params_.seuilTranformationPluieNeige;
    float neigeMeteo = meteo.neige(), pluieMeteo = meteo.pluie();

    // Si la temperature moyenne est inferieure au seuil de transformation - 2 deg,
    // la pluie est transformee en neige.
    float facteurTransformation;
    if (TJE_tempMoy <= params_.seuilTranformationPluieNeige - 2.0f)
    {
        PJE_pluie = 0.0f;
        PJN_neige = neigeMeteo + pluieMeteo;
    }
    // Si la temperature moyenne est comprise entre le seuil de transformation +/- 2 deg,
    // la pluie est transformee partiellement en neige.
    else if (TJE_tempMoy <= params_.seuilTranformationPluieNeige + 2.0f)
    {
        facteurTransformation = std::abs(TJE_tempMoy - params_.seuilTranformationPluieNeige - 2.0f) / 4.0f;
        PJE_pluie = pluieMeteo * (1.0f - facteurTransformation);
        PJN_neige = pluieMeteo * facteurTransformation + neigeMeteo;
    }
    else
    {
        PJE_pluie = pluieMeteo;
        PJN_neige = neigeMeteo;
    }

    // Output values
    // snow = PJN_neige;
    // rain = PJE_pluie;
    return std::make_tuple(PJN_neige, PJE_pluie);
}

int FonteUEB::initialiserAssimilations(const mxArray *assimilations)
{
    return 0;
    // int retCode = 0;

    // if (assimilations == NULL)
    // {
    //     return retCode;
    // }

    // // Pour isnan
    // using namespace boost::math;

    // size_t nbDonnees, nbDonneesCE;

    // mxArray *etatsFonte, *idCE;

    // double pasDeTempsData, *idCEData, *stockNeigeForetData, *stockNeigeClairiereData, *indexMurissementNeigeData;
    // double *indexTempNeigeData;
    // date datePasDeTemps;

    // EtatFonteAssimCE etatFonteAssimCE;
    // std::vector<EtatFonteAssimCE> etatsFonteAssimCE;

    // // Nombre de donnees d'assimilation (pas de temps distincts)
    // nbDonnees = mxGetNumberOfElements(assimilations);
    // for (size_t i = 0; i < nbDonnees; i++)
    // {
    //     MexHelper::chargerValeurs(assimilations, "pasDeTemps", pasDeTempsData, i);
    //     datePasDeTemps = MexHelper::datenumToDate(&pasDeTempsData);
    //     etatsFonte = MexHelper::mhMxGetField(assimilations, i, "etatsFonte");

    //     /***** Carreaux Entiers *****/
    //     // Donnees d'assimilation relatives aux CE pour ce pas de temps d'assimilation
    //     if (mxGetNumberOfElements(etatsFonte) > 0)
    //     {
    //         idCE = MexHelper::mhMxGetField(etatsFonte, 0, "id");
    //         idCEData = MexHelper::mhMxGetPr(idCE, "id");

    //         // Obtention des pointeurs de donnees
    //         // On utilise mxGetPr plutot que MexHelper::mhMxGetPr pour la possibilite
    //         // d'avoir un pointeur null
    //         // TODO New module: Your fields here.
    //         /*** Example
    //         stateVar1 = MexHelper::mhMxGetField(etatsEvapo, 0, "stateVar1");
    //         etatEvapoAssimCE.stateVar1Type = AssimilationHelper::obtenirTypeAssim(stateVar1);
    //         stateVar1Data = mxGetPr(stateVar1);

    //         stateVar2 = MexHelper::mhMxGetField(etatsEvapo, 0, "stateVar2");
    //         etatEvapoAssimCE.stateVar2Type = AssimilationHelper::obtenirTypeAssim(stateVar2);
    //         stateVar2Data = mxGetPr(stateVar2);
    //         ***/
    //         // Nombre de CE vises.
    //         nbDonneesCE = mxGetNumberOfElements(idCE);
    //         etatsFonteAssimCE.clear();
    //         for (size_t j = 0; j < nbDonneesCE; j++)
    //         {
    //             etatFonteAssimCE.idCarreauEntier = (int)idCEData[j];

    //             // TODO New module: Your fields here.
    //             /*** Example
    //             AssimilationHelper::obtenirValeursAssim(stateVar1Data, etatEvapoAssimCE.stateVar1Type,
    //                                 j, etatEvapoAssimCE.stateVar1);

    //             AssimilationHelper::obtenirValeursAssim(stateVar2Data, etatEvapoAssimCE.stateVar2Type,
    //                                 j, etatEvapoAssimCE.stateVar2);
    //             ***/

    //             etatsFonteAssimCE.push_back(etatFonteAssimCE);
    //         }

    //         assimilationsCE_.insert(std::make_pair(datePasDeTemps, etatsFonteAssimCE));
    //     }
    // }

    // return retCode;
}

//------------------------------------------------------------------
int FonteUEB::assimiler(const DateChrono &datePasDeTemps)
{
    if (assimilationsCE_.count(datePasDeTemps) > 0)
    {
        std::vector<EtatFonteCE> etatsSimules;
        std::vector<EtatFonteCE>::iterator iterCE;
        // Toutes les assimilations CE pour ce pas de temps
        std::vector<EtatFonteAssimCE> assimilations = assimilationsCE_.find(datePasDeTemps)->second;

        int idCE;
        // Parcourt des assimilations
        std::vector<EtatFonteAssimCE>::const_iterator assimilationsIter = assimilations.begin();
        for (; assimilationsIter != assimilations.end(); assimilationsIter++)
        {
            // Parcourt des etats
            idCE = 0;
            for (iterCE = etatsFonteCE_.begin(); iterCE != etatsFonteCE_.end(); iterCE++)
            {
                // Si id egaux, on fait la mise a jour
                idCE++;
                if (idCE == assimilationsIter->idCarreauEntier)
                {

                    // Sauvegarde de l'etat avant l'assimilation
                    etatsSimules.push_back(*iterCE);
                    // TODO New module: Your fields here
                    /*** Example
                    AssimilationHelper::assimilerValeur(assimilationsIter->stateVar1,
                                    assimilationsIter->stateVar1Type, iterCE->stateVar1);

                    AssimilationHelper::assimilerValeur(assimilationsIter->stateVar2,
                                    assimilationsIter->stateVar2Type, iterCE->stateVar2);

                    ***/

                    break;
                }
            } // for
        } // for

        // Conservation des etats simules pour comparaison avec etats assimiles
        if (etatsSimules.size() > 0)
        {
            avantAssimilationsCE_.insert(std::make_pair(datePasDeTemps, etatsSimules));
        }

    } // if (assimilationsCE_.count(datePasDeTemps) > 0)

    return 0;
}

//------------------------------------------------------------------
void FonteUEB::initialiserEtats(const mxArray *etatsInitiaux)
{
    etatsFonteCE_.reserve(nbCE_);

    // Pas d'etats precedents, on initialise avec les valeurs par defaut des parametres
    if (etatsInitiaux == NULL)
    {

        for (int i = 0; i < nbCE_; i++)
        {
            // TODO New module: Your fields here.
            etatFonteCE_.w = params_.w;
            etatFonteCE_.ub = params_.ub;
            etatFonteCE_.E = params_.E;
            etatFonteCE_.Mr = params_.Mr;

            etatFonteCE_.Qh = 0.0f;
            etatFonteCE_.Qe = 0.0f;
            etatFonteCE_.Qm = 0.0f;
            etatFonteCE_.Q  = 0.0f;
            etatFonteCE_.Fm = 0.0f;
            // etatFonteCE_.dW = params_.dW;
            // etatFonteCE_.dU = params_.dU;
            etatFonteCE_.tausn = params_.tausn;
            etatFonteCE_.tsurf = params_.tsurf;
            etatFonteCE_.tave = params_.tave;
            etatFonteCE_.albedo = params_.albedo;
            etatFonteCE_.SNC_stockNeigeForet = 0.0f;
            etatFonteCE_.SND_stockNeigeClairiere = 0.0f;
            etatsFonteCE_.push_back(etatFonteCE_);
        }

        etatsFonte_.push_back(etatsFonteCE_);
    }
    else
    {
        // Association entre le nom du champ des intrants et une reference sur la variable correspondante
        std::map<std::string, float *> mapChamps;
        // TODO New module: Your fields here.
        /*** Example
        mapChamps.insert(std::make_pair("stateVar1", &etatEvapoCE_.stateVar1));
        mapChamps.insert(std::make_pair("stateVar2", &etatEvapoCE_.stateVar2));
        ***/

        initialiserEtatsHelper(etatsInitiaux, mapChamps, etatFonteCE_, etatsFonteCE_);

        etatsFonte_.push_back(etatsFonteCE_);
    }
}

void FonteUEB::lireParametres(const mxArray *paramsFonte)
{
    // TODO New module: Your parameters here.

    lireParametresHelper(paramsFonte, "strne_s", params_.seuilTranformationPluieNeige);
    lireParametresHelper(paramsFonte, "aep", params_.aep);
    lireParametresHelper(paramsFonte, "z0", params_.z0);
    lireParametresHelper(paramsFonte, "z", params_.z);
    lireParametresHelper(paramsFonte, "Lc", params_.Lc);
    lireParametresHelper(paramsFonte, "fstab", params_.fstab);
    lireParametresHelper(paramsFonte, "K_sat", params_.K_sat);
    lireParametresHelper(paramsFonte, "D", params_.D);
    lireParametresHelper(paramsFonte, "rho_s", params_.rho_s);
    lireParametresHelper(paramsFonte, "K_s", params_.K_s);
    lireParametresHelper(paramsFonte, "de", params_.de);
    lireParametresHelper(paramsFonte, "melt_frac", params_.melt_frac);
    // lireParametresHelper(paramsFonte, "Rs", params_.Rs);
    lireParametresHelper(paramsFonte, "avo", params_.avo);
    lireParametresHelper(paramsFonte, "airo", params_.airo);
    lireParametresHelper(paramsFonte, "snow_temp_method", params_.snow_temp_method);
    lireParametresHelper(paramsFonte, "hours", params_.hours);

    // Set the initial conditions
    lireParametresHelper(paramsFonte, "w", params_.w);
    lireParametresHelper(paramsFonte, "ub", params_.ub);
    lireParametresHelper(paramsFonte, "E", params_.E);
    lireParametresHelper(paramsFonte, "Mr", params_.Mr);
    lireParametresHelper(paramsFonte, "tausn", params_.tausn);
    lireParametresHelper(paramsFonte, "tsurf", params_.tsurf);
    lireParametresHelper(paramsFonte, "tave", params_.tave);
    lireParametresHelper(paramsFonte, "albedo", params_.albedo);
    // lireParametresHelper(paramsFonte, "Wcoeff", params_.Wcoeff);
    /*** Example
    lireParametresHelper(paramsEvapo, "anotherParam", params_.param2);
    ***/
}

mxArray *FonteUEB::obtenirEtats(const std::vector<bool> filtreCE)
{
    return obtenirEtatsHelper(nomChamps_, etatsFonte_, filtreCE);
}

mxArray *FonteUEB::obtenirEtatsAvantAssimilations(const std::vector<bool> filtreCE)
{
    return obtenirEtatsAvantAssimHelper(filtreCE, nomChamps_, avantAssimilationsCE_);
}

void FonteUEB::preserverEtatsPasDeTemps()
{
    etatsFonte_.push_back(etatsFonteCE_);
}
