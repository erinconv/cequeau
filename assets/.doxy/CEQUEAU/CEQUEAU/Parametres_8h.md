

# File Parametres.h



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**Parametres.h**](Parametres_8h.md)





* `#include "DateChrono.h"`















## Classes

| Type | Name |
| ---: | :--- |
| class | [**ParamCE**](classParamCE.md) <br>[_**Parametres**_](classParametres.md) _spatialisables._ |
| class | [**ParamFacultatifs**](classParamFacultatifs.md) <br>_Classe de donnees pour les parametres facultatifs._  |
| class | [**ParamOption**](classParamOption.md) <br>_Classe de donnees pour les parametres optionnels._  |
| class | [**ParamPompage**](classParamPompage.md) <br> |
| class | [**ParamSol**](classParamSol.md) <br>_Classe de donnees des parametres relatifs reservoirs SOL, NAPPE et MARAIS._  |
| class | [**ParamSolInitial**](classParamSolInitial.md) <br>_Classe de donnees pour les valeurs initiales des reservoirs._  |
| class | [**ParamTransfert**](classParamTransfert.md) <br>_Classe de donnees pour les parametres relatifs a la fonction de transfert._  |
| class | [**Parametres**](classParametres.md) <br>_Classe des parametres de la simulation._  |
| class | [**ValeurZone**](classValeurZone.md) <br> |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::multimap&lt; [**DateChrono**](classDateChrono.md), [**ValeurZone**](classValeurZone.md) &gt;::const\_iterator | [**RelevesNeigeIter**](#typedef-relevesneigeiter)  <br> |
| enum  | [**TypeAjustementNeige**](#enum-typeajustementneige)  <br> |
















































## Public Types Documentation




### typedef RelevesNeigeIter 

```C++
typedef std::multimap<DateChrono,ValeurZone>::const_iterator RelevesNeigeIter;
```




<hr>



### enum TypeAjustementNeige 

```C++
enum TypeAjustementNeige {
    AUCUN,
    ZONE,
    STATIONS_3
};
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/Parametres.h`

