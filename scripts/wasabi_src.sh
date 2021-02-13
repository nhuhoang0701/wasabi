#!/usr/bin/env bash

# exit on error
set -e

warning() {
    printf '\n\033[1;33mWarning\033[0m: %s\n\n' "$1"
}

source ./scripts/set_env.sh

export outfile=./output_src.log

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
$CMAKE .. &> $outfile
$MAKE  &> $outfile
$MAKE install

echo
echo -----------------------------------
echo -------------- json ---------------
echo " $(date +"%T")"
cd $WASABI_ROOT_DIR/src/json
\rm -rf build
mkdir build
cd build
$CMAKE .. &> $outfile
$CMAKE --build .  &> $outfile
$MAKE install &> $outfile
ctest


echo "End $(date +"%T")"
