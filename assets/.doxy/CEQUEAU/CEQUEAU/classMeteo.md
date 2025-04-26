

# Class Meteo



[**ClassList**](annotated.md) **>** [**Meteo**](classMeteo.md)



_Definition d'une donnee meteo._ 

* `#include <Meteo.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Meteo**](#function-meteo-13) (float tMin, float tMax, float pluie, float neige) <br>_Constructeur._  |
|   | [**Meteo**](#function-meteo-23) (int tMin, int tMax, int pluie, int neige) <br>_Constructeur._  |
|   | [**Meteo**](#function-meteo-33) () <br>_Constructeur par defaut._  |
|  float | [**calculerTempMoy**](#function-calculertempmoy) () const<br>_Obtetion de la temperature moyenne d'une donnee meteo._  |
|  int | [**idCarreauEntier**](#function-idcarreauentier-12) () const<br>_Getter idCarreauEntier._  |
|  void | [**idCarreauEntier**](#function-idcarreauentier-22) (int idCarreauEntier) <br>_Setter idCarreauEntier._  |
|  const std::vector&lt; float &gt; & | [**meteoAutre**](#function-meteoautre-12) () const<br>_Getter donnees meteo autres (exemple: qualite)_  |
|  void | [**meteoAutre**](#function-meteoautre-22) (const std::vector&lt; float &gt; & donneesMeteo) <br>_Setter donnees meteo autres (exemple: qualite)_  |
|  const std::vector&lt; float &gt; & | [**meteoEvapo**](#function-meteoevapo-12) () const<br>_Getter donnees meteo specifique au modele d'evapotranspiration._  |
|  void | [**meteoEvapo**](#function-meteoevapo-22) (const std::vector&lt; float &gt; & donneesMeteo) <br>_Setter donnees meteo specifique au modele d'evapotranspiration._  |
|  const std::vector&lt; float &gt; & | [**meteoFonte**](#function-meteofonte-12) () const<br>_Getter donnees meteo specifique au modele de fonte._  |
|  void | [**meteoFonte**](#function-meteofonte-22) (const std::vector&lt; float &gt; & donneesMeteo) <br>_Setter donnees meteo specifique au modele de fonte._  |
|  float | [**neige**](#function-neige) () const<br>_Getter neige._  |
|  float | [**pluie**](#function-pluie) () const<br>_Getter pluie._  |
|  float | [**tMax**](#function-tmax) () const<br>_Getter tMax._  |
|  float | [**tMin**](#function-tmin) () const<br>_Getter tMin._  |
|   | [**~Meteo**](#function-meteo) () <br> |




























## Public Functions Documentation




### function Meteo [1/3]

_Constructeur._ 
```C++
Meteo::Meteo (
    float tMin,
    float tMax,
    float pluie,
    float neige
) 
```





**Parameters:**


* `tMin` Temperature manimale journaliere de l'air en degC 
* `tMax` Temperature maximale journaliere de l'air en degC 
* `pluie` Precipitation liquide en mm 
* `neige` Precipitation solide en mm d'equivalent eau 




        

<hr>



### function Meteo [2/3]

_Constructeur._ 
```C++
Meteo::Meteo (
    int tMin,
    int tMax,
    int pluie,
    int neige
) 
```





**Parameters:**


* `tMin` Temperature manimale journaliere de l'air en dixieme de degC 
* `tMax` Temperature maximale journaliere de l'air en dixieme de degC 
* `pluie` Precipitation liquide en dixieme de mm 
* `neige` Precipitation solide en dixieme de mm d'equivalent eau 




        

<hr>



### function Meteo [3/3]

_Constructeur par defaut._ 
```C++
Meteo::Meteo () 
```




<hr>



### function calculerTempMoy 

_Obtetion de la temperature moyenne d'une donnee meteo._ 
```C++
float Meteo::calculerTempMoy () const
```




<hr>



### function idCarreauEntier [1/2]

_Getter idCarreauEntier._ 
```C++
int Meteo::idCarreauEntier () const
```




<hr>



### function idCarreauEntier [2/2]

_Setter idCarreauEntier._ 
```C++
void Meteo::idCarreauEntier (
    int idCarreauEntier
) 
```




<hr>



### function meteoAutre [1/2]

_Getter donnees meteo autres (exemple: qualite)_ 
```C++
const std::vector< float > & Meteo::meteoAutre () const
```




<hr>



### function meteoAutre [2/2]

_Setter donnees meteo autres (exemple: qualite)_ 
```C++
void Meteo::meteoAutre (
    const std::vector< float > & donneesMeteo
) 
```




<hr>



### function meteoEvapo [1/2]

_Getter donnees meteo specifique au modele d'evapotranspiration._ 
```C++
const std::vector< float > & Meteo::meteoEvapo () const
```




<hr>



### function meteoEvapo [2/2]

_Setter donnees meteo specifique au modele d'evapotranspiration._ 
```C++
void Meteo::meteoEvapo (
    const std::vector< float > & donneesMeteo
) 
```




<hr>



### function meteoFonte [1/2]

_Getter donnees meteo specifique au modele de fonte._ 
```C++
const std::vector< float > & Meteo::meteoFonte () const
```




<hr>



### function meteoFonte [2/2]

_Setter donnees meteo specifique au modele de fonte._ 
```C++
void Meteo::meteoFonte (
    const std::vector< float > & donneesMeteo
) 
```




<hr>



### function neige 

_Getter neige._ 
```C++
float Meteo::neige () const
```




<hr>



### function pluie 

_Getter pluie._ 
```C++
float Meteo::pluie () const
```




<hr>



### function tMax 

_Getter tMax._ 
```C++
float Meteo::tMax () const
```




<hr>



### function tMin 

_Getter tMin._ 
```C++
float Meteo::tMin () const
```




<hr>



### function ~Meteo 

```C++
Meteo::~Meteo () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/Meteo.h`

