#!/usr/bin/env bash

echo
echo -----------------------------------
echo --------- set variables -----------
export WASABI_USE_WASM=${WASABI_USE_WASM:-yes}
echo -e "WASABI_USE_WASM: " "\t'"$WASABI_USE_WASM"'";

export WASABI_ROOT_DIR=${WASABI_ROOT_DIR:-$(pwd)}
echo -e "WASABI_ROOT_DIR: " "\t'"$WASABI_ROOT_DIR"'";
export WASABI_EXTERNAL_DIR=$WASABI_ROOT_DIR/external
echo -e "WASABI_EXTERNAL_DIR: " "\t'"$WASABI_EXTERNAL_DIR"'";

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

export WASMTIME_DIR=${WASMTIME_DIR:-$WASABI_EXTERNAL_DIR/wasmtime-v0.22.0}
export WASMTIME_LINUX_DIR=$WASMTIME_DIR-x86_64-linux
export WASMTIME=$WASMTIME_DIR/wasmtime
echo -e "WASMTIME: " "\t\t'"$WASMTIME"'";
export WASMTIME_BACKTRACE_DETAILS=1

export CJSON_DIR=$WASABI_EXTERNAL_DIR/cJSON
export SQLITE_DIR=$WASABI_EXTERNAL_DIR/sqlite

export WASABI_CMAKE_DIR=${WASABI_CMAKE_DIR:-$WASABI_EXTERNAL_DIR/cmake}
export CMAKE=${CMAKE:-$WASABI_CMAKE_DIR/bin/cmake}
echo -e "CMAKE: "  "\t\t'"$CMAKE"'";
export CTEST=${CTEST:-$WASABI_CMAKE_DIR/bin/ctest}
echo -e "CTEST: "  "\t\t'"$CTEST"'";

export MAKE=${MAKE:-make}
echo -e "MAKE: "  "\t\t\t'"$MAKE"'";

export WASABI_NINJA_DIR=${WASABI_NINJA_DIR:-$WASABI_EXTERNAL_DIR/ninja}
export NINJA=${NINJA:-$WASABI_NINJA_DIR/ninja}
echo -e "NINJA: "  "\t\t'"$NINJA"'";


export HTTP_SERVER="python3 -m http.server 8080"
echo -e "HTTP_SERVER: " "\t\t'"$HTTP_SERVER"'";


# alias
echo
echo -----------------------------------
echo ----------- set alias -------------
echo -e  "run_wasabi_server    to execute the server";
echo -e  "clean                clean the current cmake folder";
echo -e  "compile              to compile the current cmake folder";
echo -e  "test                 to test the current cmake folder";
echo -e  "build                to compile test the current cmake folder";
echo -e  "rebuild              to clean compile test the current cmake folder";

alias setenv='unset WASABI_ROOT_DIR && source ./scripts/set_env.sh'
alias run_wasabi_server='cd $WASABI_ROOT_DIR/src/wasi_browser; $HTTP_SERVER;cd -'
alias clean='$CMAKE --build ./build --target clean'
alias test='(cd build && $CTEST -V)'
alias compile='$CMAKE --build ./build'
alias build='$CMAKE -B ./build . -G Ninja -DCMAKE_MAKE_PROGRAM=$NINJA -DCMAKE_INSTALL_PREFIX:PATH=$WASABI_ROOT_DIR/src/install/ -DWASABI_USE_WASM=yes && $CMAKE --build ./build --target install && test'
alias rebuild='rm -rf ./build && mkdir build && build'

echo
