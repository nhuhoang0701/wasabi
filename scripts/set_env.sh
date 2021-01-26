#!/usr/bin/env bash

echo "root dir: " $WASABI_ROOT_DIR
export WASABI_EXTERNAL_DIR=$WASABI_ROOT_DIR/external

export LLVM_DIR=$WASABI_EXTERNAL_DIR/llvm
export LLVM_AR=$LLVM_DIR/bin/llvm-ar
export LLVM_SPLIT=$LLVM_DIR/bin/llvm-split

export C_COMPILER=$LLVM_DIR/bin/clang
export CXX_COMPILER=$LLVM_DIR/bin/clang++

export WASI_SDK_DIR=$WASABI_EXTERNAL_DIR/wasi-sdk-12
export SYSROOT_DIR=$WASI_SDK_DIR/wasi-sysroot

export WASMTIME_DIR=$WASABI_EXTERNAL_DIR/wasmtime-v0.22.0
export WASMTIME_LINUX_DIR=$WASMTIME_DIR-x86_64-linux
export WASMTIME=$WASMTIME_DIR/wasmtime

export CJSON_DIR=$WASABI_EXTERNAL_DIR/cJSON
