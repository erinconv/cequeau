

# Class Simulation



[**ClassList**](annotated.md) **>** [**Simulation**](classSimulation.md)



_Classe simulation._ [More...](#detailed-description)

* `#include <Simulation.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Simulation**](#function-simulation) ([**BassinVersant**](classBassinVersant.md) & bv, const [**Parametres**](classParametres.md) & param, const [**DonneesMeteo**](classDonneesMeteo.md) & meteo, const mxArray \* etatsPrecedents, const std::string & fichierResultats, FontePtr moduleFonte, EvapoPtr moduleEvapo, const mxArray \* assimilations=NULL) <br>_Construteur._  |
|  int | [**executer**](#function-executer) (bool estMexFile=false) <br>_Execution de la simulation._  |
|  bool | [**hasNeigeAjustee**](#function-hasneigeajustee) () const<br> |
|  mxArray \* | [**obtenirEtatsAvantAssimilations**](#function-obteniretatsavantassimilations) () <br>_Pas de temps en mxArray._  |
|  mxArray \* | [**obtenirEtatsBarrage**](#function-obteniretatsbarrage-12) () <br>_Etats barrages en mxArray._  |
|  mxArray \* | [**obtenirEtatsCE**](#function-obteniretatsce-12) () <br>_Etats CE en mxArray._  |
|  mxArray \* | [**obtenirEtatsCP**](#function-obteniretatscp-12) () <br>_Etats CP en mxArray._  |
|  mxArray \* | [**obtenirPasDeTemps**](#function-obtenirpasdetemps) () <br>_Pas de temps en mxArray._  |
|   | [**~Simulation**](#function-simulation) () <br> |




























## Detailed Description


Classe principale de l'application. 


    
## Public Functions Documentation




### function Simulation 

_Construteur._ 
```C++
Simulation::Simulation (
    BassinVersant & bv,
    const Parametres & param,
    const DonneesMeteo & meteo,
    const mxArray * etatsPrecedents,
    const std::string & fichierResultats,
    FontePtr moduleFonte,
    EvapoPtr moduleEvapo,
    const mxArray * assimilations=NULL
) 
```





**Parameters:**


* `bv` Reference sur un objet [**BassinVersant**](classBassinVersant.md). 
* `param` Reference sur un objet [**Parametres**](classParametres.md). 
* `meteo` Reference sur un objet [**DonneesMeteo**](classDonneesMeteo.md). 
* `fichierEtats` Repertoire et nom du fichier Matlab des etats precedents. 
* `fichierResultats` Repertoire et nom du fichier Matlab des resultats. 




        

<hr>



### function executer 

_Execution de la simulation._ 
```C++
int Simulation::executer (
    bool estMexFile=false
) 
```




<hr>



### function hasNeigeAjustee 

```C++
bool Simulation::hasNeigeAjustee () const
```




<hr>



### function obtenirEtatsAvantAssimilations 

_Pas de temps en mxArray._ 
```C++
mxArray * Simulation::obtenirEtatsAvantAssimilations () 
```




<hr>



### function obtenirEtatsBarrage [1/2]

_Etats barrages en mxArray._ 
```C++
mxArray * Simulation::obtenirEtatsBarrage () 
```




<hr>



### function obtenirEtatsCE [1/2]

_Etats CE en mxArray._ 
```C++
mxArray * Simulation::obtenirEtatsCE () 
```




<hr>



### function obtenirEtatsCP [1/2]

_Etats CP en mxArray._ 
```C++
mxArray * Simulation::obtenirEtatsCP () 
```




<hr>



### function obtenirPasDeTemps 

_Pas de temps en mxArray._ 
```C++
mxArray * Simulation::obtenirPasDeTemps () 
```




<hr>



### function ~Simulation 

```C++
Simulation::~Simulation () 
```




<hr>## Friends Documentation





### friend SimulationQualite 

```C++
class Simulation::SimulationQualite (
    SimulationQualite
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/Simulation.h`

