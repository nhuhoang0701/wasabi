#!/usr/bin/env bash

# exit on error
set -e

warning() {
    printf '\n\033[1;33mWarning\033[0m: %s\n\n' "$1"
}

source ./scripts/set_env.sh



echo
echo -----------------------------------
echo " Compiling/installing wasabi/src"

echo
echo -----------------------------------
echo " Cleaning up install directory"
\rm -rf $WASABI_ROOT_DIR/src/build

echo
echo -----------------------------------
echo ----------- exceptions ------------
echo " $(date +"%T")"
cd $WASABI_ROOT_DIR/src/exceptions
\rm -rf build
mkdir build
cd build
cmake ..
make
make install

echo
echo -----------------------------------
echo -------------- json ---------------
echo " $(date +"%T")"
cd $WASABI_ROOT_DIR/src/json
\rm -rf build
mkdir build
cd build
cmake ..
cmake --build .
make install
ctest


echo "End $(date +"%T")"
