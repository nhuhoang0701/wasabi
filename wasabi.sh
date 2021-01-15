#!/usr/bin/env bash


warning() {
    printf '\n\033[1;33mWarning\033[0m: %s\n\n' "$1" 
}



export WASABI_ROOT=$(pwd)

echo "Installing into " $WASABI_ROOT


echo -----------------------------------
echo ---------- install clang ----------
\rm -rf $WASABI_ROOT/llvm
\rm -rf $WASABI_ROOT/clang+llvm-11.0.0-x86_64-linux-gnu-ubuntu-20.04
wget -qO - https://github.com/llvm/llvm-project/releases/download/llvmorg-11.0.0/clang+llvm-11.0.0-x86_64-linux-gnu-ubuntu-20.04.tar.xz | tar xfJ - 
sleep 1
mv $WASABI_ROOT/clang+llvm-11.0.0-x86_64-linux-gnu-ubuntu-20.04 $WASABI_ROOT/llvm

echo
echo -----------------------------------
echo --- install wasm librt in clang ---
wget -qO - https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-12/libclang_rt.builtins-wasm32-wasi-12.0.tar.gz | tar xfz - -C $WASABI_ROOT/llvm/lib/clang/11.0.0

echo
echo -----------------------------------
echo ------ install wasm sysroot  ------
\rm -rf $WASABI_ROOT/wasi-sdk-12
mkdir -p $WASABI_ROOT/wasi-sdk-12
wget -qO - https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-12/wasi-sysroot-12.0.tar.gz | tar xfz - -C $WASABI_ROOT/wasi-sdk-12

echo
echo -----------------------------------
echo ------- install wasmtime runtime --
# see packages on https://github.com/bytecodealliance/wasmtime/releases

wget -qO - https://github.com/bytecodealliance/wasmtime/releases/download/v0.22.0/wasmtime-v0.22.0-x86_64-linux.tar.xz | tar xfJ -
mv $WASABI_ROOT/wasmtime-v0.22.0-x86_64-linux $WASABI_ROOT/wasmtime-v0.22.0


