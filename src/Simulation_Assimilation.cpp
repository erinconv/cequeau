//****************************************************************************
// Fichier: Simulation.cpp
//
// Date creation: 2012-10-01
// Auteur: 
//                Rio Tinto Alcan                     
//                Energie electrique                  
//                1954 Davis, Saguenay arr. Jonquiere,
//                G7S 4R7, QC, Canada                 
//
//****************************************************************************
#include "stdafx.h"
#include "Simulation.h"

//------------------------------------------------------------------
int Simulation::assimiler(const DateChrono& datePasDeTemps, EtatsCarreauxEntiers& etatsCarreauxEntiers)
{
  FILE_LOG(logDEBUG) << "Simulation::assimiler(date& datePasDeTemps, EtatsCarreauxEntiers& etatsCarreauxEntiers)";

  if (assimilationsCE_.count(datePasDeTemps) > 0) {
    EtatsCarreauxEntiers etatsSimules;
    EtatsCarreauxEntiers::iterator iterCE;
    // Toutes les assimilations CE pour ce pas de temps
    EtatsCarreauxEntiersAssim assimilations = assimilationsCE_.find(datePasDeTemps)->second;

    // Parcourt des assimilations
    EtatsCarreauxEntiersAssim::const_iterator assimilationsIter = assimilations.begin();
    for (; assimilationsIter != assimilations.end(); assimilationsIter++) {
      // Parcourt des etats
      for (iterCE = etatsCarreauxEntiers.begin(); iterCE != etatsCarreauxEntiers.end(); iterCE++) {
        // Si id egaux, on fait la mise a jour
        if (iterCE->idCarreauEntier == assimilationsIter->idCarreauEntier) {

          // Sauvegarde de l'etat avant l'assimilation
          etatsSimules.push_back(*iterCE);

          AssimilationHelper::assimilerValeur(assimilationsIter->niveauEauSol, 
                          assimilationsIter->niveauEauSolType, iterCE->niveauEauSol);

          AssimilationHelper::assimilerValeur(assimilationsIter->niveauEauNappe, 
                          assimilationsIter->niveauEauNappeType, iterCE->niveauEauNappe);

          AssimilationHelper::assimilerValeur(assimilationsIter->niveauEauLacsMarais, 
                          assimilationsIter->niveauEauLacsMaraisType, iterCE->niveauEauLacsMarais);

          AssimilationHelper::assimilerValeur(assimilationsIter->evapoPotJour, 
                          assimilationsIter->evapoPotJourType, iterCE->evapoPotJour);

          AssimilationHelper::assimilerValeur(assimilationsIter->production, 
                          assimilationsIter->productionType, iterCE->production);

          break;
        }
      } // for
    } // for

    // Conservation des etats simules pour comparaison avec etats assimiles
    if (etatsSimules.size() > 0) {
      avantAssimilationsCE_.insert(std::make_pair(datePasDeTemps, etatsSimules));
    }

  } // if (assimilationsCE_.count(datePasDeTemps) > 0)

  return 0;
}

//------------------------------------------------------------------
int Simulation::assimiler(const DateChrono& datePasDeTemps, EtatsCarreauxPartiels& etatsCarreauxPartiels)
{
  FILE_LOG(logDEBUG) << "Simulation::assimiler(const date& datePasDeTemps, EtatsCarreauxPartiels& etatsCarreauxPartiels)";

  if (assimilationsCP_.count(datePasDeTemps) > 0) {
    EtatsCarreauxPartiels etatsSimules;
    EtatsCarreauxPartiels::iterator iterCP;
    // Toutes les assimilations CP pour ce pas de temps
    EtatsCarreauxPartielsAssim assimilations = assimilationsCP_.find(datePasDeTemps)->second;

    // Parcourt des assimilations
    EtatsCarreauxPartielsAssim::const_iterator assimilationsIter = assimilations.begin();
    for (; assimilationsIter != assimilations.end(); assimilationsIter++) {
      // Parcourt des etats
      for (iterCP = etatsCarreauxPartiels.begin(); iterCP != etatsCarreauxPartiels.end(); iterCP++) {
        // Si id egaux, on fait la mise a jour
        if (iterCP->idCarreauPartiel == assimilationsIter->idCarreauPartiel) {
          // Sauvegarde de l'etat avant l'assimilation
          etatsSimules.push_back(*iterCP);

          AssimilationHelper::assimilerValeur(assimilationsIter->apport, 
                          assimilationsIter->apportType, iterCP->apport);

          AssimilationHelper::assimilerValeur(assimilationsIter->volume, 
                          assimilationsIter->volumeType, iterCP->volume);

          AssimilationHelper::assimilerValeur(assimilationsIter->debit, 
                          assimilationsIter->debitType, iterCP->debit);

          break;
        }
      } // for
    } // for

    // Conservation des etats simules pour comparaison avec etats assimiles
    if (etatsSimules.size() > 0) {
      avantAssimilationsCP_.insert(std::make_pair(datePasDeTemps, etatsSimules));
    }

  } // if (assimilationsCP_.count(datePasDeTemps) > 0)

  return 0;
}

//------------------------------------------------------------------
int Simulation::assimiler(const DateChrono& datePasDeTemps, EtatsBarrages& etatsBarrages)
{
    FILE_LOG(logDEBUG) << "Simulation::assimiler(const date& datePasDeTemps, EtatsBarrages& etatsBarrages)";

  if (assimilationsBarrages_.count(datePasDeTemps) > 0) {
    EtatsBarrages etatsSimules;
    auto it = assimilationsBarrages_.find(datePasDeTemps);
    const EtatsBarragesAssim& assimilations = it->second;

    for (const auto& assimilation : assimilations) {
      for (auto& etat : etatsBarrages) {
        if (etat.idCarreauPartiel == assimilation.idCarreauPartiel) {
            etatsSimules.push_back(etat); 
            
            AssimilationHelper::assimilerValeur(assimilation.volume, assimilation.volumeType, etat.volume);
            AssimilationHelper::assimilerValeur(assimilation.niveau, assimilation.niveauType, etat.niveau);
            break;
        }
      }
    }

    if (etatsSimules.size() > 0) {
      avantAssimilationsBarrages_.insert(std::make_pair(datePasDeTemps, etatsSimules));
    }

  } // if (assimilationsBarrages_.count(datePasDeTemps) > 0)

  // if (assimilationsBarrages_.count(datePasDeTemps) > 0) {
  //   EtatsBarrages etatsSimules;
  //   EtatsBarrages::iterator iterBarrages;
  //   // Toutes les assimilations Barrage pour ce pas de temps
  //   EtatsBarragesAssim assimilations = assimilationsBarrages_.find(datePasDeTemps)->second;

  //   // Parcourt des assimilations
  //   EtatsBarragesAssim::const_iterator assimilationsIter = assimilations.begin();
  //   for (; assimilationsIter != assimilations.end(); assimilationsIter++) {
  //     // Parcourt des etats
  //     for (iterBarrages = etatsBarrages.begin(); iterBarrages != etatsBarrages.end(); iterBarrages++) {
  //       // Si id egaux, on fait la mise a jour
  //       if (iterBarrages->idCarreauPartiel == assimilationsIter->idCarreauPartiel) {
  //         // Sauvegarde de l'etat avant l'assimilation
  //         etatsSimules.push_back(*iterBarrages);

  //         AssimilationHelper::assimilerValeur(assimilationsIter->volume, 
  //                         assimilationsIter->volumeType, iterBarrages->volume);

  //         AssimilationHelper::assimilerValeur(assimilationsIter->niveau, 
  //                         assimilationsIter->niveauType, iterBarrages->niveau);
          
  //         break;
  //       }
  //     } // for
  //   } // for

  //   // Conservation des etats simules pour comparaison avec etats assimiles
  //   if (etatsSimules.size() > 0) {
  //     avantAssimilationsBarrages_.insert(std::make_pair(datePasDeTemps, etatsSimules));
  //   }

  // } // if (assimilationsBarrages_.count(datePasDeTemps) > 0)

  return 0;
}

