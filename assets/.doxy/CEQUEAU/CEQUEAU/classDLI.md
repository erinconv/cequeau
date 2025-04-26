

# Class DLI



[**ClassList**](annotated.md) **>** [**DLI**](classDLI.md)








Inherits the following classes: [ModuleBase](classModuleBase.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**DLI**](#function-dli) () <br> |
|  double | [**calculeM1**](#function-calculem1) (double B) <br> |
|  double | [**calculeM2**](#function-calculem2) (double B, double TKR) <br> |
|  double | [**calculeM3**](#function-calculem3) (double B, double TKR) <br> |
|  double | [**calculeM4**](#function-calculem4) (double B, double ea) <br> |
|  double | [**calculeM5**](#function-calculem5) (double B, double TKR, double ea) <br> |
|  double | [**calculeM6**](#function-calculem6) (double B, double TKR, double ea) <br> |
|  double | [**calculeM7**](#function-calculem7) (double B, double TKR, double ea) <br> |
|  double | [**calculeM8**](#function-calculem8) (double B, double TKR, double ea) <br> |
|  double | [**calculeM9**](#function-calculem9) (double B, double ea, double LAI) <br> |
|  double | [**calculeRadiation**](#function-calculeradiation) (double sigma, double ENNUAG, double TKR, double PREVAP, double TKO, double LAI, double DLI) <br> |
|  void | [**initialiser**](#function-initialiser) (int moduleDLI, const mxArray \* structParamDLI) <br> |
|  void | [**lireParametres**](#function-lireparametres) (const mxArray \* paramSimul, int numParams) <br> |
|  int | [**moduleDLI**](#function-moduledli) () <br> |


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




### function DLI 

```C++
DLI::DLI () 
```




<hr>



### function calculeM1 

```C++
double DLI::calculeM1 (
    double B
) 
```




<hr>



### function calculeM2 

```C++
double DLI::calculeM2 (
    double B,
    double TKR
) 
```




<hr>



### function calculeM3 

```C++
double DLI::calculeM3 (
    double B,
    double TKR
) 
```




<hr>



### function calculeM4 

```C++
double DLI::calculeM4 (
    double B,
    double ea
) 
```




<hr>



### function calculeM5 

```C++
double DLI::calculeM5 (
    double B,
    double TKR,
    double ea
) 
```




<hr>



### function calculeM6 

```C++
double DLI::calculeM6 (
    double B,
    double TKR,
    double ea
) 
```




<hr>



### function calculeM7 

```C++
double DLI::calculeM7 (
    double B,
    double TKR,
    double ea
) 
```




<hr>



### function calculeM8 

```C++
double DLI::calculeM8 (
    double B,
    double TKR,
    double ea
) 
```




<hr>



### function calculeM9 

```C++
double DLI::calculeM9 (
    double B,
    double ea,
    double LAI
) 
```




<hr>



### function calculeRadiation 

```C++
double DLI::calculeRadiation (
    double sigma,
    double ENNUAG,
    double TKR,
    double PREVAP,
    double TKO,
    double LAI,
    double DLI
) 
```




<hr>



### function initialiser 

```C++
void DLI::initialiser (
    int moduleDLI,
    const mxArray * structParamDLI
) 
```




<hr>



### function lireParametres 

```C++
void DLI::lireParametres (
    const mxArray * paramSimul,
    int numParams
) 
```




<hr>



### function moduleDLI 

```C++
int DLI::moduleDLI () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/DLI.h`

