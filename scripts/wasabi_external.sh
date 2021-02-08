#!/usr/bin/env bash

# exit on error
set -e

warning() {
    printf '\n\033[1;33mWarning\033[0m: %s\n\n' "$1" 
}

source ./scripts/set_env.sh

mkdir -p $WASABI_EXTERNAL_DIR


echo
echo -----------------------------------
echo " Installing external tools into " $WASABI_EXTERNAL_DIR


echo
echo -----------------------------------
echo ---------- install clang ----------
export LLVM_VERSION=11.0.0
echo " $(date +"%T")"
echo "version: $LLVM_VERSION"
if [ ! -d "$LLVM_DIR" ]
then
    wget -qO - https://github.com/llvm/llvm-project/releases/download/llvmorg-$LLVM_VERSION/clang+llvm-$LLVM_VERSION-x86_64-linux-gnu-ubuntu-20.04.tar.xz | tar xfJ - -C $WASABI_EXTERNAL_DIR/ && \rm -rf $LLVM_DIR && mv $WASABI_EXTERNAL_DIR/clang+llvm-$LLVM_VERSION-x86_64-linux-gnu-ubuntu-20.04 $LLVM_DIR
else
	echo "Clang already installed in '$LLVM_DIR'"
fi

echo -----------------------------------
echo ---- install clang wasm librt  ----
echo " $(date +"%T")"
wget -qO - https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-12/libclang_rt.builtins-wasm32-wasi-12.0.tar.gz | tar xfz - -C $WASABI_EXTERNAL_DIR/llvm/lib/clang/11.0.0

echo
echo -----------------------------------
echo ------ install wasm sysroot  ------
echo " $(date +"%T")"
\rm -rf $WASI_SDK_DIR
mkdir -p $WASI_SDK_DIR
wget -qO - https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-12/wasi-sysroot-12.0.tar.gz | tar xfz - -C $WASI_SDK_DIR

echo
echo -----------------------------------
echo ------- install wasmtime runtime --
echo " $(date +"%T")"
# see packages on https://github.com/bytecodealliance/wasmtime/releases

\rm -rf $WASMTIME_DIR
\rm -rf $WASMTIME_LINUX_DIR
wget -qO - https://github.com/bytecodealliance/wasmtime/releases/download/v0.22.0/wasmtime-v0.22.0-x86_64-linux.tar.xz | tar xfJ -  -C $WASABI_EXTERNAL_DIR/ && mv $WASMTIME_LINUX_DIR $WASMTIME_DIR


echo
echo -----------------------------------
echo ---- test with wasmtime runtime ---
echo " $(date +"%T")"
\rm -rf $WASABI_ROOT_DIR/src/test/wasi/build
mkdir $WASABI_ROOT_DIR/src/test/wasi/build
cd $WASABI_ROOT_DIR/src/test/wasi/build
cmake .. &> compile.log
make >> compile.log
make test


echo
echo -----------------------------------
echo -------------- cJSON --------------
echo " $(date +"%T")"
\rm -rf $CJSON_DIR
cd $WASABI_EXTERNAL_DIR/
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
	-DCMAKE_SYSROOT=$SYSROOT_DIR \
	\
	-DCMAKE_C_COMPILER=$C_COMPILER \
	-DCMAKE_C_COMPILER_TARGET=wasm32-unknown-wasi \
	-DCMAKE_CXX_COMPILER_TARGET=wasm32-unknown-wasi \
	-DCMAKE_AR=$LLVM_AR \
	-DCMAKE_C_FLAGS=-fno-stack-protector \
	\
	-DCMAKE_INSTALL_PREFIX=$CJSON_DIR/install \
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
$WASMTIME $CJSON_DIR/build/cJSON_test &> /dev/null 
if [ $? -ne 0 ]
then
	echo "Error: cJSON test didn't works" 
	exit 1
else
	echo "cJSON test passed"
fi

echo "End $(date +"%T")"