#!/usr/bin/env bash
set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "${REPO_ROOT}"

if [ ! -d "vestec-wp3" ]; then
  git clone -b demonstrator_D.3.3 --single-branch \
    https://github.com/sebastien-tchitchek/vestec-wp3.git
fi

cd vestec-wp3/demonstrator/space_weather_in-situ

sudo apt update
sudo apt install -y cmake libopenmpi-dev

./make.sh

chmod +x launch-runs.sh

./launch-runs.sh