

# File DonneesMeteo.h



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**DonneesMeteo.h**](DonneesMeteo_8h.md)





* `#include "Meteo.h"`
* `#include "BassinVersant.h"`















## Classes

| Type | Name |
| ---: | :--- |
| class | [**DonneesMeteo**](classDonneesMeteo.md) <br>_Ensemble des donnees meteo._  |
| class | [**TrieurCEGrille**](classTrieurCEGrille.md) <br>_Classe utilitaire pour le tri des carreaux entiers._  |
| class | [**TrieurMeteoId**](classTrieurMeteoId.md) <br>_Classe utilitaire pour le tri des donnees meteo._  |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::vector&lt; MeteoPtr &gt; | [**MeteoGrille**](#typedef-meteogrille)  <br>_Liste des donnees meteo._  |
| typedef std::shared\_ptr&lt; [**Meteo**](classMeteo.md) &gt; | [**MeteoPtr**](#typedef-meteoptr)  <br>_"Smart pointer" sur une donnee meteo._  |
















































## Public Types Documentation




### typedef MeteoGrille 

_Liste des donnees meteo._ 
```C++
typedef std::vector<MeteoPtr> MeteoGrille;
```




<hr>



### typedef MeteoPtr 

_"Smart pointer" sur une donnee meteo._ 
```C++
typedef std::shared_ptr<Meteo> MeteoPtr;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/DonneesMeteo.h`

