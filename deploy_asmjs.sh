#!/bin/bash

echo "=========================================================="
echo "Building gmake-linux-clang..."
echo "=========================================================="
pushd Projects/gmake-linux-clang
time make -j 10 config=release
popd

echo "=========================================================="
echo "Importing resources..."
echo "=========================================================="
pushd Dist
./LD32Game-gmake-linux-clang-Release ImportPackages
popd

echo "=========================================================="
echo "Building gmake-asmjs..."
echo "=========================================================="
pushd Projects/gmake-asmjs
time make -j 10 config=release
time make -j 10 config=production
popd

sudo cp Build/gmake-asmjs/bin/* /var/www/emscripten
