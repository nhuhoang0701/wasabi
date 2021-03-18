

echo
echo -----------------------------------
echo ---------- compile musl  ----------
cd $WASABI_EXTERNAL_DIR
mkdir musl4build
cd musl4build
git clone git://git.musl-libc.org/musl
cd musl
git checkout v1.2.2
./configure --prefix=$WASABI_EXTERNAL_DIR/musl --syslibdir=--prefix=$WASABI_EXTERNAL_DIR/musl/syslib   CC=$C_COMPILER --disable-shared
make -j4
make install


