aclocal
autoconf
automake --add-missing
./configure --with-gtk=3 --with-glfw --with-precision=3
if test "`command -v gmake`"; then
	gmake
else
	make
fi
