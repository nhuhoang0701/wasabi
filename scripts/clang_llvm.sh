

echo
echo -----------------------------------
echo ---------- compile musl  ----------
cd $WASABI_EXTERNAL_DIR
mkdir -p musl4build
cd musl4build
git clone git://git.musl-libc.org/musl
cd musl
git checkout v1.2.2
./configure --prefix=$WASABI_EXTERNAL_DIR/musl --syslibdir=--prefix=$MUSL_DIR/syslib   CC=$C_COMPILER
make -j$((`nproc`-2))
make install


