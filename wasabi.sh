export cwd=$(pwd)

mkdir wasabi
export WASABI_ROOT=$cwd/wasabi

echo ---------------------
echo --- install clang ---
cd $WASABI_ROOT
wget -qO- https://github.com/llvm/llvm-project/releases/download/llvmorg-11.0.0/clang+llvm-11.0.0-x86_64-linux-gnu-ubuntu-20.04.tar.xz  | tar xfJ -
mv clang+llvm-11.0.0-x86_64-linux-gnu-ubuntu-20.04/ llvm-11.0.0

echo --- install wasm librt in clang ---
cd $WASABI_ROOT/llvm-11.0.0/lib/clang/11.0.0
wget -qO- https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-12/libclang_rt.builtins-wasm32-wasi-12.0.tar.gz | tar xfz -

echo ----------------------------
echo --- install wasm sysroot ---
cd $WASABI_ROOT
mkdir wasi-sdk-12
cd wasi-sdk-12
wget -qO- https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-12/wasi-sysroot-12.0.tar.gz | tar xfz -

echo ---------------------------
echo --- install wasm server ---
cd $WASABI_ROOT
curl https://wasmtime.dev/install.sh -sSf | bash

echo restart your terminal
