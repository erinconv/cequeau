

# Class Interpolateur



[**ClassList**](annotated.md) **>** [**Interpolateur**](classInterpolateur.md)



_Classe Interpolation._ 

* `#include <Interpolateur.h>`





Inherited by the following classes: [InterpolateurPolygoneThiessen](classInterpolateurPolygoneThiessen.md),  [InterpolateurPonderationStations](classInterpolateurPonderationStations.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Interpolateur**](#function-interpolateur) (const [**BassinVersant**](classBassinVersant.md) & bv, const [**ListeStation**](classListeStation.md) & stations, const [**Parametres**](classParametres.md) & parametres, const [**ParametresInterpolation**](classParametresInterpolation.md) & paramInterpolation, const [**DonneesMeteo**](classDonneesMeteo.md) & meteoStations, const std::vector&lt; std::string &gt; & nomsAutresMeteo, int nbStation) <br>_Constructeur._  |
| virtual void | [**executer**](#function-executer) () = 0<br> |
|  mxArray \* | [**obtenirMeteoInterpolee**](#function-obtenirmeteointerpolee) () <br> |
| virtual  | [**~Interpolateur**](#function-interpolateur) () <br> |








## Protected Attributes

| Type | Name |
| ---: | :--- |
|  const [**BassinVersant**](classBassinVersant.md) & | [**bassinVersant\_**](#variable-bassinversant_)  <br>_Definition du bassin versant._  |
|  std::vector&lt; std::vector&lt; DonneesCarreaux &gt; &gt; | [**donneesCarreauxTemps\_**](#variable-donneescarreauxtemps_)  <br>_Liste des donnees necessaires pour interpoler la meteo a chaque CE et pas de temps._  |
|  std::vector&lt; MeteoGrille &gt; | [**meteoInterpolee\_**](#variable-meteointerpolee_)  <br>_Donnees meteo interpolees._  |
|  const [**DonneesMeteo**](classDonneesMeteo.md) & | [**meteoStations\_**](#variable-meteostations_)  <br>_Donnees meteo aux stations._  |
|  const int | [**nbStation\_**](#variable-nbstation_)  <br> |
|  const std::vector&lt; std::string &gt; & | [**nomsAutresMeteo\_**](#variable-nomsautresmeteo_)  <br>_Noms des champs meteo supplementaires._  |
|  const [**ParametresInterpolation**](classParametresInterpolation.md) & | [**paramInterpolation\_**](#variable-paraminterpolation_)  <br>[_**Parametres**_](classParametres.md) _de l'interpolation._ |
|  const [**Parametres**](classParametres.md) & | [**parametres\_**](#variable-parametres_)  <br>[_**Parametres**_](classParametres.md) _de Cequeau Quantite._ |
|  const [**ListeStation**](classListeStation.md) & | [**stations\_**](#variable-stations_)  <br>_Liste des stations meteo._  |
















## Protected Functions

| Type | Name |
| ---: | :--- |
| virtual void | [**calculerCorrections**](#function-calculercorrections) (int idxTypeMeteo, int pasDeTemps) = 0<br> |
|  void | [**initialiserListeCE**](#function-initialiserlistece) () <br> |
| virtual void | [**interpoler**](#function-interpoler) (int pasDeTemps) = 0<br> |
|  int | [**obtenirNombreAutreMeteo**](#function-obtenirnombreautremeteo) () <br> |
|  void | [**trouverStationPlusPres**](#function-trouverstationpluspres) (int idxTypeMeteo, int pasDeTemps) <br> |




## Public Functions Documentation




### function Interpolateur 

_Constructeur._ 
```C++
Interpolateur::Interpolateur (
    const BassinVersant & bv,
    const ListeStation & stations,
    const Parametres & parametres,
    const ParametresInterpolation & paramInterpolation,
    const DonneesMeteo & meteoStations,
    const std::vector< std::string > & nomsAutresMeteo,
    int nbStation
) 
```




<hr>



### function executer 

```C++
virtual void Interpolateur::executer () = 0
```




<hr>



### function obtenirMeteoInterpolee 

```C++
mxArray * Interpolateur::obtenirMeteoInterpolee () 
```




<hr>



### function ~Interpolateur 

```C++
virtual Interpolateur::~Interpolateur () 
```




<hr>
## Protected Attributes Documentation




### variable bassinVersant\_ 

_Definition du bassin versant._ 
```C++
const BassinVersant& Interpolateur::bassinVersant_;
```




<hr>



### variable donneesCarreauxTemps\_ 

_Liste des donnees necessaires pour interpoler la meteo a chaque CE et pas de temps._ 
```C++
std::vector<std::vector<DonneesCarreaux> > Interpolateur::donneesCarreauxTemps_;
```




<hr>



### variable meteoInterpolee\_ 

_Donnees meteo interpolees._ 
```C++
std::vector<MeteoGrille> Interpolateur::meteoInterpolee_;
```




<hr>



### variable meteoStations\_ 

_Donnees meteo aux stations._ 
```C++
const DonneesMeteo& Interpolateur::meteoStations_;
```




<hr>



### variable nbStation\_ 

```C++
const int Interpolateur::nbStation_;
```




<hr>



### variable nomsAutresMeteo\_ 

_Noms des champs meteo supplementaires._ 
```C++
const std::vector<std::string>& Interpolateur::nomsAutresMeteo_;
```




<hr>



### variable paramInterpolation\_ 

[_**Parametres**_](classParametres.md) _de l'interpolation._
```C++
const ParametresInterpolation& Interpolateur::paramInterpolation_;
```




<hr>



### variable parametres\_ 

[_**Parametres**_](classParametres.md) _de Cequeau Quantite._
```C++
const Parametres& Interpolateur::parametres_;
```




<hr>



### variable stations\_ 

_Liste des stations meteo._ 
```C++
const ListeStation& Interpolateur::stations_;
```




<hr>
## Protected Functions Documentation




### function calculerCorrections 

```C++
virtual void Interpolateur::calculerCorrections (
    int idxTypeMeteo,
    int pasDeTemps
) = 0
```




<hr>



### function initialiserListeCE 

```C++
void Interpolateur::initialiserListeCE () 
```




<hr>



### function interpoler 

```C++
virtual void Interpolateur::interpoler (
    int pasDeTemps
) = 0
```




<hr>



### function obtenirNombreAutreMeteo 

```C++
int Interpolateur::obtenirNombreAutreMeteo () 
```




<hr>



### function trouverStationPlusPres 

```C++
void Interpolateur::trouverStationPlusPres (
    int idxTypeMeteo,
    int pasDeTemps
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/Interpolateur.h`

