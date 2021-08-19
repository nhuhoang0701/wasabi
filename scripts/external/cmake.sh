echo
echo -----------------------------------
echo ---------- install cmake ----------
echo "start at $(date +"%T")"

export CMAKE_VERSION=${CMAKE_VERSION:-3.19.5}

# Get Cmake compressed file corresponding to the platform
# https://stackoverflow.com/questions/3466166/how-to-check-if-running-in-cygwin-mac-or-linux
if [ "$(uname)" == "Darwin" ]
then
	export CMAKE_COMPRESSED_FILE=cmake-$CMAKE_VERSION-macos-universal
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]
then
	export CMAKE_COMPRESSED_FILE=cmake-$CMAKE_VERSION-Linux-x86_64
fi
echo "CMAKE version: $CMAKE_VERSION"

if [ "$CMAKE" != "$WASABI_CMAKE_DIR/bin/cmake" ]
then
	echo "    - use the local one '$CMAKE'"
elif [ ! -f "$WASABI_CMAKE_DIR/$CMAKEFile.flag" ]
then
	rm -rf $WASABI_EXTERNAL_DIR/$CMAKE_COMPRESSED_FILE
	wget -qO - https://github.com/Kitware/CMake/releases/download/v$CMAKE_VERSION/$CMAKE_COMPRESSED_FILE.tar.gz | tar xfz - -C $WASABI_EXTERNAL_DIR

	rm -rf $WASABI_CMAKE_DIR
	mkdir -p $WASABI_CMAKE_DIR
	cp -rf $WASABI_EXTERNAL_DIR/$CMAKE_COMPRESSED_FILE/* $WASABI_CMAKE_DIR

	rm -rf $WASABI_EXTERNAL_DIR/$CMAKE_COMPRESSED_FILE

	# Change structure of Cmake folder on Mac OS
	if [ "$(uname)" == "Darwin" ]
	then
		mv $WASABI_CMAKE_DIR/CMake.app/Contents/* $WASABI_CMAKE_DIR
		rm -rf $WASABI_CMAKE_DIR/CMake.app
	fi

	touch $WASABI_CMAKE_DIR/$CMAKE_COMPRESSED_FILE.flag
else
	echo "    - already installed in '$WASABI_CMAKE_DIR'"
fi
echo "end at $(date +"%T")"
