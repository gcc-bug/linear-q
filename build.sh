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
echo "test find steiner tree:"
./test_steiner
echo "test row operation:"
./test_ro
echo "test linear synthesis:"
./test_ls
# Pause to keep the terminal open (optional)
read -p "Press Enter to exit..."
