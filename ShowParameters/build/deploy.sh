#!/bin/bash

# script is supposed to be executed in /build directory

# remove target directory if it exists
if [ -d ShowParameters ]; then
  rm -rf ShowParameters 
fi &&

# remove target FMU if it exists
if [ -f ShowParameters.fmu ]; then
    rm ShowParameters.fmu 
fi &&

# create subdir and change into it
mkdir -p ShowParameters &&
cd ShowParameters &&

# create binary dir for Linux
mkdir -p binaries/linux64 &&

# copy shared library, we expect it to be already renamed correctly
cp ../../bin/release/ShowParameters.so binaries/linux64/ShowParameters.so &&
cp ../../data/modelDescription.xml . &&

# create zip archive
7za a ../ShowParameters.zip . | cat > /dev/null &&
cd .. && 
mv ShowParameters.zip ShowParameters.fmu &&
echo "Created ShowParameters.fmu" &&

# change working directory back to original dir
cd -

