#include <array>
#include "DateChrono.h"

typedef std::array<float, 12> MoyenneMois;

const int NB_MAX_PARAM_EQUATION = 7;
typedef std::array<float, NB_MAX_PARAM_EQUATION> ParamEquation;

class InfoEvacuationExt
{
public:
  //InfoEvacuationExt();
  // TODO: Besoins du id du carreau externe?
  // Date de debut de l'application de cette loi d'evacuation.
  DateChrono dateDebut;
  // Date de fin de l'application de cette loi d'evacuation.
  DateChrono dateFin;
  // Paramettres de calcul de cette loi d'evacuation.
  ParamEquation paramDebit;
};
