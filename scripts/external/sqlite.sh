echo
echo -----------------------------------
echo ------------- sqlite --------------
echo "start at $(date +"%T")"
#if [ ! -f "$WASABI_INSTAL_DIR/sqlite/sqlite.flag" ]
#then
	cd $WASABI_EXTERNAL_DIR/
	echo --------------- git ---------------
	#git clone https://github.com/wapm-packages/sqlite.git  >> $outfile
	cd sqlite
	mkdir -p $WASABI_BUILD_DIR_NAME/sqlite >> $outfile

	echo ------------- cmake ---------------
	$CMAKE -B $WASABI_BUILD_DIR_NAME/sqlite \
		-G Ninja -DCMAKE_MAKE_PROGRAM=$NINJA \
		-Wno-dev \
		-DCMAKE_INSTALL_PREFIX=$WASABI_INSTAL_DIR/sqlite \
		\
		-DCMAKE_TOOLCHAIN_FILE=$WASABI_ROOT_DIR/scripts/cmake/wasabi.cmake \
		-DCMAKE_C_FLAGS=-fno-stack-protector

	echo ------------- build ---------------
	$CMAKE --build $WASABI_BUILD_DIR_NAME/sqlite --target install
	touch $WASABI_INSTAL_DIR/sqlite/sqlite.flag
#else
#	echo "already installed in '$WASABI_INSTAL_DIR/sqlite'"
#fi
echo "end at $(date +"%T")"
