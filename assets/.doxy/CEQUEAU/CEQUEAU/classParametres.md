

# Class Parametres



[**ClassList**](annotated.md) **>** [**Parametres**](classParametres.md)



_Classe des parametres de la simulation._ 

* `#include <Parametres.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Parametres**](#function-parametres-12) () <br> |
|   | [**Parametres**](#function-parametres-22) ([**DateChrono**](classDateChrono.md) dateDebut, [**DateChrono**](classDateChrono.md) dateFin) <br> |
|  const [**DateChrono**](classDateChrono.md) & | [**dateDebut**](#function-datedebut) () const<br>_Getter Date de debut de la simulation._  |
|  const [**DateChrono**](classDateChrono.md) & | [**dateFin**](#function-datefin) () const<br>_Getter Date de fin de la simulation._  |
|  int | [**dureeHeuresPasSimulation**](#function-dureeheurespassimulation) () const<br>_Getter Duree en heure d'un pas de simulation: 1,2,3,4,6,8,12,24 heures._  |
|  const [**ParamFacultatifs**](classParamFacultatifs.md) & | [**facultatifs**](#function-facultatifs) () const<br>_Getter param facultatifs._  |
|  void | [**initialiser**](#function-initialiser) (const mxArray \* paramExec, const mxArray \* paramSimul, int nbCE, int nbCP) <br>_Initialisation des parametres a partir de variables Matlab (mxArray)_  |
|  void | [**initialiserFichier**](#function-initialiserfichier) (std::string nomFichierParamExec, std::string nomFichierParamSimul, int nbCE, int nbCP) <br>_Initialisation des parametres a partir de fichiers d'intrants Matlab._  |
|  int | [**nbJoursSimulation**](#function-nbjourssimulation) () const<br>_Getter nombre jours de simulation._  |
|  int | [**nbPasParJourSimulation**](#function-nbpasparjoursimulation) () const<br>_Getter nombre pas pas jour de simulation._  |
|  int | [**nbPasSimulation**](#function-nbpassimulation) () const<br>_Getter nombre pas de simulation._  |
|  const [**ParamOption**](classParamOption.md) & | [**option**](#function-option) () const<br>_Getter param option._  |
|  const [**ParamPompage**](classParamPompage.md) & | [**pompage**](#function-pompage) () const<br>_Getter param pompage._  |
|  const std::vector&lt; bool &gt; & | [**resultatsIdCE**](#function-resultatsidce) () const<br>_Getter selection des carreaux entiers qu'on desire en sortie._  |
|  const std::vector&lt; bool &gt; & | [**resultatsIdCP**](#function-resultatsidcp) () const<br>_Getter selection des carreaux partiels qu'on desire en sortie._  |
|  const [**ParamSol**](classParamSol.md) & | [**sol**](#function-sol) () const<br>_Getter param sol._  |
|  const [**ParamSolInitial**](classParamSolInitial.md) & | [**solInitial**](#function-solinitial) () const<br>_Getter param solInitial._  |
|  const [**ParamTransfert**](classParamTransfert.md) & | [**transfert**](#function-transfert) () const<br>_Getter param transfert._  |
|   | [**~Parametres**](#function-parametres) () <br> |




























## Public Functions Documentation




### function Parametres [1/2]

```C++
Parametres::Parametres () 
```




<hr>



### function Parametres [2/2]

```C++
Parametres::Parametres (
    DateChrono dateDebut,
    DateChrono dateFin
) 
```




<hr>



### function dateDebut 

_Getter Date de debut de la simulation._ 
```C++
const DateChrono & Parametres::dateDebut () const
```




<hr>



### function dateFin 

_Getter Date de fin de la simulation._ 
```C++
const DateChrono & Parametres::dateFin () const
```




<hr>



### function dureeHeuresPasSimulation 

_Getter Duree en heure d'un pas de simulation: 1,2,3,4,6,8,12,24 heures._ 
```C++
int Parametres::dureeHeuresPasSimulation () const
```




<hr>



### function facultatifs 

_Getter param facultatifs._ 
```C++
const ParamFacultatifs & Parametres::facultatifs () const
```




<hr>



### function initialiser 

_Initialisation des parametres a partir de variables Matlab (mxArray)_ 
```C++
void Parametres::initialiser (
    const mxArray * paramExec,
    const mxArray * paramSimul,
    int nbCE,
    int nbCP
) 
```




<hr>



### function initialiserFichier 

_Initialisation des parametres a partir de fichiers d'intrants Matlab._ 
```C++
void Parametres::initialiserFichier (
    std::string nomFichierParamExec,
    std::string nomFichierParamSimul,
    int nbCE,
    int nbCP
) 
```




<hr>



### function nbJoursSimulation 

_Getter nombre jours de simulation._ 
```C++
int Parametres::nbJoursSimulation () const
```




<hr>



### function nbPasParJourSimulation 

_Getter nombre pas pas jour de simulation._ 
```C++
int Parametres::nbPasParJourSimulation () const
```




<hr>



### function nbPasSimulation 

_Getter nombre pas de simulation._ 
```C++
int Parametres::nbPasSimulation () const
```




<hr>



### function option 

_Getter param option._ 
```C++
const ParamOption & Parametres::option () const
```




<hr>



### function pompage 

_Getter param pompage._ 
```C++
const ParamPompage & Parametres::pompage () const
```




<hr>



### function resultatsIdCE 

_Getter selection des carreaux entiers qu'on desire en sortie._ 
```C++
const std::vector< bool > & Parametres::resultatsIdCE () const
```




<hr>



### function resultatsIdCP 

_Getter selection des carreaux partiels qu'on desire en sortie._ 
```C++
const std::vector< bool > & Parametres::resultatsIdCP () const
```




<hr>



### function sol 

_Getter param sol._ 
```C++
const ParamSol & Parametres::sol () const
```




<hr>



### function solInitial 

_Getter param solInitial._ 
```C++
const ParamSolInitial & Parametres::solInitial () const
```




<hr>



### function transfert 

_Getter param transfert._ 
```C++
const ParamTransfert & Parametres::transfert () const
```




<hr>



### function ~Parametres 

```C++
Parametres::~Parametres () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/Parametres.h`

