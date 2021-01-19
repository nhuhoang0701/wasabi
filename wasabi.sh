#!/usr/bin/env bash

# exit on error
set -e

warning() {
    printf '\n\033[1;33mWarning\033[0m: %s\n\n' "$1" 
}



export WASABI_ROOT=$(pwd)
export WASABI_EXTERNAL=$(pwd)/external

mkdir -p $WASABI_EXTERNAL


echo
echo -----------------------------------
echo " Installing external tools into " $WASABI_EXTERNAL


echo
echo -----------------------------------
echo ---------- install clang ----------
export LLVM_VERSION=11.0.0
echo " $(date +"%T")"
echo "version: $LLVM_VERSION"
if [ ! -d "$WASABI_EXTERNAL/llvm" ]
then
    wget -qO - https://github.com/llvm/llvm-project/releases/download/llvmorg-$LLVM_VERSION/clang+llvm-$LLVM_VERSION-x86_64-linux-gnu-ubuntu-20.04.tar.xz | tar xfJ - -C $WASABI_EXTERNAL/ && \rm -rf $WASABI_EXTERNAL/llvm && mv $WASABI_EXTERNAL/clang+llvm-$LLVM_VERSION-x86_64-linux-gnu-ubuntu-20.04 $WASABI_EXTERNAL/llvm
else
	echo "Clang already installed in '$WASABI_EXTERNAL/llvm'"
fi

echo -----------------------------------
echo ---- install clang wasm librt  ----
echo " $(date +"%T")"
wget -qO - https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-12/libclang_rt.builtins-wasm32-wasi-12.0.tar.gz | tar xfz - -C $WASABI_EXTERNAL/llvm/lib/clang/11.0.0

echo
echo -----------------------------------
echo ------ install wasm sysroot  ------
echo " $(date +"%T")"
\rm -rf $WASABI_EXTERNAL/wasi-sdk-12
mkdir -p $WASABI_EXTERNAL/wasi-sdk-12
wget -qO - https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-12/wasi-sysroot-12.0.tar.gz | tar xfz - -C $WASABI_EXTERNAL/wasi-sdk-12

echo
echo -----------------------------------
echo ------- install wasmtime runtime --
echo " $(date +"%T")"
# see packages on https://github.com/bytecodealliance/wasmtime/releases

\rm -rf $WASABI_EXTERNAL/wasmtime-v0.22.0
\rm -rf $WASABI_EXTERNAL/wasmtime-v0.22.0-x86_64-linux
wget -qO - https://github.com/bytecodealliance/wasmtime/releases/download/v0.22.0/wasmtime-v0.22.0-x86_64-linux.tar.xz | tar xfJ -  -C $WASABI_EXTERNAL/ && mv $WASABI_EXTERNAL/wasmtime-v0.22.0-x86_64-linux $WASABI_EXTERNAL/wasmtime-v0.22.0


echo
echo -----------------------------------
echo ---- test with wasmtime runtime ---
echo " $(date +"%T")"
\rm -rf $WASABI_ROOT/src/test/wasi/build
mkdir $WASABI_ROOT/src/test/wasi/build
cd $WASABI_ROOT/src/test/wasi/build
cmake .. &> compile.log
make >> compile.log
$WASABI_EXTERNAL/wasmtime-v0.22.0/wasmtime ./MyExample.wasm


echo
echo -----------------------------------
echo -------------- cJSON --------------
echo " $(date +"%T")"
\rm -rf $WASABI_EXTERNAL/cJSON
cd $WASABI_EXTERNAL/
echo --------------- git ---------------
git clone https://github.com/DaveGamble/cJSON.git
cd cJSON
mkdir build
cd build

echo ------------- cmake ---------------
cmake .. \
	-DCMAKE_SYSTEM_NAME=Generic \
	-DCMAKE_SYSTEM_PROCESSOR=wasm \
	-DCMAKE_C_COMPILER_WORKS=ON \
	-DCMAKE_C_COMPILER_FORCED=ON \
	-DCMAKE_CROSSCOMPILING=ON \
	-DCMAKE_SYSROOT=$WASABI_EXTERNAL/wasi-sdk-12/wasi-sysroot \
	\
	-DCMAKE_C_COMPILER=$WASABI_EXTERNAL/llvm/bin/clang \
	-DCMAKE_C_COMPILER_TARGET=wasm32-unknown-wasi \
	-DCMAKE_CXX_COMPILER_TARGET=wasm32-unknown-wasi \
	-DCMAKE_AR=$WASABI_EXTERNAL/llvm/bin/llvm-ar \
	-DCMAKE_C_FLAGS=-fno-stack-protector \
	\
	-DCMAKE_INSTALL_PREFIX=$WASABI_EXTERNAL/cJSON/install \
	\
	-DENABLE_CJSON_TEST=on \
	-DENABLE_CJSON_UTILS=off \
	-DBUILD_SHARED_LIBS=off \
	-DENABLE_VALGRIND=off \
	-DENABLE_SANITIZERS=off \
	-DENABLE_CUSTOM_COMPILER_FLAGS=off

echo -------------- make ---------------
make cjson
make cJSON_test

echo -------------- test ---------------
$WASABI_EXTERNAL/wasmtime-v0.22.0/wasmtime $WASABI_EXTERNAL/cJSON/build/cJSON_test

echo "End $(date +"%T")"
