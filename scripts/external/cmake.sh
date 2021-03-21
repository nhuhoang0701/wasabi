echo
echo -----------------------------------
echo ---------- install cmake ----------
echo "start at $(date +"%T")"
export CMAKE_VERSION=${CMAKE_VERSION:-3.19.5}
export CMAKEFile=cmake-$CMAKE_VERSION-Linux-x86_64
echo "CMAKE version: $CMAKE_VERSION"
if [ ! -f "$WASABI_CMAKE_DIR/$CMAKEFile.flag" ]
then
	rm -rf $WASABI_EXTERNAL_DIR/$CMAKEFile
	wget -qO - https://github.com/Kitware/CMake/releases/download/v$CMAKE_VERSION/$CMAKEFile.tar.gz | tar xfz - -C $WASABI_EXTERNAL_DIR
	
	rm -rf $WASABI_CMAKE_DIR
	mkdir -p $WASABI_CMAKE_DIR
	cp -rf $WASABI_EXTERNAL_DIR/$CMAKEFile/* $WASABI_CMAKE_DIR
	
	rm -rf $WASABI_EXTERNAL_DIR/$CMAKEFile
	touch $WASABI_CMAKE_DIR/$CMAKEFile.flag
else
	echo "already installed in '$WASABI_CMAKE_DIR'"
fi
echo "end at $(date +"%T")"