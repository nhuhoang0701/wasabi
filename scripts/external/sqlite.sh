echo
echo -----------------------------------
echo ------------- sqlite --------------
echo "start at $(date +"%T")"
if [ ! -f "$SQLITE_DIR/sqlite.flag" ]
then
	cd $WASABI_EXTERNAL_DIR/
	echo --------------- git ---------------
	rm -rf sqlite
	git clone https://github.com/wapm-packages/sqlite.git  >> $outfile
	cd sqlite
	mkdir build
	cd build

	echo ------------- cmake ---------------
	$CMAKE .. \
		-G Ninja -DCMAKE_MAKE_PROGRAM=$NINJA \
		-DCMAKE_TOOLCHAIN_FILE=$WASABI_ROOT_DIR/scripts/cmake/wasabi.cmake \
		-DCMAKE_C_FLAGS=-fno-stack-protector \
		\
		-DCMAKE_INSTALL_PREFIX=$SQLITE_DIR/install >> $outfile

	echo ------------- build ---------------
	$CMAKE --build . >> $outfile
	touch $SQLITE_DIR/sqlite.flag
else
	echo "already installed in '$SQLITE_DIR'"
fi
echo "end at $(date +"%T")"
