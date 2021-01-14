export cwd=$(pwd)

export WASABI_ROOT=$cwd

echo -----------------------------------
echo ---------- install clang ----------
rm -rf $WASABI_ROOT/llvm
rm -rf $WASABI_ROOT/clang+llvm-11.0.0-x86_64-linux-gnu-ubuntu-20.04
wget -qO- https://github.com/llvm/llvm-project/releases/download/llvmorg-11.0.0/clang+llvm-11.0.0-x86_64-linux-gnu-ubuntu-20.04.tar.xz  | tar xfJ - -C $WASABI_ROOT/
mv $WASABI_ROOT/clang+llvm-11.0.0-x86_64-linux-gnu-ubuntu-20.04 $WASABI_ROOT/llvm

echo -----------------------------------
echo --- install wasm librt in clang ---
wget -qO- https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-12/libclang_rt.builtins-wasm32-wasi-12.0.tar.gz | tar xfz - -C $WASABI_ROOT/llvm/lib/clang/11.0.0

echo -----------------------------------
echo ------ install wasm sysroot  ------
rm -rf $WASABI_ROOT/wasi-sdk-12
mkdir -p $WASABI_ROOT/wasi-sdk-12
wget -qO- https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-12/wasi-sysroot-12.0.tar.gz | tar xfz - -C $WASABI_ROOT/wasi-sdk-12

echo -----------------------------------
echo ------- install wasm server -------
cd $WASABI_ROOT
curl https://wasmtime.dev/install.sh -sSf | bash

echo restart your terminal
