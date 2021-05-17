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
	mkdir -p build
	cd build

	echo ------------- cmake ---------------
	$CMAKE .. \
		-G Ninja -DCMAKE_MAKE_PROGRAM=${NINJA} \
		-DCMAKE_C_COMPILER=${C_COMPILER} \
		-DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
		-DCMAKE_BUILD_TYPE=${WASABI_BUILD_TYPE} \
		-DCMAKE_INSTALL_PREFIX=$WASABI_INSTAL_DIR/binaryen

	echo ------------- build ---------------
	$CMAKE --build . >> $outfile
	echo ------------ install --------------
	$CMAKE --build .  --target install >> $outfile
	touch $BINARYEN_DIR/binaryen.flag
else
	echo "already installed in '$BINARYEN_DIR'"
fi
echo "end at $(date +"%T")"
