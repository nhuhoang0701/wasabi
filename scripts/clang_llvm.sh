

cd $WASABI_EXTERNAL_DIR
rm -rf llvm4build || true
mkdir -p llvm4build || true
cd llvm4build
git clone https://github.com/llvm/llvm-project.git
cd llvm-project
git checkout llvmorg-$LLVM_VERSION
llvmorg-12.0.0-rc3
rm -rf build || true
mkdir build || true
cd build
$CMAKE  -G "Ninja" \
        -DCMAKE_MAKE_PROGRAM=$NINJA \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_C_COMPILER=gcc \
        -DCMAKE_C_FLAGS="-static" \
        -DCMAKE_CXX_COMPILER=g++ \
        -DCMAKE_CXX_FLAGS="-static-libstdc++" \
        -DCMAKE_INSTALL_PREFIX=$LLVM_DIR \
        -DLLVM_ENABLE_PROJECTS="clang;lld" \
        -DLLVM_STATIC_LINK_CXX_STDLIB=ON \
        -DLLVM_TARGETS_TO_BUILD="X86;WebAssembly" \
        ../llvm
$CMAKE --build . --target install
touch $LLVM_DIR/$LLVMFile.flag
