

# File conversionParametresV2-V4.m



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**conversionParametresV2-V4.m**](conversionParametresV2-V4_8m.md)


























## Public Attributes

| Type | Name |
| ---: | :--- |
|  parametresWork fonte | [**cequeau**](#variable-cequeau)   = `paramIn.neige`<br> |
|  parametresWork evapo cequeau | [**evnap**](#variable-evnap)   = `paramIn.sol.evnap`<br> |
|  parametresWork evapo cequeau | [**joeva**](#variable-joeva)   = `paramIn.option.joeva`<br> |
|  parametresWork fonte cequeau | [**jonei**](#variable-jonei)   = `paramIn.option.jonei`<br> |
|  parametresWork option | [**moduleFonte**](#variable-modulefonte)   = `1`<br> |
|  parametresWork | [**option**](#variable-option)   = `rmfield(paramIn.option, {'calculFonteExterne', 'jonei', 'joeva'})`<br> |
|  function | [**paramconv**](#variable-paramconv)   = `/* multi line expression */`<br> |
|   | [**parametresWork**](#variable-parametreswork)   = `rmfield(paramIn, 'neige')`<br> |
|  parametresWork | [**sol**](#variable-sol)   = `rmfield(paramIn.sol, {'evnap', 'xaa', 'xit'})`<br> |
|  parametresWork | [**solInitial**](#variable-solinitial)   = `rmfield(paramIn.solInitial, {'tmur', 'tstock'})`<br> |
|  parametresWork fonte cequeau | [**tmur**](#variable-tmur)   = `paramIn.solInitial.tmur`<br> |
|  parametresWork fonte cequeau | [**tstock**](#variable-tstock)   = `paramIn.solInitial.tstock`<br> |
|  parametresWork evapo cequeau | [**xaa**](#variable-xaa)   = `paramIn.sol.xaa`<br> |
|  parametresWork evapo cequeau | [**xit**](#variable-xit)   = `paramIn.sol.xit`<br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  catch err | [**disp**](#function-disp) (getReport(err, 'extended')) <br> |
|   | [**disp**](#function-disp) ('CONVERSION INCOMPLETE') <br> |




























## Public Attributes Documentation




### variable cequeau 

```Objective-C
parametresWork fonte cequeau;
```




<hr>



### variable evnap 

```Objective-C
parametresWork evapo cequeau evnap;
```




<hr>



### variable joeva 

```Objective-C
parametresWork evapo cequeau joeva;
```




<hr>



### variable jonei 

```Objective-C
parametresWork fonte cequeau jonei;
```




<hr>



### variable moduleFonte 

```Objective-C
parametresWork option moduleFonte;
```




<hr>



### variable option 

```Objective-C
parametresWork option;
```




<hr>



### variable paramconv 

```Objective-C
function paramconv;
```




<hr>



### variable parametresWork 

```Objective-C
parametresWork;
```




<hr>



### variable sol 

```Objective-C
parametresWork sol;
```




<hr>



### variable solInitial 

```Objective-C
parametresWork solInitial;
```




<hr>



### variable tmur 

```Objective-C
parametresWork fonte cequeau tmur;
```




<hr>



### variable tstock 

```Objective-C
parametresWork fonte cequeau tstock;
```




<hr>



### variable xaa 

```Objective-C
parametresWork evapo cequeau xaa;
```




<hr>



### variable xit 

```Objective-C
parametresWork evapo cequeau xit;
```




<hr>
## Public Functions Documentation




### function disp 

```Objective-C
catch err disp (
    getReport(err, 'extended')
) 
```




<hr>



### function disp 

```Objective-C
disp (
    'CONVERSION INCOMPLETE'
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/conversionParametresV2-V4.m`

