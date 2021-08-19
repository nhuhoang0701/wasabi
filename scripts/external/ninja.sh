echo
echo -----------------------------------
echo ---------- install ninja ----------
echo "start at $(date +"%T")"
export NINJA_VERSION=${NINJA_VERSION:-1.10.2}
echo "NINJA version: $NINJA_VERSION"

# Get Ninja compressed file corresponding to the platform
if [ "$(uname)" == "Darwin" ]
then
    export NINJA_COMPRESSED_FILE=ninja-mac
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]
then
    export NINJA_COMPRESSED_FILE=ninja-linux
fi

if [ "$NINJA" != "$WASABI_NINJA_DIR/ninja" ]
then
	echo "    - use the local one '$NINJA'"
elif [ ! -f "$WASABI_NINJA_DIR/$NINJAFile.flag" ]
then
	rm -rf $WASABI_NINJA_DIR
	mkdir -p $WASABI_NINJA_DIR
	rm -rf $WASABI_EXTERNAL_DIR/$NINJA_COMPRESSED_FILE

	if [ "$(uname)" == "Darwin" ]
	then
		wget -qO - wget -qO - https://github.com/ninja-build/ninja/releases/download/v$NINJA_VERSION/$NINJA_COMPRESSED_FILE.zip | tar xfz - -C $WASABI_NINJA_DIR
	elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]
	then
		wget -qO - wget -qO - https://github.com/ninja-build/ninja/releases/download/v$NINJA_VERSION/$NINJA_COMPRESSED_FILE.zip | gunzip -c > $WASABI_NINJA_DIR/ninja
	fi

	rm -f $WASABI_EXTERNAL_DIR/$NINJA_COMPRESSED_FILE
	chmod 755 $WASABI_NINJA_DIR/ninja

	touch $WASABI_NINJA_DIR/$NINJA_COMPRESSED_FILE.flag
else
	echo "already installed in '$WASABI_NINJA_DIR'"
fi
echo "end at $(date +"%T")"
