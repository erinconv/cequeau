

# Class BassinVersant



[**ClassList**](annotated.md) **>** [**BassinVersant**](classBassinVersant.md)



_Definition du bassin versant._ [More...](#detailed-description)

* `#include <BassinVersant.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**BassinVersant**](#function-bassinversant) () <br>_Constructeur._  |
|  std::vector&lt; BarragePtr &gt; & | [**barrages**](#function-barrages) () <br>_Getter barrage._  |
|  const std::vector&lt; CarreauEntierPtr &gt; & | [**carreauxEntiers**](#function-carreauxentiers) () const<br>_Getter liste des carreaux entiers._  |
|  const std::vector&lt; CarreauPartielPtr &gt; & | [**carreauxPartiels**](#function-carreauxpartiels) () const<br>_Getter liste des carreaux partiels._  |
|  void | [**initialiser**](#function-initialiser) (const mxArray \* bassinVersant) <br>_Initialisation du bassin versant a partir d'un variable Matlab (mxArray)_  |
|  void | [**initialiserBarrage**](#function-initialiserbarrage-13) (int idCP, int idCPAval, float volumeInitial, ParamEquation & coeffNiveau, ParamEquation & coeffDebit) <br>_Initialisation barrage type 2._  |
|  void | [**initialiserBarrage**](#function-initialiserbarrage-23) (int idCP, int idCPAval, float volumeInitial, ParamEquation & coeffNiveau, ParamEquation & coeffVolume, std::vector&lt; ParamEquation &gt; & coeffDebitInt, std::vector&lt; [**InfoEvacuationExt**](classInfoEvacuationExt.md) &gt; & coeffDebitExt) <br>_Initialisation barrage type 5._  |
|  void | [**initialiserBarrage**](#function-initialiserbarrage-33) (int idCPAval, std::vector&lt; float &gt; & debitsExternesMesures) <br>_Initialisation barrage type 3._  |
|  void | [**initialiserFichier**](#function-initialiserfichier) (const std::string nomFichierintrants) <br>_Initialisation du bassin versant a partir d'un fichier Matlab._  |
|  int | [**nbBarrages**](#function-nbbarrages) () const<br>_Getter nbBarrages._  |
|  int | [**nbCarreauxEntiers**](#function-nbcarreauxentiers) () const<br>_Getter nbCarreauxEntiers._  |
|  int | [**nbCarreauxPartiels**](#function-nbcarreauxpartiels) () const<br>_Getter nbCarreauxPartiels._  |
|  int | [**nbCarreauxPartielsCheminLong**](#function-nbcarreauxpartielscheminlong) () const<br>_Getter nbCarreauxPartielsCheminLong._  |
|  std::string | [**nomBassinVersant**](#function-nombassinversant) () const<br>_Getter nomBassinVersant._  |
|  bool | [**possedeBarrage**](#function-possedebarrage-12) () const<br>_Le bassin possede des barrages?_  |
|  bool | [**possedeBarrage**](#function-possedebarrage-22) (int idCP) const<br>_Le CP possede un barrage?_  |
|  const std::map&lt; int, std::vector&lt; PuitsPtr &gt; &gt; & | [**puits**](#function-puits) () const<br>_Getter liste des puits._  |
|  const std::vector&lt; PuitsPtr &gt; & | [**puitsParIdCE**](#function-puitsparidce) (int idCE) const<br> |
|  float | [**superficieCarreauEntier**](#function-superficiecarreauentier) () const<br>_Getter superficieCarreauEntier._  |
|  const BarragePtr | [**trouverBarrageParIdCP**](#function-trouverbarrageparidcp) (int idCP) const<br>_Recherche d'un barrage situe sur ce CP._  |
|  const BarragePtr | [**trouverBarrageParIdCpAval**](#function-trouverbarrageparidcpaval) (int idCP) const<br>_Recherche d'un barrage ayant un CP particuler en aval._  |
|  const CarreauEntierPtr | [**trouverCarreauEntierParId**](#function-trouvercarreauentierparid) (int id) const<br>_Obtention d'un carreau entier selon son id._  |
|   | [**~BassinVersant**](#function-bassinversant) () <br> |




























## Detailed Description


Definition du bassin versant. NOTE: Il s'agit de la representation des donnees physiographiques PREPAREES. 


    
## Public Functions Documentation




### function BassinVersant 

_Constructeur._ 
```C++
BassinVersant::BassinVersant () 
```




<hr>



### function barrages 

_Getter barrage._ 
```C++
std::vector< BarragePtr > & BassinVersant::barrages () 
```




<hr>



### function carreauxEntiers 

_Getter liste des carreaux entiers._ 
```C++
const std::vector< CarreauEntierPtr > & BassinVersant::carreauxEntiers () const
```




<hr>



### function carreauxPartiels 

_Getter liste des carreaux partiels._ 
```C++
const std::vector< CarreauPartielPtr > & BassinVersant::carreauxPartiels () const
```




<hr>



### function initialiser 

_Initialisation du bassin versant a partir d'un variable Matlab (mxArray)_ 
```C++
void BassinVersant::initialiser (
    const mxArray * bassinVersant
) 
```




<hr>



### function initialiserBarrage [1/3]

_Initialisation barrage type 2._ 
```C++
void BassinVersant::initialiserBarrage (
    int idCP,
    int idCPAval,
    float volumeInitial,
    ParamEquation & coeffNiveau,
    ParamEquation & coeffDebit
) 
```




<hr>



### function initialiserBarrage [2/3]

_Initialisation barrage type 5._ 
```C++
void BassinVersant::initialiserBarrage (
    int idCP,
    int idCPAval,
    float volumeInitial,
    ParamEquation & coeffNiveau,
    ParamEquation & coeffVolume,
    std::vector< ParamEquation > & coeffDebitInt,
    std::vector< InfoEvacuationExt > & coeffDebitExt
) 
```




<hr>



### function initialiserBarrage [3/3]

_Initialisation barrage type 3._ 
```C++
void BassinVersant::initialiserBarrage (
    int idCPAval,
    std::vector< float > & debitsExternesMesures
) 
```




<hr>



### function initialiserFichier 

_Initialisation du bassin versant a partir d'un fichier Matlab._ 
```C++
void BassinVersant::initialiserFichier (
    const std::string nomFichierintrants
) 
```




<hr>



### function nbBarrages 

_Getter nbBarrages._ 
```C++
int BassinVersant::nbBarrages () const
```




<hr>



### function nbCarreauxEntiers 

_Getter nbCarreauxEntiers._ 
```C++
int BassinVersant::nbCarreauxEntiers () const
```




<hr>



### function nbCarreauxPartiels 

_Getter nbCarreauxPartiels._ 
```C++
int BassinVersant::nbCarreauxPartiels () const
```




<hr>



### function nbCarreauxPartielsCheminLong 

_Getter nbCarreauxPartielsCheminLong._ 
```C++
int BassinVersant::nbCarreauxPartielsCheminLong () const
```




<hr>



### function nomBassinVersant 

_Getter nomBassinVersant._ 
```C++
std::string BassinVersant::nomBassinVersant () const
```




<hr>



### function possedeBarrage [1/2]

_Le bassin possede des barrages?_ 
```C++
bool BassinVersant::possedeBarrage () const
```




<hr>



### function possedeBarrage [2/2]

_Le CP possede un barrage?_ 
```C++
bool BassinVersant::possedeBarrage (
    int idCP
) const
```




<hr>



### function puits 

_Getter liste des puits._ 
```C++
const std::map< int, std::vector< PuitsPtr > > & BassinVersant::puits () const
```




<hr>



### function puitsParIdCE 

```C++
const std::vector< PuitsPtr > & BassinVersant::puitsParIdCE (
    int idCE
) const
```




<hr>



### function superficieCarreauEntier 

_Getter superficieCarreauEntier._ 
```C++
float BassinVersant::superficieCarreauEntier () const
```




<hr>



### function trouverBarrageParIdCP 

_Recherche d'un barrage situe sur ce CP._ 
```C++
const BarragePtr BassinVersant::trouverBarrageParIdCP (
    int idCP
) const
```




<hr>



### function trouverBarrageParIdCpAval 

_Recherche d'un barrage ayant un CP particuler en aval._ 
```C++
const BarragePtr BassinVersant::trouverBarrageParIdCpAval (
    int idCP
) const
```




<hr>



### function trouverCarreauEntierParId 

_Obtention d'un carreau entier selon son id._ 
```C++
const CarreauEntierPtr BassinVersant::trouverCarreauEntierParId (
    int id
) const
```




<hr>



### function ~BassinVersant 

```C++
BassinVersant::~BassinVersant () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/BassinVersant.h`

