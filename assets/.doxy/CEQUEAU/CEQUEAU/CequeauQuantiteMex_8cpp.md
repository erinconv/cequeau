

# File CequeauQuantiteMex.cpp



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**CequeauQuantiteMex.cpp**](CequeauQuantiteMex_8cpp.md)





* `#include "stdafx.h"`
* `#include "BassinVersant.h"`
* `#include "Simulation.h"`
* `#include "Parametres.h"`
* `#include "DonneesMeteo.h"`
* `#include "FonteCequeau.h"`
* `#include "EvapoCequeau.h"`
* `#include "ParametresQualite.h"`
* `#include "SimulationQualite.h"`
* `#include "DLI.h"`
* `#include "FonteTemplate.h"`
* `#include "FonteUEB.h"`
* `#include "EvapoTemplate.h"`
* `#include "EvapoKPenman.h"`
* `#include "EvapoPriestleyTaylor.h"`
* `#include "EvapoPenmanMont.h"`
* `#include "EvapoMcGuinness.h"`
* `#include "FonteCemaNeige.h"`
* `#include "EvapoMorton.h"`
* `#include "InterpolateurPolygoneThiessen.h"`
* `#include "InterpolateurPonderationStations.h"`
* `#include "ParametresInterpolation.h"`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::shared\_ptr&lt; [**Interpolateur**](classInterpolateur.md) &gt; | [**InterpolateurPtr**](#typedef-interpolateurptr)  <br> |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**idMex**](#variable-idmex)   = `"Cequeau Quantite v4.6.1 et Qualite v2.0.0"`<br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**mexFunction**](#function-mexfunction) (int nlhs, mxArray \* plhs, int nrhs, const mxArray \* prhs) <br> |




























## Public Types Documentation




### typedef InterpolateurPtr 

```C++
typedef std::shared_ptr<Interpolateur> InterpolateurPtr;
```




<hr>
## Public Attributes Documentation




### variable idMex 

```C++
const std::string idMex;
```




<hr>
## Public Functions Documentation




### function mexFunction 

```C++
void mexFunction (
    int nlhs,
    mxArray * plhs,
    int nrhs,
    const mxArray * prhs
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/CequeauQuantiteMex.cpp`

