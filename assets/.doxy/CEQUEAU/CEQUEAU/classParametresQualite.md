

# Class ParametresQualite



[**ClassList**](annotated.md) **>** [**ParametresQualite**](classParametresQualite.md)



_Classe des parametres de la simulation._ 

* `#include <ParametresQualite.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ParametresQualite**](#function-parametresqualite) () <br>_Constructeur._  |
|  float | [**colarg**](#function-colarg) () const<br>_Getter._  |
|  float | [**coprom**](#function-coprom) () const<br>_Getter._  |
|  void | [**initialiser**](#function-initialiser-13) () <br>_Initialisation avec valeurs fixes pour fin de tests._  |
|  void | [**initialiser**](#function-initialiser-23) (const mxArray \* paramQualite) <br>_Initialisation._  |
|  void | [**initialiser**](#function-initialiser-33) (const mxArray \* paramQualite, const mxArray \* paramDLI) <br> |
|  void | [**initialiserOmbrage**](#function-initialiserombrage) (const mxArray \* paramQualite) <br> |
|  const [**ParamTemperature**](classParamTemperature.md) & | [**temperature**](#function-temperature) () const<br>_Getter._  |
|   | [**~ParametresQualite**](#function-parametresqualite) () <br>_Destructeur._  |




























## Public Functions Documentation




### function ParametresQualite 

_Constructeur._ 
```C++
ParametresQualite::ParametresQualite () 
```




<hr>



### function colarg 

_Getter._ 
```C++
float ParametresQualite::colarg () const
```




<hr>



### function coprom 

_Getter._ 
```C++
float ParametresQualite::coprom () const
```




<hr>



### function initialiser [1/3]

_Initialisation avec valeurs fixes pour fin de tests._ 
```C++
void ParametresQualite::initialiser () 
```




<hr>



### function initialiser [2/3]

_Initialisation._ 
```C++
void ParametresQualite::initialiser (
    const mxArray * paramQualite
) 
```




<hr>



### function initialiser [3/3]

```C++
void ParametresQualite::initialiser (
    const mxArray * paramQualite,
    const mxArray * paramDLI
) 
```




<hr>



### function initialiserOmbrage 

```C++
void ParametresQualite::initialiserOmbrage (
    const mxArray * paramQualite
) 
```




<hr>



### function temperature 

_Getter._ 
```C++
const ParamTemperature & ParametresQualite::temperature () const
```




<hr>



### function ~ParametresQualite 

_Destructeur._ 
```C++
ParametresQualite::~ParametresQualite () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/ParametresQualite.h`

