echo
echo -----------------------------------
echo ---------- install ninja ----------
echo " $(date +"%T")"
export NINJA_VERSION=${NINJA_VERSION:-1.10.2}
export NINJAFile=ninja-$NINJA_VERSION-linux
echo "NINJA version: $NINJA_VERSION"
if [ ! -f "$WASABI_NINJA_DIR/$NINJAFile.flag" ]
then
	rm -rf $WASABI_NINJA_DIR
	mkdir -p $WASABI_NINJA_DIR
	rm -rf $WASABI_EXTERNAL_DIR/$NINJAFile

	wget -qO - wget -qO - https://github.com/ninja-build/ninja/releases/download/v$NINJA_VERSION/ninja-linux.zip | gunzip -c > $WASABI_NINJA_DIR/ninja
	chmod 777 $WASABI_NINJA_DIR/ninja
	
	touch $WASABI_NINJA_DIR/$NINJAFile.flag
else
	echo "ninja already installed in '$WASABI_NINJA_DIR'"
fi