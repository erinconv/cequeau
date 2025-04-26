# CEQUEAU STREAMFLOW AND RIVER TEMPERATURE

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Documentation](https://github.com/erinconv/CEQUEAU/actions/workflows/docs-deploy.yml/badge.svg)](https://github.com/erinconv/CEQUEAU/actions/workflows/docs-deploy.yml)

A hydrological model for simulating streamflow and river temperature, implemented in C++ with interfaces for MATLAB and Octave.

---

## Overview

CEQUEAU is a physically-based, distributed hydrological model designed to simulate both streamflow (streamflow, snowpack, soil moisture, etc.) and river temperature parameters within a watershed. It is built using C++ for performance and provides MEX interfaces for easy integration and use within MATLAB and GNU Octave environments.

## Features

*   **Hydrological Processes:** Simulates key components of the hydrological cycle including:
    *   Snow accumulation and melt (multiple models: `CEQUEAU`, `CemaNeige`, `UEB`)
    *   Evapotranspiration (multiple models: `CEQUEAU`, `Priestley-Taylor`, `Penman-Monteith`, `Morton`, `KPenman`, `McGuinness`)
    *   Infiltration and runoff generation
    *   Streamflow routing
*   **River Temperature:** Simulates relevant river temperature parameters (specifics depend on model configuration). Includes shading effects (`SimulationQualite_Ombrage`).
*   **Spatial Representation:** Represents watersheds using grid cells (`CarreauEntier`, `CarreauPartiel`) and includes components like reservoirs (`Barrage`) and wells (`Puits`).
*   **Data Assimilation:** Includes capabilities for assimilating observed data (`Simulation_Assimilation`).
*   **Input/Output:** Handles model inputs and outputs efficiently.
*   **Cross-Platform:** Compilable and runnable on systems with MATLAB or Octave and a compatible C++14 compiler.

## Installation and Compilation

### Prerequisites

*   **MATLAB** or **GNU Octave**
*   A **C++ Compiler** supporting C++14 standard (e.g., GCC/G++, MSVC). The compilation scripts attempt to auto-detect the compiler.

### Compilation Steps

1.  Navigate to the `src/` directory in your MATLAB or Octave environment.
2.  Run the appropriate compilation script:
    *   **For Quantity Simulation (MATLAB):** `compileCequeauMat`
    *   **For Quantity Simulation (Octave):** `compileCequeauOct`
3.  The compiled MEX binaries will be placed in the `mex/` directory (e.g., `mex/cequeauQuantiteMat.mex*`).

*Note: You can modify the compilation scripts (e.g., `compileCequeauMat.m`) to enable debug symbols (`debug = true;`) or disable logging (`log = true;`).*

## Usage

Once compiled, the MEX functions can be called directly from MATLAB or Octave like standard functions.

*   **Primary Simulation:** Use the compiled `cequeauQuantite*` function for running hydrological simulations.

Refer to the detailed documentation in the `docs/` folder for specific usage instructions, function signatures, input/output formats, and parameter definitions:

*   `cequeau_guide.docx`: User guide for using Cequeau.
*   `Intrants – Extrants v5.docx`: Details on input and output data structures.
*   `cequeau_octave.docx`: Specific notes for Octave users.

## Folder Structure

```
cequeau/
├── docs/         # Documentation files (.pdf, .docx)
├── mex/          # Compiled MEX binaries (output of compilation)
├── src/          # C++ source (.cpp, .h, .hpp) and compilation scripts (.m)
├── tests/        # Test scripts and data
├── .gitignore
└── README.md     # This file
```

