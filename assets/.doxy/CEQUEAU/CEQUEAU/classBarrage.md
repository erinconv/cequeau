

# Class Barrage



[**ClassList**](annotated.md) **>** [**Barrage**](classBarrage.md)



_Definition d'un barrage._ 

* `#include <Barrage.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Barrage**](#function-barrage-13) (int idCP, int idCPAval, float volume, const ParamEquation & paramNiveau, const ParamEquation & paramDebit) <br>_Constructeur barrage type 2._  |
|   | [**Barrage**](#function-barrage-23) (int idCP, int idCPAval, float volume, const ParamEquation & paramNiveau, const ParamEquation & paramVolume, const std::vector&lt; ParamEquation &gt; & paramDebitInt, const std::vector&lt; [**InfoEvacuationExt**](classInfoEvacuationExt.md) &gt; & paramDebitExt) <br>_Constructeur barrage type 5._  |
|   | [**Barrage**](#function-barrage-33) (int idCPAval, const std::vector&lt; float &gt; & debitsMesures) <br>_Constructeur barrage type 3._  |
|  float | [**calculerDebitAvecVolume**](#function-calculerdebitavecvolume-13) () const<br>_Calcul du debit selon le volume du barrage._  |
|  float | [**calculerDebitAvecVolume**](#function-calculerdebitavecvolume-23) (const ParamEquation & paramDebit) const<br>_Calcul du debit selon le volume du barrage en specifiant les parametres d'equation._  |
|  float | [**calculerDebitAvecVolume**](#function-calculerdebitavecvolume-33) (float volume, const ParamEquation & paramDebit) const<br>_Calcul du debit avec un volume en specifiant les parametres d'equation._  |
|  float | [**calculerNiveauAvecVolume**](#function-calculerniveauavecvolume-12) () const<br>_Calcul de la hauteur selon le volume du barrage._  |
|  float | [**calculerNiveauAvecVolume**](#function-calculerniveauavecvolume-22) (float volume) const<br>_Calcul de la hauteur avec un volume._  |
|  float | [**calculerVolumeCpAval**](#function-calculervolumecpaval) (float volumeEntrant, float deltaTemps, float volumeInitialCpAval, const [**DateChrono**](classDateChrono.md) & datePasDeTemps, std::vector&lt; float &gt; & volumesSortants) <br>_Calcul du volume provenant du barrage sur le CP en aval._  |
|  const std::vector&lt; float &gt; & | [**debitsMesures**](#function-debitsmesures) () const<br> |
|  int | [**idCP**](#function-idcp) () const<br>_Getter idCP._  |
|  int | [**idCPAval**](#function-idcpaval) () const<br>_Getter idCPAval._  |
|  TypeBarrage | [**typeBarrage**](#function-typebarrage) () const<br>_Getter type barrage._  |
|  float | [**volume**](#function-volume-12) () const<br>_Getter volume._  |
|  void | [**volume**](#function-volume-22) (float valeur) <br>_Setter volume._  |
|   | [**~Barrage**](#function-barrage) () <br> |




























## Public Functions Documentation




### function Barrage [1/3]

_Constructeur barrage type 2._ 
```C++
Barrage::Barrage (
    int idCP,
    int idCPAval,
    float volume,
    const ParamEquation & paramNiveau,
    const ParamEquation & paramDebit
) 
```




<hr>



### function Barrage [2/3]

_Constructeur barrage type 5._ 
```C++
Barrage::Barrage (
    int idCP,
    int idCPAval,
    float volume,
    const ParamEquation & paramNiveau,
    const ParamEquation & paramVolume,
    const std::vector< ParamEquation > & paramDebitInt,
    const std::vector< InfoEvacuationExt > & paramDebitExt
) 
```




<hr>



### function Barrage [3/3]

_Constructeur barrage type 3._ 
```C++
Barrage::Barrage (
    int idCPAval,
    const std::vector< float > & debitsMesures
) 
```




<hr>



### function calculerDebitAvecVolume [1/3]

_Calcul du debit selon le volume du barrage._ 
```C++
float Barrage::calculerDebitAvecVolume () const
```




<hr>



### function calculerDebitAvecVolume [2/3]

_Calcul du debit selon le volume du barrage en specifiant les parametres d'equation._ 
```C++
float Barrage::calculerDebitAvecVolume (
    const ParamEquation & paramDebit
) const
```




<hr>



### function calculerDebitAvecVolume [3/3]

_Calcul du debit avec un volume en specifiant les parametres d'equation._ 
```C++
float Barrage::calculerDebitAvecVolume (
    float volume,
    const ParamEquation & paramDebit
) const
```




<hr>



### function calculerNiveauAvecVolume [1/2]

_Calcul de la hauteur selon le volume du barrage._ 
```C++
float Barrage::calculerNiveauAvecVolume () const
```




<hr>



### function calculerNiveauAvecVolume [2/2]

_Calcul de la hauteur avec un volume._ 
```C++
float Barrage::calculerNiveauAvecVolume (
    float volume
) const
```




<hr>



### function calculerVolumeCpAval 

_Calcul du volume provenant du barrage sur le CP en aval._ 
```C++
float Barrage::calculerVolumeCpAval (
    float volumeEntrant,
    float deltaTemps,
    float volumeInitialCpAval,
    const DateChrono & datePasDeTemps,
    std::vector< float > & volumesSortants
) 
```




<hr>



### function debitsMesures 

```C++
const std::vector< float > & Barrage::debitsMesures () const
```




<hr>



### function idCP 

_Getter idCP._ 
```C++
int Barrage::idCP () const
```




<hr>



### function idCPAval 

_Getter idCPAval._ 
```C++
int Barrage::idCPAval () const
```




<hr>



### function typeBarrage 

_Getter type barrage._ 
```C++
TypeBarrage Barrage::typeBarrage () const
```




<hr>



### function volume [1/2]

_Getter volume._ 
```C++
float Barrage::volume () const
```




<hr>



### function volume [2/2]

_Setter volume._ 
```C++
void Barrage::volume (
    float valeur
) 
```




<hr>



### function ~Barrage 

```C++
Barrage::~Barrage () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/Barrage.h`

