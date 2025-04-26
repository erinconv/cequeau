//****************************************************************************
// Fichier:  Barrage.cpp
//
// Date creation: 2012-11-16
// Auteur: 
//                Rio Tinto Alcan                     
//                Energie electrique                  
//                1954 Davis, Saguenay arr. Jonquiere,
//                G7S 4R7, QC, Canada                 
//
//****************************************************************************
#include "stdafx.h" 
#include "Barrage.h" 
#include "DateChrono.h"

//------------------------------------------------------------------
Barrage::Barrage(int idCP, int idCPAval, float volume, const ParamEquation& paramNiveau, const ParamEquation& paramDebit)
  : typeBarrage_(EVACUATION_CALCULEE), idCP_(idCP), idCPAval_(idCPAval), volume_(volume), paramNiveau_(paramNiveau), 
    paramDebit_(paramDebit)
{
  FILE_LOG(logDEBUG) << "Barrage::Barrage_Type2(...)";
  paramVolume_.fill(0.0f);
}

//------------------------------------------------------------------
Barrage::Barrage(int idCP, int idCPAval, float volume, const ParamEquation& paramNiveau, const ParamEquation& paramVolume, 
                 const std::vector<ParamEquation>& paramDebitInt, const std::vector<InfoEvacuationExt>& paramDebitExt)
  : typeBarrage_(EVACUATION_CALCULEE_SPECIAL), idCP_(idCP), idCPAval_(idCPAval), volume_(volume), paramNiveau_(paramNiveau), 
    paramVolume_(paramVolume), paramDebitEvacuationInt_(paramDebitInt), paramDebitEvacuationExt_(paramDebitExt)
{
  FILE_LOG(logDEBUG) << "Barrage::Barrage_Type5(...)";
  paramDebit_.fill(0.0f);
}

//------------------------------------------------------------------
Barrage::Barrage(int idCPAval, const std::vector<float>& debitsMesures)
  : typeBarrage_(EVACUATION_MESUREE_EXT), idCP_(0), idCPAval_(idCPAval), volume_(0), debitsMesures_(debitsMesures)
{
  FILE_LOG(logDEBUG) << "Barrage::Barrage_Type3(...)";
  paramVolume_.fill(0.0f);
  paramDebit_.fill(0.0f);
  paramNiveau_.fill(0.0f);
}

//------------------------------------------------------------------
Barrage::~Barrage()
{
  FILE_LOG(logDEBUG) << "Barrage::~Barrage()";
}

//------------------------------------------------------------------
int Barrage::idCP() const
{
  FILE_LOG(logDEBUG) << "Barrage::idCP()";
  return idCP_;
}

//------------------------------------------------------------------
int Barrage::idCPAval() const
{
  FILE_LOG(logDEBUG) << "Barrage::idCPAval()";
  return idCPAval_;
}

//------------------------------------------------------------------
TypeBarrage Barrage::typeBarrage() const
{
  FILE_LOG(logDEBUG) << "Barrage::typeBarrage()";
  return typeBarrage_;
}

//------------------------------------------------------------------
float Barrage::volume() const
{
  FILE_LOG(logDEBUG) << "Barrage::volume()";
  return volume_;
}

//------------------------------------------------------------------
void Barrage::volume(float valeur)
{
  FILE_LOG(logDEBUG) << "Barrage::volume(float valeur)";
  volume_ = valeur;
}

//------------------------------------------------------------------
const std::vector<float>& Barrage::debitsMesures() const
{
  FILE_LOG(logDEBUG) << "Barrage::debitsMesures()";
  return debitsMesures_;
}

//------------------------------------------------------------------
float Barrage::calculerNiveauAvecVolume() const
{
  FILE_LOG(logDEBUG) << "Barrage::calculerNiveauAvecVolume()";
  return calculerNiveauAvecVolume(volume_);
}

//------------------------------------------------------------------
float Barrage::calculerNiveauAvecVolume(float volume) const
{
  FILE_LOG(logDEBUG) << "Barrage::calculerNiveauAvecVolume(float volume)";
  // H = A + B * V + C * V**1/2 + D * V**1/3 + E * V**1/4 + F * V**1/5 + CO

  float resultat = paramNiveau_[0] + 
                   paramNiveau_[1] * volume + 
                   paramNiveau_[2] * std::pow(volume, 0.50000000f) +
                   paramNiveau_[3] * std::pow(volume, 0.33333333f) + 
                   paramNiveau_[4] * std::pow(volume, 0.25000000f) + 
                   paramNiveau_[5] * std::pow(volume, 0.20000000f) + 
                   paramNiveau_[6];

  if (resultat < paramNiveau_[6])
    resultat = paramNiveau_[6];

  return resultat;
}
//------------------------------------------------------------------
float Barrage::calculerDebitAvecVolume() const
{
  FILE_LOG(logDEBUG) << "Barrage::calculerDebitAvecVol()";
  return calculerDebitAvecVolume(volume_, paramDebit_);
}

//------------------------------------------------------------------
float Barrage::calculerDebitAvecVolume(const ParamEquation& paramDebit) const
{
  FILE_LOG(logDEBUG) << "Barrage::calculerDebitAvecVol(ParamEquation& paramDebit)";
  return calculerDebitAvecVolume(volume_, paramDebit);
}

//------------------------------------------------------------------
float Barrage::calculerDebitAvecVolume(float volume, const ParamEquation& paramDebit) const
{
  FILE_LOG(logDEBUG) << "Barrage::calculerDebitAvecVol(float volume, ParamEquation& paramDebit)";
  // debit = P + Q * V + R * V**2 + S * V**3 + T * V**4 + U * V**5

  float resultat = paramDebit[0] + 
                   paramDebit[1] * volume + 
                   paramDebit[2] * std::pow(volume, 2.0f) +
                   paramDebit[3] * std::pow(volume, 3.0f) + 
                   paramDebit[4] * std::pow(volume, 4.0f) + 
                   paramDebit[5] * std::pow(volume, 5.0f);

  return resultat;
}

//------------------------------------------------------------------
float Barrage::calculerVolumeCpAval(float volumeEntrant, float deltaTemps, float volumeInitialCpAval, const DateChrono& datePasDeTemps,
                                    std::vector<float>& volumesEvacues) 
{
  FILE_LOG(logDEBUG) << "Barrage::calculerVolumeCpAval(...)";

  // Contient le ou les volumes (type 5) sortant(s) du barrage
  volumesEvacues.clear();

  // volumeEvacue, volumeBarrage
  float volumeEvacue;
  float volumeCpAval = volumeInitialCpAval;
  if (typeBarrage_ == EVACUATION_CALCULEE) {
    volumeEvacue = calculerVolumeEvacue(volumeEntrant, deltaTemps, paramDebit_);
    volumesEvacues.push_back(volumeEvacue);
    volumeCpAval += volumeEvacue;
  }
  else if (typeBarrage_ == EVACUATION_CALCULEE_SPECIAL) {
    const ParamEquation* paramEquationDebitExt = trouverParamDebitExt(datePasDeTemps);
    int nbEvacuationInterne = (int)paramDebitEvacuationInt_.size();
    int nbEvacuation = nbEvacuationInterne;
    
    if (paramEquationDebitExt != NULL) {
      nbEvacuation++;
    }

    float apportMoy = volumeEntrant / (float)nbEvacuation;
    // Parcourt des evacuations internes
    for (int i = 0; i < nbEvacuationInterne; i++) {
      volumeEvacue = calculerVolumeEvacue(apportMoy, deltaTemps, paramDebitEvacuationInt_[i]);
      volumesEvacues.push_back(volumeEvacue);
      volumeCpAval += volumeEvacue;
    }

    if (paramEquationDebitExt != NULL) {
      // Appele pour mettre a jour le volume du barrage 
      volumeEvacue = calculerVolumeEvacue(apportMoy, deltaTemps, *paramEquationDebitExt);
      volumesEvacues.push_back(volumeEvacue);
    }
  }

  return volumeCpAval;
}

//------------------------------------------------------------------
const ParamEquation* const Barrage::trouverParamDebitExt(const DateChrono& jour) const
{
  FILE_LOG(logDEBUG) << "Barrage::trouverParamDebitExt(const date& jour)";

  std::vector<InfoEvacuationExt>::const_iterator iter = paramDebitEvacuationExt_.begin();
  for (; iter != paramDebitEvacuationExt_.end(); iter++) {
    if (jour >= iter->dateDebut && jour <= iter->dateFin) {
      break;
    }
  }
  
  if (iter != paramDebitEvacuationExt_.end())
    return &(iter->paramDebit);
  else
    return nullptr;
}

//------------------------------------------------------------------
float Barrage::calculerVolumeEvacue(float volumeEntrant, float deltaTemps, const ParamEquation& paramDebit)
{
  FILE_LOG(logDEBUG) << "Barrage::calculerVolumeEvacue(...)";

  float volumeEvacue = deltaTemps * calculerDebitEvacue(volumeEntrant, deltaTemps, paramDebit);
  volumeEvacue = minf(1000000.0f * volume_, volumeEvacue);
  
  float volumeBarrage = volume_ + (volumeEntrant - volumeEvacue) / 1000000.0f;
  // Nouveau volume d'eau au barrage.
  volume_ = volumeBarrage < 0.0f ? 0.0f : volumeBarrage;

  return volumeEvacue;

}
//------------------------------------------------------------------
float Barrage::calculerDebitEvacue(float volumeEntrant, float deltaTemps, const ParamEquation& paramDebit) const
{
  // TODO: Reecriture sans goto ou bien utilisation d'une lib?
  float resultat = 0.0f;
  int iteration = 0, nbChangementBornes = 0;
  float debit1 = maxf(0.0f, calculerDebitAvecVolume(paramDebit));
  float volume = volume_ * 1000000.0f;
  float constante = (2.0f * volume / deltaTemps) + (2.0f * volumeEntrant / deltaTemps) - debit1; 
  constante = maxf(0.0f, constante);
 
  float ga, b, gb, eps, c, gc, debit2, dx;
   
  float a = volume + volumeEntrant;

  if (a == 0.0f) goto l_99;
  ga = resoudreGoodrich(a, deltaTemps, constante, paramDebit);
  ga += 0.002f;

  if (std::abs(ga) > 0.1f) goto l_320;
  b = a - 0.1f;
  gb = resoudreGoodrich(b, deltaTemps, constante, paramDebit);
  gb -= .002f;
  goto l_250;

l_320:
  b = maxf(0.0f, volume - debit1 * deltaTemps);
  gb = resoudreGoodrich(b, deltaTemps, constante, paramDebit);
  gb -= .002f;
  if (std::abs(gb) > 0.1f) goto l_330;
  a = b + 0.1f;
  ga = resoudreGoodrich(a, deltaTemps, constante, paramDebit);
  ga += 0.002f;
  goto l_250;

l_330:
  eps=maxf(10.0f, a / 100000.0f);

l_100:
  if (ga == gb) goto l_350;
  if ((ga * gb) > 0.0f) goto l_300;

// La solution est dans l'intervalle
l_120:
  iteration++;
  c = (a + b) / 2.0f;
  gc = resoudreGoodrich(c, deltaTemps, constante, paramDebit);
  gc += 0.002f;
  if ((ga * gc) > 0.0f) goto l_150;
  b = c;
  gb = gc;
  goto l_200;

l_150:
  a = c;
  ga = gc;

l_200:
  if (std::abs(b - a) < eps) goto l_250;
  if (iteration < 50) goto l_120;

l_250:
  if (std::abs(ga) < std::abs(gb)) goto l_260;
   c = b + ((a - b) * gb / (gb - ga));
  goto l_270;

l_260:
  c = a - ((a - b) * ga / (ga - gb));

l_270:
  debit2 = maxf(0.0f, constante - 2.0f * c / deltaTemps);
  resultat = (debit1 + debit2) / 2.0f;
  goto l_99;

// Changement d'intervalle
l_300:
  nbChangementBornes++;
  if (nbChangementBornes >= 50) {
    std::string erreur = "Impossible de calculer le debit sortant du reservoir";
    throw std::runtime_error(erreur);
  }

l_350:
  if(ga != gb) goto l_355;
  if(ga < 0.0f) goto l_417;
  dx = eps;
  if (ga < gb) dx=-eps;
  goto l_358;

l_417:
  dx = -eps;
  if (ga < gb) dx = eps;
  goto l_358;

l_355:
  dx = maxf(eps, 2.0f * std::abs((a - b) * gb / (ga - gb)));

l_358:
  if (ga < 0.0f) goto l_430;
  if (ga < gb) goto l_410;
  a = b;
  ga = gb;
  b = b - dx;
  gb = resoudreGoodrich(b, deltaTemps, constante, paramDebit);
  goto l_100;

l_410:
  b = a;
  gb = ga;
  a = a + dx;
  ga = resoudreGoodrich(a, deltaTemps, constante, paramDebit);
  goto l_100;

l_430:
  if (ga < gb) goto l_450;
  b = a;
  gb = ga;
  a = a - dx;
  ga = resoudreGoodrich(a, deltaTemps, constante, paramDebit);
  goto l_100;

l_450:
  a = b;
  ga = gb;
  b = b + dx;
  gb = resoudreGoodrich(b, deltaTemps, constante, paramDebit);
  goto l_100;

l_99:
  return resultat;
}

float Barrage::resoudreGoodrich(float volume, float deltaTemps, float constante, const ParamEquation& paramDebit) const
{
  float resultat = (2.0f * volume) / deltaTemps;
  float debit = calculerDebitAvecVolume(volume / 1000000.0f, paramDebit);
  resultat = resultat - constante + maxf(0.0f, debit);
  
  return resultat;
}

