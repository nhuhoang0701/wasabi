echo
echo -----------------------------------
echo -------------- cJSON --------------
echo "start at $(date +"%T")"
if [ ! -f "$CJSON_DIR/cjson.flag" ]
then
	cd $WASABI_EXTERNAL_DIR/
	echo --------------- git ---------------
	rm -rf cJSON
	git clone https://github.com/DaveGamble/cJSON.git  >> $outfile
	cd cJSON
	mkdir build
	cd build

	echo ------------- cmake ---------------
	$CMAKE .. \
		-G Ninja -DCMAKE_MAKE_PROGRAM=$NINJA \
		-Wno-dev \
		-DCMAKE_TOOLCHAIN_FILE=$WASABI_ROOT_DIR/scripts/cmake/wasabi.cmake \
		-DCMAKE_C_FLAGS=-fno-stack-protector \
		\
		-DCMAKE_INSTALL_PREFIX=$CJSON_DIR/install \
		\
		-DENABLE_CJSON_TEST=on \
		-DENABLE_CJSON_UTILS=off \
		-DBUILD_SHARED_LIBS=off \
		-DENABLE_VALGRIND=off \
		-DENABLE_SANITIZERS=off \
		-DENABLE_CUSTOM_COMPILER_FLAGS=off >> $outfile

	echo ------------- build ---------------
	$CMAKE --build . --target cJSON_test >> $outfile
else
	echo "already installed in '$CJSON_DIR'"
fi
echo -------------- test ---------------

if [ "$WASABI_USE_WASM" = "no" ]
then
	$CJSON_DIR/install/bin/cJSON_test >> $outfile
else
	$WASMTIME $CJSON_DIR/install/bin/cJSON_test >> $outfile
fi

if [ $? -ne 0 ]
then
	echo "Error: cJSON test didn't works" 
	exit 1
else
	echo "cJSON test passed"
	touch $CJSON_DIR/cjson.flag
fi
echo "end at $(date +"%T")"