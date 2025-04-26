

# Class CarreauEntier



[**ClassList**](annotated.md) **>** [**CarreauEntier**](classCarreauEntier.md)



_Definition d'un carreau entier._ [More...](#detailed-description)

* `#include <CarreauEntier.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CarreauEntier**](#function-carreauentier-15) (int id, int pctLacRiviere, int pctForet, int pctMarais, int pctLacExutoire) <br>_Constructeur._  |
|   | [**CarreauEntier**](#function-carreauentier-25) (int id, int i, int j) <br>_Constructeur._  |
|   | [**CarreauEntier**](#function-carreauentier-35) (int id, int i, int j, int pctLacRiviere, int pctForet, int pctMarais, int pctLacExutoire, float altitude) <br>_Constructeur._  |
|   | [**CarreauEntier**](#function-carreauentier-45) (int id, int i, int j, int pctLacRiviere, int pctForet, int pctMarais, int pctLacExutoire, float altitude, float latitude, float longitude) <br>_Constructeur._  |
|   | [**CarreauEntier**](#function-carreauentier-55) (int id, int i, int j, int pctLacRiviere, int pctForet, int pctMarais, int pctLacExutoire, float altitude, float latitude, float longitude, float meanSlope) <br>_Constructeur._  |
|  float | [**altitude**](#function-altitude-12) () const<br> |
|  void | [**altitude**](#function-altitude-22) (float val) <br>_Setter altitude._  |
|  float | [**calculerCoeffPonderation**](#function-calculercoeffponderation) () const<br>_Coefficient de ponderation._  |
|  int | [**i**](#function-i-12) () const<br>_Getter i (abscise) du carreau sur la grille._  |
|  void | [**i**](#function-i-22) (int val) <br>_Setter i._  |
|  int | [**id**](#function-id) () const<br>_Getter id._  |
|  int | [**j**](#function-j-12) () const<br>_Getter j (ordonnee) du carreau sur la grille._  |
|  void | [**j**](#function-j-22) (int val) <br>_Setter j._  |
|  float | [**latitude**](#function-latitude) () const<br>_Getter latitude._  |
|  float | [**longitude**](#function-longitude) () const<br>_Getter longitude._  |
|  float | [**meanSlope**](#function-meanslope) () const<br>_Getter meanSlope._  |
|  [**ParamCE**](classParamCE.md) & | [**param**](#function-param-12) () <br>_Getter/Setter parametres spatialisables._  |
|  const [**ParamCE**](classParamCE.md) & | [**param**](#function-param-22) () const<br>_Getter parametres spatialisables._  |
|  float | [**pctEau**](#function-pcteau) () const<br>_Obtention du poucentage d'eau sur le carreau entier._  |
|  float | [**pctForet**](#function-pctforet-12) () const<br>_Getter pctForet._  |
|  void | [**pctForet**](#function-pctforet-22) (int pctForet) <br>_Setter pctForet._  |
|  float | [**pctLacExutoire**](#function-pctlacexutoire-12) () const<br>_Getter pctLacExutoire._  |
|  void | [**pctLacExutoire**](#function-pctlacexutoire-22) (int pctLacExutoire) <br>_Setter pctLacExutoire._  |
|  float | [**pctLacRiviere**](#function-pctlacriviere-12) () const<br>_Getter pctLacRiviere._  |
|  void | [**pctLacRiviere**](#function-pctlacriviere-22) (int pctLacRiviere) <br>_Setter pctLacRiviere._  |
|  float | [**pctMarais**](#function-pctmarais-12) () const<br>_Getter pctMarais._  |
|  void | [**pctMarais**](#function-pctmarais-22) (int pctMarais) <br>_Setter pctMarais._  |
|  float | [**pctSol**](#function-pctsol) () const<br>_Obtention du pourcentage de sol sur le carreau entier._  |
|   | [**~CarreauEntier**](#function-carreauentier) () <br> |




























## Detailed Description


Definition d'un carreau entier selon les donnees physiographiques PREPAREES. 


    
## Public Functions Documentation




### function CarreauEntier [1/5]

_Constructeur._ 
```C++
CarreauEntier::CarreauEntier (
    int id,
    int pctLacRiviere,
    int pctForet,
    int pctMarais,
    int pctLacExutoire
) 
```




<hr>



### function CarreauEntier [2/5]

_Constructeur._ 
```C++
CarreauEntier::CarreauEntier (
    int id,
    int i,
    int j
) 
```




<hr>



### function CarreauEntier [3/5]

_Constructeur._ 
```C++
CarreauEntier::CarreauEntier (
    int id,
    int i,
    int j,
    int pctLacRiviere,
    int pctForet,
    int pctMarais,
    int pctLacExutoire,
    float altitude
) 
```




<hr>



### function CarreauEntier [4/5]

_Constructeur._ 
```C++
CarreauEntier::CarreauEntier (
    int id,
    int i,
    int j,
    int pctLacRiviere,
    int pctForet,
    int pctMarais,
    int pctLacExutoire,
    float altitude,
    float latitude,
    float longitude
) 
```




<hr>



### function CarreauEntier [5/5]

_Constructeur._ 
```C++
CarreauEntier::CarreauEntier (
    int id,
    int i,
    int j,
    int pctLacRiviere,
    int pctForet,
    int pctMarais,
    int pctLacExutoire,
    float altitude,
    float latitude,
    float longitude,
    float meanSlope
) 
```




<hr>



### function altitude [1/2]

```C++
float CarreauEntier::altitude () const
```




<hr>



### function altitude [2/2]

_Setter altitude._ 
```C++
void CarreauEntier::altitude (
    float val
) 
```




<hr>



### function calculerCoeffPonderation 

_Coefficient de ponderation._ 
```C++
float CarreauEntier::calculerCoeffPonderation () const
```



Coefficient de ponderation utilise dans les calculs d'evapotranspiration et d'infiltration. varie de 0.8 si le carreau entier est completement deboise, a 1.0 si la foret recouvre totalement le carreau. 


        

<hr>



### function i [1/2]

_Getter i (abscise) du carreau sur la grille._ 
```C++
int CarreauEntier::i () const
```




<hr>



### function i [2/2]

_Setter i._ 
```C++
void CarreauEntier::i (
    int val
) 
```




<hr>



### function id 

_Getter id._ 
```C++
int CarreauEntier::id () const
```




<hr>



### function j [1/2]

_Getter j (ordonnee) du carreau sur la grille._ 
```C++
int CarreauEntier::j () const
```




<hr>



### function j [2/2]

_Setter j._ 
```C++
void CarreauEntier::j (
    int val
) 
```




<hr>



### function latitude 

_Getter latitude._ 
```C++
float CarreauEntier::latitude () const
```




<hr>



### function longitude 

_Getter longitude._ 
```C++
float CarreauEntier::longitude () const
```




<hr>



### function meanSlope 

_Getter meanSlope._ 
```C++
float CarreauEntier::meanSlope () const
```




<hr>



### function param [1/2]

_Getter/Setter parametres spatialisables._ 
```C++
ParamCE & CarreauEntier::param () 
```




<hr>



### function param [2/2]

_Getter parametres spatialisables._ 
```C++
const ParamCE & CarreauEntier::param () const
```




<hr>



### function pctEau 

_Obtention du poucentage d'eau sur le carreau entier._ 
```C++
float CarreauEntier::pctEau () const
```




<hr>



### function pctForet [1/2]

_Getter pctForet._ 
```C++
float CarreauEntier::pctForet () const
```




<hr>



### function pctForet [2/2]

_Setter pctForet._ 
```C++
void CarreauEntier::pctForet (
    int pctForet
) 
```




<hr>



### function pctLacExutoire [1/2]

_Getter pctLacExutoire._ 
```C++
float CarreauEntier::pctLacExutoire () const
```




<hr>



### function pctLacExutoire [2/2]

_Setter pctLacExutoire._ 
```C++
void CarreauEntier::pctLacExutoire (
    int pctLacExutoire
) 
```




<hr>



### function pctLacRiviere [1/2]

_Getter pctLacRiviere._ 
```C++
float CarreauEntier::pctLacRiviere () const
```




<hr>



### function pctLacRiviere [2/2]

_Setter pctLacRiviere._ 
```C++
void CarreauEntier::pctLacRiviere (
    int pctLacRiviere
) 
```




<hr>



### function pctMarais [1/2]

_Getter pctMarais._ 
```C++
float CarreauEntier::pctMarais () const
```




<hr>



### function pctMarais [2/2]

_Setter pctMarais._ 
```C++
void CarreauEntier::pctMarais (
    int pctMarais
) 
```




<hr>



### function pctSol 

_Obtention du pourcentage de sol sur le carreau entier._ 
```C++
float CarreauEntier::pctSol () const
```




<hr>



### function ~CarreauEntier 

```C++
CarreauEntier::~CarreauEntier () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/CarreauEntier.h`

