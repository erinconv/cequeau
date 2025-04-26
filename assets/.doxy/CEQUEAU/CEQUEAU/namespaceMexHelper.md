

# Namespace MexHelper



[**Namespace List**](namespaces.md) **>** [**MexHelper**](namespaceMexHelper.md)



_Fonctions utilitaires pour faciliter l'usage de l'API Matlab._ 






































## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**chargerValeurs**](#function-chargervaleurs) (const mxArray \* pStruct, const std::string & nomChamp, std::vector&lt; Type &gt; & param, mwIndex index=0) <br>_Chargement de valeurs dans un vecteur._  |
|  void | [**chargerValeurs**](#function-chargervaleurs) (const mxArray \* pStruct, const std::string & nomChamp, Type & param, mwIndex index=0) <br>_Chargement d'une valeur dans un type de base._  |
|  void | [**chargerValeurs**](#function-chargervaleurs) (const mxArray \* pStruct, const std::string & nomChamp, Type & param, Type & valDefaut, mwIndex index) <br>_Chargement d'une valeur dans un type de base avec une valeur par defaut si le champ est absent._  |
|  void | [**chargerValeurs**](#function-chargervaleurs) (const mxArray \* pStruct, const std::string & nomChamp, int \* param, unsigned int nbItems, mwIndex index=0) <br>_Chargement des valeurs dans un array de int._  |
|  void | [**chargerValeurs**](#function-chargervaleurs) (const mxArray \* pStruct, const std::string & nomChamp, std::string & param, mwIndex index=0) <br>_Chargement d'une valeur dans un string._  |
|  void | [**chargerValeurs**](#function-chargervaleurs) (const mxArray \* pStruct, const std::string & nomChamp, ParamEquation & param, mwIndex index=0) <br>_Chargement des valeurs dans un ParamEquation._  |
|  void | [**chargerValeurs**](#function-chargervaleurs) (const mxArray \* pStruct, const std::string & nomChamp, MoyenneMois & param, mwIndex index=0) <br>_Chargement des valeurs dans un MoyenneMois._  |
|  void | [**chargerValeurs**](#function-chargervaleurs) (const mxArray \* pStruct, const std::string & nomChamp, std::vector&lt; ParamEquation &gt; & param, mwIndex index=0) <br>_Chargement des valeurs dans un vecteur de ParamEquation._  |
|  void | [**chargerValeurs**](#function-chargervaleurs) (const mxArray \* pStruct, const std::string & nomChamp, std::vector&lt; [**InfoEvacuationExt**](classInfoEvacuationExt.md) &gt; & param, mwIndex index=0) <br>_Chargement des valeurs dans un vecteur de_ [_**InfoEvacuationExt**_](classInfoEvacuationExt.md) _._ |
|  void | [**chargerValeurs**](#function-chargervaleurs) (const mxArray \* pStruct, const std::string & nomChamp, Type & param, Type valDefaut, mwIndex index) <br> |
|  double | [**dateToDatenum**](#function-datetodatenum) ([**DateChrono**](classDateChrono.md) & aDate) <br>_Conversion d'une date en format Matlab datenum._  |
|  [**DateChrono**](classDateChrono.md) | [**datenumToDate**](#function-datenumtodate) (double \* aDatenum) <br>_Conversion d'un date Matlab de format datenum en date._  |
|  bool | [**hasField**](#function-hasfield) (const mxArray \* pm, mwIndex index, const std::string & fieldname) <br>_Est-ce que le champ existe dans la struct?_  |
|  Type \* | [**mhMxGetData**](#function-mhmxgetdata) (const mxArray \* pm, const std::string & fieldname) <br>_mxGetData generique avec gestion d'erreur._  |
|  mxArray \* | [**mhMxGetField**](#function-mhmxgetfield) (const mxArray \* pm, mwIndex index, const std::string & fieldname) <br>_Equivalent d'un matOpen mais avec gestion d'erreur._  |
|  double \* | [**mhMxGetPr**](#function-mhmxgetpr) (const mxArray \* pm, const std::string & fieldname) <br>_Equivalent d'un mxGetPr mais avec gestion d'erreur._  |




























## Public Functions Documentation




### function chargerValeurs 

_Chargement de valeurs dans un vecteur._ 
```C++
template<typename Type>
inline void MexHelper::chargerValeurs (
    const mxArray * pStruct,
    const std::string & nomChamp,
    std::vector< Type > & param,
    mwIndex index=0
) 
```




<hr>



### function chargerValeurs 

_Chargement d'une valeur dans un type de base._ 
```C++
template<typename Type>
inline void MexHelper::chargerValeurs (
    const mxArray * pStruct,
    const std::string & nomChamp,
    Type & param,
    mwIndex index=0
) 
```




<hr>



### function chargerValeurs 

_Chargement d'une valeur dans un type de base avec une valeur par defaut si le champ est absent._ 
```C++
template<typename Type>
void MexHelper::chargerValeurs (
    const mxArray * pStruct,
    const std::string & nomChamp,
    Type & param,
    Type & valDefaut,
    mwIndex index
) 
```




<hr>



### function chargerValeurs 

_Chargement des valeurs dans un array de int._ 
```C++
inline void MexHelper::chargerValeurs (
    const mxArray * pStruct,
    const std::string & nomChamp,
    int * param,
    unsigned int nbItems,
    mwIndex index=0
) 
```




<hr>



### function chargerValeurs 

_Chargement d'une valeur dans un string._ 
```C++
inline void MexHelper::chargerValeurs (
    const mxArray * pStruct,
    const std::string & nomChamp,
    std::string & param,
    mwIndex index=0
) 
```




<hr>



### function chargerValeurs 

_Chargement des valeurs dans un ParamEquation._ 
```C++
inline void MexHelper::chargerValeurs (
    const mxArray * pStruct,
    const std::string & nomChamp,
    ParamEquation & param,
    mwIndex index=0
) 
```




<hr>



### function chargerValeurs 

_Chargement des valeurs dans un MoyenneMois._ 
```C++
inline void MexHelper::chargerValeurs (
    const mxArray * pStruct,
    const std::string & nomChamp,
    MoyenneMois & param,
    mwIndex index=0
) 
```




<hr>



### function chargerValeurs 

_Chargement des valeurs dans un vecteur de ParamEquation._ 
```C++
inline void MexHelper::chargerValeurs (
    const mxArray * pStruct,
    const std::string & nomChamp,
    std::vector< ParamEquation > & param,
    mwIndex index=0
) 
```




<hr>



### function chargerValeurs 

_Chargement des valeurs dans un vecteur de_ [_**InfoEvacuationExt**_](classInfoEvacuationExt.md) _._
```C++
inline void MexHelper::chargerValeurs (
    const mxArray * pStruct,
    const std::string & nomChamp,
    std::vector< InfoEvacuationExt > & param,
    mwIndex index=0
) 
```




<hr>



### function chargerValeurs 

```C++
template<typename Type>
inline void MexHelper::chargerValeurs (
    const mxArray * pStruct,
    const std::string & nomChamp,
    Type & param,
    Type valDefaut,
    mwIndex index
) 
```




<hr>



### function dateToDatenum 

_Conversion d'une date en format Matlab datenum._ 
```C++
inline double MexHelper::dateToDatenum (
    DateChrono & aDate
) 
```




<hr>



### function datenumToDate 

_Conversion d'un date Matlab de format datenum en date._ 
```C++
inline DateChrono MexHelper::datenumToDate (
    double * aDatenum
) 
```




<hr>



### function hasField 

_Est-ce que le champ existe dans la struct?_ 
```C++
inline bool MexHelper::hasField (
    const mxArray * pm,
    mwIndex index,
    const std::string & fieldname
) 
```




<hr>



### function mhMxGetData 

_mxGetData generique avec gestion d'erreur._ 
```C++
template<typename Type>
inline Type * MexHelper::mhMxGetData (
    const mxArray * pm,
    const std::string & fieldname
) 
```




<hr>



### function mhMxGetField 

_Equivalent d'un matOpen mais avec gestion d'erreur._ 
```C++
inline mxArray * MexHelper::mhMxGetField (
    const mxArray * pm,
    mwIndex index,
    const std::string & fieldname
) 
```



Equivalent d'un mxGetField mais avec gestion d'erreur. 


        

<hr>



### function mhMxGetPr 

_Equivalent d'un mxGetPr mais avec gestion d'erreur._ 
```C++
inline double * MexHelper::mhMxGetPr (
    const mxArray * pm,
    const std::string & fieldname
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/MexHelper.hpp`

