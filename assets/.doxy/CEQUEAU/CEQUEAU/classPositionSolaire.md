

# Class PositionSolaire



[**ClassList**](annotated.md) **>** [**PositionSolaire**](classPositionSolaire.md)



_Class pour calcul des positions solaires._ 

* `#include <SimulationQualite.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PositionSolaire**](#function-positionsolaire-66) () <br> |
|  void | [**calcul\_altitude\_et\_azimut\_du\_soleil**](#function-calcul_altitude_et_azimut_du_soleil-12) (double jd, double lat, double lon, double \* azimut, double \* altitude, double elevation=0) <br> |
|  void | [**calcul\_altitude\_et\_azimut\_du\_soleil**](#function-calcul_altitude_et_azimut_du_soleil-22) ([**Qualite::spa\_data**](structQualite_1_1spa__data.md) & spa) <br> |
|   | [**~PositionSolaire**](#function-positionsolaire) () <br> |




























## Public Functions Documentation




### function PositionSolaire [6/6]

```C++
PositionSolaire::PositionSolaire () 
```




<hr>



### function calcul\_altitude\_et\_azimut\_du\_soleil [1/2]

```C++
void PositionSolaire::calcul_altitude_et_azimut_du_soleil (
    double jd,
    double lat,
    double lon,
    double * azimut,
    double * altitude,
    double elevation=0
) 
```




<hr>



### function calcul\_altitude\_et\_azimut\_du\_soleil [2/2]

```C++
void PositionSolaire::calcul_altitude_et_azimut_du_soleil (
    Qualite::spa_data & spa
) 
```




<hr>



### function ~PositionSolaire 

```C++
PositionSolaire::~PositionSolaire () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/SimulationQualite.h`

