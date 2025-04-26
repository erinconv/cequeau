

# Class ParamSol



[**ClassList**](annotated.md) **>** [**ParamSol**](classParamSol.md)



_Classe de donnees des parametres relatifs reservoirs SOL, NAPPE et MARAIS._ 

* `#include <Parametres.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  float | [**coeffCorrectionPluieAnAltitude**](#variable-coeffcorrectionpluieanaltitude)  <br>_Coefficient de correction des prï¿½cipitations annuelles en fonction de l'altitude (mm/mï¿½tre/an)._  |
|  std::vector&lt; float &gt; | [**coeffInfiltrationNappe**](#variable-coeffinfiltrationnappe)  <br>_Coefficient d'infiltration dans le rï¿½servoir NAPPE._  |
|  std::vector&lt; float &gt; | [**coeffVidangeBasseNappe**](#variable-coeffvidangebassenappe)  <br>_Coefficient de vidange basse du rï¿½servoir NAPPE._  |
|  float | [**coeffVidangeBasseSol**](#variable-coeffvidangebassesol)  <br>_Coefficient de vidange basse du rï¿½servoir SOL._  |
|  std::vector&lt; float &gt; | [**coeffVidangeHauteNappe**](#variable-coeffvidangehautenappe)  <br>_Coefficient de vidange haute du rï¿½servoir NAPPE._  |
|  std::vector&lt; float &gt; | [**coeffVidangeIntermediaireSol**](#variable-coeffvidangeintermediairesol)  <br>_Coefficient de vidange intermï¿½diaire du rï¿½servoir SOL._  |
|  float | [**coeffVidangeLacsMarais**](#variable-coeffvidangelacsmarais)  <br>_Coefficient de vidange du rï¿½servoir LACS et MARAIS._  |
|  float | [**correctionTempAltitude**](#variable-correctiontempaltitude)  <br>_Correction des tempï¿½ratures en fonction de l'altitude (degC/1 000 m)._  |
|  std::vector&lt; float &gt; | [**fractionImpermeableCE**](#variable-fractionimpermeablece)  <br>_Fraction de l'ï¿½vapotranspiration prise dans le rï¿½servoir NAPPE (de 0.0 a 1.0);._  |
|  std::vector&lt; float &gt; | [**hauteurReservoirSol**](#variable-hauteurreservoirsol)  <br>_Hauteur du rï¿½servoir SOL (mm)._  |
|  float | [**infiltrationMax**](#variable-infiltrationmax)  <br>_Infiltration maximale (mm/jour)._  |
|  std::vector&lt; float &gt; | [**lameEauDebutRuisellement**](#variable-lameeaudebutruisellement)  <br>_Lame d'eau nï¿½cessaire pour que dï¿½bute le ruissellement sur les surfaces impermeables (mm)._  |
|  float | [**latitudeMoyenneBV**](#variable-latitudemoyennebv)  <br>_Latitude moyenne du bassin versant en degrï¿½s et minutes sexagï¿½simales._  |
|  std::vector&lt; float &gt; | [**seuilInfiltrationSolVersNappe**](#variable-seuilinfiltrationsolversnappe)  <br>_Seuil d'infiltration du rï¿½servoir SOL vers le rï¿½servoir NAPPE (mm)._  |
|  std::vector&lt; float &gt; | [**seuilPrelevementEauTauxPotentiel**](#variable-seuilprelevementeautauxpotentiel)  <br>_Seuil de prï¿½lï¿½vement de l'eau ï¿½ taux potentiel, par ï¿½vapotranspiration (mm)._  |
|  std::vector&lt; float &gt; | [**seuilVidangeHauteNappe**](#variable-seuilvidangehautenappe)  <br>_Seuil de vidange supï¿½rieure du rï¿½servoir NAPPE (mm)._  |
|  std::vector&lt; float &gt; | [**seuilVidangeIntermediaireSol**](#variable-seuilvidangeintermediairesol)  <br>_Seuil de vidange intermï¿½diaire du rï¿½servoir SOL (mm)._  |
|  float | [**seuilVidangeLacsMarais**](#variable-seuilvidangelacsmarais)  <br>_Seuil de vidange du rï¿½servoir LACS et MARAIS (mm)._  |












































## Public Attributes Documentation




### variable coeffCorrectionPluieAnAltitude 

_Coefficient de correction des prï¿½cipitations annuelles en fonction de l'altitude (mm/mï¿½tre/an)._ 
```C++
float ParamSol::coeffCorrectionPluieAnAltitude;
```




<hr>



### variable coeffInfiltrationNappe 

_Coefficient d'infiltration dans le rï¿½servoir NAPPE._ 
```C++
std::vector<float> ParamSol::coeffInfiltrationNappe;
```




<hr>



### variable coeffVidangeBasseNappe 

_Coefficient de vidange basse du rï¿½servoir NAPPE._ 
```C++
std::vector<float> ParamSol::coeffVidangeBasseNappe;
```




<hr>



### variable coeffVidangeBasseSol 

_Coefficient de vidange basse du rï¿½servoir SOL._ 
```C++
float ParamSol::coeffVidangeBasseSol;
```




<hr>



### variable coeffVidangeHauteNappe 

_Coefficient de vidange haute du rï¿½servoir NAPPE._ 
```C++
std::vector<float> ParamSol::coeffVidangeHauteNappe;
```




<hr>



### variable coeffVidangeIntermediaireSol 

_Coefficient de vidange intermï¿½diaire du rï¿½servoir SOL._ 
```C++
std::vector<float> ParamSol::coeffVidangeIntermediaireSol;
```




<hr>



### variable coeffVidangeLacsMarais 

_Coefficient de vidange du rï¿½servoir LACS et MARAIS._ 
```C++
float ParamSol::coeffVidangeLacsMarais;
```




<hr>



### variable correctionTempAltitude 

_Correction des tempï¿½ratures en fonction de l'altitude (degC/1 000 m)._ 
```C++
float ParamSol::correctionTempAltitude;
```




<hr>



### variable fractionImpermeableCE 

_Fraction de l'ï¿½vapotranspiration prise dans le rï¿½servoir NAPPE (de 0.0 a 1.0);._ 
```C++
std::vector<float> ParamSol::fractionImpermeableCE;
```



Fraction de surface impermï¿½able des carreaux entiers (de 0.0 ï¿½ 1.0). 


        

<hr>



### variable hauteurReservoirSol 

_Hauteur du rï¿½servoir SOL (mm)._ 
```C++
std::vector<float> ParamSol::hauteurReservoirSol;
```




<hr>



### variable infiltrationMax 

_Infiltration maximale (mm/jour)._ 
```C++
float ParamSol::infiltrationMax;
```




<hr>



### variable lameEauDebutRuisellement 

_Lame d'eau nï¿½cessaire pour que dï¿½bute le ruissellement sur les surfaces impermeables (mm)._ 
```C++
std::vector<float> ParamSol::lameEauDebutRuisellement;
```




<hr>



### variable latitudeMoyenneBV 

_Latitude moyenne du bassin versant en degrï¿½s et minutes sexagï¿½simales._ 
```C++
float ParamSol::latitudeMoyenneBV;
```



(ex.: XLA = 4245 pour une latitude de 42o45'). 


        

<hr>



### variable seuilInfiltrationSolVersNappe 

_Seuil d'infiltration du rï¿½servoir SOL vers le rï¿½servoir NAPPE (mm)._ 
```C++
std::vector<float> ParamSol::seuilInfiltrationSolVersNappe;
```




<hr>



### variable seuilPrelevementEauTauxPotentiel 

_Seuil de prï¿½lï¿½vement de l'eau ï¿½ taux potentiel, par ï¿½vapotranspiration (mm)._ 
```C++
std::vector<float> ParamSol::seuilPrelevementEauTauxPotentiel;
```




<hr>



### variable seuilVidangeHauteNappe 

_Seuil de vidange supï¿½rieure du rï¿½servoir NAPPE (mm)._ 
```C++
std::vector<float> ParamSol::seuilVidangeHauteNappe;
```




<hr>



### variable seuilVidangeIntermediaireSol 

_Seuil de vidange intermï¿½diaire du rï¿½servoir SOL (mm)._ 
```C++
std::vector<float> ParamSol::seuilVidangeIntermediaireSol;
```




<hr>



### variable seuilVidangeLacsMarais 

_Seuil de vidange du rï¿½servoir LACS et MARAIS (mm)._ 
```C++
float ParamSol::seuilVidangeLacsMarais;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/Parametres.h`

