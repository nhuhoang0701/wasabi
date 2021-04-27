echo
echo -----------------------------------
echo ------ install wasi sysroot  ------
echo "start at $(date +"%T")"
export WASISDK_VERSION=${WASISDK_VERSION:-12}
echo "WASISDK version: $WASISDK_VERSION"
if [ ! -f "$SYSROOT_WASI_DIR/$WASISDK_VERSION.flag" ]
then
	mkdir -p $SYSROOT_DIR
	wget -qO - https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-$WASISDK_VERSION/wasi-sysroot-$WASISDK_VERSION.0.tar.gz | tar xfz - -C $SYSROOT_DIR
	touch $SYSROOT_WASI_DIR/$WASISDK_VERSION.flag
else
	echo "already installed in '$SYSROOT_WASI_DIR'"
fi
if [ ! -f "$LLVM_DIR/lib/clang/$LLVM_VERSION/lib/wasi/$WASISDK_VERSION.flag" ]
then
	mkdir -p $WASABI_EXTERNAL_DIR/llvm/lib/clang/$LLVM_VERSION
	wget -qO - https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-$WASISDK_VERSION/libclang_rt.builtins-wasm32-wasi-$WASISDK_VERSION.0.tar.gz | tar xfz - -C $LLVM_DIR/lib/clang/$LLVM_VERSION
	touch $LLVM_DIR/lib/clang/$LLVM_VERSION/lib/wasi/$WASISDK_VERSION.flag
else
	echo "already installed in '$LLVM_DIR/lib/clang/$LLVM_VERSION/lib/wasi'"
fi
echo "end at $(date +"%T")"
