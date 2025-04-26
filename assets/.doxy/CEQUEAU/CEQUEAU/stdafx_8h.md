

# File stdafx.h



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**stdafx.h**](stdafx_8h.md)





* `#include "targetver.h"`
* `#include <unistd.h>`
* `#include <algorithm>`
* `#include <cstring>`
* `#include <cmath>`
* `#include <fstream>`
* `#include <iomanip>`
* `#include <iostream>`
* `#include <climits>`
* `#include <map>`
* `#include <sstream>`
* `#include <vector>`
* `#include <ctime>`
* `#include <chrono>`
* `#include <array>`
* `#include <memory>`
* `#include <string>`
* `#include "DateChrono.h"`
* `#include "log.h"`
* `#include "MexHelper.hpp"`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef int | [**matError**](#typedef-materror)  <br> |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  const float | [**FUSEAU\_HORAIRE**](#variable-fuseau_horaire)   = `-4.0f`<br> |
|  const float | [**PI**](#variable-pi)   = `3.1416f`<br> |











































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**GetCurrentDir**](stdafx_8h.md#define-getcurrentdir)  `getcwd`<br> |
| define  | [**absf**](stdafx_8h.md#define-absf) (float\_a) `std::abs&lt;float&gt;(float\_a)`<br> |
| define  | [**maxf**](stdafx_8h.md#define-maxf) (float\_a, float\_b) `std::max&lt;float&gt;(float\_a,float\_b)`<br> |
| define  | [**minf**](stdafx_8h.md#define-minf) (float\_a, float\_b) `std::min&lt;float&gt;(float\_a,float\_b)`<br> |

## Public Types Documentation




### typedef matError 

```C++
typedef int matError;
```




<hr>
## Public Attributes Documentation




### variable FUSEAU\_HORAIRE 

```C++
const float FUSEAU_HORAIRE;
```




<hr>



### variable PI 

```C++
const float PI;
```




<hr>
## Macro Definition Documentation





### define GetCurrentDir 

```C++
#define GetCurrentDir `getcwd`
```




<hr>



### define absf 

```C++
#define absf (
    float_a
) `std::abs<float>(float_a)`
```




<hr>



### define maxf 

```C++
#define maxf (
    float_a,
    float_b
) `std::max<float>(float_a,float_b)`
```




<hr>



### define minf 

```C++
#define minf (
    float_a,
    float_b
) `std::min<float>(float_a,float_b)`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/stdafx.h`

