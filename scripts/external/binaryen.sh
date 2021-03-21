echo
echo -----------------------------------
echo ------------ binaryen -------------
echo "start at $(date +"%T")"
if [ ! -f "$BINARYEN_DIR/binaryen.flag" ]
then
	cd $WASABI_EXTERNAL_DIR/
	echo --------------- git ---------------
	mkdir -p binaryen4compile
	cd binaryen4compile
	git clone https://github.com/WebAssembly/binaryen.git  >> $outfile
	cd binaryen
	git checkout version_100
	rm -rf build
	mkdir build
	cd build

	echo ------------- cmake ---------------
	$CMAKE .. \
		-G Ninja -DCMAKE_MAKE_PROGRAM=$NINJA \
		-DCMAKE_C_COMPILER=${LLVM_DIR}/bin/clang \
		-DCMAKE_CXX_COMPILER=${LLVM_DIR}/bin/clang++ \
		-DCMAKE_INSTALL_PREFIX=$BINARYEN_DIR/install >> $outfile

	echo ------------- build ---------------
	$CMAKE --build . >> $outfile
	echo ------------ install --------------
	$CMAKE --build .  --target install >> $outfile
	touch $BINARYEN_DIR/binaryen.flag
else
	echo "already installed in '$BINARYEN_DIR'"
fi
echo "end at $(date +"%T")"
