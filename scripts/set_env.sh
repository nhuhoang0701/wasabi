#!/usr/bin/env bash

echo
echo -----------------------------------
echo --------- set variables -----------
export WASABI_PLATFORM_TARGET=${WASABI_PLATFORM_TARGET:-wasm}
echo -e "WASABI_PLATFORM_TARGET{wasm|linux}: " "\t'"$WASABI_PLATFORM_TARGET"'";
export WASABI_LLVM=${WASABI_LLVM:-external}
echo -e "WASABI_LLVM{compiled|external|git|local}: " "'"$WASABI_LLVM"'";

export WASABI_ROOT_DIR=${WASABI_ROOT_DIR:-$(pwd)}
echo -e "WASABI_ROOT_DIR: " "\t'"$WASABI_ROOT_DIR"'";
export WASABI_INSTAL_DIR=$WASABI_ROOT_DIR/install_$WASABI_PLATFORM_TARGET
echo -e "WASABI_INSTAL_DIR: " "\t'"$WASABI_INSTAL_DIR"'";
export WASABI_EXTERNAL_DIR=$WASABI_ROOT_DIR/external
echo -e "WASABI_EXTERNAL_DIR: " "\t'"$WASABI_EXTERNAL_DIR"'";

export LLVM_DIR=${LLVM_DIR:-$WASABI_EXTERNAL_DIR/llvm}
echo -e "LLVM_DIR: " "\t\t'"$LLVM_DIR"'";
export LLVM_AR=$LLVM_DIR/bin/llvm-ar
export LLVM_SPLIT=$LLVM_DIR/bin/llvm-split
export C_COMPILER=$LLVM_DIR/bin/clang
export CXX_COMPILER=$LLVM_DIR/bin/clang++

export SYSROOT_DIR=$WASABI_EXTERNAL_DIR/sysroot

export SYSROOT_LINUX_DIR=$SYSROOT_DIR/linux-sysroot
echo -e "SYSROOT_LINUX_DIR: " "\t\t'"$SYSROOT_LINUX_DIR"'";
export SYSROOT_WASI_DIR=$SYSROOT_DIR/wasi-sysroot
echo -e "SYSROOT_WASI_DIR: " "\t\t'"$SYSROOT_WASI_DIR"'";

export MUSL_DIR=${MUSL_DIR:-$WASABI_EXTERNAL_DIR/musl}
echo -e "MUSL_DIR: " "\t\t'"$MUSL_DIR"'";

export BINARYEN_DIR=$WASABI_EXTERNAL_DIR/binaryen
echo -e "BINARYEN_DIR: " "\t\t'"$BINARYEN_DIR"'";

export WASMTIME_DIR=$WASABI_EXTERNAL_DIR/wasmtime-v0.22.0
export WASMTIME_LINUX_DIR=$WASMTIME_DIR-x86_64-linux
export WASMTIME=${WASMTIME:-$WASMTIME_DIR/wasmtime}
echo -e "WASMTIME: " "\t\t'"$WASMTIME"'";
export WASMTIME_BACKTRACE_DETAILS=1

export WASABI_CMAKE_DIR=$WASABI_EXTERNAL_DIR/cmake
export CMAKE=${CMAKE:-$WASABI_CMAKE_DIR/bin/cmake}
echo -e "CMAKE: "  "\t\t'"$CMAKE"'";
export CTEST=${CTEST:-$WASABI_CMAKE_DIR/bin/ctest}
echo -e "CTEST: "  "\t\t'"$CTEST"'";

export WASABI_NINJA_DIR=$WASABI_EXTERNAL_DIR/ninja
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
echo -e  "install              to install the current cmake folder";
echo -e  "test                 to test the current cmake folder";
echo -e  "build                to compile test the current cmake folder";
echo -e  "rebuild              to clean compile test the current cmake folder";

alias setenv='unset WASABI_ROOT_DIR && source ./scripts/set_env.sh'
alias run_server='cd $WASABI_INSTAL_DIR; $HTTP_SERVER;cd -'
alias clean='$CMAKE --build ./build_$WASABI_PLATFORM_TARGET --target clean'
alias compile='$CMAKE --build ./build_$WASABI_PLATFORM_TARGET'
alias test='(cd build_$WASABI_PLATFORM_TARGET && $CTEST -j8 -T test --output-on-failure)'
alias install='$CMAKE --build ./build_$WASABI_PLATFORM_TARGET --target install'
alias build='$CMAKE -B ./build_$WASABI_PLATFORM_TARGET . -G Ninja -DCMAKE_MAKE_PROGRAM=$NINJA -DCMAKE_INSTALL_PREFIX:PATH=$WASABI_INSTAL_DIR && install && test'
alias rebuild='rm -rf ./build_$WASABI_PLATFORM_TARGET && mkdir build_$WASABI_PLATFORM_TARGET && build'

echo
