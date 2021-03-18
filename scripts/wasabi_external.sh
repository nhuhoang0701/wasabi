#!/usr/bin/env bash

# exit on error
set -e

warning() {
    printf '\n\033[1;33mWarning\033[0m: %s\n\n' "$1" 
}

source ./scripts/set_env.sh

mkdir -p $WASABI_EXTERNAL_DIR

export outfile=$WASABI_EXTERNAL_DIR/output_ext.log

echo
echo -----------------------------------
echo " Installing external tools into '$WASABI_EXTERNAL_DIR'"
echo "     log of this script will be in : '$outfile'"
echo " Installing external tools into $WASABI_EXTERNAL_DIR" > $outfile


echo
echo -----------------------------------
echo ---------- install cmake ----------
echo " $(date +"%T")"
export CMAKE_VERSION=${CMAKE_VERSION:-3.19.5}
export CMAKEFile=cmake-$CMAKE_VERSION-Linux-x86_64
echo "CMAKE version: $CMAKE_VERSION"
if [ ! -f "$WASABI_CMAKE_DIR/$CMAKEFile.flag" ]
then
	rm -rf $WASABI_EXTERNAL_DIR/$CMAKEFile
	wget -qO - https://github.com/Kitware/CMake/releases/download/v$CMAKE_VERSION/$CMAKEFile.tar.gz | tar xfz - -C $WASABI_EXTERNAL_DIR
	
	rm -rf $WASABI_CMAKE_DIR
	mkdir -p $WASABI_CMAKE_DIR
	cp -rf $WASABI_EXTERNAL_DIR/$CMAKEFile/* $WASABI_CMAKE_DIR
	
	rm -rf $WASABI_EXTERNAL_DIR/$CMAKEFile
	touch $WASABI_CMAKE_DIR/$CMAKEFile.flag
else
	echo "CMake already installed in '$WASABI_CMAKE_DIR'"
fi

echo
echo -----------------------------------
echo ---------- install ninja ----------
echo " $(date +"%T")"
export NINJA_VERSION=${NINJA_VERSION:-1.10.2}
export NINJAFile=ninja-$NINJA_VERSION-linux
echo "NINJA version: $NINJA_VERSION"
if [ ! -f "$WASABI_NINJA_DIR/$NINJAFile.flag" ]
then
	rm -rf $WASABI_NINJA_DIR
	mkdir -p $WASABI_NINJA_DIR
	rm -rf $WASABI_EXTERNAL_DIR/$NINJAFile

	wget -qO - wget -qO - https://github.com/ninja-build/ninja/releases/download/v$NINJA_VERSION/ninja-linux.zip | gunzip -c > $WASABI_NINJA_DIR/ninja
	chmod 777 $WASABI_NINJA_DIR/ninja
	
	touch $WASABI_NINJA_DIR/$NINJAFile.flag
else
	echo "ninja already installed in '$WASABI_NINJA_DIR'"
fi


echo
echo -----------------------------------
echo ---------- install clang ----------
echo " $(date +"%T")"
export LLVM_VERSION=${LLVM_VERSION:-11.0.0}
echo "LLVM version: $LLVM_VERSION"
export LLVM_ARCH=${LLVM_ARCH:-x86_64}
export LLVM_OS=${LLVM_OS:-linux-gnu-ubuntu-20.04}
export LLVMFile=clang+llvm-$LLVM_VERSION-$LLVM_ARCH-$LLVM_OS
echo "LLVM version: $LLVMFile"
echo "LLVM WASABI_LLVM: $WASABI_LLVM"
if [ ! -f "$LLVM_DIR/$LLVMFile.$WASABI_LLVM.flag" ]
then
	if [ "$WASABI_LLVM" = "external" ]
	then
		rm -rf $LLVM_DIR
		rm -rf $WASABI_EXTERNAL_DIR/$LLVMFile
		wget -qO - https://github.com/llvm/llvm-project/releases/download/llvmorg-$LLVM_VERSION/$LLVMFile.tar.xz | tar xfJ - -C $WASABI_EXTERNAL_DIR/ && \rm -rf $LLVM_DIR && cp -rf $WASABI_EXTERNAL_DIR/$LLVMFile $LLVM_DIR
		rm -rf $WASABI_EXTERNAL_DIR/$LLVMFile
		touch $LLVM_DIR/$LLVMFile.$WASABI_LLVM.flag
	elif [ "$WASABI_LLVM" = "compiled" ]
	then
		#source clang_llvm.sh
		cd $WASABI_EXTERNAL_DIR
		#rm -rf llvm4build || true
		#mkdir -p llvm4build
		cd llvm4build
		#git clone https://github.com/llvm/llvm-project.git
		cd llvm-project
		#git checkout llvmorg-$LLVM_VERSION
		#llvmorg-12.0.0-rc3
		#rm -rf build || true
		mkdir -p build
		cd build
		$CMAKE  -G "Ninja" \
				-DCMAKE_MAKE_PROGRAM=$NINJA \
				-DCMAKE_BUILD_PARALLEL_LEVEL=11 \
				-DCMAKE_BUILD_TYPE=Release \
				-DCMAKE_C_COMPILER=gcc \
				-DCMAKE_EXE_LINKER_FLAGS="-static" \
				-DCMAKE_C_FLAGS="-static" \
				-DCMAKE_CXX_COMPILER=g++ \
				-DCMAKE_CXX_FLAGS="-static-libstdc++" \
				-DCMAKE_INSTALL_PREFIX=$LLVM_DIR \
				-DLLVM_ENABLE_PROJECTS="lld" \
				-DBUILD_SHARED_LIBS=OFF \
				-DLLVM_STATIC_LINK_CXX_STDLIB=ON \
				-DLLVM_TARGETS_TO_BUILD="X86;WebAssembly" \
				-DLLVM_BUILD_TESTS=OFF \
				-DLLVM_INCLUDE_TESTS=OFF \
				-DCLANG_ENABLE_ARCMT=OFF \
				../llvm
		$CMAKE --build . --target install
		touch $LLVM_DIR/$LLVMFile.$WASABI_LLVM.flag
	elif [ "$WASABI_LLVM" = "git" ]
	then
		cd $WASABI_EXTERNAL_DIR
		rm -rf $LLVM_DIR
		tar --extract --file=llvm.tar.gz
		touch $LLVM_DIR/$LLVMFile.$WASABI_LLVM.flag
	elif [ "$WASABI_LLVM" = "local" ]
	then
		echo "LLVM will be used from: '$LLVM_DIR'" 
	else
		echo "WASABI_LLVM should be one of {compiled|external|git|local} not '$WASABI_LLVM'"
		return 1;
	fi
else
	echo "Clang already installed in '$LLVM_DIR'"
fi
echo "clang version:"
$LLVM_DIR/bin/clang --version || true
echo "lld version:"
$LLVM_DIR/bin/lld --version || true
ldd $LLVM_DIR/bin/clang || true
ldd $LLVM_DIR/bin/lld || true


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
	touch $WASI_SDK_DIR/wasisdk_$WASISDK_VERSION.flag
else
	echo "sysroot already installed in '$WASI_SDK_DIR'"
fi
if [ ! -f "$LLVM_DIR/lib/clang/$LLVM_VERSION/wasisdk_librt_$WASISDK_VERSION.flag" ]
then
	mkdir -p $WASABI_EXTERNAL_DIR/llvm/lib/clang/$LLVM_VERSION
	wget -qO - https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-$WASISDK_VERSION/libclang_rt.builtins-wasm32-wasi-$WASISDK_VERSION.0.tar.gz | tar xfz - -C $WASABI_EXTERNAL_DIR/llvm/lib/clang/$LLVM_VERSION
	touch $LLVM_DIR/lib/clang/$LLVM_VERSION/wasisdk_librt_$WASISDK_VERSION.flag
else
	echo "wasisdk_librt already installed in '$LLVM_DIR/lib/clang/$LLVM_VERSION'"
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
	wget -qO - https://github.com/bytecodealliance/wasmtime/releases/download/$WASMTIME_VERSION/wasmtime-$WASMTIME_VERSION-x86_64-linux.tar.xz | tar xfJ -  -C $WASABI_EXTERNAL_DIR/ && cp -rf $WASMTIME_LINUX_DIR $WASMTIME_DIR
	touch $WASMTIME_DIR/wasmtime_$WASMTIME_VERSION.flag
else
	echo "wasmtime already installed in '$WASMTIME_DIR'"
fi


echo
echo -----------------------------------
echo -------------- cJSON --------------
echo " $(date +"%T")"
if [ ! -f "$CJSON_DIR/cjson.flag" ]
then
	cd $WASABI_EXTERNAL_DIR/
	echo --------------- git ---------------
	rm -rf cJSON
	git clone https://github.com/DaveGamble/cJSON.git  >> $outfile
	cd cJSON
	mkdir build
	cd build

	echo ------------- cmake ---------------
	$CMAKE .. \
		-G Ninja -DCMAKE_MAKE_PROGRAM=$NINJA \
		-Wno-dev \
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
		-DENABLE_CUSTOM_COMPILER_FLAGS=off >> $outfile

	echo ------------- build ---------------
	$CMAKE --build . --target cJSON_test >> $outfile
else
	echo "cjson already installed in '$CJSON_DIR'"
fi
echo -------------- test ---------------

if [ "$WASABI_USE_WASM" = "no" ]
then
	$CJSON_DIR/install/bin/cJSON_test >> $outfile
else
	$WASMTIME $CJSON_DIR/install/bin/cJSON_test >> $outfile
fi

if [ $? -ne 0 ]
then
	echo "Error: cJSON test didn't works" 
	exit 1
else
	echo "cJSON test passed"
	touch $CJSON_DIR/cjson.flag
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
	git clone https://github.com/wapm-packages/sqlite.git  >> $outfile
	cd sqlite
	mkdir build
	cd build

	echo ------------- cmake ---------------
	$CMAKE .. \
		-G Ninja -DCMAKE_MAKE_PROGRAM=$NINJA \
		-DCMAKE_TOOLCHAIN_FILE=$WASABI_ROOT_DIR/scripts/cmake/wasabi.cmake \
		-DCMAKE_C_FLAGS=-fno-stack-protector \
		\
		-DCMAKE_INSTALL_PREFIX=$SQLITE_DIR/install >> $outfile

	echo ------------- build ---------------
	$CMAKE --build . >> $outfile
	touch $SQLITE_DIR/sqlite.flag
else
	echo "sqlite already installed in '$SQLITE_DIR'"
fi


echo "End $(date +"%T")"
