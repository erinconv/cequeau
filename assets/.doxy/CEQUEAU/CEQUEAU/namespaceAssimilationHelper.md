

# Namespace AssimilationHelper



[**Namespace List**](namespaces.md) **>** [**AssimilationHelper**](namespaceAssimilationHelper.md)



_Fonctions utilitaires pour l'assimilation._ 






































## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**assimilerValeur**](#function-assimilervaleur) (const float valeurAssim, TypeAssim typeAssim, float & valeur) <br>_Remplacement d'une valeur par une donnee d'assimilation._  |
|  TypeAssim | [**obtenirTypeAssim**](#function-obtenirtypeassim) (const mxArray \* varAssimilation) <br>_Trouver le type d'assimilation pour une variable d'assimilation._  |
|  void | [**obtenirValeursAssim**](#function-obtenirvaleursassim) (const double \* valeursAssim, TypeAssim typeAssim, size\_t indexCol, float valeurs) <br>_Trouver les valeurs d'assimilation pour une variable d'assimilation._  |




























## Public Functions Documentation




### function assimilerValeur 

_Remplacement d'une valeur par une donnee d'assimilation._ 
```C++
inline void AssimilationHelper::assimilerValeur (
    const float valeurAssim,
    TypeAssim typeAssim,
    float & valeur
) 
```




<hr>



### function obtenirTypeAssim 

_Trouver le type d'assimilation pour une variable d'assimilation._ 
```C++
inline TypeAssim AssimilationHelper::obtenirTypeAssim (
    const mxArray * varAssimilation
) 
```




<hr>



### function obtenirValeursAssim 

_Trouver les valeurs d'assimilation pour une variable d'assimilation._ 
```C++
inline void AssimilationHelper::obtenirValeursAssim (
    const double * valeursAssim,
    TypeAssim typeAssim,
    size_t indexCol,
    float valeurs
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/AssimilationHelper.hpp`

