AC_PREREQ([2.69])
AC_INIT(surcos, [5.7])

# Checks for operative systems
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE
AC_MSG_NOTICE([Build: $build])
AC_MSG_NOTICE([Host: $host])
win=0
openbsd=0
rpath="-rpath="
if test $host = "x86_64-w64-mingw32"; then
	AC_MSG_NOTICE([Microsoft Windows 64 bits])
	AC_SUBST(ccflags, "-march=corei7 -mfpmath=sse -msse3 -mwindows")
	AC_SUBST(libgcc, "libgcc_s_seh-1")
	win=1
	windir=win64
	mingw=mingw64
elif test $host = "i686-w64-mingw32"; then
	AC_MSG_NOTICE([Microsoft Windows 32 bits])
	LDFLAGS="$LDFLAGS -lfreeglut -lopengl32"
	AC_SUBST(ccflags, "-march=pentium4 -mfpmath=sse -msse2 -mwindows")
	AC_SUBST(libgcc, "libgcc_s_dw2-1")
	win=1
	windir=win32
	mingw=mingw32
elif test `uname -s` = "Darwin"; then
	AC_MSG_NOTICE([MacOS])
	LDFLAGS="$LDFLAGS -L/System/Library/Frameworks -framework GLUT -framework OpenGL"
	rpath="-rpath,"
elif test `uname -s` = "NetBSD"; then
	AC_MSG_NOTICE([NetBSD])
	LDFLAGS="$LDFLAGS -L/usr/pkg/lib"
	CPPFLAGS="$CPPFLAGS -I/usr/pkg/include"
	PATH="/usr/pkg/gcc48/bin:$PATH"
elif test `uname -s` = "OpenBSD"; then
	AC_MSG_NOTICE([OpenBSD])
	LDFLAGS="$LDFLAGS -L/usr/X11R6/lib"
	CPPFLAGS="$CPPFLAGS -I/usr/X11R6/include"
	openbsd=1
elif test `uname -s` = "DragonFly"; then
	AC_MSG_NOTICE([DragonFly BSD])
	LDFLAGS="$LDFLAGS -L/usr/local/lib"
	CPPFLAGS="$CPPFLAGS -I/usr/local/include"
fi
if test $win = 0; then
	AC_MSG_NOTICE([Unix type system])
	AC_SUBST(latex, "pdflatex -shell-escape")
	AC_SUBST(objdir, [src/])
	AC_SUBST(bindir, [./])
	AC_SUBST(localdir, [locale/])
	AC_SUBST(exegui, surcos_gui)
	if test $openbsd = 0; then
		AC_SUBST(ccflags, [-march=native])
	fi
	AC_SUBST(SO, [.so])
else
	AC_CHECK_TOOL(WINDRES, windres)
	AC_SUBST(latex, "pdflatex -enable-write18")
	AC_SUBST(exegui, winsurcos)
	AC_SUBST(sysdir, [/$mingw/])
	AC_SUBST(objdir, [$windir/obj/])
	AC_SUBST(bindir, [$windir/bin/])
	AC_SUBST(etcdir, [$windir/etc/])
	AC_SUBST(libdir, [$windir/lib/])
	AC_SUBST(sharedir, [$windir/share/])
	AC_SUBST(basedir, [$windir/])
	AC_SUBST(icon, [$windir/obj/logo.o])
	AC_SUBST(WIN, [$windir/])
	AC_SUBST(SO, [.dll])
	test -d $windir/obj || mkdir -p $windir/obj
	test -d $windir/bin || mkdir -p $windir/bin
fi
AC_SUBST(srcdir, [src/])

# Checks for programs
AC_PROG_CC([gcc10 gcc9 gcc8 gcc7 egcc gcc clang])
AC_PROG_MAKE_SET
AC_LANG([C])
PKG_PROG_PKG_CONFIG
if test $CC = "clang"; then
	rpath="-rpath,"
fi
AC_SUBST(RPATH,[$rpath])
AC_MSG_NOTICE([Compiler: $CC])
$CC -v

# Checks for compiler flags
compiler=$CC

CC="$CC -flto"
AC_TRY_LINK([], [int main(int argc, char **argv){return 0;}], [lto=1], [lto=0])
CC=$compiler
if test $lto = 0; then
	AC_MSG_NOTICE("checking for -flto... no")
	AC_SUBST(LTO, "")
else
	AC_MSG_NOTICE("checking for -flto... yes")
	AC_SUBST(LTO, "-flto")
fi

CC="$CC -std=gnu11"
AC_TRY_LINK([], [int main(int argc, char **argv){return 0;}], [std=1], [std=0])
CC=$compiler
if test $std = 0; then
	AC_MSG_NOTICE("checking for -std=gnu11... no")
	CC="$CC -std=gnu99"
	AC_TRY_LINK([], [int main(int argc, char **argv){return 0;}], [std=1],
		[std=0])
	CC=$compiler
	if test $std = 0; then
		AC_MSG_NOTICE("checking for -std=gnu99... no")
		AC_SUBST(STD, "")
	else
		AC_MSG_NOTICE("checking for -std=gnu99... yes")
		AC_SUBST(STD, "-std=gnu99")
	fi
else
	AC_MSG_NOTICE("checking for -std=gnu11... yes")
	AC_SUBST(STD, "-std=gnu11")
fi

if test $win = 0 && test $openbsd = 0; then
	CC="$CC -march=native"
	AC_TRY_LINK([], [int main(int argc, char **argv){return 0;}], [native=1],
		[native=0])
	CC=$compiler
	if test $native = 0; then
		AC_MSG_NOTICE("checking for -march=native... no")
		AC_SUBST(NATIVE, "")
	else
		AC_MSG_NOTICE("checking for -march=native... yes")
		AC_SUBST(NATIVE, "-march=native")
	fi
fi

# Checks for header files.
AC_CHECK_HEADERS_ONCE([libintl.h locale.h stdlib.h string.h unistd.h])

# Checks for libraries
if test -f "src/jb/jb_math.h"; then
	AC_MSG_NOTICE("JB headers: OK")
else
	AC_ERROR("No JB headers")
fi
AC_SEARCH_LIBS([pow], [m])
PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([GTHREAD], [gthread-2.0])
AC_SEARCH_LIBS([textdomain], [intl])
PKG_CHECK_MODULES([GTK], [gtk+-3.0])
PKG_CHECK_MODULES([PNG], [libpng])
PKG_CHECK_MODULES([FREETYPE], [freetype2])
PKG_CHECK_MODULES([GLEW], [glew])
PKG_CHECK_MODULES([GTOP], [libgtop-2.0], , AC_MSG_NOTICE([No libgtop]))
AC_SEARCH_LIBS([glFlush], [GL opengl32])

# Checks for GtkGLArea, FreeGLUT, GLFW or SDL to draw the graphic window
AC_ARG_WITH([gtkglarea],
	AS_HELP_STRING([--with-gtkglarea], [Build graphic window with GtkGLArea]))
AC_ARG_WITH([freeglut],
	AS_HELP_STRING([--with-freeglut], [Build graphic window with FreeGLUT]))
AC_ARG_WITH([glfw],
	AS_HELP_STRING([--with-glfw], [Build graphic window with GLFW]))
AC_ARG_WITH([sdl],
	AS_HELP_STRING([--with-sdl], [Build graphic window with SDL]))
if test "x$with_freeglut" = "xyes"; then
	AC_SUBST(GRAPHIC,"-DHAVE_FREEGLUT=1")
	AC_SEARCH_LIBS([glutInit], [freeglut glut])
else
	if test "x$with_glfw" = "xyes"; then
		AC_SUBST(GRAPHIC,"-DHAVE_GLFW=1")
		PKG_CHECK_MODULES([GLFW], [glfw3])
	else
		if test "x$with_sdl" = "xyes"; then
			AC_SUBST(GRAPHIC,"-DHAVE_SDL=1")
			PKG_CHECK_MODULES([SDL], [sdl2])
		else
			AC_SUBST(GRAPHIC,"-DHAVE_GTKGLAREA=1")
		fi
	fi
fi

# Check for FreeMono font
AC_MSG_NOTICE([searching FreeMono font])
font=`find /usr -type f -iname freemono.otf`
if test "x$font" = "x"; then
	font=`find /usr -type f -iname freemono.ttf`
	if test "x$font" = "x"; then
		AC_MSG_ERROR([font not found])
	fi
fi
AC_MSG_NOTICE([Font: $font])
AC_SUBST(FONT, "-DFONT=\\\"$font\\\"")

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_MKTIME
AC_CHECK_FUNCS([floor pow sqrt setlocale])

# Checks for types
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T

# Set precision
AC_SUBST(PRECISION, "-DJBM_PRECISION=2")

# Output
AC_CONFIG_SRCDIR([src/field.c])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
