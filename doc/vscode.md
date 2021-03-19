https://code.visualstudio.com/docs/remote/wsl  

Extension :
 - https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd  
        "clangd.path": "/home/ghislain/B/wasabi/external/llvm/bin/clangd"
  
 - https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools  
"cmake.configureArgs": [  
"-DCMAKE_MAKE_PROGRAM=/home/ghislain/B/wasabi/external/ninja/ninja",  
"-DWASABI_USE_WASM=yes",  
"-DCMAKE_INSTALL_PREFIX:PATH=/home/ghislain/B/wasabi/install/",  
"-DLLVM_DIR=/home/ghislain/B/wasabi/external/llvm",  
"-DSYSROOT_DIR=/home/ghislain/B/wasabi/external/wasi-sdk-12/wasi-sysroot",  
"-DWASMTIME=/home/ghislain/B/wasabi/external/wasmtime-v0.22.0/wasmtime"  
]  
