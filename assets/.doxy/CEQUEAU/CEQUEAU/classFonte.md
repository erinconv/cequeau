

# Class Fonte



[**ClassList**](annotated.md) **>** [**Fonte**](classFonte.md)



_Interface module de fonte._ 

* `#include <Fonte.h>`



Inherits the following classes: [ModuleBase](classModuleBase.md)


Inherited by the following classes: [FonteCemaNeige](classFonteCemaNeige.md),  [FonteCequeau](classFonteCequeau.md),  [FonteTemplate](classFonteTemplate.md),  [FonteUEB](classFonteUEB.md)




















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Fonte**](#function-fonte-12) () <br>_Constructeur._  |
|   | [**Fonte**](#function-fonte-22) (int nbCE, std::string nomModele) <br> |
| virtual int | [**assimiler**](#function-assimiler) (const [**DateChrono**](classDateChrono.md) & datePasDeTemps) = 0<br> |
| virtual int | [**calculerFonte**](#function-calculerfonte) (const [**DateChrono**](classDateChrono.md) & datePasDeTemps, const [**Meteo**](classMeteo.md) & meteo, const [**CarreauEntier**](classCarreauEntier.md) & carreauEntier, float & precipationsTotales, float & eauDisponible) = 0<br> |
| virtual int | [**initialiserAssimilations**](#function-initialiserassimilations) (const mxArray \* assimilations) = 0<br> |
| virtual void | [**initialiserEtats**](#function-initialiseretats) (const mxArray \* etatsInitiaux) = 0<br> |
| virtual void | [**lireParametres**](#function-lireparametres) (const mxArray \* paramSimul) = 0<br> |
| virtual mxArray \* | [**obtenirEtats**](#function-obteniretats) (const std::vector&lt; bool &gt; filtreCE) = 0<br> |
| virtual mxArray \* | [**obtenirEtatsAvantAssimilations**](#function-obteniretatsavantassimilations) (const std::vector&lt; bool &gt; filtreCE) = 0<br> |
| virtual void | [**preserverEtatsPasDeTemps**](#function-preserveretatspasdetemps) () = 0<br> |
| virtual  | [**~Fonte**](#function-fonte) () <br> |


## Public Functions inherited from ModuleBase

See [ModuleBase](classModuleBase.md)

| Type | Name |
| ---: | :--- |
|   | [**ModuleBase**](classModuleBase.md#function-modulebase-12) () <br>_Constructeur._  |
|   | [**ModuleBase**](classModuleBase.md#function-modulebase-22) (int nbCE, std::string nomModele) <br> |
|  std::string | [**nomModele**](classModuleBase.md#function-nommodele) () <br> |
|  std::vector&lt; std::string &gt; & | [**nomsChampsMeteo**](classModuleBase.md#function-nomschampsmeteo) () <br>_Nom des intrants meteo propre au modele._  |
| virtual  | [**~ModuleBase**](classModuleBase.md#function-modulebase) () <br> |
















## Protected Attributes inherited from ModuleBase

See [ModuleBase](classModuleBase.md)

| Type | Name |
| ---: | :--- |
|  int | [**nbCE\_**](classModuleBase.md#variable-nbce_)  <br>_Nombre de carreaux entiers._  |
































## Protected Functions inherited from ModuleBase

See [ModuleBase](classModuleBase.md)

| Type | Name |
| ---: | :--- |
|  void | [**initialiserEtatsHelper**](classModuleBase.md#function-initialiseretatshelper) (const mxArray \* etats, std::map&lt; std::string, float \* &gt; & champs, Type & etatCE, std::vector&lt; Type &gt; & etatsCE) <br>_Helper generique pour l'initialisation des etats._  |
|  void | [**lireParametresHelper**](classModuleBase.md#function-lireparametreshelper-12) (const mxArray \* paramFonte, const std::string & nomChamp, std::vector&lt; float &gt; & champ) <br>_Helper pour la lecture d'un parametre spatialisable._  |
|  void | [**lireParametresHelper**](classModuleBase.md#function-lireparametreshelper-22) (const mxArray \* paramFonte, const std::string & nomChamp, Type & champ) <br>_Helper generique pour la lecture d'un parametre._  |
|  mxArray \* | [**obtenirEtatsAvantAssimHelper**](classModuleBase.md#function-obteniretatsavantassimhelper) (const std::vector&lt; bool &gt; & filtreCE, const std::vector&lt; std::string &gt; & nomChamps, const std::map&lt; [**DateChrono**](classDateChrono.md), std::vector&lt; Type &gt; &gt; & etatsAvantAssim) <br>_Helper generique pour la sortie des sauvegarde de etats avant assimilation en mxArray._  |
|  mxArray \* | [**obtenirEtatsHelper**](classModuleBase.md#function-obteniretatshelper) (const std::vector&lt; std::string &gt; & nomsChamps, const std::vector&lt; std::vector&lt; Type &gt; &gt; & etats, const std::vector&lt; bool &gt; & filtreCE) <br>_Helper generique pour la sortie des etats en mxArray._  |






## Public Functions Documentation




### function Fonte [1/2]

_Constructeur._ 
```C++
inline Fonte::Fonte () 
```




<hr>



### function Fonte [2/2]

```C++
inline Fonte::Fonte (
    int nbCE,
    std::string nomModele
) 
```




<hr>



### function assimiler 

```C++
virtual int Fonte::assimiler (
    const DateChrono & datePasDeTemps
) = 0
```




<hr>



### function calculerFonte 

```C++
virtual int Fonte::calculerFonte (
    const DateChrono & datePasDeTemps,
    const Meteo & meteo,
    const CarreauEntier & carreauEntier,
    float & precipationsTotales,
    float & eauDisponible
) = 0
```




<hr>



### function initialiserAssimilations 

```C++
virtual int Fonte::initialiserAssimilations (
    const mxArray * assimilations
) = 0
```




<hr>



### function initialiserEtats 

```C++
virtual void Fonte::initialiserEtats (
    const mxArray * etatsInitiaux
) = 0
```




<hr>



### function lireParametres 

```C++
virtual void Fonte::lireParametres (
    const mxArray * paramSimul
) = 0
```




<hr>



### function obtenirEtats 

```C++
virtual mxArray * Fonte::obtenirEtats (
    const std::vector< bool > filtreCE
) = 0
```




<hr>



### function obtenirEtatsAvantAssimilations 

```C++
virtual mxArray * Fonte::obtenirEtatsAvantAssimilations (
    const std::vector< bool > filtreCE
) = 0
```




<hr>



### function preserverEtatsPasDeTemps 

```C++
virtual void Fonte::preserverEtatsPasDeTemps () = 0
```




<hr>



### function ~Fonte 

```C++
inline virtual Fonte::~Fonte () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/Fonte.h`

