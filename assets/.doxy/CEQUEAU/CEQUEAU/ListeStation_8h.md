

# File ListeStation.h



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**ListeStation.h**](ListeStation_8h.md)





* `#include "Station.h"`
* `#include <vector>`















## Classes

| Type | Name |
| ---: | :--- |
| class | [**ListeStation**](classListeStation.md) <br>_Definition d'une liste de stations meteo._  |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::shared\_ptr&lt; [**Station**](classStation.md) &gt; | [**StationPtr**](#typedef-stationptr)  <br>_"Smart pointer" de station._  |
| typedef std::vector&lt; StationPtr &gt; | [**Stations**](#typedef-stations)  <br> |
















































## Public Types Documentation




### typedef StationPtr 

_"Smart pointer" de station._ 
```C++
typedef std::shared_ptr<Station> StationPtr;
```




<hr>



### typedef Stations 

```C++
typedef std::vector<StationPtr> Stations;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/ListeStation.h`

