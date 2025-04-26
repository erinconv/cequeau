

# Class Log

**template &lt;typename T&gt;**



[**ClassList**](annotated.md) **>** [**Log**](classLog.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|  std::ostringstream & | [**Get**](#function-get) (TLogLevel level=logINFO) <br> |
|   | [**Log**](#function-log-12) () <br> |
| virtual  | [**~Log**](#function-log) () <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  TLogLevel | [**FromString**](#function-fromstring) (const std::string & level) <br> |
|  TLogLevel & | [**ReportingLevel**](#function-reportinglevel) () <br> |
|  std::string | [**ToString**](#function-tostring) (TLogLevel level) <br> |






## Protected Attributes

| Type | Name |
| ---: | :--- |
|  std::ostringstream | [**os**](#variable-os)  <br> |




















## Public Functions Documentation




### function Get 

```C++
std::ostringstream & Log::Get (
    TLogLevel level=logINFO
) 
```




<hr>



### function Log [1/2]

```C++
Log::Log () 
```




<hr>



### function ~Log 

```C++
virtual Log::~Log () 
```




<hr>
## Public Static Functions Documentation




### function FromString 

```C++
static TLogLevel Log::FromString (
    const std::string & level
) 
```




<hr>



### function ReportingLevel 

```C++
static TLogLevel & Log::ReportingLevel () 
```




<hr>



### function ToString 

```C++
static std::string Log::ToString (
    TLogLevel level
) 
```




<hr>
## Protected Attributes Documentation




### variable os 

```C++
std::ostringstream Log< T >::os;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/log.h`

