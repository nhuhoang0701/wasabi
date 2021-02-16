#!/usr/bin/env bash

# exit on error
set -e

warning() {
    printf '\n\033[1;33mWarning\033[0m: %s\n\n' "$1"
}

source ./scripts/set_env.sh

export outfile=$WASABI_EXTERNAL_DIR/output_src.log

echo
echo -----------------------------------
echo " Compiling/installing wasabi/src"
echo " Compiling/installing wasabi/src" > $outfile

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
$CMAKE .. >> $outfile
$CMAKE --build . >> $outfile
$CMAKE --install . >> $outfile

echo
echo -----------------------------------
echo ---------- test C -----------------
echo " $(date +"%T")"
cd $WASABI_ROOT_DIR/src/test/C
\rm -rf build
mkdir build
cd build
$CMAKE .. >> $outfile
$CMAKE --build . >> $outfile
$CTEST

echo
echo -----------------------------------
echo --------- test CPP ----------------
echo " $(date +"%T")"
cd $WASABI_ROOT_DIR/src/test/CPP
\rm -rf build
mkdir build
cd build
$CMAKE .. >> $outfile
$CMAKE --build . >> $outfile
$CTEST

echo
echo -----------------------------------
echo -------------- json ---------------
echo " $(date +"%T")"
cd $WASABI_ROOT_DIR/src/json
\rm -rf build
mkdir build
cd build
$CMAKE .. >> $outfile
$CMAKE --build . >> $outfile
$CMAKE --install . >> $outfile
$CTEST


echo "End $(date +"%T")"
