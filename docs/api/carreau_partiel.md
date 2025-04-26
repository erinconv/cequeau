# CarreauPartiel Class

The `CarreauPartiel` class represents partial grid cells that form the watershed drainage network in the CEQUEAU model. These are sub-units of whole grid cells (CarreauEntier) that define how water flows through the watershed.

## Overview

CarreauPartiel (partial grid cells) are the fundamental units for streamflow routing in CEQUEAU. Each CarreauPartiel:

- Represents a portion of a CarreauEntier with a distinct drainage direction
- Forms part of the watershed's drainage network
- Connects to other CarreauPartiel cells in an upstream-downstream relationship
- Contains physical characteristics relevant to water routing

## Key Properties

### Identification
- **id_**: Unique identifier for the partial grid cell
- **codeCarreauPartiel_**: Special code for the partial cell
- **idCarreauEntier_**: Reference to the parent whole grid cell
- **iCarreauEntier_**, **jCarreauEntier_**: Grid coordinates of the parent whole grid cell

### Connectivity
- **idCarreauPartielAval_**: ID of the downstream partial grid cell
- **idCarreauxPartielsAmont_**: IDs of upstream partial grid cells

### Physical Characteristics
- **pctSurface_**: Percentage of the parent whole grid cell's area
- **pctEau_**: Percentage covered by water bodies
- **pctForet_**: Percentage covered by forest
- **pctMarais_**: Percentage covered by marshes
- **pctSolNu_**: Percentage of bare soil

### Hydrographic Properties
- **altitudeMoyenne_**: Average elevation
- **longueurCoursEauPrincipal_**: Length of the main stream
- **largeurCoursEauPrincipal_**: Width of the main stream
- **penteRiviere_**: River slope
- **azimutCoursEau_**: Stream azimuth (direction)

### Routing Parameters
- **coeffTransfert_**: Water transfer coefficient
- **volumeInitial_**: Initial water volume

## Main Methods

### Property Access
- `id()`: Get the cell's unique identifier
- `idCarreauEntier()`: Get the parent whole grid cell ID
- `idCarreauPartielAval()`: Get the downstream cell ID
- `idCarreauxPartielsAmont()`: Get the upstream cells IDs

### Hydrological Properties
- `pctSurface()`: Get the percentage of whole cell area
- `pctEau()`, `pctForet()`, `pctMarais()`: Get land cover percentages
- `altitudeMoyenne()`: Get average elevation
- `penteRiviere()`: Get river slope

### Routing
- `coeffTransfert()`: Get/set the water transfer coefficient
- `volumeInitial()`: Get/set initial water volume
- `calculerCorrectionDebit()`: Calculate flow correction factors

## Example Usage

```cpp
// Access basic properties
int cpId = carreauPartiel.id();
int parentCellId = carreauPartiel.idCarreauEntier();

// Check downstream connectivity
int downstreamId = carreauPartiel.idCarreauPartielAval();
bool isOutlet = (downstreamId == 0);  // 0 indicates watershed outlet

// Get upstream cells
const auto& upstreamIds = carreauPartiel.idCarreauxPartielsAmont();
bool isHeadwater = upstreamIds.empty();

// Access physical properties
float forestPercentage = carreauPartiel.pctForet();
float streamSlope = carreauPartiel.penteRiviere();
```