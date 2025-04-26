# Usage

The function is called in the following way:

```matlab
% General form
[outputs...] = CequeauQuantiteMex(inputs...);

% Specific example
[etatsCE, etatsCP, etatsFonte, etatsEvapo, etatsBarrage, pasDeTemps, ...
 avantAssimilationsCE, avantAssimilationsFonte, avanAssimilationEvapo, ...
 etatsQualCP, avAssimQual] = ...
    CequeauQuantiteMex(execution, parametres, bassinVersant, meteo, ...
                       etatsPrecedents, assimilations, stations);
```

Inputs:

-   execution

-   parametres

-   bassinVersant

-   meteo

-   etatsPrecedents

-   assimilations

-   stations

# Execution

The execution struct contains the start and end dates for the
simulation. These dates can be modified to simulate a specific subset of
the overall data range. However, it is important that the input data
matches the specified range.

This is important for:

-   All **meteo** data

-   **barrage** data (bassinVersant.barrage): **debit**

-   **Puits** data (bassinVersant.puits): **debitPompage** and
    **niveauxPuits**

The intersect function in Matlab/Octave
can find the matching date range, which can then be used to extract the
relevant dates, as shown below:

```matlab
struct_ex.execution.dateDebut = datenum(2005, 3, 1);
struct_ex.execution.dateFin = datenum(2005, 11, 31);

dates.sim = [struct_ex.execution.dateDebut:struct_ex.execution.dateFin]';

[C, I, J] = intersect(struct_ex.meteoPointGrille.t, dates.sim);
struct_ex.meteoPointGrille.t = struct_ex.meteoPointGrille.t(I);
struct_ex.meteoPointGrille.TMin = struct_ex.meteoPointGrille.TMin(I, :);
```

# DLI

DLI parameters need to be added within the structure in the latest
version of Cequeau. The `.mat` file `dli_params.mat` is included in the
`cequeau/tests/mat_files` folder.

```matlab
% parametres pour DLI
load('dli_params.mat');
structSMA.parametres.dli = dli_params;
```

# Pumping

## Overview

The pumping module takes into account the water being pumped out of the
ground for each whole square (CE) at each timestep. Pumping data is
provided through data about wells located on the river basin.

To turn on the pumping module, the option must be set to 1. Setting it
to 0 (default) will turn it off.

```matlab
StructFinal.parametres.option.modulePompage = 1; % 0 = off, 1 = on
```

Data about water wells is placed within the `bassinVersant` struct. Each
well needs to have the index of the CE, the distance from the well to
the river, the initial water level h0, all the water levels and the
pumping rate in m^3^ per day. There is also a variable to
activate/deactivate the well.

```matlab
% Initialize the structure to hold all wells
structPuits = struct();
numTimeSteps = size(meteo.dates, 1); % Example: Get number of timesteps

structPuits(1).idCE = 41; % index of whole square
structPuits(1).active = 1; % activate the well
structPuits(1).distanceRiviere = 0; % distance between well and river (m)
structPuits(1).h0 = 20; % initial water level (m)
structPuits(1).niveauxPuits = ones(numTimeSteps, 1) * 20; % water level at each timestep (m)
structPuits(1).debitPompage = Debit_Pomp.Well_1; % pumping rate m3/day at each timestep

structPuits(2).idCE = 1;
structPuits(2).active = 0; % deactivate the well
structPuits(2).distanceRiviere = 0; % distance between well and river (m)
structPuits(2).h0 = 20; % initial water level (m)
structPuits(2).niveauxPuits = ones(numTimeSteps, 1) * 20; % water level at each timestep (m)
structPuits(2).debitPompage = Debit_Pomp.Well_2; % pumping rate m3/day at each timestep

StructFinal.bassinVersant.puits = structPuits;
```

Other parameters are set within the `parametres` struct.

The `delai` parameter delays the effect of the pumping by the given
number of timesteps.

The `conductiviteHydraulique_s` is used to specify the hydraulic
conductivity. A single value can be given to assign the same values to
all whole squares, or a `1 x number of CE` array can be provided to assign
a value to each CE.

The `coeffPompage` is used to adjust the extracted water from the
reservoir (to be calibrated)

```matlab
% creating pumping parameter struct
StructFinal.parametres.pompage = struct();
StructFinal.parametres.pompage.delai = 1; % Delay in timesteps
StructFinal.parametres.pompage.coeffPompage= 0.01; % Calibration coefficient

% Hydraulic conductivity (K) examples:
% 1) a single value for all CEs
StructFinal.parametres.pompage.conductiviteHydraulique_s = 1; % m/day

% 2) a value for each CE, the index indicates the idCE [1 x numCE]
numCE = numel(StructFinal.bassinVersant.carreauxEntiers);
StructFinal.parametres.pompage.conductiviteHydraulique_s = ones(1, numCE); % m/day
```

### Theory and Equation

To take into account the wells distance from the river, the radius of
influence is calculated for each well during pumping. The following
variables and units are used:

-   Pumped discharge: $Q_i$ [m$^3$/day]
-   Hydraulic conductivity: $K$ [m/day]
-   Initial head in the pumping well: $h_0$ [m]
-   Hydraulic gradient: $\beta$
-   Radius of influence: $r_0$ [m]

$$r_{o} = \frac{Q_{i}}{2\pi Kh_{0}\beta}$$

The hydraulic gradient ($\beta$) is approximated as the mean surface slope of the
whole square.

To adjust the volume of water removed from the reservoir, `coeffPompage`
($\gamma$) is used:

$$Q_{extracted} = Q_{i} \times (1 - e^{- \gamma r_{o}})$$

### Pumping Code

The core logic for handling well (`Puits`) data and pumping calculations can be found primarily within the `src/Puits.cpp` source file, interacting with the main simulation loop.

# Shading

## Overview

There are two algorithms used to take into account the effects of
shading from trees. To activate and choose the algorithm, the
struct.parametres.option.moduleOmbrage option is used:

-   moduleOmbrage = 0: No shading algorithm is used.

-   moduleOmbrage = 1: Shading algorithm based on tree height is used.

-   moduleOmbrage = 2: Shading algorithm based on leaf area index (LAI)
    is used.

## Tree Height Shading Algorithm

### Usage

-   Set `moduleOmbrage = 1`

-   Add average tree height for each CP:
    `struct.bassinVersant.carreauxPartiels.hautMoyenneArbre`

The following example shows the average tree height for each CP being
set to 12 meters.

```matlab
structSMA.parametres.option.moduleOmbrage = 1;

% Assuming structSMA.bassinVersant.carreauxPartiels is already populated
% Set average tree height to 12m for all CPs
[structSMA.bassinVersant.carreauxPartiels(1:end).hautMoyenneArbre] = deal(12);

% Example function call (assuming other structs are defined)
[structSMA.etatsCE, structSMA.etatsCP, structSMA.etatsFonte, structSMA.etatsEvapo, structSMA.etatsBarrage, ...
 structSMA.pasDeTemps, structSMA.avantAssimilationsCE, structSMA.avantAssimilationsFonte, ...
 structSMA.avantAssimilationsEvapo, structSMA.etatsQualCP, structSMA.avAssimQual] = ...
    cequeauQuantiteMex(structSMA.execution, structSMA.parametres, ...
                       structSMA.bassinVersant, structSMA.meteoPointGrille, ...
                       [], [], [], []); % Assuming no initial states, etc.
```

### Algorithm

The following is calculated for each partial square (CP)

1.  Extract data: latitude, longitude, river width, orientation of the
    river, tree height, and date.

2.  Calculate solar position using SPA algorithm ([NREL's Solar Position
    Algorithm for Solar Radiations
    Applications](https://midcdmz.nrel.gov/spa/#agree))

3.  Difference between the sun's orientation and the river's orientation

$$\theta = sun^{'}s\ azimuth\ angle\ (rad) - river^{'}s\ azimuth\ angle\ (rad)$$

4.  Calculate average shadow length based on average tree height

> $$\omega = average\ shadow\ length$$
>
> $$\tau = average\ tree\ height$$

$$\varphi = solar\ altitude$$

$$\omega = \frac{\tau}{\tan(\varphi)}$$

5.  Calculate average shadow length perpendicular to the river

$$\mu = average\ perpendicular\ shadow\ length\ $$

$$\mu = sin(\theta) \bullet \omega$$

6.  Shadow ratio covering the river

$$\rho = shade\ ratio$$

$$\rho = \ \frac{\mu}{river\ width}$$

## LAI Shading Algorithm

### Usage

-   Set `moduleOmbrage = 2`

-   Obtain LAI data

    -   Data should be in the same format as weather data: \[timesteps x
        CE\]

-   Normalize LAI data using the provided normalize_lai MATLAB function

-   Add normalized data into the struct:
    `struct.meteoPointGrille.lai_norm`

```matlab
% normalize LAI values based on given percentile
percentile = 75;
structSMA.meteoPointGrille.lai_norm = normalize_lai(lai_data,
percentile);

structSMA.parametres.option.moduleOmbrage = 2;

[structSMA.etatsCE, structSMA.etatsCP, structSMA.etatsFonte, structSMA.etatsEvapo, structSMA.etatsBarrage,
structSMA.pasDeTemps,...
structSMA.avantAssimilationsCE, structSMA.avantAssimilationsFonte,
structSMA.avantAssimilationsEvapo,structSMA.etatsQualCP, structSMA.avAssimQual] = ...
cequeauQuantiteMex(structSMA.execution, structSMA.parametres,
structSMA.bassinVersant, ...
structSMA.meteoPointGrille, [], [], [], []);
```

### Algorithm

1.  LAI data is normalized in a range from 0 to 1, based on a given
    percentile value.

2.  The normalized LAI value is used to scale crayso
    (`struct.parametres.qualite.cequeau.temperat.crayso`)

3.  The scaled crayso value is used to calculate solar radiation.

### Code

```cpp
// Example C++ logic snippet (illustrative)
double normalized_lai = meteoPointGrille.lai_norm[i]; // Get normalized LAI for timestep i

// Reduce the maximum effect to avoid excessive reduction
if (normalized_lai > 0.8) {
    normalized_lai = 0.8;
}

// Scale the solar radiation coefficient
double original_crayso = parametres.qualite.cequeau.temperat.crayso;
double scaled_crayso = original_crayso * (1.0 - normalized_lai);

// ... use scaled_crayso in subsequent solar radiation calculations ...
```

The normalized LAI value is reduced to a maximum of 0.8 to prevent the
`scaled_crayso` to be reduced to less than 0. The `scaled_crayso` is then
used to calculate the solar radiation.

# Qualite Time Steps

## Overview

To use sub-daily time steps with the Cequeau Qualite module, the ipassim
variable in the input struct's option must be modified. The value can be
set to 1, 2, 3, 4, 6, 8, 12, and 24 (daily). This will change the time
steps for both the Quantite and Qualite module.

InputStruct.parametres.option.ipassim = 12;

*Note: data assimilation is only available for daily time steps.*

The calculQualite option must also be enabled to use the Qualite module.

InputStruct.parametres.option.calculQualite = 1;

It is also important to match simulated date range
(Struct.execution.dateDebut and Struct.execution.dateFin) to align with
the number of data points in the input structures, such as for
meteorological data and dam (barrage) data.

## Example Usage:

The file `cequeau/run/runCequeau_Qualite.m` demonstrates examples.

For now, testing file is linearly interpolating the input data
(`processInput.m`) to match the number of data points to test the
sub-daily water temperature model.

```matlab
% Example from testing script

% every 12 hours
InputStruct12 = processInput(InputStruct, indices, dateDebut, dateFin, 12);

[y12.etatsCE, y12.etatsCP, y12.etatsFonte, y12.etatsEvapo, ...
 y12.etatsBarrage, y12.pasDeTemps,...
 y12.avantAssimilationsCE, y12.avantAssimilationsFonte, ...
 y12.avantAssimilationsEvapo, y12.etatsQualCP, y12.avAssimQual] = ...
    cequeauQuantiteMex_test(InputStruct12.execution, ...
                          InputStruct12.parametres, ...
                          InputStruct12.bassinVersant, ...
                          InputStruct12.meteoPointGrille, ...
                          [], [], [], []);
```

The same process was used to also simulate using a time step of 6. The
following graphs demonstrate the results compared to actual water
temperature from stations.

![](/media/image3.png)

![](/media/image4.png)
