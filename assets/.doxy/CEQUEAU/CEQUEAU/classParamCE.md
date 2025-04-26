

# Class ParamCE



[**ClassList**](annotated.md) **>** [**ParamCE**](classParamCE.md)



[_**Parametres**_](classParametres.md) _spatialisables._[More...](#detailed-description)

* `#include <Parametres.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  float | [**coeffEmmagasinement**](#variable-coeffemmagasinement)  <br> |
|  float | [**coeffInfiltrationNappe**](#variable-coeffinfiltrationnappe)  <br>_Coefficient d'infiltration dans le rï¿½servoir NAPPE._  |
|  float | [**coeffVidangeBasseNappe**](#variable-coeffvidangebassenappe)  <br>_Coefficient de vidange basse du rï¿½servoir NAPPE._  |
|  float | [**coeffVidangeHauteNappe**](#variable-coeffvidangehautenappe)  <br>_Coefficient de vidange haute du rï¿½servoir NAPPE._  |
|  float | [**coeffVidangeIntermediaireSol**](#variable-coeffvidangeintermediairesol)  <br>_Coefficient de vidange intermï¿½diaire du rï¿½servoir SOL._  |
|  float | [**conductiviteHydraulique**](#variable-conductivitehydraulique)  <br> |
|  float | [**fractionImpermeableCE**](#variable-fractionimpermeablece)  <br>_Fraction de surface impermï¿½able des carreaux entiers (de 0.0 ï¿½ 1.0)._  |
|  float | [**hauteurReservoirSol**](#variable-hauteurreservoirsol)  <br>_Hauteur du rï¿½servoir SOL (mm)._  |
|  float | [**lameEauDebutRuisellement**](#variable-lameeaudebutruisellement)  <br>_Lame d'eau nï¿½cessaire pour que dï¿½bute le ruissellement sur les surfaces impermeables (mm)._  |
|  float | [**seuilInfiltrationSolVersNappe**](#variable-seuilinfiltrationsolversnappe)  <br>_Seuil d'infiltration du rï¿½servoir SOL vers le rï¿½servoir NAPPE (mm)._  |
|  float | [**seuilPrelevementEauTauxPotentiel**](#variable-seuilprelevementeautauxpotentiel)  <br>_Seuil de prï¿½lï¿½vement de l'eau ï¿½ taux potentiel, par ï¿½vapotranspiration (mm)._  |
|  float | [**seuilTempFonteClairiere**](#variable-seuiltempfonteclairiere)  <br>_Seuil de tempï¿½rature de fonte en clairiere (degC)._  |
|  float | [**seuilTempFonteForet**](#variable-seuiltempfonteforet)  <br>_Seuil de tempï¿½rature de fonte en foret (degC)._  |
|  float | [**seuilTranformationPluieNeige**](#variable-seuiltranformationpluieneige)  <br>_Seuil de transformation pluie-neige (degC)._  |
|  float | [**seuilVidangeHauteNappe**](#variable-seuilvidangehautenappe)  <br>_Seuil de vidange supï¿½rieure du rï¿½servoir NAPPE (mm)._  |
|  float | [**seuilVidangeIntermediaireSol**](#variable-seuilvidangeintermediairesol)  <br>_Seuil de vidange intermï¿½diaire du rï¿½servoir SOL (mm)._  |
|  float | [**tauxPotentielFonteClairiere**](#variable-tauxpotentielfonteclairiere)  <br>_Taux potentiel de fonte en clairiï¿½re (mm/degC/jour)._  |
|  float | [**tauxPotentielFonteForet**](#variable-tauxpotentielfonteforet)  <br>_Taux potentiel de fonte en forï¿½t (mm/degC/jour)._  |
|  float | [**tempMurissementNeige**](#variable-tempmurissementneige)  <br>_Temperature du murissement du stock de neige (degC)._  |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ParamCE**](#function-paramce) () <br> |




























## Detailed Description


[**Parametres**](classParametres.md) obligatoires dont la valeur peut etre modifiee


pour chaque carreau entier a l'aide de vecteurs facultatifs. 


    
## Public Attributes Documentation




### variable coeffEmmagasinement 

```C++
float ParamCE::coeffEmmagasinement;
```




<hr>



### variable coeffInfiltrationNappe 

_Coefficient d'infiltration dans le rï¿½servoir NAPPE._ 
```C++
float ParamCE::coeffInfiltrationNappe;
```




<hr>



### variable coeffVidangeBasseNappe 

_Coefficient de vidange basse du rï¿½servoir NAPPE._ 
```C++
float ParamCE::coeffVidangeBasseNappe;
```




<hr>



### variable coeffVidangeHauteNappe 

_Coefficient de vidange haute du rï¿½servoir NAPPE._ 
```C++
float ParamCE::coeffVidangeHauteNappe;
```




<hr>



### variable coeffVidangeIntermediaireSol 

_Coefficient de vidange intermï¿½diaire du rï¿½servoir SOL._ 
```C++
float ParamCE::coeffVidangeIntermediaireSol;
```




<hr>



### variable conductiviteHydraulique 

```C++
float ParamCE::conductiviteHydraulique;
```




<hr>



### variable fractionImpermeableCE 

_Fraction de surface impermï¿½able des carreaux entiers (de 0.0 ï¿½ 1.0)._ 
```C++
float ParamCE::fractionImpermeableCE;
```




<hr>



### variable hauteurReservoirSol 

_Hauteur du rï¿½servoir SOL (mm)._ 
```C++
float ParamCE::hauteurReservoirSol;
```




<hr>



### variable lameEauDebutRuisellement 

_Lame d'eau nï¿½cessaire pour que dï¿½bute le ruissellement sur les surfaces impermeables (mm)._ 
```C++
float ParamCE::lameEauDebutRuisellement;
```




<hr>



### variable seuilInfiltrationSolVersNappe 

_Seuil d'infiltration du rï¿½servoir SOL vers le rï¿½servoir NAPPE (mm)._ 
```C++
float ParamCE::seuilInfiltrationSolVersNappe;
```




<hr>



### variable seuilPrelevementEauTauxPotentiel 

_Seuil de prï¿½lï¿½vement de l'eau ï¿½ taux potentiel, par ï¿½vapotranspiration (mm)._ 
```C++
float ParamCE::seuilPrelevementEauTauxPotentiel;
```




<hr>



### variable seuilTempFonteClairiere 

_Seuil de tempï¿½rature de fonte en clairiere (degC)._ 
```C++
float ParamCE::seuilTempFonteClairiere;
```




<hr>



### variable seuilTempFonteForet 

_Seuil de tempï¿½rature de fonte en foret (degC)._ 
```C++
float ParamCE::seuilTempFonteForet;
```




<hr>



### variable seuilTranformationPluieNeige 

_Seuil de transformation pluie-neige (degC)._ 
```C++
float ParamCE::seuilTranformationPluieNeige;
```




<hr>



### variable seuilVidangeHauteNappe 

_Seuil de vidange supï¿½rieure du rï¿½servoir NAPPE (mm)._ 
```C++
float ParamCE::seuilVidangeHauteNappe;
```




<hr>



### variable seuilVidangeIntermediaireSol 

_Seuil de vidange intermï¿½diaire du rï¿½servoir SOL (mm)._ 
```C++
float ParamCE::seuilVidangeIntermediaireSol;
```




<hr>



### variable tauxPotentielFonteClairiere 

_Taux potentiel de fonte en clairiï¿½re (mm/degC/jour)._ 
```C++
float ParamCE::tauxPotentielFonteClairiere;
```




<hr>



### variable tauxPotentielFonteForet 

_Taux potentiel de fonte en forï¿½t (mm/degC/jour)._ 
```C++
float ParamCE::tauxPotentielFonteForet;
```




<hr>



### variable tempMurissementNeige 

_Temperature du murissement du stock de neige (degC)._ 
```C++
float ParamCE::tempMurissementNeige;
```




<hr>
## Public Functions Documentation




### function ParamCE 

```C++
ParamCE::ParamCE () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/Parametres.h`

