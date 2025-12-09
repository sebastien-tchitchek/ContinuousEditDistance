#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "${SCRIPT_DIR}"

if [ -d "./data" ]; then
  echo "Cleaning ./data/ directory (previous TVPDs)..."
  rm -rf ./data/*
fi

PV_PYTHON="./ttk-paraview/build/bin/pvpython"

echo "Running 002.py..."
"${PV_PYTHON}" 002.py

echo "Running 004.py..."
"${PV_PYTHON}" 004.py

echo "Running 1952.py..."
"${PV_PYTHON}" 1952.py

echo "Running 1954.py..."
"${PV_PYTHON}" 1954.py

echo "All ParaView scripts completed successfully."
