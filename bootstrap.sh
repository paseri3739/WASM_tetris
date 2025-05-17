#!/bin/bash
set -e

mkdir -p build
cd build

if [ "$1" == "wasm" ]; then
  source ~/emsdk/emsdk_env.sh
  emcmake cmake ..
else
  cmake ..
fi

make -j$(sysctl -n hw.ncpu)
