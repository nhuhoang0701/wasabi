#!/usr/bin/env bash

# exit on error
set -e

warning() {
    printf '\n\033[1;33mWarning\033[0m: %s\n\n' "$1" 
}

source ./scripts/set_env.sh

mkdir -p $WASABI_EXTERNAL_DIR

export outfile=$WASABI_EXTERNAL_DIR/output_ext.log

echo
echo -----------------------------------
echo " Installing external tools into '$WASABI_EXTERNAL_DIR'"
echo "     log of this script will be in : '$outfile'"
echo " Installing external tools into $WASABI_EXTERNAL_DIR" > $outfile


$WASABI_ROOT_DIR/scripts/external/cmake.sh | tee -a $outfile
$WASABI_ROOT_DIR/scripts/external/ninja.sh | tee -a $outfile
export LLVM_VERSION=11.0.0
$WASABI_ROOT_DIR/scripts/external/clang_llvm.sh | tee -a $outfile
if [ "$WASABI_TOOLCHAIN" = "yes" ]
then
	echo
	echo -----------------------------------
	echo -------- compile toolhain ---------
	$WASABI_ROOT_DIR/scripts/external/musl.sh | tee -a $outfile
fi
#$WASABI_ROOT_DIR/scripts/external/binaryen.sh | tee -a $outfile
$WASABI_ROOT_DIR/scripts/external/wasmtime.sh | tee -a $outfile
$WASABI_ROOT_DIR/scripts/external/wasmsysroot.sh | tee -a $outfile


$WASABI_ROOT_DIR/scripts/external/sqlite.sh | tee -a $outfile
$WASABI_ROOT_DIR/scripts/external/cjson.sh | tee -a $outfile


echo "End $(date +"%T")"
