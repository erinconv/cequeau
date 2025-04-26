

# Class EvapoTemplate



[**ClassList**](annotated.md) **>** [**EvapoTemplate**](classEvapoTemplate.md)








Inherits the following classes: [Evapo](classEvapo.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**EvapoTemplate**](#function-evapotemplate-12) () <br>_Constructeur._  |
|   | [**EvapoTemplate**](#function-evapotemplate-22) (int latitudeMoyenneBV, int nbCE, int pasParJour) <br> |
| virtual int | [**assimiler**](#function-assimiler) (const [**DateChrono**](classDateChrono.md) & datePasDeTemps) <br> |
| virtual int | [**calculerEvapo**](#function-calculerevapo) (const int & noJour, const [**Meteo**](classMeteo.md) & meteo, const [**CarreauEntier**](classCarreauEntier.md) & carreauEntier, const float niveauEauNappe, float & evapotranspirationSol, float & evapotranspirationNappe, float & evaporationLac, float & evapotranspirationPotentielle) <br> |
| virtual int | [**initialiserAssimilations**](#function-initialiserassimilations) (const mxArray \* assimilations) <br> |
| virtual void | [**initialiserEtats**](#function-initialiseretats) (const mxArray \* etatsInitiaux) <br> |
| virtual void | [**lireParametres**](#function-lireparametres) (const mxArray \* paramSimul) <br> |
| virtual mxArray \* | [**obtenirEtats**](#function-obteniretats) (const std::vector&lt; bool &gt; filtreCE) <br> |
| virtual mxArray \* | [**obtenirEtatsAvantAssimilations**](#function-obteniretatsavantassimilations) (const std::vector&lt; bool &gt; filtreCE) <br> |
| virtual void | [**preserverEtatsPasDeTemps**](#function-preserveretatspasdetemps) () <br> |
|   | [**~EvapoTemplate**](#function-evapotemplate) () <br> |


## Public Functions inherited from Evapo

See [Evapo](classEvapo.md)

| Type | Name |
| ---: | :--- |
|   | [**Evapo**](classEvapo.md#function-evapo-12) () <br>_Constructeur._  |
|   | [**Evapo**](classEvapo.md#function-evapo-22) (int nbCE, std::string nomModele) <br> |
| virtual int | [**assimiler**](classEvapo.md#function-assimiler) (const [**DateChrono**](classDateChrono.md) & datePasDeTemps) = 0<br> |
| virtual int | [**calculerEvapo**](classEvapo.md#function-calculerevapo) (const int & noJour, const [**Meteo**](classMeteo.md) & meteo, const [**CarreauEntier**](classCarreauEntier.md) & carreauEntier, const float niveauEauNappe, float & evapotranspirationSol, float & evapotranspirationNappe, float & evaporationLac, float & evapotranspirationPotentielle) = 0<br> |
| virtual int | [**initialiserAssimilations**](classEvapo.md#function-initialiserassimilations) (const mxArray \* assimilations) = 0<br> |
| virtual void | [**initialiserEtats**](classEvapo.md#function-initialiseretats) (const mxArray \* etatsInitiaux) = 0<br> |
| virtual void | [**lireParametres**](classEvapo.md#function-lireparametres) (const mxArray \* paramSimul) = 0<br> |
| virtual mxArray \* | [**obtenirEtats**](classEvapo.md#function-obteniretats) (const std::vector&lt; bool &gt; filtreCE) = 0<br> |
| virtual mxArray \* | [**obtenirEtatsAvantAssimilations**](classEvapo.md#function-obteniretatsavantassimilations) (const std::vector&lt; bool &gt; filtreCE) = 0<br> |
| virtual void | [**preserverEtatsPasDeTemps**](classEvapo.md#function-preserveretatspasdetemps) () = 0<br> |
| virtual  | [**~Evapo**](classEvapo.md#function-evapo) () <br> |


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




### function EvapoTemplate [1/2]

_Constructeur._ 
```C++
EvapoTemplate::EvapoTemplate () 
```




<hr>



### function EvapoTemplate [2/2]

```C++
EvapoTemplate::EvapoTemplate (
    int latitudeMoyenneBV,
    int nbCE,
    int pasParJour
) 
```




<hr>



### function assimiler 

```C++
virtual int EvapoTemplate::assimiler (
    const DateChrono & datePasDeTemps
) 
```



Implements [*Evapo::assimiler*](classEvapo.md#function-assimiler)


<hr>



### function calculerEvapo 

```C++
virtual int EvapoTemplate::calculerEvapo (
    const int & noJour,
    const Meteo & meteo,
    const CarreauEntier & carreauEntier,
    const float niveauEauNappe,
    float & evapotranspirationSol,
    float & evapotranspirationNappe,
    float & evaporationLac,
    float & evapotranspirationPotentielle
) 
```



Implements [*Evapo::calculerEvapo*](classEvapo.md#function-calculerevapo)


<hr>



### function initialiserAssimilations 

```C++
virtual int EvapoTemplate::initialiserAssimilations (
    const mxArray * assimilations
) 
```



Implements [*Evapo::initialiserAssimilations*](classEvapo.md#function-initialiserassimilations)


<hr>



### function initialiserEtats 

```C++
virtual void EvapoTemplate::initialiserEtats (
    const mxArray * etatsInitiaux
) 
```



Implements [*Evapo::initialiserEtats*](classEvapo.md#function-initialiseretats)


<hr>



### function lireParametres 

```C++
virtual void EvapoTemplate::lireParametres (
    const mxArray * paramSimul
) 
```



Implements [*Evapo::lireParametres*](classEvapo.md#function-lireparametres)


<hr>



### function obtenirEtats 

```C++
virtual mxArray * EvapoTemplate::obtenirEtats (
    const std::vector< bool > filtreCE
) 
```



Implements [*Evapo::obtenirEtats*](classEvapo.md#function-obteniretats)


<hr>



### function obtenirEtatsAvantAssimilations 

```C++
virtual mxArray * EvapoTemplate::obtenirEtatsAvantAssimilations (
    const std::vector< bool > filtreCE
) 
```



Implements [*Evapo::obtenirEtatsAvantAssimilations*](classEvapo.md#function-obteniretatsavantassimilations)


<hr>



### function preserverEtatsPasDeTemps 

```C++
virtual void EvapoTemplate::preserverEtatsPasDeTemps () 
```



Implements [*Evapo::preserverEtatsPasDeTemps*](classEvapo.md#function-preserveretatspasdetemps)


<hr>



### function ~EvapoTemplate 

```C++
EvapoTemplate::~EvapoTemplate () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/EvapoTemplate.h`

