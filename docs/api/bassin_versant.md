# BassinVersant Class

The `BassinVersant` class represents the entire watershed in the CEQUEAU model. It manages all the spatial components, including whole grid cells (CarreauEntier) and partial grid cells (CarreauPartiel), as well as other watershed components like dams and reservoirs.

## Overview

BassinVersant is the core container class for the entire hydrological model. It:

- Holds collections of grid cells (CarreauEntier) and drainage cells (CarreauPartiel) 
- Manages dams, reservoirs, and wells in the watershed
- Provides methods for routing water through the drainage network
- Implements watershed initialization from input data

## Key Components

### Spatial Elements
- **CarreauEntier**: Collection of whole grid cells covering the watershed
- **CarreauPartiel**: Collection of partial grid cells forming the drainage network
- **Width and Height**: Grid dimensions in terms of number of cells

### Water Management Structures
- **Barrages**: Dams/reservoirs within the watershed
- **Puits**: Water extraction wells

## Main Methods

### Initialization
- `initialiserCarreauxEntiers`: Initialize the whole grid cells from input data
- `initialiserCarreauxPartiels`: Initialize the partial grid cells and drainage network
- `initialiserBarrage`: Set up dams and reservoirs within the watershed

### Grid Cell Access
- `trouverCarreauEntierParId`: Find a whole grid cell by its ID
- `trouverCarreauPartielParId`: Find a partial grid cell by its ID
- `trouverBarrageParIdCP`: Find a dam by its partial grid cell ID

### Water Management
- `possedeBarrage`: Check if a specific location has a dam
- `puits`: Access water extraction wells
- `calculerCoeffTransfert`: Calculate flow transfer coefficients

## Example Usage

The BassinVersant class is typically used as follows:

```cpp
// Initialize the watershed
BassinVersant bassinVersant;
bassinVersant.initialiserCarreauxEntiers(carreauxEntiersData);
bassinVersant.initialiserCarreauxPartiels(carreauxPartielsData);

// Simulate water flow through the watershed
for (auto& cp : bassinVersant.carreauxPartiels()) {
    // Process each partial grid cell
}

// Check for dams at a specific location
if (bassinVersant.possedeBarrage(idCP)) {
    // Handle dam operations
}
```