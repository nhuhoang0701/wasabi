#!/usr/bin/env bash

echo
echo -----------------------------------
echo --------- set variables -----------
export WASABI_PLATFORM_TARGET=${WASABI_PLATFORM_TARGET:-wasm}
echo -e "WASABI_PLATFORM_TARGET=wasm|linux: " "\t'"$WASABI_PLATFORM_TARGET"'";
export WASABI_LLVM=${WASABI_LLVM:-external}
echo -e "WASABI_LLVM=compiled|external|git|loca}: " "'"$WASABI_LLVM"'";

export WASABI_BUILD_TYPE=${WASABI_BUILD_TYPE:-Debug}
echo -e "WASABI_BUILD_TYPE=Debug|Release|RelWithDebInfo: " "\t'"$WASABI_BUILD_TYPE"'";

export WASABI_ROOT_DIR=${WASABI_ROOT_DIR:-$(pwd)}
echo -e "WASABI_ROOT_DIR: " "\t'"$WASABI_ROOT_DIR"'";
export WASABI_INSTAL_DIR=$WASABI_ROOT_DIR/install/$WASABI_PLATFORM_TARGET'/'$WASABI_BUILD_TYPE
echo -e "WASABI_INSTAL_DIR: " "\t'"$WASABI_INSTAL_DIR"'";
export WASABI_EXTERNAL_DIR=$WASABI_ROOT_DIR/external
echo -e "WASABI_EXTERNAL_DIR: " "\t'"$WASABI_EXTERNAL_DIR"'";

export WASABI_BUILD_DIR_NAME=./build/$WASABI_PLATFORM_TARGET'/'$WASABI_BUILD_TYPE
echo -e "WASABI_BUILD_DIR_NAME: " "\t'"$WASABI_BUILD_DIR_NAME"'";

export LLVM_VERSION=11.0.0
LLVM_ARCH=${LLVM_ARCH:-x86_64}

# LLVM_OS
if [ "$(uname)" = "Darwin" ]
then
    export LLVM_OS=apple-darwin
elif [ "$(expr substr $(uname -s) 1 5)" = "Linux" ]
then
    export LLVM_OS=linux-gnu-ubuntu-20.04
fi

export LLVM_COMPRESSED_FILE=clang+llvm-$LLVM_VERSION-$LLVM_ARCH-$LLVM_OS
export LLVM_DIR=${LLVM_DIR:-$WASABI_EXTERNAL_DIR/$LLVM_COMPRESSED_FILE}

echo -e "LLVM_DIR: " "\t\t'"$LLVM_DIR"'";

export LLVM_AR=$LLVM_DIR/bin/llvm-ar
export LLVM_SPLIT=$LLVM_DIR/bin/llvm-split
export C_COMPILER=$LLVM_DIR/bin/clang
export CXX_COMPILER=$LLVM_DIR/bin/clang++

export SYSROOT_DIR=$WASABI_EXTERNAL_DIR/sysroot

export SYSROOT_LINUX_DIR=$SYSROOT_DIR/linux-sysroot
echo -e "SYSROOT_LINUX_DIR: " "\t\t'"$SYSROOT_LINUX_DIR"'";
# export SYSROOT_WASI_DIR=$SYSROOT_DIR/wasi-sysroot
# echo -e "SYSROOT_WASI_DIR: " "\t\t'"$SYSROOT_WASI_DIR"'";

export MUSL_DIR=${MUSL_DIR:-$WASABI_EXTERNAL_DIR/musl}
echo -e "MUSL_DIR: " "\t\t'"$MUSL_DIR"'";

export BINARYEN_DIR=$WASABI_EXTERNAL_DIR/binaryen
echo -e "BINARYEN_DIR: " "\t\t'"$BINARYEN_DIR"'";

# export WASMSDK_DIR=$WASABI_EXTERNAL_DIR/wasmsdk4compile
# echo -e "WASMSDK_DIR: " "\t\t'"$WASMSDK_DIR"'";

export WASMTIME=${WASMTIME:-$WASABI_EXTERNAL_DIR/wasmtime/wasmtime}
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

# Needed to get call with symbolsfrom
export ASAN_SYMBOLIZER_PATH=$LLVM_DIR/bin/llvm-symbolizer
export MSAN_SYMBOLIZER_PATH=$ASAN_SYMBOLIZER_PATH

# alias
echo
echo -----------------------------------
echo ----------- set alias -------------
echo -e  "run_server           to execute the server";
echo -e  "clean                clean the current cmake folder";
echo -e  "compile              to compile the current cmake folder";
echo -e  "install              to install the current cmake folder";
echo -e  "test                 to test the current cmake folder";
echo -e  "build                to compile test the current cmake folder";
echo -e  "rebuild              to clean compile test the current cmake folder";

alias setenv='unset WASABI_ROOT_DIR && source ./scripts/set_env.sh'
alias run_server='python3 -m http.server 8080 -d $WASABI_INSTAL_DIR'
alias clean='$CMAKE --build $WASABI_BUILD_DIR_NAME --target clean'
alias compile='$CMAKE --build $WASABI_BUILD_DIR_NAME'
alias test='(cd "$WASABI_BUILD_DIR_NAME" && $CTEST -V)'
alias test_none_reg='(cd "$WASABI_BUILD_DIR_NAME" && $CTEST -j8 -T test --output-on-failure)'
alias install='$CMAKE --build $WASABI_BUILD_DIR_NAME --target install'
alias build='$CMAKE -B $WASABI_BUILD_DIR_NAME . -G Ninja -DCMAKE_BUILD_TYPE=$WASABI_BUILD_TYPE -DCMAKE_MAKE_PROGRAM=$NINJA -DCMAKE_INSTALL_PREFIX:PATH=$WASABI_INSTAL_DIR && install && test_none_reg'
alias rebuild='rm -rf $WASABI_BUILD_DIR_NAME && mkdir -p $WASABI_BUILD_DIR_NAME && build'

export METHOD_ASYNC=doIt,processQuery*,getResponse*,ina::metadata::Cube::Cube*,wasabi::metadata::Catalog::Catalog*,dbproxy::DBProxy::getDBProxy*,dbproxy::DBSQLite::DBSQLite*,sqlite3_open_v2,openDatabase*,sqlite3BtreeOpen*,sqlite3PagerOpen*,sqlite3OsOpen*,demoOpen,open*,__wasilibc_openat_nomode
alias asyncify='($WASABI_EXTERNAL_DIR/binaryen/install/bin/wasm-opt -Oz --pass-arg=asyncify-addlist@$METHOD_ASYNC --pass-arg=asyncify-imports@wasi_snapshot_preview1.path_open --asyncify $WASABI_INSTAL_DIR/InA_Interpreter.wasm -o $WASABI_INSTAL_DIR/InA_Interpreter.wasm )'
alias asyncify_test='($WASABI_EXTERNAL_DIR/binaryen/install/bin/wasm-opt --pass-arg=asyncify-ignore-indirect --pass-arg=asyncify-imports@wasi_snapshot_preview1.path_open --asyncify $WASABI_INSTAL_DIR/bin/C_read.wasm -o $WASABI_INSTAL_DIR/bin/C_read.wasm)'

alias wasmopt='(export LD_LIBRARY_PATH=$WASABI_EXTERNAL_DIR/binaryen/install/lib && $WASABI_EXTERNAL_DIR/binaryen/install/bin/wasm-opt $WASABI_INSTAL_DIR/InA_Interpreter.wasm -Oz -o $WASABI_INSTAL_DIR/InA_Interpreter.wasm && chmod 777 $WASABI_INSTAL_DIR/InA_Interpreter.wasm)'
alias exportwasm2nexus='(mvn -e -f $WASABI_ROOT_DIR/nexus/wasm/pom.xml clean deploy)'
alias exportlinux2nexus='(mvn -e -f $WASABI_ROOT_DIR/nexus/native/pom.xml clean deploy)'

alias export2firefly='(
scp $WASABI_INSTAL_DIR/InA_Interpreter.wasm ccloud@10.47.240.98:/srv/tomcat/webapps/ROOT/wasabi/resources/sap/zen/commons/thirdparty/wasabi/InA_Interpreter.wasm
scp $WASABI_INSTAL_DIR/Microcube_worker.js  ccloud@10.47.240.98:/srv/tomcat/webapps/ROOT/wasabi/resources/sap/zen/commons/thirdparty/wasabi/Microcube_worker.js
scp $WASABI_INSTAL_DIR/resources/response_getSerververInfo.json  ccloud@10.47.240.98:/srv/tomcat/webapps/ROOT/wasabi/resources/sap/zen/commons/thirdparty/wasabi/resources/response_getSerververInfo.json
scp $WASABI_INSTAL_DIR/resources/sqlite/efashion_lite/efashion_lite.db  ccloud@10.47.240.98:/srv/tomcat/webapps/ROOT/wasabi/resources/sap/zen/commons/thirdparty/wasabi/resources/sqlite/efashion_lite/efashion_lite.db
)'

echo
