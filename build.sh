#!/bin/sh

cd build
make

# move licenses into bin directory
cd ..
mkdir -p bin/doc/spdlog
cp thirdparty/spdlog/LICENSE bin/doc/spdlog/LICENSE
