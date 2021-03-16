Extension :
  https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd
  
  extension cmake
        -DWASMTIME=/home/ghislain/B/wasabi/external/wasmtime-v0.22.0/wasmtime
        -DSYSROOT_DIR=/home/ghislain/B/wasabi/external/wasi-sdk-12/wasi-sysroot
        -DLLVM_DIR=/home/ghislain/B/wasabi/external/llvm
        -DCMAKE_INSTALL_PREFIX:PATH=/home/ghislain/B/wasabi/install/
        -DWASABI_USE_WASM=yes
        -DCMAKE_MAKE_PROGRAM=/home/ghislain/B/wasabi/external/ninja/ninja
