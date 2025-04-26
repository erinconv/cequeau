# API Documentation

This section contains documentation for the CEQUEAU codebase, both hand-written overviews and automatically generated API references.

## Core Classes

The CEQUEAU model consists of several core classes that implement the hydrological and temperature modeling functionality:

- [BassinVersant](api/bassin_versant.md): Represents the entire watershed
- [CarreauEntier](api/carreau_entier.md): Represents whole grid cells in the watershed
- [CarreauPartiel](api/carreau_partiel.md): Represents partial grid cells in the watershed
- [Simulation](api/simulation.md): Handles the simulation process

## Other Key Components

CEQUEAU includes several other important classes:

- **Barrage**: Manages reservoir operations and water release
- **Meteo**: Handles meteorological inputs and processing
- **DateChrono**: Provides date and time handling capabilities
- **Parametres**: Manages model parameters and configurations

## Simulation Modules

CEQUEAU implements multiple simulation modules for different processes:

- **Evapotranspiration Models**: Several implementations available
  - EvapoCequeau: The original CEQUEAU evapotranspiration model
  - EvapoPenmanMont: Penman-Monteith evapotranspiration model
  - EvapoPriestleyTaylor: Priestley-Taylor evapotranspiration model
  - EvapoMorton: Morton's evapotranspiration model
  - EvapoKPenman: Modified Penman evapotranspiration model
  - EvapoMcGuinness: McGuinness-Bordne evapotranspiration model

- **Snowmelt Models**: Options for different snow accumulation and melt calculations
  - FonteCequeau: Original CEQUEAU snowmelt model
  - FonteUEB: Utah Energy Balance snowmelt model
  - FonteCemaNeige: CemaNeige snowmelt model

- **Solar Radiation**:
  - DLI: Direct and diffuse light calculations
  - SolarFunctions: Sun position and solar radiation calculations

## Auto-Generated Documentation

For complete API details including all classes, methods, and properties, see the [Auto-Generated Class List](CEQUEAU/annotated.md).

The auto-generated documentation includes:

- **Class List**: All classes with brief descriptions
- **Class Index**: Alphabetical index of all classes
- **Class Hierarchy**: Inheritance diagram for class relationships
- **Class Members**: Functions, variables, and types for each class
- **File List**: Source code files with documentation

When viewing the auto-generated documentation:

1. Start from the [Class List](CEQUEAU/annotated.md) to browse all available classes
2. Use the search function (top-right) to find specific classes or functions
3. Each class page includes properties, methods, inheritance information, and function details 