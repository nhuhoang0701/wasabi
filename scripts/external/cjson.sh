echo
echo -----------------------------------
echo -------------- cJSON --------------
echo "start at $(date +"%T")"
if [ ! -f "$WASABI_INSTAL_DIR/cJSON/cjson.flag" ]
then
	cd $WASABI_EXTERNAL_DIR/
	echo --------------- git ---------------
	git clone https://github.com/DaveGamble/cJSON.git  >> $outfile
	cd cJSON
	mkdir -p build_$WASABI_PLATFORM_TARGET
	cd build_$WASABI_PLATFORM_TARGET

	echo ------------- cmake ---------------
	$CMAKE .. \
		-G Ninja -DCMAKE_MAKE_PROGRAM=$NINJA \
		-Wno-dev \
		-DCMAKE_TOOLCHAIN_FILE=$WASABI_ROOT_DIR/scripts/cmake/wasabi.cmake \
		-DCMAKE_C_FLAGS=-fno-stack-protector \
		\
		-DCMAKE_INSTALL_PREFIX=$WASABI_INSTAL_DIR/cJSON \
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
	echo "already installed in '$WASABI_INSTAL_DIR/cJSON'"
fi
echo -------------- test ---------------

if [ "$WASABI_PLATFORM_TARGET" = "linux" ]
then
	$WASABI_INSTAL_DIR/cJSON/bin/cJSON_test >> $outfile
else
	$WASMTIME $WASABI_INSTAL_DIR/cJSON/bin/cJSON_test >> $outfile
fi

if [ $? -ne 0 ]
then
	echo "Error: cJSON test didn't works" 
	exit 1
else
	echo "cJSON test passed"
	touch $WASABI_INSTAL_DIR/cJSON/cjson.flag
fi
echo "end at $(date +"%T")"