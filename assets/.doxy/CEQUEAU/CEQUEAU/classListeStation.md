

# Class ListeStation



[**ClassList**](annotated.md) **>** [**ListeStation**](classListeStation.md)



_Definition d'une liste de stations meteo._ 

* `#include <ListeStation.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ListeStation**](#function-listestation) () <br>_Constructeur._  |
|  void | [**initialiser**](#function-initialiser-12) () <br> |
|  void | [**initialiser**](#function-initialiser-22) (const mxArray \* stations) <br> |
|  int | [**quantiteStations**](#function-quantitestations) () const<br> |
|  const Stations & | [**stations**](#function-stations) () const<br> |
|  const StationPtr | [**trouverStation**](#function-trouverstation) (int id) const<br> |
|   | [**~ListeStation**](#function-listestation) () <br> |




























## Public Functions Documentation




### function ListeStation 

_Constructeur._ 
```C++
ListeStation::ListeStation () 
```




<hr>



### function initialiser [1/2]

```C++
void ListeStation::initialiser () 
```




<hr>



### function initialiser [2/2]

```C++
void ListeStation::initialiser (
    const mxArray * stations
) 
```




<hr>



### function quantiteStations 

```C++
int ListeStation::quantiteStations () const
```




<hr>



### function stations 

```C++
const Stations & ListeStation::stations () const
```




<hr>



### function trouverStation 

```C++
const StationPtr ListeStation::trouverStation (
    int id
) const
```




<hr>



### function ~ListeStation 

```C++
ListeStation::~ListeStation () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/ListeStation.h`

