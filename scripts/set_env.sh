#!/usr/bin/env bash


echo -----------------------------------
echo --------- set variables  ----------
if [ ! -d "$WASABI_ROOT_DIR" ];
then
echo "error: variable WASABI_ROOT_DIR not set";
return 1;
else
echo "WASABI_ROOT_DIR: " "'"$WASABI_ROOT_DIR"'";
fi


export WASABI_EXTERNAL_DIR=$WASABI_ROOT_DIR/external
echo "WASABI_EXTERNAL_DIR: " "'"$WASABI_EXTERNAL_DIR"'";

export LLVM_DIR=$WASABI_EXTERNAL_DIR/llvm
export LLVM_AR=$LLVM_DIR/bin/llvm-ar
export LLVM_SPLIT=$LLVM_DIR/bin/llvm-split

export C_COMPILER=$LLVM_DIR/bin/clang
export CXX_COMPILER=$LLVM_DIR/bin/clang++

export WASI_SDK_DIR=$WASABI_EXTERNAL_DIR/wasi-sdk-12
export SYSROOT_DIR=$WASI_SDK_DIR/wasi-sysroot
echo "SYSROOT_DIR: " "'"$SYSROOT_DIR"'";

export WASMTIME_DIR=$WASABI_EXTERNAL_DIR/wasmtime-v0.22.0
export WASMTIME_LINUX_DIR=$WASMTIME_DIR-x86_64-linux
export WASMTIME=$WASMTIME_DIR/wasmtime
echo "WASMTIME: " "'"$WASMTIME"'";

export CJSON_DIR=$WASABI_EXTERNAL_DIR/cJSON
export SQLITE_DIR=$WASABI_EXTERNAL_DIR/sqlite

export HTTP_SERVER="python3 -m http.server 8080"
echo "HTTP_SERVER: " "'"$HTTP_SERVER"'";

echo
