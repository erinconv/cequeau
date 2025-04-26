# CarreauEntier Class

The `CarreauEntier` class represents whole grid cells that make up the spatial foundation of the CEQUEAU model. Each CarreauEntier corresponds to a square geographic area within the watershed.

## Overview

CarreauEntier (whole grid cells) serve as the basic geographic units for modeling hydrological processes. Each cell:

- Has a fixed position in the watershed grid (i, j coordinates)
- Contains physical characteristics relevant to hydrological processes
- Serves as the spatial unit for meteorological inputs
- Can contain one or more partial grid cells (CarreauPartiel)

## Key Properties

### Identification
- **id_**: Unique identifier for the grid cell
- **i_**: Row index in the watershed grid
- **j_**: Column index in the watershed grid

### Geographic Properties
- **latitude_**: Geographic latitude of the cell center
- **longitude_**: Geographic longitude of the cell center
- **altitude_**: Average elevation of the cell (meters above sea level)
- **meanSlope_**: Average terrain slope within the cell

### Land Cover Properties
- **pctForet_**: Percentage of the cell covered by forest
- **pctLacRiviere_**: Percentage of the cell covered by lakes and rivers
- **pctMarais_**: Percentage of the cell covered by marshes/wetlands
- **pctLacExutoire_**: Percentage of the cell covered by outlet lakes

## Main Methods

### Property Access
- `id()`: Get the cell's unique identifier
- `i()`, `j()`: Get the cell's grid coordinates
- `latitude()`, `longitude()`: Get geographic coordinates
- `altitude()`: Get the cell's average elevation
- `pctForet()`, `pctMarais()`, etc.: Access land cover percentages

### Hydrological Calculations
- `pctEau()`: Calculate total percentage of water bodies
- `pctSol()`: Calculate percentage of land areas
- `calculerCoeffPonderation()`: Calculate weighting coefficients for hydrological processes

## Example Usage

```cpp
// Access cell properties
int cellId = carreau.id();
float forestCover = carreau.pctForet();
float elevation = carreau.altitude();

// Calculate derived properties
float waterArea = carreau.pctEau();
float landArea = carreau.pctSol();

// Grid position
int row = carreau.i();
int col = carreau.j();

// Geographic coordinates
float lat = carreau.latitude();
float lon = carreau.longitude();
``` 