

# Class ModuleBase



[**ClassList**](annotated.md) **>** [**ModuleBase**](classModuleBase.md)



_Classe de base pour les modules de fonte et d'evapotranspiration._ 

* `#include <ModuleBase.hpp>`





Inherited by the following classes: [DLI](classDLI.md),  [Evapo](classEvapo.md),  [Fonte](classFonte.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ModuleBase**](#function-modulebase-12) () <br>_Constructeur._  |
|   | [**ModuleBase**](#function-modulebase-22) (int nbCE, std::string nomModele) <br> |
|  std::string | [**nomModele**](#function-nommodele) () <br> |
|  std::vector&lt; std::string &gt; & | [**nomsChampsMeteo**](#function-nomschampsmeteo) () <br>_Nom des intrants meteo propre au modele._  |
| virtual  | [**~ModuleBase**](#function-modulebase) () <br> |








## Protected Attributes

| Type | Name |
| ---: | :--- |
|  int | [**nbCE\_**](#variable-nbce_)  <br>_Nombre de carreaux entiers._  |
















## Protected Functions

| Type | Name |
| ---: | :--- |
|  void | [**initialiserEtatsHelper**](#function-initialiseretatshelper) (const mxArray \* etats, std::map&lt; std::string, float \* &gt; & champs, Type & etatCE, std::vector&lt; Type &gt; & etatsCE) <br>_Helper generique pour l'initialisation des etats._  |
|  void | [**lireParametresHelper**](#function-lireparametreshelper-12) (const mxArray \* paramFonte, const std::string & nomChamp, std::vector&lt; float &gt; & champ) <br>_Helper pour la lecture d'un parametre spatialisable._  |
|  void | [**lireParametresHelper**](#function-lireparametreshelper-22) (const mxArray \* paramFonte, const std::string & nomChamp, Type & champ) <br>_Helper generique pour la lecture d'un parametre._  |
|  mxArray \* | [**obtenirEtatsAvantAssimHelper**](#function-obteniretatsavantassimhelper) (const std::vector&lt; bool &gt; & filtreCE, const std::vector&lt; std::string &gt; & nomChamps, const std::map&lt; [**DateChrono**](classDateChrono.md), std::vector&lt; Type &gt; &gt; & etatsAvantAssim) <br>_Helper generique pour la sortie des sauvegarde de etats avant assimilation en mxArray._  |
|  mxArray \* | [**obtenirEtatsHelper**](#function-obteniretatshelper) (const std::vector&lt; std::string &gt; & nomsChamps, const std::vector&lt; std::vector&lt; Type &gt; &gt; & etats, const std::vector&lt; bool &gt; & filtreCE) <br>_Helper generique pour la sortie des etats en mxArray._  |




## Public Functions Documentation




### function ModuleBase [1/2]

_Constructeur._ 
```C++
inline ModuleBase::ModuleBase () 
```




<hr>



### function ModuleBase [2/2]

```C++
inline ModuleBase::ModuleBase (
    int nbCE,
    std::string nomModele
) 
```




<hr>



### function nomModele 

```C++
inline std::string ModuleBase::nomModele () 
```




<hr>



### function nomsChampsMeteo 

_Nom des intrants meteo propre au modele._ 
```C++
inline std::vector< std::string > & ModuleBase::nomsChampsMeteo () 
```




<hr>



### function ~ModuleBase 

```C++
inline virtual ModuleBase::~ModuleBase () 
```




<hr>
## Protected Attributes Documentation




### variable nbCE\_ 

_Nombre de carreaux entiers._ 
```C++
int ModuleBase::nbCE_;
```




<hr>
## Protected Functions Documentation




### function initialiserEtatsHelper 

_Helper generique pour l'initialisation des etats._ 
```C++
template<typename Type>
inline void ModuleBase::initialiserEtatsHelper (
    const mxArray * etats,
    std::map< std::string, float * > & champs,
    Type & etatCE,
    std::vector< Type > & etatsCE
) 
```




<hr>



### function lireParametresHelper [1/2]

_Helper pour la lecture d'un parametre spatialisable._ 
```C++
inline void ModuleBase::lireParametresHelper (
    const mxArray * paramFonte,
    const std::string & nomChamp,
    std::vector< float > & champ
) 
```




<hr>



### function lireParametresHelper [2/2]

_Helper generique pour la lecture d'un parametre._ 
```C++
template<typename Type>
inline void ModuleBase::lireParametresHelper (
    const mxArray * paramFonte,
    const std::string & nomChamp,
    Type & champ
) 
```




<hr>



### function obtenirEtatsAvantAssimHelper 

_Helper generique pour la sortie des sauvegarde de etats avant assimilation en mxArray._ 
```C++
template<typename Type>
inline mxArray * ModuleBase::obtenirEtatsAvantAssimHelper (
    const std::vector< bool > & filtreCE,
    const std::vector< std::string > & nomChamps,
    const std::map< DateChrono , std::vector< Type > > & etatsAvantAssim
) 
```




<hr>



### function obtenirEtatsHelper 

_Helper generique pour la sortie des etats en mxArray._ 
```C++
template<typename Type>
inline mxArray * ModuleBase::obtenirEtatsHelper (
    const std::vector< std::string > & nomsChamps,
    const std::vector< std::vector< Type > > & etats,
    const std::vector< bool > & filtreCE
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/ModuleBase.hpp`

