

# File compileInterpolationOct.m



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**compileInterpolationOct.m**](compileInterpolationOct_8m.md)


























## Public Attributes

| Type | Name |
| ---: | :--- |
|   | [**COMPFLAGS**](#variable-compflags)   = `" "`<br> |
|   | [**CXXFLAGS**](#variable-cxxflags)   = `" -std=c++14 "`<br> |
|  if debug | [**DBG\_FLAG**](#variable-dbg_flag)   = `" -g "`<br> |
|   | [**FLAGS**](#variable-flags)   = `" -DENV\_OCTAVE "`<br> |
|  end | [**OUTFILE**](#variable-outfile)   = `" ../mex/cequeauInterpolationOct"`<br> |
|   | [**SOURCES**](#variable-sources)   = `[SOURCES 'Interpolateur.cpp ']`<br> |
|   | [**command**](#variable-command)   = `strcat("mkoctfile --mex ", FLAGS, CXXFLAGS, DBG\_FLAG, COMPFLAGS, " -o ", OUTFILE, " ", SOURCES)`<br> |
|   | [**dbg**](#variable-dbg)   = `"\_DBG "`<br> |
|  end | [**debug**](#variable-debug)   = `false`<br> |
|   | [**log**](#variable-log)   = `false`<br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  function | [**compileInterpolationOct**](#function-compileinterpolationoct) () <br> |
|   | [**disp**](#function-disp) (command) <br> |
|   | [**eval**](#function-eval) (command) <br> |




























## Public Attributes Documentation




### variable COMPFLAGS 

```Objective-C
if ~log COMPFLAGS;
```




<hr>



### variable CXXFLAGS 

```Objective-C
CXXFLAGS;
```




<hr>



### variable DBG\_FLAG 

```Objective-C
else DBG_FLAG;
```




<hr>



### variable FLAGS 

```Objective-C
FLAGS;
```




<hr>



### variable OUTFILE 

```Objective-C
OUTFILE;
```




<hr>



### variable SOURCES 

```Objective-C
SOURCES;
```




<hr>



### variable command 

```Objective-C
command;
```




<hr>



### variable dbg 

```Objective-C
dbg;
```




<hr>



### variable debug 

```Objective-C
end debug;
```




<hr>



### variable log 

```Objective-C
log;
```




<hr>
## Public Functions Documentation




### function compileInterpolationOct 

```Objective-C
function compileInterpolationOct () 
```




<hr>



### function disp 

```Objective-C
disp (
    command
) 
```




<hr>



### function eval 

```Objective-C
eval (
    command
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/compileInterpolationOct.m`

