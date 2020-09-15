#!/bin/bash

base_dir="$(pwd)"
build_type="Release"

# Parse the script argument case insensitive.
if [ "${1,,}" = "debug" ]; then
    build_type="Debug"
fi

rm -rf binaries

mkdir -p build
cd build

cmake -DCMAKE_BUILD_TYPE=$build_type $base_dir -DCMAKE_INSTALL_PREFIX=$base_dir/binaries && \
cmake --build . -- install
