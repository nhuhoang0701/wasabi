echo
echo -----------------------------------
echo ------------ wasm sdk -------------
echo "start at $(date +"%T")"
if [ ! -f "$WASMSDK_DIR/wasmsdk.flag" ]
then
	cd $WASABI_EXTERNAL_DIR/
	echo --------------- git ---------------
	mkdir -p wasmsdk4compile
	cd wasmsdk4compile
	git clone https://github.com/WebAssembly/wasi-libc.git  >> $outfile
	cd wasi-libc
	make WASM_CC=$LLVM_DIR/bin/clang WASM_AR=$LLVM_DIR/bin/llvm-ar WASM_NM=$LLVM_DIR/bin/llvm-nm INSTALL_DIR=$WASABI_INSTALL_DIR/wasi_libc WASM_CFLAGS=-g -j$((`nproc` -2))
	echo ------------ install --------------
	make WASM_CC=$LLVM_DIR/bin/clang WASM_AR=$LLVM_DIR/bin/llvm-ar WASM_NM=$LLVM_DIR/bin/llvm-nm INSTALL_DIR=$WASABI_INSTALL_DIR/wasi_libc WASM_CFLAGS=-g -j$((`nproc` -2)) install
	touch $WASMSDK_DIR/wasmsdk.flag
else
	echo "already installed in '$WASMSDK_DIR'"
fi
echo "end at $(date +"%T")"
