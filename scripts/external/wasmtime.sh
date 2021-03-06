echo
echo -----------------------------------
echo ------- install wasmtime runtime --
echo "start at $(date +"%T")"
export WASMTIME_VERSION=${WASMTIME_VERSION:-v0.22.0}
echo "WASMTIME version: $WASMTIME_VERSION"
# see packages on https://github.com/bytecodealliance/wasmtime/releases
if [ "$WASMTIME" != "$WASMTIME_DIR/wasmtime" ]
then
	echo "    - use the local one '$WASMTIME'"
elif [ ! -f "$WASMTIME_DIR/wasmtime_$WASMTIME_VERSION.flag" ]
then
	wget -qO - https://github.com/bytecodealliance/wasmtime/releases/download/$WASMTIME_VERSION/wasmtime-$WASMTIME_VERSION-x86_64-linux.tar.xz | tar xfJ -  -C $WASABI_EXTERNAL_DIR/ && cp -rf $WASMTIME_LINUX_DIR $WASMTIME_DIR
	touch $WASMTIME_DIR/wasmtime_$WASMTIME_VERSION.flag
else
	echo "already installed in '$WASMTIME_DIR'"
fi
echo "end at $(date +"%T")"