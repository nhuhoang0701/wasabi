#!/usr/bin/env bash

# exit on error
set -e

warning() {
    printf '\n\033[1;33mWarning\033[0m: %s\n\n' "$1" 
}

source ./scripts/set_env.sh

mkdir -p $WASABI_EXTERNAL_DIR

export outfile=$WASABI_EXTERNAL_DIR/output.log

echo
echo -----------------------------------
echo " Installing external tools into " $WASABI_EXTERNAL_DIR


echo
echo -----------------------------------
echo ---------- install clang ----------
echo " $(date +"%T")"
export LLVM_VERSION=${LLVM_VERSION:-11.0.0}
export LLVM_ARCH=${LLVM_ARCH:-x86_64}
export LLVM_OS=${LLVM_OS:-linux-gnu-ubuntu-20.04}
export LLVMFile=clang+llvm-$LLVM_VERSION-$LLVM_ARCH-$LLVM_OS
echo "LLVM version: $LLVMFile"
if [ ! -f "$LLVM_DIR/$LLVMFile.flag" ]
then
	rm -rf $LLVM_DIR
	rm -rf $WASABI_EXTERNAL_DIR/$LLVMFile
    wget -qO - https://github.com/llvm/llvm-project/releases/download/llvmorg-$LLVM_VERSION/$LLVMFile.tar.xz | tar xfJ - -C $WASABI_EXTERNAL_DIR/ && \rm -rf $LLVM_DIR && mv $WASABI_EXTERNAL_DIR/$LLVMFile $LLVM_DIR
	touch $LLVM_DIR/$LLVMFile.flag
else
	echo "Clang already installed in '$LLVM_DIR'"
fi


echo
echo -----------------------------------
echo ------ install wasm sysroot  ------
echo " $(date +"%T")"
export WASISDK_VERSION=${WASISDK_VERSION:-12}
echo "WASISDK version: $WASISDK_VERSION"
if [ ! -f "$WASI_SDK_DIR/wasisdk_$WASISDK_VERSION.flag" ]
then
	mkdir -p $WASI_SDK_DIR
	wget -qO - https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-$WASISDK_VERSION/wasi-sysroot-$WASISDK_VERSION.0.tar.gz | tar xfz - -C $WASI_SDK_DIR
	wget -qO - https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-$WASISDK_VERSION/libclang_rt.builtins-wasm32-wasi-$WASISDK_VERSION.0.tar.gz | tar xfz - -C $WASABI_EXTERNAL_DIR/llvm/lib/clang/$LLVM_VERSION
	touch $WASI_SDK_DIR/wasisdk_$WASISDK_VERSION.flag
else
	echo "sysroot already installed in '$WASI_SDK_DIR'"
fi

echo
echo -----------------------------------
echo ------- install wasmtime runtime --
echo " $(date +"%T")"
export WASMTIME_VERSION=${WASMTIME_VERSION:-v0.22.0}
echo "WASMTIME version: $WASMTIME_VERSION"
# see packages on https://github.com/bytecodealliance/wasmtime/releases
if [ ! -f "$WASMTIME_DIR/wasmtime_$WASMTIME_VERSION.flag" ]
then
	wget -qO - https://github.com/bytecodealliance/wasmtime/releases/download/$WASMTIME_VERSION/wasmtime-$WASMTIME_VERSION-x86_64-linux.tar.xz | tar xfJ -  -C $WASABI_EXTERNAL_DIR/ && mv $WASMTIME_LINUX_DIR $WASMTIME_DIR
	touch $WASMTIME_DIR/wasmtime_$WASMTIME_VERSION.flag
else
	echo "wasmtime already installed in '$WASMTIME_DIR'"
fi

echo
echo -----------------------------------
echo ---- test with wasmtime runtime ---
echo " $(date +"%T")"
\rm -rf $WASABI_ROOT_DIR/src/test/wasi/build
mkdir $WASABI_ROOT_DIR/src/test/wasi/build
cd $WASABI_ROOT_DIR/src/test/wasi/build
$CMAKE .. &> $outfile
$MAKE &> $outfile
$MAKE test


echo
echo -----------------------------------
echo -------------- cJSON --------------
echo " $(date +"%T")"
if [ ! -f "$CJSON_DIR/cjson.flag" ]
then
	cd $WASABI_EXTERNAL_DIR/
	echo --------------- git ---------------
	rm -rf cJSON
	git clone https://github.com/DaveGamble/cJSON.git  &> $outfile
	cd cJSON
	mkdir build
	cd build

	echo ------------- cmake ---------------
	$CMAKE .. \
		-DCMAKE_TOOLCHAIN_FILE=$WASABI_ROOT_DIR/scripts/cmake/wasabi.cmake \
		-DCMAKE_C_FLAGS=-fno-stack-protector \
		\
		-DCMAKE_INSTALL_PREFIX=$CJSON_DIR/install \
		\
		-DENABLE_CJSON_TEST=on \
		-DENABLE_CJSON_UTILS=off \
		-DBUILD_SHARED_LIBS=off \
		-DENABLE_VALGRIND=off \
		-DENABLE_SANITIZERS=off \
		-DENABLE_CUSTOM_COMPILER_FLAGS=off &> $outfile

	echo -------------- make ---------------
	$MAKE cjson  &> $outfile
	$MAKE cJSON_test &> $outfile
	touch $CJSON_DIR/cjson.flag
else
	echo "cjson already installed in '$CJSON_DIR'"
fi
echo -------------- test ---------------
$WASMTIME $CJSON_DIR/build/cJSON_test &> $outfile
if [ $? -ne 0 ]
then
	echo "Error: cJSON test didn't works" 
	exit 1
else
	echo "cJSON test passed"
fi



echo
echo -----------------------------------
echo ------------- sqlite --------------
echo " $(date +"%T")"
if [ ! -f "$SQLITE_DIR/sqlite.flag" ]
then
	cd $WASABI_EXTERNAL_DIR/
	echo --------------- git ---------------
	rm -rf sqlite
	git clone https://github.com/wapm-packages/sqlite.git  &> $outfile
	cd sqlite
	mkdir build
	cd build

	echo ------------- cmake ---------------
	$CMAKE .. \
		-DCMAKE_TOOLCHAIN_FILE=$WASABI_ROOT_DIR/scripts/cmake/wasabi.cmake \
		-DCMAKE_C_FLAGS=-fno-stack-protector \
		\
		-DCMAKE_INSTALL_PREFIX=$SQLITE_DIR/install &> $outfile

	echo -------------- make ---------------
	$MAKE &> $outfile
	touch $SQLITE_DIR/sqlite.flag
else
	echo "sqlite already installed in '$SQLITE_DIR'"
fi
echo -------------- test ---------------
\rm -rf $WASABI_ROOT_DIR/src/test/sqlite/build
mkdir $WASABI_ROOT_DIR/src/test/sqlite/build
cd $WASABI_ROOT_DIR/src/test/sqlite/build
$CMAKE .. &> $outfile
$MAKE &> $outfile
$MAKE test


echo "End $(date +"%T")"
