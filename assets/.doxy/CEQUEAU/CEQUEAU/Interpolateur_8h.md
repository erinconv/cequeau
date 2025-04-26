

# File Interpolateur.h



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**Interpolateur.h**](Interpolateur_8h.md)





* `#include "ListeStation.h"`
* `#include "BassinVersant.h"`
* `#include "ParametresInterpolation.h"`
* `#include "DonneesMeteo.h"`















## Classes

| Type | Name |
| ---: | :--- |
| class | [**DonneesInterpolation**](classDonneesInterpolation.md) <br>_Donnees necessaires pour interpoler la meteo a chaque CE._  |
| class | [**Interpolateur**](classInterpolateur.md) <br>_Classe Interpolation._  |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::vector&lt; [**DonneesInterpolation**](classDonneesInterpolation.md) &gt; | [**DonneesCarreaux**](#typedef-donneescarreaux)  <br> |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  const int | [**IDX\_PRECIPITATION**](#variable-idx_precipitation)   = `1`<br> |
|  const int | [**IDX\_TEMPERATURE**](#variable-idx_temperature)   = `0`<br> |
|  const int | [**NB\_TYPE\_STATION**](#variable-nb_type_station)   = `2`<br> |












































## Public Types Documentation




### typedef DonneesCarreaux 

```C++
typedef std::vector<DonneesInterpolation> DonneesCarreaux;
```




<hr>
## Public Attributes Documentation




### variable IDX\_PRECIPITATION 

```C++
const int IDX_PRECIPITATION;
```




<hr>



### variable IDX\_TEMPERATURE 

```C++
const int IDX_TEMPERATURE;
```




<hr>



### variable NB\_TYPE\_STATION 

```C++
const int NB_TYPE_STATION;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/Interpolateur.h`

