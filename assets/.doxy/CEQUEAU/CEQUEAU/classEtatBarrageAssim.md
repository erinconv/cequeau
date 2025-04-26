

# Class EtatBarrageAssim



[**ClassList**](annotated.md) **>** [**EtatBarrageAssim**](classEtatBarrageAssim.md)



_Classe de donnees pour l'assimilation des etats d'un barrage._ [More...](#detailed-description)

* `#include <Simulation.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  int | [**idCarreauPartiel**](#variable-idcarreaupartiel)  <br> |
|  float | [**niveau**](#variable-niveau)  <br> |
|  TypeAssim | [**niveauType**](#variable-niveautype)  <br> |
|  float | [**volume**](#variable-volume)  <br> |
|  TypeAssim | [**volumeType**](#variable-volumetype)  <br> |












































## Detailed Description


Utilise pour assimiler des etats aux barrage. Une valeur: on substitue cette valeur. Deux valeurs: La premiere est un facteur multiplicatif et la seconde une valeur a additionner. 


    
## Public Attributes Documentation




### variable idCarreauPartiel 

```C++
int EtatBarrageAssim::idCarreauPartiel;
```




<hr>



### variable niveau 

```C++
float EtatBarrageAssim::niveau[2];
```




<hr>



### variable niveauType 

```C++
TypeAssim EtatBarrageAssim::niveauType;
```




<hr>



### variable volume 

```C++
float EtatBarrageAssim::volume[2];
```




<hr>



### variable volumeType 

```C++
TypeAssim EtatBarrageAssim::volumeType;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/Simulation.h`

