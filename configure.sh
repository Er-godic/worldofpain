#!/bin/sh

mkdir -p build
mkdir -p bin
cp -r resources bin/resources

cd build
cmake ../
