

# File InterpolationMex.cpp



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**InterpolationMex.cpp**](InterpolationMex_8cpp.md)





* `#include "stdafx.h"`
* `#include "InterpolateurPolygoneThiessen.h"`
* `#include "InterpolateurPonderationStations.h"`
* `#include "ParametresInterpolation.h"`
* `#include "mex.h"`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::shared\_ptr&lt; [**Interpolateur**](classInterpolateur.md) &gt; | [**InterpolateurPtr**](#typedef-interpolateurptr)  <br> |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**idMex**](#variable-idmex)   = `"Cequeau Interpolation v1.0.1"`<br> |
















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
The documentation for this class was generated from the following file `src/InterpolationMex.cpp`

