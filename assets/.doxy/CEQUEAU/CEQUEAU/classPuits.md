

# Class Puits



[**ClassList**](annotated.md) **>** [**Puits**](classPuits.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Puits**](#function-puits) (int id, int active, double distance, double niveauInitial, double rayonP, std::vector&lt; double &gt; niveaux, std::vector&lt; double &gt; debit) <br> |
|  int | [**getActive**](#function-getactive) () const<br> |
|  const std::vector&lt; double &gt; & | [**getDebitPompage**](#function-getdebitpompage) () const<br> |
|  double | [**getDebitPompageParIndex**](#function-getdebitpompageparindex) (int idxPasDeTemps) const<br> |
|  double | [**getDistanceRiviere**](#function-getdistanceriviere) () const<br> |
|  int | [**getIdCE**](#function-getidce) () const<br> |
|  double | [**getNiveauInitial**](#function-getniveauinitial) () const<br> |
|  double | [**getNiveauPuitsParIndex**](#function-getniveaupuitsparindex) (int idxPasDeTemps) const<br> |
|  const std::vector&lt; double &gt; & | [**getNiveauxPuits**](#function-getniveauxpuits) () const<br> |
|  double | [**getRayonP**](#function-getrayonp) () const<br> |




























## Public Functions Documentation




### function Puits 

```C++
Puits::Puits (
    int id,
    int active,
    double distance,
    double niveauInitial,
    double rayonP,
    std::vector< double > niveaux,
    std::vector< double > debit
) 
```




<hr>



### function getActive 

```C++
int Puits::getActive () const
```




<hr>



### function getDebitPompage 

```C++
const std::vector< double > & Puits::getDebitPompage () const
```




<hr>



### function getDebitPompageParIndex 

```C++
double Puits::getDebitPompageParIndex (
    int idxPasDeTemps
) const
```




<hr>



### function getDistanceRiviere 

```C++
double Puits::getDistanceRiviere () const
```




<hr>



### function getIdCE 

```C++
int Puits::getIdCE () const
```




<hr>



### function getNiveauInitial 

```C++
double Puits::getNiveauInitial () const
```




<hr>



### function getNiveauPuitsParIndex 

```C++
double Puits::getNiveauPuitsParIndex (
    int idxPasDeTemps
) const
```




<hr>



### function getNiveauxPuits 

```C++
const std::vector< double > & Puits::getNiveauxPuits () const
```




<hr>



### function getRayonP 

```C++
double Puits::getRayonP () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/Puits.h`

