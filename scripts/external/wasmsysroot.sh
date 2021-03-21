echo
echo -----------------------------------
echo ------ install wasm sysroot  ------
echo "start at $(date +"%T")"
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
	echo "already installed in '$LLVM_DIR/lib/clang/$LLVM_VERSION'"
fi
echo "end at $(date +"%T")"