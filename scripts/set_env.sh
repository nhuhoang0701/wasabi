#!/usr/bin/env bash


echo -----------------------------------
echo --------- set variables  ----------
export WASABI_ROOT_DIR=$(pwd)
echo -e "WASABI_ROOT_DIR: " "\t'"$WASABI_ROOT_DIR"'";
export WASABI_EXTERNAL_DIR=$WASABI_ROOT_DIR/external
echo -e "WASABI_EXTERNAL_DIR: " "\t'"$WASABI_EXTERNAL_DIR"'";

export WASABI_CMAKE_DIR=${WASABI_CMAKE_DIR:-$WASABI_EXTERNAL_DIR/cmake}

export LLVM_DIR=${LLVM_DIR:-$WASABI_EXTERNAL_DIR/llvm}
export LLVM_AR=$LLVM_DIR/bin/llvm-ar
export LLVM_SPLIT=$LLVM_DIR/bin/llvm-split

export C_COMPILER=$LLVM_DIR/bin/clang
echo -e "C_COMPILER: " "\t\t'"$C_COMPILER"'";

export CXX_COMPILER=$LLVM_DIR/bin/clang++
echo -e "CXX_COMPILER: " "\t\t'"$CXX_COMPILER"'";

export WASI_SDK_DIR=$WASABI_EXTERNAL_DIR/wasi-sdk-12
export SYSROOT_DIR=$WASI_SDK_DIR/wasi-sysroot
echo -e "SYSROOT_DIR: " "\t\t'"$SYSROOT_DIR"'";

export WASMTIME_DIR=$WASABI_EXTERNAL_DIR/wasmtime-v0.22.0
export WASMTIME_LINUX_DIR=$WASMTIME_DIR-x86_64-linux
export WASMTIME=$WASMTIME_DIR/wasmtime
echo -e "WASMTIME: " "\t\t'"$WASMTIME"'";

export CJSON_DIR=$WASABI_EXTERNAL_DIR/cJSON
export SQLITE_DIR=$WASABI_EXTERNAL_DIR/sqlite

export CMAKE=${CMAKE:-$WASABI_CMAKE_DIR/bin/cmake}
echo -e "CMAKE: "  "\t\t'"$CMAKE"'";
export CTEST=${CTEST:-$WASABI_CMAKE_DIR/bin/ctest}
echo -e "CTEST: "  "\t\t'"$CTEST"'";

export MAKE=${MAKE:-make}
echo -e "MAKE: "  "\t\t\t'"$MAKE"'";

export NINJA=${NINJA:-ninja}
echo -e "NINJA: "  "\t\t'"$NINJA"'";


export HTTP_SERVER="python3 -m http.server 8080"
echo -e "HTTP_SERVER: " "\t\t'"$HTTP_SERVER"'";

alias run_wasabi_server='cd $WASABI_ROOT_DIR/src/wasi_browser; $HTTP_SERVER;cd -'

echo
