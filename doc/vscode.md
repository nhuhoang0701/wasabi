https://code.visualstudio.com/docs/remote/wsl  

Extension :
 - https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd  
        "clangd.path": "/home/ghislain/B/wasabi/external/llvm/bin/clangd"
  
 - https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools  
"cmake.configureArgs": [
  "-DCMAKE_MAKE_PROGRAM=${workspaceFolder}/external/ninja/ninja",
  "-DWASABI_USE_WASM=yes",
  "-DCMAKE_INSTALL_PREFIX:PATH=${workspaceFolder}/install_wasm/",
  "-DLLVM_DIR=${workspaceFolder}/external/llvm",
  "-DSYSROOT_DIR=${workspaceFolder}/external/wasi-sdk-12/wasi-sysroot",
  "-DWASMTIME=${workspaceFolder}/external/wasmtime-v0.22.0/wasmtime",
  "-DCMAKE_TOOLCHAIN_FILE=${workspaceFolder}/scripts/cmake/wasabi.cmake"
]
"cmake.cmakePath": "${workspaceFolder}/external/cmake/bin/cmake"
