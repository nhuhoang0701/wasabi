

echo
echo -----------------------------------
echo ---------- compile musl  ----------
echo "start at $(date +"%T")"
if [ ! -f "$SYSROOT_LINUX_DIR/musl.flag" ]
then
    cd $WASABI_EXTERNAL_DIR
    mkdir -p musl4build
    cd musl4build
    git clone git://git.musl-libc.org/musl
    cd musl
    git checkout v1.2.2
    ./configure --prefix=$SYSROOT_LINUX_DIR --syslibdir=--prefix=$SYSROOT_LINUX_DIR   CC=$C_COMPILER
    make -j$((`nproc`-2))
    make install

	touch $SYSROOT_LINUX_DIR/musl.flag
else
	echo "already installed in '$MUSL_DIR'"
fi
echo "end at $(date +"%T")"
