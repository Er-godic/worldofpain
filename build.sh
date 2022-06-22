#!/bin/sh

cd build
make

# move licenses into bin directory
cd ..

mkdir -p bin/doc
cp LICENSE bin/doc/LICENSE

mkdir -p bin/doc/spdlog
cp thirdparty/spdlog/LICENSE bin/doc/spdlog/LICENSE

mkdir -p bin/doc/glfw
cp thirdparty/glfw/LICENSE.md bin/doc/glfw/LICENSE.md

mkdir -p bin/doc/glad
cp thirdparty/glad/LICENSE bin/doc/glad/LICENSE

mkdir -p bin/doc/glm
cp thirdparty/glm/copying.txt bin/doc/glm/copying.txt
