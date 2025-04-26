

# Class FonteCequeau



[**ClassList**](annotated.md) **>** [**FonteCequeau**](classFonteCequeau.md)



_Definition d'une classe de calcul de la fonte._ 

* `#include <FonteCequeau.h>`



Inherits the following classes: [Fonte](classFonte.md)












## Classes

| Type | Name |
| ---: | :--- |
| class | [**EtatFonteCE**](classFonteCequeau_1_1EtatFonteCE.md) <br> |






























































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**FonteCequeau**](#function-fontecequeau-12) () <br>_Constructeur._  |
|   | [**FonteCequeau**](#function-fontecequeau-22) (int latitudeMoyenneBV, int nbCE) <br> |
| virtual int | [**assimiler**](#function-assimiler) (const [**DateChrono**](classDateChrono.md) & datePasDeTemps) <br> |
| virtual int | [**calculerFonte**](#function-calculerfonte) (const [**DateChrono**](classDateChrono.md) & datePasDeTemps, const [**Meteo**](classMeteo.md) & meteo, const [**CarreauEntier**](classCarreauEntier.md) & carreauEntier, float & precipationsTotales, float & eauDisponible) <br> |
|  const std::vector&lt; std::vector&lt; [**EtatFonteCE**](classFonteCequeau_1_1EtatFonteCE.md) &gt; &gt; & | [**etatsFonte**](#function-etatsfonte) () const<br> |
| virtual int | [**initialiserAssimilations**](#function-initialiserassimilations) (const mxArray \* assimilations) <br> |
| virtual void | [**initialiserEtats**](#function-initialiseretats) (const mxArray \* etatsInitiaux) <br> |
| virtual void | [**lireParametres**](#function-lireparametres) (const mxArray \* paramSimul) <br> |
| virtual mxArray \* | [**obtenirEtats**](#function-obteniretats) (const std::vector&lt; bool &gt; filtreCE) <br> |
| virtual mxArray \* | [**obtenirEtatsAvantAssimilations**](#function-obteniretatsavantassimilations) (const std::vector&lt; bool &gt; filtreCE) <br> |
| virtual void | [**preserverEtatsPasDeTemps**](#function-preserveretatspasdetemps) () <br> |
|   | [**~FonteCequeau**](#function-fontecequeau) () <br> |


## Public Functions inherited from Fonte

See [Fonte](classFonte.md)

| Type | Name |
| ---: | :--- |
|   | [**Fonte**](classFonte.md#function-fonte-12) () <br>_Constructeur._  |
|   | [**Fonte**](classFonte.md#function-fonte-22) (int nbCE, std::string nomModele) <br> |
| virtual int | [**assimiler**](classFonte.md#function-assimiler) (const [**DateChrono**](classDateChrono.md) & datePasDeTemps) = 0<br> |
| virtual int | [**calculerFonte**](classFonte.md#function-calculerfonte) (const [**DateChrono**](classDateChrono.md) & datePasDeTemps, const [**Meteo**](classMeteo.md) & meteo, const [**CarreauEntier**](classCarreauEntier.md) & carreauEntier, float & precipationsTotales, float & eauDisponible) = 0<br> |
| virtual int | [**initialiserAssimilations**](classFonte.md#function-initialiserassimilations) (const mxArray \* assimilations) = 0<br> |
| virtual void | [**initialiserEtats**](classFonte.md#function-initialiseretats) (const mxArray \* etatsInitiaux) = 0<br> |
| virtual void | [**lireParametres**](classFonte.md#function-lireparametres) (const mxArray \* paramSimul) = 0<br> |
| virtual mxArray \* | [**obtenirEtats**](classFonte.md#function-obteniretats) (const std::vector&lt; bool &gt; filtreCE) = 0<br> |
| virtual mxArray \* | [**obtenirEtatsAvantAssimilations**](classFonte.md#function-obteniretatsavantassimilations) (const std::vector&lt; bool &gt; filtreCE) = 0<br> |
| virtual void | [**preserverEtatsPasDeTemps**](classFonte.md#function-preserveretatspasdetemps) () = 0<br> |
| virtual  | [**~Fonte**](classFonte.md#function-fonte) () <br> |


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




### function FonteCequeau [1/2]

_Constructeur._ 
```C++
FonteCequeau::FonteCequeau () 
```




<hr>



### function FonteCequeau [2/2]

```C++
FonteCequeau::FonteCequeau (
    int latitudeMoyenneBV,
    int nbCE
) 
```




<hr>



### function assimiler 

```C++
virtual int FonteCequeau::assimiler (
    const DateChrono & datePasDeTemps
) 
```



Implements [*Fonte::assimiler*](classFonte.md#function-assimiler)


<hr>



### function calculerFonte 

```C++
virtual int FonteCequeau::calculerFonte (
    const DateChrono & datePasDeTemps,
    const Meteo & meteo,
    const CarreauEntier & carreauEntier,
    float & precipationsTotales,
    float & eauDisponible
) 
```



Implements [*Fonte::calculerFonte*](classFonte.md#function-calculerfonte)


<hr>



### function etatsFonte 

```C++
inline const std::vector< std::vector< EtatFonteCE > > & FonteCequeau::etatsFonte () const
```




<hr>



### function initialiserAssimilations 

```C++
virtual int FonteCequeau::initialiserAssimilations (
    const mxArray * assimilations
) 
```



Implements [*Fonte::initialiserAssimilations*](classFonte.md#function-initialiserassimilations)


<hr>



### function initialiserEtats 

```C++
virtual void FonteCequeau::initialiserEtats (
    const mxArray * etatsInitiaux
) 
```



Implements [*Fonte::initialiserEtats*](classFonte.md#function-initialiseretats)


<hr>



### function lireParametres 

```C++
virtual void FonteCequeau::lireParametres (
    const mxArray * paramSimul
) 
```



Implements [*Fonte::lireParametres*](classFonte.md#function-lireparametres)


<hr>



### function obtenirEtats 

```C++
virtual mxArray * FonteCequeau::obtenirEtats (
    const std::vector< bool > filtreCE
) 
```



Implements [*Fonte::obtenirEtats*](classFonte.md#function-obteniretats)


<hr>



### function obtenirEtatsAvantAssimilations 

```C++
virtual mxArray * FonteCequeau::obtenirEtatsAvantAssimilations (
    const std::vector< bool > filtreCE
) 
```



Implements [*Fonte::obtenirEtatsAvantAssimilations*](classFonte.md#function-obteniretatsavantassimilations)


<hr>



### function preserverEtatsPasDeTemps 

```C++
virtual void FonteCequeau::preserverEtatsPasDeTemps () 
```



Implements [*Fonte::preserverEtatsPasDeTemps*](classFonte.md#function-preserveretatspasdetemps)


<hr>



### function ~FonteCequeau 

```C++
FonteCequeau::~FonteCequeau () 
```




<hr>## Friends Documentation





### friend Simulation 

```C++
class FonteCequeau::Simulation (
    Simulation
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/FonteCequeau.h`

