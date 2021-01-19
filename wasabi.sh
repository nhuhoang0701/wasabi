#!/usr/bin/env bash


warning() {
    printf '\n\033[1;33mWarning\033[0m: %s\n\n' "$1" 
}



export WASABI_ROOT=$(pwd)
export WASABI_EXTERNAL=$(pwd)/external

mkdir $WASABI_EXTERNAL


echo
echo -----------------------------------
echo " Installing external tools into " $WASABI_EXTERNAL


echo
echo -----------------------------------
echo ---------- install clang ----------
echo " $(date +"%T")"
\rm -rf $WASABI_EXTERNAL/llvm
\rm -rf $WASABI_EXTERNAL/clang+llvm-11.0.0-x86_64-linux-gnu-ubuntu-20.04
wget -qO - https://github.com/llvm/llvm-project/releases/download/llvmorg-11.0.0/clang+llvm-11.0.0-x86_64-linux-gnu-ubuntu-20.04.tar.xz | tar xfJ - -C $WASABI_EXTERNAL/ && mv $WASABI_EXTERNAL/clang+llvm-11.0.0-x86_64-linux-gnu-ubuntu-20.04 $WASABI_EXTERNAL/llvm

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
echo ---- cJSON install and compile ----
echo " $(date +"%T")"
\rm -rf $WASABI_EXTERNAL/cJSON
cd $WASABI_EXTERNAL/
git clone https://github.com/DaveGamble/cJSON.git
cd cJSON
mkdir build
cd build
cmake .. \
	-DCMAKE_SYSTEM_NAME=Generic \
	-DCMAKE_SYSTEM_PROCESSOR=wasm \
	-DCMAKE_C_COMPILER_WORKS=1 \
	-DCMAKE_CROSSCOMPILING=1 \
	-DCLANG_TARGET_TRIPLE=wasm32-unknown-wasi \
	-DCMAKE_SYSROOT=$WASABI_EXTERNAL/wasi-sdk-12/wasi-sysroot \
	-DCMAKE_C_COMPILER=$WASABI_EXTERNAL/llvm/bin/clang \
	-DCMAKE_C_COMPILER_TARGET=wasm32-unknown-wasi \
	-DCMAKE_CXX_COMPILER_TARGET=wasm32-unknown-wasi \
	-DCMAKE_AR=$WASABI_EXTERNAL/llvm/bin/llvm-ar \
	-DENABLE_CJSON_TEST=off \
	-DBUILD_SHARED_LIBS=off
make

echo "End $(date +"%T")"
