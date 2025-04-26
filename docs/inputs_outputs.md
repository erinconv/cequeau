# CEQUEAU MEX Function Inputs and Outputs

!!! warning
    THIS DOCUMENTATION IS OUTDATED AND NEEDS CAREFUL REVISION

---

## `CequeauQuantiteMex`

This is the main function for running CEQUEAU streamflow and river temperature simulations.

!!! warning "Version Note"
    Starting from version 4.x.x, the `cequeauQuantiteMex` mexfile is used for both streamflow (**quantity**) and river temperature (**quality**) simulations.
    - If `parametres.option.calculQualite = 0`, only the streamflow simulation is executed.
    - If `parametres.option.calculQualite = 1`, the streamflow simulation is executed first, followed by the river temperature simulation.

### Usage

```matlab
[etatsCE, etatsCP, etatsFonte, etatsEvapo, etatsBarrage, pasDeTemps, ...
 avantAssimilationsCE, avantAssimilationsFonte, avantAssimilationsEvapo, ...
 etatsQualCP, avAssimQual] = ...
    CequeauQuantiteMex(execution, parametres, bassinVersant, meteo, ...
                       etatsPrecedents, assimilations, stations);
```

!!! note
    - The `stations` input was added in v5.0.0.
    - If you do not wish to use `etatsPrecedents`, `assimilations`, or `stations`, provide an empty array (`[]`) for that input.

### Inputs (`Intrants`)

The required input variables are detailed below.

#### 1. `execution` (`struct`)

Defines the simulation period and optional output filtering.

-   `dateDebut`: `double` - Start date of the simulation (Matlab `datenum` format).
-   `dateFin`: `double` - End date of the simulation (Matlab `datenum` format).
-   `resultatsIdCE` (Optional): `[1 x N double]` - List of Whole Cell (CE) IDs for which outputs are desired in the `etatsCE` variable. If omitted, all CEs are output.
-   `resultatsIdCP` (Optional): `[1 x M double]` - List of Partial Cell (CP) IDs for which outputs are desired in the `etatsCP` variable. If omitted, all CPs are output.

#### 2. `parametres` (`struct`)

Contains all model parameters. **All fields are mandatory.** Refer to the main Cequeau User Guide for the physical meaning of these parameters.

```matlab
parametres:
  .option      % [1x1 struct] General simulation options
  .sol         % [1x1 struct] Soil parameters
  .solInitial  % [1x1 struct] Initial soil states
  .transfert   % [1x1 struct] Transfer parameters
  .ctp         % [1 x nbCP double] or 0 - CTP parameters (?)
  .lac         % [1 x nbCP double] or 0 - Lake parameters (?)
  .surface     % [1 x nbCP double] or 0 - Surface parameters (?)
  .fonte       % [1x1 struct] Snowmelt module parameters
  .evapo       % [1x1 struct] Evapotranspiration module parameters
  .qualite     % [1x1 struct] Water quality (temperature) module parameters
  .pompage     % [1x1 struct] Pumping module parameters
  .dli         % [1x1 struct] Downwelling Longwave Irradiation (DLI) parameters
```

!!! note "CTP, Lac, Surface Parameters"
    The original documentation for `parametres.ctp`, `parametres.lac`, and `parametres.surface` does not explicitly define CTP or how these parameters are used. They can be set to `0` if not used, otherwise, values must be provided for all Partial Cells (CPs).

##### `parametres.option` (`struct`)

-   `ipassim`: `double` - Assimilation time step divisor (e.g., 1 for daily, 12 for 12-hourly). See [Quality Time Steps](#qualité-time-steps) in the User Guide.
-   `moduleFonte`: `double` - Snowmelt model selection (1 = `cequeau`, 2 = `cemaNeige`, 3 = `ueb`, etc.).
-   `moduleEvapo`: `double` - Evapotranspiration model selection (1 = `cequeau`, others...). Needs clarification.
-   `calculQualite`: `double` - Enable water quality simulation (0 = quantity only, 1 = quantity and quality).
-   `modulePompage`: `double` - Enable pumping module (0 = off, 1 = on).
-   `moduleDLI`: `double` - Downwelling Longwave Irradiation model selection (0 to 10).
-   `moduleOmbrage`: `double` - Shading model selection (0 = no shading, 1 = tree height based, 2 = LAI based).

##### `parametres.sol` (`struct`)

!!! info "Spatially Variable Parameters (`_s` suffix)"
    Parameters ending with `_s` can be spatially variable. You can provide either:
    1.  A single `double` value: Applied uniformly to all Whole Cells (CEs).
    2.  A `[1 x nbCE double]` array: A specific value for each Whole Cell.
    If an array is provided but its size does not match `nbCE`, only the *first* value is used and applied uniformly.

-   `cin_s`: `double` or `[1 x nbCE double]`
-   `cvmar`: `double` or `[1 x nbCE double]`
-   `cvnb_s`: `double` or `[1 x nbCE double]`
-   `cvnh_s`: `double` or `[1 x nbCE double]`
-   `cvsb`: `double`
-   `cvsi_s`: `double` or `[1 x nbCE double]`
-   `xinfma`: `double`
-   `hinf_s`: `double` or `[1 x nbCE double]`
-   `hint_s`: `double` or `[1 x nbCE double]`
-   `hmar`: `double`
-   `hnap_s`: `double` or `[1 x nbCE double]`
-   `hpot_s`: `double` or `[1 x nbCE double]`
-   `hsol_s`: `double` or `[1 x nbCE double]`
-   `hrimp_s`: `double` or `[1 x nbCE double]`
-   `tri_s`: `double` or `[1 x nbCE double]`
-   `xla`: `double`

##### `parametres.solInitial` (`struct`)

Note: These values are **not** used if initial states are loaded via the `etatsPrecedents` input.

-   `hsini`: `double`
-   `hnini`: `double`
-   `hmini`: `double`
-   `q0`: `double`

##### `parametres.transfert` (`struct`)

-   `exxkt`: `double`
-   `zn`: `double`

##### `parametres.ctp`, `parametres.lac`, `parametres.surface`

-   Each is `0` or `[1 x nbCP double]`. (See note above regarding their definition).
-   If set to `0`, the feature is not used. If an array is provided, values must be specified for all Partial Cells (use 0 for CPs where the parameter is not applicable or should be ignored).

##### `parametres.fonte` (`struct`)

Contains parameters specific to the selected snowmelt model (`moduleFonte`). Parameters for each model are nested under a field matching the model name (e.g., `parametres.fonte.cequeau`).

###### `parametres.fonte.cequeau` (`struct`)

(Spatially variable parameters (`_s`) follow the same rules as in `parametres.sol`)

-   `strne_s`: `double` or `[1 x nbCE double]`
-   `tfc_s`: `double` or `[1 x nbCE double]`
-   `tfd_s`: `double` or `[1 x nbCE double]`
-   `tsc_s`: `double` or `[1 x nbCE double]`
-   `tsd_s`: `double` or `[1 x nbCE double]`
-   `ttd`: `double`
-   `tts_s`: `double` or `[1 x nbCE double]`
-   `jonei`: `double`
-   `tmur`: `double`
-   `tstock`: `double`

##### `parametres.evapo` (`struct`)

Contains parameters specific to the selected evapotranspiration model (`moduleEvapo`). Parameters are nested (e.g., `parametres.evapo.cequeau`).

###### `parametres.evapo.cequeau` (`struct`)

-   `joeva`: `double`
-   `evnap`: `double`
-   `xaa`: `double`
-   `xit`: `double`

##### `parametres.qualite` (`struct`)

Parameters specific to the water quality (temperature) simulation.

###### `parametres.qualite.cequeau` (`struct`)

!!! note "See Cequeau Guide"
    Refer to the main Cequeau guide for the physical meaning of quality parameters.

-   `paramode`: `[1 x 1 struct]` - Used to determine desired quality simulation types. **Currently unused** as only temperature is simulated.
-   `coprom`: `double`
-   `colarg`: `double`
-   `temperat`: `[1 x 1 struct]`

###### `parametres.qualite.cequeau.temperat` (`struct`)

-   `crayso`: `double`
-   `crayin`: `double`
-   `cevapo`: `double`
-   `cconve`: `double`
-   `crigel`: `double`
-   `tnap`: `double`
-   `panap`: `double`
-   `tinit`: `double`
-   `bassol`: `double`
-   `corsol`: `double`
-   `radiasol`: `[1 x 12 double]`
-   `nebulosi`: `[1 x 12 double]`
-   `pressvap`: `[1 x 12 double]`
-   `vitesven`: `[1 x 12 double]`

##### `parametres.dli` (`struct`)

Contains parameters for calculating Downwelling Longwave Irradiation (DLI) based on the selected `moduleDLI`.

-   `m1`: `[1 x 1 struct]` (Parameters for model 1)
-   `m2`: `[1 x 1 struct]` (Parameters for model 2)
-   ... (up to `m9`)

##### `parametres.pompage` (`struct`)

Parameters for the groundwater pumping module.

-   `delai`: `double` - Delay (in time steps) for the pumping effect.
-   `conductiviteHydraulique_s`: `double` or `[1 x nbCE double]` - Hydraulic conductivity (K). Spatially variable (`_s`).
-   `coeffPompage`: `double` - Pumping coefficient used to adjust extracted water (for calibration).

#### 3. `bassinVersant` (`struct`)

Contains the watershed definition based on prepared physiographic data.

-   `nbCpCheminLong`: `double` - Number of CPs in the longest path (? - *Needs clarification*).
-   `superficieCE`: `double` - Area of a Whole Cell.
-   `nomBassinVersant`: `string` - Name of the watershed.
-   `carreauxEntiers`: `[1 x nbCE struct]` - Structure array for Whole Cells.
-   `carreauxPartiels`: `[1 x nbCP struct]` - Structure array for Partial Cells.
-   `barrage` (Optional): `[1 x nbBarrages struct]` - Structure array for dams/reservoirs.
-   `puits`: `[1 x nbPuits struct]` - Structure array for wells.

##### `bassinVersant.carreauxEntiers` (`struct` array field)

-   `i`: `double` - Grid column index.
-   `j`: `double` - Grid row index.
-   `pctLacRiviere`: `double` - Percentage of lakes/rivers.
-   `pctForet`: `double` - Percentage of forest.
-   `pctMarais`: `double` - Percentage of marshland.
-   `altitude`: `double` - Altitude.
-   `Latitude`: `double` - Latitude.
-   `Longitude`: `double` - Longitude.
-   `meanSlope`: `double` - Mean slope.

##### `bassinVersant.carreauxPartiels` (`struct` array field)

-   `i`: `double` - Grid column index.
-   `j`: `double` - Grid row index.
-   `code`: `double` - ASCII code (A, B, C, or D).
-   `pctSurface`: `double` - Percentage of surface area.
-   `idCPAval`: `double` - ID of the downstream Partial Cell.
-   `idCPsAmont`: `[1 x 5 double]` - IDs of upstream Partial Cells.
-   `idCE`: `double` - ID of the corresponding Whole Cell.
-   `pctEau`: `double` - Percentage of water.
-   `pctForet`: `double` - Percentage of forest.
-   `pctMarais`: `double` - Percentage of marshland.
-   `pctSolNu`: `double` - Percentage of bare soil.
-   `altitudeMoy`: `double` - Mean altitude.
-   `profondeurMin`: `double` - Minimum depth (? - *Needs clarification*).
-   `longueurCoursEauPrincipal`: `double` - Length of the main watercourse.
-   `largeurCoursEauPrincipal`: `double` - Width of the main watercourse.
-   `penteRiviere`: `double` - River slope.
-   `cumulPctSuperficieCPAmont`: `double` - Cumulative upstream CP area percentage.
-   `cumulPctSuperficieLacsAmont`: `double` - Cumulative upstream lake area percentage.
-   `cumulPctSuperficieMaraisAmont`: `double` - Cumulative upstream marsh area percentage.
-   `cumulPctSuperficieForetAmont`: `double` - Cumulative upstream forest area percentage.
-   `lat`: `double` - Latitude.
-   `lon`: `double` - Longitude.
-   `hautMoyenneArbre`: `double` - Average tree height (used for shading module 1).
-   `azimutCoursEau`: `double` - Azimuth of the watercourse.

##### `bassinVersant.barrage` (`struct` array field)

Structure varies by dam type.

-   **Type 2**:
    -   `idCP`: `double`
    -   `idCPAval`: `double`
    -   `type`: 2
    -   `volumeInitial`: `double`
    -   `niveau`: `[1 x 7 double]`
    -   `debit`: `[1 x 7 double]`
-   **Type 3**:
    -   External dam whose outflows enter a CP within the watershed; outflows are known.
    -   Uses the same structure as Type 2, but only the **bold** fields are required. Other fields can be set to 0.
    -   `idCP`: `double`
    -   **`idCPAval`**: `double`
    -   **`type`**: 3
    -   `volumeInitial`: `double`
    -   `niveau`: `[1 x 7 double]`
    -   **`debit`**: `[1 x nbPasDeTemps double]` - Known outflow for each time step.
-   **Type 5**:
    -   `idCP`: `double`
    -   `idCPAval`: `double`
    -   `type`: 5
    -   `volumeInitial`: `double`
    -   `niveau`: `[1 x 7 double]`
    -   `volume`: `[1 x 7 double]`
    -   `debitsInterne`: `[nb internal rules x 7 double]`
    -   `debitsExterne`: `[1 x nb external rules struct]`
        -   `dateDebut`: `double` (YYYYMMDD format)
        -   `dateFin`: `double` (YYYYMMDD format)
        -   `parametres`: `[1 x 7 double]`
-   **Type 10**:
    -   *New type (not in original Cequeau) where outflow depends on gate opening and level.*
    -   *Note: Calculations are specific to RTA structures. Use only in an RTA context.*
    -   *`idCP`*: `double`
    -   *`idCPAval`*: `double`
    -   *`type`*: 10
    -   *`volumeInitial`*: `double`
    -   *`ouverture`*: `[1 x nbPasDeTemps double]` - Gate opening at each time step.

##### `bassinVersant.puits` (`struct` array field)

-   `idCE`: `double` - ID of the corresponding Whole Cell.
-   `distanceRiviere`: `double` - Distance from the well to the river (m).
-   `debitPompage`: `[nbPasDeTemps x 1 double]` - Pumping rate (m³/day) for each time step.
-   `niveauxPuits`: `[nbPasDeTemps x 1 double]` - Water level in the well (m) for each time step.
-   `h0`: `double` - Initial water level in the well (m).
-   `active`: `double` - Well status (0 = inactive, 1 = active).

#### 4. `meteo` (`struct`)

Meteorological data for the simulation period. Data for each time step must be ordered consistently with the Whole Cells (`carreauxEntiers`).

-   `tMin`: `[nbPasDeTemps x nbCE double or single]` - Minimum temperature.
-   `tMax`: `[nbPasDeTemps x nbCE double or single]` - Maximum temperature.
-   `pTot`: `[nbPasDeTemps x nbCE double or single]` - Total precipitation.
-   `pluie`: `[nbPasDeTemps x nbCE double or single]` - Rainfall.
-   `neige`: `[nbPasDeTemps x nbCE double or single]` - Snowfall.
-   `rayonnement`: `[nbPasDeTemps x nbCE double or single]` - Radiation.
-   `nebulosite`: `[nbPasDeTemps x nbCE double or single]` - Cloud cover.
-   `pression`: `[nbPasDeTemps x nbCE double or single]` - Pressure.
-   `vitesseVent`: `[nbPasDeTemps x nbCE double or single]` - Wind speed.
-   `t`: `[nbPasDeTemps x nbCE double or single]` - Temperature (average?).
-   `lai`: `[nbPasDeTemps x nbCE double or single]` - Leaf Area Index.
-   `lai_norm`: `[nbPasDeTemps x nbCE double or single]` - Normalized LAI.
-   `dli`: `[nbPasDeTemps x nbCE double or single]` - Downwelling Longwave Irradiation.

!!! note "Data Types"
    Meteorological data can be provided as `double` or `single`, but all fields must use the same type.

#### 5. `etatsPrecedents` (`struct`, Optional)

Allows initializing a simulation with the final states from a previous run. If provided, `parametres.solInitial` values are ignored.

-   `quantite`: (`struct`)
    -   `etatsCE`: State structure for Whole Cells (typically `etatsCE(end)` from previous run).
    -   `etatsCP`: State structure for Partial Cells (typically `etatsCP(end)` from previous run).
    -   `etatsFonte`: Snowmelt states for Whole Cells (typically `etatsFonte(end)` from previous run).
    -   `etatsEvapo`: Evapotranspiration states for Whole Cells (typically `etatsEvapo(end)` from previous run).
    -   `etatsBarrage` (Optional): Dam states (typically `etatsBarrage(end)` from previous run).
-   `qualite`: (`struct`)
    -   `etatsCP`: Water quality states for Partial Cells (typically `etatsQualCP(end)` from previous run).

Example initialization:
```matlab
etatsPrecedents.quantite.etatsCE = previous_etatsCE(end);
etatsPrecedents.quantite.etatsCP = previous_etatsCP(end);
etatsPrecedents.quantite.etatsFonte = previous_etatsFonte(end);
etatsPrecedents.quantite.etatsEvapo = previous_etatsEvapo(end);
% if previous_etatsBarrage exists:
etatsPrecedents.quantite.etatsBarrage = previous_etatsBarrage(end);

% if previous_etatsQualCP exists:
etatsPrecedents.qualite.etatsCP = previous_etatsQualCP(end);
```

#### 6. `assimilations` (`struct`, Optional)

Structure for applying data assimilation at specific time steps.

-   `quantite`: `[1 x nAssimSteps struct]` - Assimilation data for streamflow variables.
    -   `pasDeTemps`: `double` - Time step for this assimilation record (`datenum` format).
    -   `etatsCE`: (`struct`, Optional) - Assimilation values for Whole Cell states. Provide `[]` if not assimilating CE states at this time step.
    -   `etatsCP`: (`struct`, Optional) - Assimilation values for Partial Cell states. Provide `[]` if not assimilating CP states at this time step.
    -   `etatsFonte`: (`struct`, Optional) - Assimilation values for snowmelt states. Provide `[]` if not assimilating snowmelt states at this time step.
    -   `etatsEvapo`: (`struct`, Optional) - Assimilation values for evapotranspiration states. Provide `[]` if not assimilating ET states at this time step.
    -   `etatsBarrage`: (`struct`, Optional) - Assimilation values for dam states. Provide `[]` if not assimilating dam states at this time step.
-   `qualite`: `[1 x nAssimStepsQual struct]` - Assimilation data for quality variables.
    -   `pasDeTemps`: `double` - Time step for this assimilation record (`datenum` format).
    -   `etatsCP`: (`struct`, Optional) - Assimilation values for Partial Cell quality states. Provide `[]` if not assimilating quality states at this time step.

##### Assimilation State Structures (`assimilations.quantite.etats*`, `assimilations.qualite.etatsCP`)

Each state structure (e.g., `etatsCE`, `etatsCP`) follows a pattern:

-   `id`: `[1 x nbAssimilations double]` - Array of cell/dam IDs to assimilate.
-   *`variableName1`*: `[1 x nbAssimilations double]` OR `[2 x nbAssimilations double]` - Values or factors for the first variable.
-   *`variableName2`*: `[1 x nbAssimilations double]` OR `[2 x nbAssimilations double]` - Values or factors for the second variable.
-   ... and so on for other variables within that state category (CE, CP, Fonte, Barrage, QualiteCP).

**Assimilation Value Formats:**

1.  **Direct Value:** Provide a `[1 x nbAssimilations double]` array containing the absolute value to assimilate for each ID. Use `NaN` for specific IDs if no assimilation is desired for that variable/ID combination.
2.  **Factor Format:** Provide a `[2 x nbAssimilations double]` array:
    -   Row 1: Multiplicative factors.
    -   Row 2: Additive factors.
    The assimilated value is calculated as: $Assimilated = Calculated 	imes Factor_{mult} + Factor_{add}$.
    -   Both factors (rows) must be present for this format to be used for a variable.
    -   Use `NaN` in the first row (multiplicative factor) to skip assimilation for specific IDs.

!!! note "Mixing Formats"
    You can use the Direct Value format for some variables and the Factor Format for others within the *same* assimilation time step structure (e.g., use direct values for `niveauEauSol` and factors for `niveauEauNappe` in `etatsCE`), but you cannot mix formats *within the same variable* array.

**Specific Assimilation Variables:**

-   `assimilations.quantite.etatsCE`:
    -   `id`: CE IDs.
    -   `niveauEauSol`, `niveauEauNappe`, `niveauEauLacsMarais`, `evapoPotJour`, `production`.
-   `assimilations.quantite.etatsCP`:
    -   `id`: CP IDs.
    -   `apport`, `volume`, `debit`.
-   `assimilations.quantite.etatsFonte` (Depends on `moduleFonte`, example for Cequeau):
    -   `id`: CE IDs.
    -   `stockNeigeForet`, `stockNeigeClairiere`, `indexMurissementNeige`, `indexTempNeige`.
-   `assimilations.quantite.etatsEvapo`:
    -   Depends on `moduleEvapo`. (No states for Cequeau model).
-   `assimilations.quantite.etatsBarrage`:
    -   `idCarreauPartiel`: CP IDs associated with dams.
    -   `volume`, `niveau`.
-   `assimilations.qualite.etatsCP`:
    -   `id`: CP IDs.
    -   `temperature`.

#### 7. `stations` (`struct`)

Information about meteorological stations, used for interpolation if applicable (though interpolation is typically done via `CequeauInterpolationMex`). Structure details likely match the `stations` input for the interpolation function (see below).

---

### Outputs (`Extrants`)

The function returns multiple structures containing simulation results for each time step.

!!! note "Log File"
    An execution log file named `CequeauQuantite.log` may also be produced in the execution directory, depending on compilation options.

#### 1. `etatsCE` `[1 x nbPasDeTemps struct]`

States for Whole Cells (or subset defined by `execution.resultatsIdCE`).

-   `id`: `[1 x nbCE double]`
-   `iCarreauEntier`: `[1 x nbCE double]`
-   `jCarreauEntier`: `[1 x nbCE double]`
-   `niveauEauSol`: `[1 x nbCE double]` - Soil water level.
-   `niveauEauNappe`: `[1 x nbCE double]` - Groundwater level.
-   `niveauEauLacsMarais`: `[1 x nbCE double]` - Lake/marsh water level.
-   `evapoPotJour`: `[1 x nbCE double]` - Daily potential evapotranspiration.
-   `production`: `[1 x nbCE double]` - Water production.

#### 2. `etatsCP` `[1 x nbPasDeTemps struct]`

States for Partial Cells (or subset defined by `execution.resultatsIdCP`).

-   `id`: `[1 x nbCP double]`
-   `apport`: `[1 x nbCP double]` - Inflow.
-   `volume`: `[1 x nbCP double]` - Water volume.
-   `debit`: `[1 x nbCP double]` - Outflow discharge.

#### 3. `etatsFonte` `[1 x nbPasDeTemps struct]`

Snowmelt states for Whole Cells. Structure depends on the selected `moduleFonte`.

-   **(For Cequeau model):**
    -   `stockNeigeForet`: `[1 x nbCE double]` - Snowpack in forest.
    -   `stockNeigeClairiere`: `[1 x nbCE double]` - Snowpack in open area.
    -   `indexMurissementNeige`: `[1 x nbCE double]` - Snow ripening index.
    -   `indexTempNeige`: `[1 x nbCE double]` - Snow temperature index.

#### 4. `etatsEvapo` `[1 x nbPasDeTemps struct]`

Evapotranspiration states for Whole Cells. Structure depends on the selected `moduleEvapo`.

!!! note
    For the Cequeau ET model, the only state variable is `evapoPotJour`, which is already included in `etatsCE` for use by the quality module. This output structure might be empty or contain states for other ET models.

#### 5. `etatsBarrage` `[1 x nbPasDeTemps struct]`

States for dams/reservoirs.

-   `idCarreauPartiel`: `double`
-   `volume`: `double`
-   `niveau`: `double` - Water level.
-   `debitEntrant`: `double` - Inflow.
-   `debitsSortants`: `[1 x nbReglesEvacuation double]` - Outflows according to evacuation rules.

#### 6. `pasDeTemps` `[nbPasDeTemps x 1 double]`

Array containing the date/time at the *end* of each simulation time step (Matlab `datenum` format).

#### 7. `etatsQualiteCP` `[1 x nbPasDeTemps struct]`

Water quality (temperature) states for Partial Cells. Contains an empty structure if quality simulation was not run (`parametres.option.calculQualite = 0`).

-   `id`: `[1 x nbCP double]` - Partial Cell ID.
-   `temperature`: `[1 x nbCP double]` - Water temperature in the CP.
-   `qruiss`: `[1 x nbCP double]`
-   `qnappe`: `[1 x nbCP double]`
-   `qhypo`: `[1 x nbCP double]`
-   `qlacma`: `[1 x nbCP double]`
-   `qradso`: `[1 x nbCP double]`
-   `qradin`: `[1 x nbCP double]`
-   `qevap`: `[1 x nbCP double]`
-   `qconv`: `[1 x nbCP double]`

!!! info "Quality Energy Fluxes"
    The fields following `temperature` represent calculated values participating in the thermal balance:
    *   **Advective Energy Inputs:**
        *   `qruiss`: Surface runoff.
        *   `qnappe`: Groundwater inflow.
        *   `qhypo`: Hypodermic (subsurface) runoff.
        *   `qlacma`: Lake/marsh overflow.
    *   **Surface Energy Exchanges:**
        *   `qradso`: Solar radiation.
        *   `qradin`: Infrared radiation.
        *   `qevap`: Evaporation.
        *   `qconv`: Convection.

#### 8. `avantAssimilations*` (`struct`)

These outputs store the model states *before* assimilation was applied, but only for the time steps where assimilation occurred. They have the same structure as the corresponding `assimilations.*` input structures.

-   `avantAssimilationsCE`: Corresponds to `assimilations.quantite.etatsCE`.
-   `avantAssimilationsFonte`: Corresponds to `assimilations.quantite.etatsFonte`.
-   `avantAssimilationsEvapo`: Corresponds to `assimilations.quantite.etatsEvapo`.
-   `avAssimQual` (*Note: Name differs slightly*): Corresponds to `assimilations.qualite.etatsCP`.

---

## `CequeauInterpolationMex`

This function performs spatial interpolation of meteorological data from stations to the CEQUEAU grid cells.

### Usage

```matlab
meteoInterpolee = CequeauInterpolationMex(execution, parametres, ...
                                        bassinVersant, stations, meteoStations);
```

### Inputs (`Intrants`)

!!! warning "Reused Input Structures"
    Some input structures from the quantity simulation (`parametres`, `bassinVersant`) are reused here for convenience in this initial version of the interpolator. Although the full structures must be provided, only specific fields (detailed below) are actually used by the interpolation function.

#### 1. `execution` (`struct`)

Specifies the period for which data should be interpolated.

-   `dateDebut`: `double` - Start date for interpolation (`datenum` format).
-   `dateFin`: `double` - End date for interpolation (`datenum` format).

#### 2. `parametres` (`struct`)

Contains interpolation parameters. **All fields shown in the `CequeauQuantiteMex` section must be present**, but only the `.interpolation` sub-structure is used.

```matlab
parametres:
  % --- Fields required but NOT USED by interpolation --- %
  .option, .sol, .solInitial, .transfert, .ctp, .lac, .surface, .fonte, .evapo, .qualite, .pompage, .dli
  % ---------------------------------------------------- %

  .interpolation % [1x1 struct] Interpolation parameters (USED)
```

##### `parametres.interpolation` (`struct`)

-   `type`: `double` - Interpolation method selection:
    -   1: Thiessen polygons.
    -   3: Inverse distance weighting using the 3 nearest stations.
-   `coep`: `double` - Annual precipitation correction coefficient based on altitude (mm/meter/year).
-   `coet`: `double` - Temperature correction coefficient based on altitude (°C / 1000 m).

#### 3. `bassinVersant` (`struct`)

Watershed definition. Must contain the `carreauxEntiers` structure, which requires the `altitude` field for interpolation.

-   See `bassinVersant` structure under `CequeauQuantiteMex` inputs.
-   The essential field used here is `bassinVersant.carreauxEntiers.altitude`.

#### 4. `stations` `[1 x nbStations struct]`

Information about the meteorological stations.

-   `id`: `char` - Station identifier.
-   `nom`: `char` - Station name (not used by the interpolator).
-   `i`: `double` - Station's x-coordinate (abscissa) in the watershed's "I, J" grid system.
-   `j`: `double` - Station's y-coordinate (ordinate) in the watershed's "I, J" grid system.
-   `tp`: `double` - Mean interannual precipitation at the station (mm).
-   `altitude`: `double` - Station altitude.

#### 5. `meteoStations` (`struct`)

Meteorological data recorded at the stations. Must contain fields corresponding to the variables being interpolated (e.g., `tMin`, `tMax`, `pTot`).

-   Structure is the same as the `meteo` input for `CequeauQuantiteMex`, but dimensions are `[nbPasDeTemps x nbStations]` instead of `[nbPasDeTemps x nbCE]`.
    Example: `tMin: [nbPasDeTemps x nbStations double or single]`

!!! warning "Station Order"
    The order of stations in the `meteoStations` data arrays **must** match the order of stations in the `stations` input structure.

### Outputs (`Extrants`)

#### 1. `meteoInterpolee` (`struct`)

The interpolated meteorological data on the CEQUEAU Whole Cell (CE) grid.

-   This output structure has the same format as the `meteo` input for `CequeauQuantiteMex` (e.g., `tMin: [nbPasDeTemps x nbCE double]`).
