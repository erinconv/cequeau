#include "stdafx.h"

class Puits {
private:
    int idCE;
    int active;
    double distanceRiviere;
    double niveauInitial;
    double rayonP;
    std::vector<double> niveauxPuits;
    std::vector<double> debitPompage;

public:
    Puits(int id, int active, double distance, double niveauInitial, double rayonP, std::vector<double> niveaux, std::vector<double> debit);
    
    int getIdCE() const;
    int getActive() const;
    double getDistanceRiviere() const;
    double getNiveauInitial() const;
    double getRayonP() const;

    const std::vector<double>& getNiveauxPuits() const;
    const std::vector<double>& getDebitPompage() const;

    double getNiveauPuitsParIndex(int idxPasDeTemps) const;
    double getDebitPompageParIndex(int idxPasDeTemps) const;
};
