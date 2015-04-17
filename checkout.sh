#!/bin/bash

git submodule init
git submodule update
pushd Psybrus
git submodule init
git submodule update
popd

