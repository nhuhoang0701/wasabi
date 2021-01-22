#!/usr/bin/env bash

export WASABI_ROOT=$(pwd)
export WASABI_EXTERNAL=$WASABI_ROOT/external

export LLVM=$WASABI_EXTERNAL/llvm
export LLVM_AR=$LLVM/bin/llvm-ar

export C_COMPILER=$LLVM/bin/clang
export CXX_COMPILER=$LLVM/bin/clang++

export WASI_SDK=$WASABI_EXTERNAL/wasi-sdk-12
export SYSROOT=$WASI_SDK/wasi-sysroot

export WASMTIME=$WASABI_EXTERNAL/wasmtime-v0.22.0
export WASMTIME_LINUX=$WASMTIME-x86_64-linux

export CJSON=$WASABI_EXTERNAL/cJSON