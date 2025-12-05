#!/usr/bin/env bash
set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "${REPO_ROOT}"

sudo apt update
sudo apt-get install -y git

if [ ! -f ttk-paraview-ubuntu-20.04.deb ]; then
  wget https://github.com/topology-tool-kit/ttk-paraview/releases/download/v5.8.1/ttk-paraview-ubuntu-20.04.deb
fi

if [ ! -f ttk-0.9.9-ubuntu-20.04.deb ]; then
  wget https://github.com/topology-tool-kit/ttk/releases/download/0.9.9/ttk-0.9.9-ubuntu-20.04.deb
fi

sudo apt install -y ./ttk-paraview-ubuntu-20.04.deb
sudo apt install -y ./ttk-0.9.9-ubuntu-20.04.deb

rm -f ./ttk-paraview-ubuntu-20.04.deb ./ttk-0.9.9-ubuntu-20.04.deb

if [ ! -d "vestec-wp3" ]; then
  git clone -b demonstrator_D.3.3 --single-branch \
    https://github.com/sebastien-tchitchek/vestec-wp3.git
fi

cd vestec-wp3/demonstrator/space_weather_in-situ

sudo apt install -y cmake libopenmpi-dev

./make.sh

chmod +x launch-runs.sh

./launch-runs.sh