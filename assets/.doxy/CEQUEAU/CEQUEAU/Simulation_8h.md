

# File Simulation.h



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**Simulation.h**](Simulation_8h.md)





* `#include "BassinVersant.h"`
* `#include "Parametres.h"`
* `#include "DonneesMeteo.h"`
* `#include "Fonte.h"`
* `#include "Evapo.h"`
* `#include "DateChrono.h"`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**ModuleEvapo**](namespaceModuleEvapo.md) <br> |
| namespace | [**ModuleFonte**](namespaceModuleFonte.md) <br> |


## Classes

| Type | Name |
| ---: | :--- |
| class | [**EtatBarrage**](classEtatBarrage.md) <br>_Classe de donnees de l'etat d'un barrage._  |
| class | [**EtatBarrageAssim**](classEtatBarrageAssim.md) <br>_Classe de donnees pour l'assimilation des etats d'un barrage._  |
| class | [**EtatCarreauEntier**](classEtatCarreauEntier.md) <br>_Classe de donnees de l'etat d'un carreau entier._  |
| class | [**Qualite**](classEtatCarreauEntier_1_1Qualite.md) <br> |
| class | [**EtatCarreauEntierAssim**](classEtatCarreauEntierAssim.md) <br>_Classe de donnees pour l'assimilation des etats d'un carreau entier._  |
| class | [**EtatCarreauPartiel**](classEtatCarreauPartiel.md) <br>_Classe de donnees de l'etat d'un carreau partiel._  |
| class | [**Qualite**](classEtatCarreauPartiel_1_1Qualite.md) <br> |
| class | [**EtatCarreauPartielAssim**](classEtatCarreauPartielAssim.md) <br>_Classe de donnees pour l'assimilation des etats d'un carreau partiel._  |
| class | [**ReleveNeige**](classReleveNeige.md) <br> |
| class | [**Simulation**](classSimulation.md) <br>_Classe simulation._  |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::vector&lt; [**EtatBarrage**](classEtatBarrage.md) &gt; | [**EtatsBarrages**](#typedef-etatsbarrages)  <br>_Liste des etats des barrages a chaque pas de temps._  |
| typedef std::vector&lt; [**EtatBarrageAssim**](classEtatBarrageAssim.md) &gt; | [**EtatsBarragesAssim**](#typedef-etatsbarragesassim)  <br>_Liste des assimilations des barrages a chaque pas de temps._  |
| typedef std::vector&lt; [**EtatCarreauEntier**](classEtatCarreauEntier.md) &gt; | [**EtatsCarreauxEntiers**](#typedef-etatscarreauxentiers)  <br>_Liste des etats des carreaux entiers a chaque pas de temps._  |
| typedef std::vector&lt; [**EtatCarreauEntierAssim**](classEtatCarreauEntierAssim.md) &gt; | [**EtatsCarreauxEntiersAssim**](#typedef-etatscarreauxentiersassim)  <br>_Liste des assimilations des carreaux entiers a chaque pas de temps._  |
| typedef std::vector&lt; [**EtatCarreauPartiel**](classEtatCarreauPartiel.md) &gt; | [**EtatsCarreauxPartiels**](#typedef-etatscarreauxpartiels)  <br>_Liste des etats des carreaux partiels a chaque pas de temps._  |
| typedef std::vector&lt; [**EtatCarreauPartielAssim**](classEtatCarreauPartielAssim.md) &gt; | [**EtatsCarreauxPartielsAssim**](#typedef-etatscarreauxpartielsassim)  <br>_Liste des assimilations des carreaux partiels a chaque pas de temps._  |
| typedef std::shared\_ptr&lt; [**Evapo**](classEvapo.md) &gt; | [**EvapoPtr**](#typedef-evapoptr)  <br> |
| typedef std::shared\_ptr&lt; [**Fonte**](classFonte.md) &gt; | [**FontePtr**](#typedef-fonteptr)  <br> |
| typedef std::vector&lt; [**ReleveNeige**](classReleveNeige.md) &gt; | [**RelevesNeige**](#typedef-relevesneige)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  bool | [**sortByDistance**](#function-sortbydistance) (const [**ReleveNeige**](classReleveNeige.md) & lhs, const [**ReleveNeige**](classReleveNeige.md) & rhs) <br> |




























## Public Types Documentation




### typedef EtatsBarrages 

_Liste des etats des barrages a chaque pas de temps._ 
```C++
typedef std::vector<EtatBarrage> EtatsBarrages;
```




<hr>



### typedef EtatsBarragesAssim 

_Liste des assimilations des barrages a chaque pas de temps._ 
```C++
typedef std::vector<EtatBarrageAssim> EtatsBarragesAssim;
```




<hr>



### typedef EtatsCarreauxEntiers 

_Liste des etats des carreaux entiers a chaque pas de temps._ 
```C++
typedef std::vector<EtatCarreauEntier> EtatsCarreauxEntiers;
```




<hr>



### typedef EtatsCarreauxEntiersAssim 

_Liste des assimilations des carreaux entiers a chaque pas de temps._ 
```C++
typedef std::vector<EtatCarreauEntierAssim> EtatsCarreauxEntiersAssim;
```




<hr>



### typedef EtatsCarreauxPartiels 

_Liste des etats des carreaux partiels a chaque pas de temps._ 
```C++
typedef std::vector<EtatCarreauPartiel> EtatsCarreauxPartiels;
```




<hr>



### typedef EtatsCarreauxPartielsAssim 

_Liste des assimilations des carreaux partiels a chaque pas de temps._ 
```C++
typedef std::vector<EtatCarreauPartielAssim> EtatsCarreauxPartielsAssim;
```




<hr>



### typedef EvapoPtr 

```C++
typedef std::shared_ptr<Evapo> EvapoPtr;
```




<hr>



### typedef FontePtr 

```C++
typedef std::shared_ptr<Fonte> FontePtr;
```




<hr>



### typedef RelevesNeige 

```C++
typedef std::vector<ReleveNeige> RelevesNeige;
```




<hr>
## Public Functions Documentation




### function sortByDistance 

```C++
bool sortByDistance (
    const ReleveNeige & lhs,
    const ReleveNeige & rhs
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/Simulation.h`

