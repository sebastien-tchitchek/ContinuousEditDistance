#!/usr/bin/env bash
set -euo pipefail

TTK_DEB_PACKAGES="$(dpkg -l | awk '/^ii/ && $2 ~ /^ttk/ {print $2}')"

if [ -n "$TTK_DEB_PACKAGES" ]; then
  echo "Removing previously installed TTK .deb packages: $TTK_DEB_PACKAGES"
  sudo apt-get remove --purge -y $TTK_DEB_PACKAGES
  sudo apt-get autoremove --purge -y
else
  echo "No TTK .deb packages found, skipping removal step."
fi

sudo apt update

sudo apt-get install -y cmake-qt-gui libboost-system-dev libpython3.8-dev libxt-dev libxcursor-dev libopengl-dev
sudo apt-get install -y qt5-default qttools5-dev libqt5x11extras5-dev libqt5svg5-dev qtxmlpatterns5-dev-tools
sudo apt-get install -y libeigen3-dev graphviz-dev python3-sklearn libsqlite3-dev zlib1g-dev
sudo apt-get install -y git unzip

wget https://github.com/Kitware/CMake/releases/download/v3.25.1/cmake-3.25.1-linux-x86_64.tar.gz
tar -xzvf cmake-3.25.1-linux-x86_64.tar.gz
rm cmake-3.25.1-linux-x86_64.tar.gz

CMAKE_BIN="$(pwd)/cmake-3.25.1-linux-x86_64/bin/cmake"

git clone https://github.com/topology-tool-kit/ttk-paraview.git
cd ttk-paraview
git checkout 70075740e5a78fa28b4c399e2fcdfd2665151521

mkdir build
cd build

cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DTTK_PARAVIEW_HEADLESS_DEPS=OFF

make -j"$(nproc)"
sudo make install

cd ../..   

rm -rf ttk-tchitchek-timevaryingpersistencediagram
unzip -q ttk-tchitchek-timevaryingpersistencediagram.zip

cd ttk-tchitchek-timevaryingpersistencediagram

mkdir build
cd build

"$CMAKE_BIN" .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=/usr/local \
  -DTTK_BUILD_DOCUMENTATION=OFF \
  -DTTK_BUILD_PARAVIEW_PLUGINS=ON \
  -DTTK_BUILD_STANDALONE_APPS=ON \
  -DTTK_BUILD_VTK_PYTHON_MODULE=ON \
  -DTTK_BUILD_VTK_WRAPPERS=ON \
  -DTTK_CELL_ARRAY_LAYOUT=OffsetAndConnectivity \
  -DTTK_ENABLE_CPU_OPTIMIZATION=ON \
  -DTTK_ENABLE_EIGEN=ON \
  -DTTK_ENABLE_GRAPHVIZ=ON \
  -DTTK_ENABLE_OPENMP=ON \
  -DTTK_ENABLE_SCIKIT_LEARN=ON \
  -DTTK_ENABLE_SQLITE3=ON \
  -DTTK_ENABLE_ZLIB=ON \
  -DTTK_INSTALL_PLUGIN_DIR=bin/plugins \
  -DTTK_PYTHON_MODULE_DIR=lib/python3.8/site-packages \
  -DTTK_SCRIPTS_PATH=scripts/ttk

make -j"$(nproc)"
sudo make install

cd ../..   

echo 'export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/usr/local/lib' >> ~/.bashrc
echo 'export PV_PLUGIN_PATH=${PV_PLUGIN_PATH}:/usr/local/bin/plugins/' >> ~/.bashrc
echo 'export PYTHONPATH=${PYTHONPATH}:/usr/local/lib/python3.8/site-packages/' >> ~/.bashrc

source ~/.bashrc

echo
echo "Installation complete."