# ContinuousEditDistance
This repository contains the code used for the manuscript Continuous Edit Distance, Geodesics and Barycenters of Time-varying Persistence Diagrams.

## Installation

### Scripted installation

The following script was tested on a fresh installation of Ubuntu 20.04 in a VirtualBox VM.

Go to the root of this repository and run the `install.sh` script:

```
chmod +x install.sh
. ./install.sh
```

At this point, you can launch paraview from the command line with 

```
paraview
```

### 2. Generate VESTEC space-weather data

This step reproduces the in-situ space-weather pipeline from the VESTEC WP3 demonstrator (branch `demonstrator_D.3.3`).  
It will:

- clone the `vestec-wp3` repository (fork under `sebastien-tchitchek`),
- build the `ipicmini` simulator in `demonstrator/space_weather_in-situ`,
- run the 4 space-weather simulations,
- compute persistence diagrams in-situ at each time step and store them in Cinema databases.

From the root of this repository, run:

```bash
chmod +x generation_vestec_data.sh
./generation_vestec_data.sh