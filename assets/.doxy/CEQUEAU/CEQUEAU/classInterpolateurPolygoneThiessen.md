

# Class InterpolateurPolygoneThiessen



[**ClassList**](annotated.md) **>** [**InterpolateurPolygoneThiessen**](classInterpolateurPolygoneThiessen.md)








Inherits the following classes: [Interpolateur](classInterpolateur.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**InterpolateurPolygoneThiessen**](#function-interpolateurpolygonethiessen) (const [**BassinVersant**](classBassinVersant.md) & bv, const [**ListeStation**](classListeStation.md) & stations, const [**Parametres**](classParametres.md) & parametres, const [**ParametresInterpolation**](classParametresInterpolation.md) & paramInterpolation, const [**DonneesMeteo**](classDonneesMeteo.md) & meteoStations, const std::vector&lt; std::string &gt; & nomsAutresMeteo) <br> |
| virtual void | [**executer**](#function-executer) () <br> |
|   | [**~InterpolateurPolygoneThiessen**](#function-interpolateurpolygonethiessen) () <br> |


## Public Functions inherited from Interpolateur

See [Interpolateur](classInterpolateur.md)

| Type | Name |
| ---: | :--- |
|   | [**Interpolateur**](classInterpolateur.md#function-interpolateur) (const [**BassinVersant**](classBassinVersant.md) & bv, const [**ListeStation**](classListeStation.md) & stations, const [**Parametres**](classParametres.md) & parametres, const [**ParametresInterpolation**](classParametresInterpolation.md) & paramInterpolation, const [**DonneesMeteo**](classDonneesMeteo.md) & meteoStations, const std::vector&lt; std::string &gt; & nomsAutresMeteo, int nbStation) <br>_Constructeur._  |
| virtual void | [**executer**](classInterpolateur.md#function-executer) () = 0<br> |
|  mxArray \* | [**obtenirMeteoInterpolee**](classInterpolateur.md#function-obtenirmeteointerpolee) () <br> |
| virtual  | [**~Interpolateur**](classInterpolateur.md#function-interpolateur) () <br> |
















## Protected Attributes inherited from Interpolateur

See [Interpolateur](classInterpolateur.md)

| Type | Name |
| ---: | :--- |
|  const [**BassinVersant**](classBassinVersant.md) & | [**bassinVersant\_**](classInterpolateur.md#variable-bassinversant_)  <br>_Definition du bassin versant._  |
|  std::vector&lt; std::vector&lt; DonneesCarreaux &gt; &gt; | [**donneesCarreauxTemps\_**](classInterpolateur.md#variable-donneescarreauxtemps_)  <br>_Liste des donnees necessaires pour interpoler la meteo a chaque CE et pas de temps._  |
|  std::vector&lt; MeteoGrille &gt; | [**meteoInterpolee\_**](classInterpolateur.md#variable-meteointerpolee_)  <br>_Donnees meteo interpolees._  |
|  const [**DonneesMeteo**](classDonneesMeteo.md) & | [**meteoStations\_**](classInterpolateur.md#variable-meteostations_)  <br>_Donnees meteo aux stations._  |
|  const int | [**nbStation\_**](classInterpolateur.md#variable-nbstation_)  <br> |
|  const std::vector&lt; std::string &gt; & | [**nomsAutresMeteo\_**](classInterpolateur.md#variable-nomsautresmeteo_)  <br>_Noms des champs meteo supplementaires._  |
|  const [**ParametresInterpolation**](classParametresInterpolation.md) & | [**paramInterpolation\_**](classInterpolateur.md#variable-paraminterpolation_)  <br>[_**Parametres**_](classParametres.md) _de l'interpolation._ |
|  const [**Parametres**](classParametres.md) & | [**parametres\_**](classInterpolateur.md#variable-parametres_)  <br>[_**Parametres**_](classParametres.md) _de Cequeau Quantite._ |
|  const [**ListeStation**](classListeStation.md) & | [**stations\_**](classInterpolateur.md#variable-stations_)  <br>_Liste des stations meteo._  |
































## Protected Functions inherited from Interpolateur

See [Interpolateur](classInterpolateur.md)

| Type | Name |
| ---: | :--- |
| virtual void | [**calculerCorrections**](classInterpolateur.md#function-calculercorrections) (int idxTypeMeteo, int pasDeTemps) = 0<br> |
|  void | [**initialiserListeCE**](classInterpolateur.md#function-initialiserlistece) () <br> |
| virtual void | [**interpoler**](classInterpolateur.md#function-interpoler) (int pasDeTemps) = 0<br> |
|  int | [**obtenirNombreAutreMeteo**](classInterpolateur.md#function-obtenirnombreautremeteo) () <br> |
|  void | [**trouverStationPlusPres**](classInterpolateur.md#function-trouverstationpluspres) (int idxTypeMeteo, int pasDeTemps) <br> |






## Public Functions Documentation




### function InterpolateurPolygoneThiessen 

```C++
InterpolateurPolygoneThiessen::InterpolateurPolygoneThiessen (
    const BassinVersant & bv,
    const ListeStation & stations,
    const Parametres & parametres,
    const ParametresInterpolation & paramInterpolation,
    const DonneesMeteo & meteoStations,
    const std::vector< std::string > & nomsAutresMeteo
) 
```




<hr>



### function executer 

```C++
virtual void InterpolateurPolygoneThiessen::executer () 
```



Implements [*Interpolateur::executer*](classInterpolateur.md#function-executer)


<hr>



### function ~InterpolateurPolygoneThiessen 

```C++
InterpolateurPolygoneThiessen::~InterpolateurPolygoneThiessen () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/InterpolateurPolygoneThiessen.h`

