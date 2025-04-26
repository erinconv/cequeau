

# File log.h



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**log.h**](log_8h.md)





* `#include <sstream>`
* `#include <string>`
* `#include <stdio.h>`
* `#include <sys/time.h>`















## Classes

| Type | Name |
| ---: | :--- |
| class | [**FILELog**](classFILELog.md) <br> |
| class | [**Log**](classLog.md) &lt;typename T&gt;<br> |
| class | [**Output2FILE**](classOutput2FILE.md) <br> |


## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**TLogLevel**](#enum-tloglevel)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  std::string | [**NowTime**](#function-nowtime) () <br> |



























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**FILELOG\_DECLSPEC**](log_8h.md#define-filelog_declspec)  <br> |
| define  | [**FILELOG\_MAX\_LEVEL**](log_8h.md#define-filelog_max_level)  `logDEBUG4`<br> |
| define  | [**FILE\_LOG**](log_8h.md#define-file_log) (level) `/* multi line expression */`<br> |

## Public Types Documentation




### enum TLogLevel 

```C++
enum TLogLevel {
    logERROR,
    logWARNING,
    logINFO,
    logDEBUG,
    logDEBUG1,
    logDEBUG2,
    logDEBUG3,
    logDEBUG4
};
```




<hr>
## Public Functions Documentation




### function NowTime 

```C++
inline std::string NowTime () 
```




<hr>
## Macro Definition Documentation





### define FILELOG\_DECLSPEC 

```C++
#define FILELOG_DECLSPEC 
```




<hr>



### define FILELOG\_MAX\_LEVEL 

```C++
#define FILELOG_MAX_LEVEL `logDEBUG4`
```




<hr>



### define FILE\_LOG 

```C++
#define FILE_LOG (
    level
) `/* multi line expression */`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/log.h`

