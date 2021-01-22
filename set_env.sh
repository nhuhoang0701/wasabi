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

export PYTHON=python3
export MAKE=make
export CMAKE=cmake

echo -----------------------------------
echo ----- check $PYTHON install  ------
dpkg -s $PYTHON &> /dev/null 

if [ $? -ne 0 ]

        then
            echo "$PYTHON not installed. Installing..."  
            sudo apt-get update
            sudo apt-get install $PYTHON

        else
            echo    "$PYTHON installed."
    fi

echo -----------------------------------
echo ------ check $MAKE install  -------
dpkg -s $MAKE &> /dev/null 

if [ $? -ne 0 ]

        then
            echo "$MAKE not installed. Installing..."  
            sudo apt-get update
            sudo apt-get install $MAKE

        else
            echo    "$MAKE installed."
    fi

echo -----------------------------------
echo ------ check $CMAKE install  ------
dpkg -s $CMAKE &> /dev/null 

if [ $? -ne 0 ]

        then
            echo "$CMAKE not installed. InstalLing..."  
            sudo apt-get update
            sudo apt-get install $CMAKE

        else
            echo    "$CMAKE installed."
    fi