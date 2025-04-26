

# Class SimulationQualite



[**ClassList**](annotated.md) **>** [**SimulationQualite**](classSimulationQualite.md)



_Classe_ [_**SimulationQualite**_](classSimulationQualite.md) _._

* `#include <SimulationQualite.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**SimulationQualite**](#function-simulationqualite) ([**Simulation**](classSimulation.md) & simulation, [**ParametresQualite**](classParametresQualite.md) & param, [**DLI**](classDLI.md) & dli, const mxArray \* etatsPrecedents, const mxArray \* assimilations=NULL) <br>_Construteur._  |
|  int | [**executer**](#function-executer) (bool estMexFile=false) <br>_Execution de la simulation._  |
|  mxArray \* | [**obtenirEtatsAvantAssimilations**](#function-obteniretatsavantassimilations) () <br> |
|  mxArray \* | [**obtenirEtatsCP**](#function-obteniretatscp-12) () <br>_Etats CP Qualite en mxArray._  |
|   | [**~SimulationQualite**](#function-simulationqualite) () <br> |




























## Public Functions Documentation




### function SimulationQualite 

_Construteur._ 
```C++
SimulationQualite::SimulationQualite (
    Simulation & simulation,
    ParametresQualite & param,
    DLI & dli,
    const mxArray * etatsPrecedents,
    const mxArray * assimilations=NULL
) 
```




<hr>



### function executer 

_Execution de la simulation._ 
```C++
int SimulationQualite::executer (
    bool estMexFile=false
) 
```




<hr>



### function obtenirEtatsAvantAssimilations 

```C++
mxArray * SimulationQualite::obtenirEtatsAvantAssimilations () 
```




<hr>



### function obtenirEtatsCP [1/2]

_Etats CP Qualite en mxArray._ 
```C++
mxArray * SimulationQualite::obtenirEtatsCP () 
```




<hr>



### function ~SimulationQualite 

```C++
SimulationQualite::~SimulationQualite () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/SimulationQualite.h`

