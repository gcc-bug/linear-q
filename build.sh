#!/bin/bash

# Remove the "build" directory and its contents
rm -rf build

# Create the "build" directory
mkdir build

# Change to the "build" directory
cd build

# Run CMake to generate Makefiles
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Build your project
make

cd test
./test_ro
# Pause to keep the terminal open (optional)
read -p "Press Enter to exit..."
