# Simulation Class

The `Simulation` class is the central orchestrator of the hydrological modeling process in CEQUEAU. It coordinates all aspects of the simulation, from input processing to output generation.

## Overview

The Simulation class is responsible for:

- Managing the entire hydrological simulation workflow
- Implementing the water balance and routing algorithms
- Coordinating the various modeling components (snowmelt, evapotranspiration, etc.)
- Processing meteorological inputs and generating hydrological outputs
- Handling the temporal aspects of the simulation (time steps, periods, etc.)

## Key Components

### Watershed Representation
- **BassinVersant**: The watershed object containing all spatial components
- **Meteo**: Meteorological data for the simulation period

### Water Balance Models
- **Evapotranspiration**: Multiple models available (EvapoCequeau, EvapoPenmanMont, etc.)
- **Snowmelt**: Multiple models available (FonteCequeau, FonteUEB, FonteCemaNeige)
- **Soil Water Balance**: Processes for infiltration, percolation, and subsurface flow

### Time Management
- **dateDebut_**: Start date of the simulation
- **dateFin_**: End date of the simulation
- **intervalleCalcul_**: Calculation time step (typically days)

### Model Parameters
- **parametres_**: Configuration parameters for the flow simulation
- **paramQual_**: Parameters for water temperature simulation (if enabled)

## Main Methods

### Initialization
- `initialiser()`: Set up all components of the simulation
- `initialiserSimulationContinue()`: Configure for continuous simulation mode

### Simulation Execution
- `simuler()`: Execute the complete simulation
- `simulerPasDeTemps()`: Process a single time step

### Process Modules
- `simulerProduction()`: Calculate water production processes (including snowmelt and evapotranspiration)
- `simulerTransfert()`: Route water through the watershed network

### Output Handling
- `calculerDebitExutoire()`: Calculate outflow at the watershed outlet
- `enregistrerDebitSimule()`: Record simulated discharge values

## Example Usage

```cpp
// Initialize simulation
Simulation simulation;
simulation.initialiser(parametres, donneeMeteo, bassinVersant);

// Run the entire simulation
simulation.simuler();

// Access results
const auto& results = simulation.resultats();

// Or run step by step
for (int step = 0; step < simulation.nbPasDeTemps(); ++step) {
    simulation.simulerPasDeTemps();
    // Process intermediate results if needed
}
```