

# Class Qualite::EtatCarreauPartielAssim



[**ClassList**](annotated.md) **>** [**Qualite**](namespaceQualite.md) **>** [**EtatCarreauPartielAssim**](classQualite_1_1EtatCarreauPartielAssim.md)



_Classe de donnees pour l'assimilation des etats d'un carreau partiel._ [More...](#detailed-description)

* `#include <SimulationQualite.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  int | [**idCarreauPartiel**](#variable-idcarreaupartiel)  <br> |
|  float | [**temperature**](#variable-temperature)  <br> |
|  TypeAssim | [**temperatureType**](#variable-temperaturetype)  <br> |












































## Detailed Description


Utilise pour assimiler des etats aux carreaux partiels. Une valeur: on substitue cette valeur. Deux valeurs: La premiere est un facteur multiplicatif et la seconde une valeur a additionner. 


    
## Public Attributes Documentation




### variable idCarreauPartiel 

```C++
int Qualite::EtatCarreauPartielAssim::idCarreauPartiel;
```




<hr>



### variable temperature 

```C++
float Qualite::EtatCarreauPartielAssim::temperature[2];
```




<hr>



### variable temperatureType 

```C++
TypeAssim Qualite::EtatCarreauPartielAssim::temperatureType;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/SimulationQualite.h`

