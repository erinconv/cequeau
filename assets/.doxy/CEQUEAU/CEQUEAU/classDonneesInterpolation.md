

# Class DonneesInterpolation



[**ClassList**](annotated.md) **>** [**DonneesInterpolation**](classDonneesInterpolation.md)



_Donnees necessaires pour interpoler la meteo a chaque CE._ 

* `#include <Interpolateur.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  CarreauEntierPtr | [**CE**](#variable-ce)  <br>_Un carreau entier._  |
|  float | [**correction**](#variable-correction)  <br>_Valeur calculee selon la methode d'interpolation._  |
|  std::vector&lt; float &gt; | [**distancesCE**](#variable-distancesce)  <br>_Distance des stations par rapport au CE._  |
|  std::vector&lt; float &gt; | [**facteurPonderationCE**](#variable-facteurponderationce)  <br>_Facteur de ponderation des stations par rapport au CE._  |
|  Stations | [**stations**](#variable-stations)  <br>_Stations attribuees au CE selon la methode d'interpolation._  |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  float | [**calculerAltitudeMoyStation**](#function-calculeraltitudemoystation) () <br> |
|  float | [**calculerSommePrecipitaionStation**](#function-calculersommeprecipitaionstation) () <br> |




























## Public Attributes Documentation




### variable CE 

_Un carreau entier._ 
```C++
CarreauEntierPtr DonneesInterpolation::CE;
```




<hr>



### variable correction 

_Valeur calculee selon la methode d'interpolation._ 
```C++
float DonneesInterpolation::correction;
```




<hr>



### variable distancesCE 

_Distance des stations par rapport au CE._ 
```C++
std::vector<float> DonneesInterpolation::distancesCE;
```




<hr>



### variable facteurPonderationCE 

_Facteur de ponderation des stations par rapport au CE._ 
```C++
std::vector<float> DonneesInterpolation::facteurPonderationCE;
```




<hr>



### variable stations 

_Stations attribuees au CE selon la methode d'interpolation._ 
```C++
Stations DonneesInterpolation::stations;
```




<hr>
## Public Functions Documentation




### function calculerAltitudeMoyStation 

```C++
float DonneesInterpolation::calculerAltitudeMoyStation () 
```




<hr>



### function calculerSommePrecipitaionStation 

```C++
float DonneesInterpolation::calculerSommePrecipitaionStation () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/Interpolateur.h`

