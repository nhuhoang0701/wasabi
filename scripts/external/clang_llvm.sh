echo 
echo
echo -----------------------------------
echo ---------- install clang ----------
echo "start at $(date +"%T")"
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
		cd $WASABI_EXTERNAL_DIR
		mkdir -p llvm4build
		cd llvm4build
		git clone https://github.com/llvm/llvm-project.git
		cd llvm-project
		git checkout llvmorg-$LLVM_VERSION
		mkdir -p build
		cd build
		$CMAKE  -G "Ninja" \
				-DCMAKE_MAKE_PROGRAM=$NINJA \
				-DCMAKE_BUILD_PARALLEL_LEVEL=$((`nproc`-2)) \
				-DCMAKE_BUILD_TYPE=Release \
				-DCMAKE_C_COMPILER=gcc \
				-DCMAKE_EXE_LINKER_FLAGS="-static" \
				-DCMAKE_C_FLAGS="-static" \
				-DCMAKE_CXX_COMPILER=g++ \
				-DCMAKE_CXX_FLAGS="-static-libstdc++" \
				-DCMAKE_INSTALL_PREFIX=$LLVM_DIR \
				-DBUILD_SHARED_LIBS=OFF \
				-DLLVM_STATIC_LINK_CXX_STDLIB=ON \
				-DLLVM_TARGETS_TO_BUILD="X86;WebAssembly" \
				-DLLVM_BUILD_TESTS=OFF \
				-DLLVM_INCLUDE_TESTS=OFF \
				-DCLANG_ENABLE_ARCMT=OFF \
				../llvm >> $outfile
		$CMAKE --build . --target install >> $outfile
		touch $LLVM_DIR/$LLVMFile.$WASABI_LLVM.flag
	elif [ "$WASABI_LLVM" = "git" ]
	then
		cd $WASABI_EXTERNAL_DIR
		rm -rf $LLVM_DIR
		tar --extract --file=llvm-$LLVM_VERSION.tar.gz
		touch $LLVM_DIR/$LLVMFile.$WASABI_LLVM.flag
	elif [ "$WASABI_LLVM" = "local" ]
	then
		echo "LLVM will be used from: '$LLVM_DIR'" 
	else
		echo "WASABI_LLVM should be one of {compiled|external|git|local} not '$WASABI_LLVM'"
		return 1;
	fi
else
	echo "already installed in '$LLVM_DIR'"
fi
echo -----------------------------------
echo "clang version:"
$LLVM_DIR/bin/clang --version || true
echo --------------
ldd $LLVM_DIR/bin/clang || true
echo -----------------------------------
echo "lld version:"
$LLVM_DIR/bin/lld --version || true
echo --------------
ldd $LLVM_DIR/bin/lld || true
echo -----------------------------------
echo "end at $(date +"%T")"
