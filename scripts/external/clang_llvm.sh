echo
echo
echo -----------------------------------
echo ---------- install clang ----------
echo "start at $(date +"%T")"
echo "LLVM version: $LLVM_VERSION"
echo "LLVM WASABI_LLVM: $WASABI_LLVM"
if [ "$WASABI_LLVM" = "local" ]
then
	echo "    - use the local one '$LLVM_DIR'"
elif [ "$WASABI_LLVM" = "compiled" ]
then
	echo "    - clone/compile clang '$LLVM_VERSION'"
	cd $WASABI_EXTERNAL_DIR
	mkdir -p llvm4build
	cd llvm4build
	git clone https://github.com/llvm/llvm-project.git
	cd llvm-project
	git checkout llvmorg-$LLVM_VERSION
	rm -rf build
	mkdir -p build
	cd build
	$CMAKE  -G Ninja \
		-DCMAKE_MAKE_PROGRAM=$NINJA \
		\
		-DCMAKE_C_COMPILER=${C_COMPILER} \
		-DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
		-DCMAKE_LINKER=${C_COMPILER} \
		\
		-DCMAKE_BUILD_PARALLEL_LEVEL=$((`nproc`-2)) \
		-DCMAKE_BUILD_TYPE=Release \
		\
		-DCMAKE_CXX_FLAGS="-stdlib=libc++  -I${LLVM_DIR}/include/c++/v1" \
		-DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=lld" \
		-DPOSITION_INDEPENDENT_CODE=ON \
		\
		-DCMAKE_INSTALL_PREFIX=$LLVM_DIR \
		\
		-DLLVM_ENABLE_PROJECTS="clang;libcxx;libcxxabi;libunwind;lld" \
		-DLLVM_TARGETS_TO_BUILD="X86;WebAssembly" \
		-DLLVM_BUILD_TESTS=OFF \
		-DLLVM_INCLUDE_TESTS=OFF \
		\
		-DCLANG_ENABLE_ARCMT=OFF \
		../llvm >> $outfile
	$CMAKE --build . --target install >> $outfile
elif [ ! -f "$LLVM_DIR/$LLVM_VERSION.$WASABI_LLVM.flag" ]
then
	if [ "$WASABI_LLVM" = "external" ]
	then
		echo "    - download clang '$LLVM_VERSION'"
		rm -rf $LLVM_DIR
		rm -rf $WASABI_EXTERNAL_DIR/$LLVM_COMPRESSED_FILE
		wget -qO - https://github.com/llvm/llvm-project/releases/download/llvmorg-$LLVM_VERSION/$LLVM_COMPRESSED_FILE.tar.xz | tar xfJ - -C $WASABI_EXTERNAL_DIR/
	elif [ "$WASABI_LLVM" = "git" ]
	then
		echo "    - extract clang '$LLVM_VERSION' from git"
		cd $WASABI_EXTERNAL_DIR
		rm -rf $LLVM_DIR
		tar --extract --file=llvm-$LLVM_VERSION.tar.gz
	else
		echo "ERROR: WASABI_LLVM should be one of {compiled|external|git|local} not '$WASABI_LLVM'"
		return 1;
	fi
	touch $LLVM_DIR/$LLVM_VERSION.$WASABI_LLVM.flag
else
	echo "    - already installed in '$LLVM_DIR'"
fi

echo -----------------------
echo "clang version:"
$LLVM_DIR/bin/clang --version
echo --------------

echo "lld version:"
if [ "$(uname)" == "Darwin" ]
then
	otool -L $LLVM_DIR/bin/clang || true
	echo --------------
	$LLVM_DIR/bin/lld --version
	echo --------------
	otool -L $LLVM_DIR/bin/lld || true
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]
then
	lld $LLVM_DIR/bin/clang || true
	echo --------------
	$LLVM_DIR/bin/lld --version
	echo --------------
	ldd $LLVM_DIR/bin/lld || true
fi

echo -----------------------------------
echo "end at $(date +"%T")"
