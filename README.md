# ContinuousEditDistance

This repository contains the code used for the manuscript **“Continuous Edit Distance, Geodesics and Barycenters of Time-varying Persistence Diagrams”**.

All scripts below were tested on a fresh installation of **Ubuntu 20.04** in a VirtualBox VM.

> **Shortcut (precomputed data).**  
> If desired, this repository already provides the four time-varying persistence diagrams (TVPDs), based on the precomputed VESTEC space-weather data, used in the paper:
>
> - four TVPDs used in the experiments:  
>   `data/002.cdb`, `data/004.cdb`, `data/1952.cdb`, `data/1954.cdb`
>
> If you are only interested in reproducing the **CED experiment** (Section 4 below), you may **skip Steps 1 and 3** and proceed directly by:
>
> 1. running **Step 2 (CED environment installation)**, and  
> 2. following the instructions given in **Step 4**.

---

## 1. Generate VESTEC space-weather data

This step reproduces the in-situ space-weather pipeline from the **VESTEC WP3 demonstrator** (branch `demonstrator_D.3.3`).  
It will:

- install the official **TTK 0.9.9** and **ParaView 5.8.1** binary packages for Ubuntu 20.04  
  (via `ttk-paraview-ubuntu-20.04.deb` and `ttk-0.9.9-ubuntu-20.04.deb`),
- clone the `vestec-wp3` repository (fork under `sebastien-tchitchek`),
- build the `ipicmini` simulator in `demonstrator/space_weather_in-situ`,
- run the 4 space-weather simulations,
- compute persistence diagrams in-situ at each time step and store them in Cinema databases.

From the root of this repository, run:

```bash
chmod +x generation_vestec_data.sh
./generation_vestec_data.sh
```

After this step, the VESTEC space-weather simulations and their corresponding persistence diagrams are available in the `vestec-wp3/demonstrator/space_weather_in-situ` directory.

> **Note.**
> This step is **optional** if you are satisfied with the precomputed TVPDs data already provided in this repository.

---

## 2. Installation (CED environment)

This step installs the environment used for the **Continuous Edit Distance (CED)** experiments.

### 2.1 Scripted installation

The `install.sh` script will:

- remove any TTK Debian packages installed on the system (in particular, the `ttk-0.9.9` and `ttk-paraview-ubuntu-20.04.deb` packages from step 1, as well as any other packages whose name starts with `ttk`).  
- install all required system dependencies,
- download and install **CMake 3.25.1** locally in this repository,
- clone and build **`ttk-paraview`** at the exact commit used in the paper (`70075740e5a78fa28b4c399e2fcdfd2665151521`),
- unpack and build the custom **`ttk-tchitchek`** snapshot (including the Continuous Edit Distance filter),
- update environment variables (`LD_LIBRARY_PATH`, `PV_PLUGIN_PATH`, `PYTHONPATH`) in `~/.bashrc`.

From the root of this repository, run:

```bash
chmod +x install.sh
. ./install.sh
```

Once the installation is finished, you can launch ParaView with:

```bash
paraview
```

ParaView should then load with the TTK plugins and all custom CED-related filters (from the `ttk-tchitchek` snapshot) available.

## 3. Generate VESTEC space-weather TVPDs

From the root of this repository, run:

```bash
chmod +x separate_vestec_data.sh
./separate_vestec_data.sh
````

This script sequentially runs the ParaView/TTK Python traces (`002.py`, `004.py`, `1952.py`, `1954.py`) and splits the VESTEC space-weather dataset into the four time-varying persistence diagrams (TVPDs) used and described in the manuscript. It also converts persistence diagrams from the legacy TTK 0.9.9 format to the newer TTK 1.2 format. 

The resulting Cinema databases are written into the `data/` directory:

* `data/002.cdb`
* `data/004.cdb`
* `data/1952.cdb`
* `data/1954.cdb`

> **Note.**
> These four TVPDs are **already provided** in the repository.
> If you do not need to recompute them, you may skip this step and directly use the precomputed `*.cdb` files in `data/` for the CED experiments (step 4).
