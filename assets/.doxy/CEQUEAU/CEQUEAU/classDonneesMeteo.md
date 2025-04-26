

# Class DonneesMeteo



[**ClassList**](annotated.md) **>** [**DonneesMeteo**](classDonneesMeteo.md)



_Ensemble des donnees meteo._ [More...](#detailed-description)

* `#include <DonneesMeteo.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**DonneesMeteo**](#function-donneesmeteo-12) () <br>_Constructeur._  |
|   | [**DonneesMeteo**](#function-donneesmeteo-22) (int nbCarreauxEntiers, int nbPasDeTemps) <br> |
|  bool | [**estPtot**](#function-estptot) () const<br> |
|  void | [**initialiser**](#function-initialiser-13) (const mxArray \* meteo) <br>_Initialisation des donnes meteo a partir d'un mxArray._  |
|  void | [**initialiser**](#function-initialiser-23) (const mxArray \* meteo, const std::vector&lt; std::string &gt; & champsFonte, const std::vector&lt; std::string &gt; & champsEvapo, const std::vector&lt; std::string &gt; & champsAutre) <br>_Initialisation des donnes meteo a partir d'un mxArray._  |
|  void | [**initialiserFichier**](#function-initialiserfichier) (const std::string fichierEntree) <br>_Initialisation des donnes meteo a partir d'un fichier Matlab._  |
|  int | [**nbCarreauxEntiers**](#function-nbcarreauxentiers) () const<br> |
|  int | [**nbPasDeTemps**](#function-nbpasdetemps) () const<br> |
|  void | [**trierParId**](#function-trierparid) (const std::vector&lt; CarreauEntierPtr &gt; & carreauxEntiers) <br>_Tri des donnees meteo selon les id des carreaux entiers._  |
|  const std::vector&lt; MeteoGrille &gt; & | [**valeurs**](#function-valeurs) () const<br>_Getter des donnees meteo._  |
|   | [**~DonneesMeteo**](#function-donneesmeteo) () <br> |




























## Detailed Description


Donnees meteo grille pour chaque pas de temps de la simulation. 


    
## Public Functions Documentation




### function DonneesMeteo [1/2]

_Constructeur._ 
```C++
DonneesMeteo::DonneesMeteo () 
```




<hr>



### function DonneesMeteo [2/2]

```C++
DonneesMeteo::DonneesMeteo (
    int nbCarreauxEntiers,
    int nbPasDeTemps
) 
```




<hr>



### function estPtot 

```C++
bool DonneesMeteo::estPtot () const
```




<hr>



### function initialiser [1/3]

_Initialisation des donnes meteo a partir d'un mxArray._ 
```C++
void DonneesMeteo::initialiser (
    const mxArray * meteo
) 
```




<hr>



### function initialiser [2/3]

_Initialisation des donnes meteo a partir d'un mxArray._ 
```C++
void DonneesMeteo::initialiser (
    const mxArray * meteo,
    const std::vector< std::string > & champsFonte,
    const std::vector< std::string > & champsEvapo,
    const std::vector< std::string > & champsAutre
) 
```




<hr>



### function initialiserFichier 

_Initialisation des donnes meteo a partir d'un fichier Matlab._ 
```C++
void DonneesMeteo::initialiserFichier (
    const std::string fichierEntree
) 
```




<hr>



### function nbCarreauxEntiers 

```C++
int DonneesMeteo::nbCarreauxEntiers () const
```




<hr>



### function nbPasDeTemps 

```C++
int DonneesMeteo::nbPasDeTemps () const
```




<hr>



### function trierParId 

_Tri des donnees meteo selon les id des carreaux entiers._ 
```C++
void DonneesMeteo::trierParId (
    const std::vector< CarreauEntierPtr > & carreauxEntiers
) 
```




<hr>



### function valeurs 

_Getter des donnees meteo._ 
```C++
const std::vector< MeteoGrille > & DonneesMeteo::valeurs () const
```



Retourne une reference sur un vecteur de vecteur de meteo. Pour chacun des jours on a un vecteur de meteo de chaque carreau entier. 


        

<hr>



### function ~DonneesMeteo 

```C++
DonneesMeteo::~DonneesMeteo () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/DonneesMeteo.h`

