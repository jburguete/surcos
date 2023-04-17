# Header
AC_PREREQ([2.69])
AC_INIT([surcos],[6.0])

# Checks for operative systems
AC_CANONICAL_TARGET
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
	if test $MSYSTEM = "UCRT64"; then
		mingw=ucrt64
	else
		mingw=mingw64
	fi
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
	so=".so"
	bindir="./"
	AC_SUBST(latex, "pdflatex -shell-escape")
	AC_SUBST(objdir, [obj/])
	AC_SUBST(objbindir, [objbin/])
	AC_SUBST(localdir, [locale/])
	AC_SUBST(exegui, surcos_gui)
else
	AC_CHECK_TOOL(WINDRES, windres)
	so=".dll"
	bindir="$windir/bin/"
	AC_SUBST(latex, "pdflatex -enable-write18")
	AC_SUBST(exegui, winsurcos)
	AC_SUBST(sysdir, [/$mingw/])
	AC_SUBST(objdir, [$windir/obj/])
	AC_SUBST(objbindir, [$windir/objbin/])
	AC_SUBST(etcdir, [$windir/etc/])
	AC_SUBST(libdir, [$windir/lib/])
	AC_SUBST(sharedir, [$windir/share/])
	AC_SUBST(basedir, [$windir/])
	AC_SUBST(icon, [$windir/obj/logo.o])
	AC_SUBST(WIN, [$windir/])
	AC_SUBST(EXE, [.exe])
	test -d $windir/obj || mkdir -p $windir/obj
	test -d $windir/bin || mkdir -p $windir/bin
fi
AC_SUBST(SO, [$so])
AC_SUBST(srcdir, [src/])
AC_SUBST(bindir, [$bindir])

# Checks for programs
AC_PROG_CC([gcc-12 gcc12 gcc-11 gcc11 gcc-10 gcc10 egcc gcc clang])
AC_MSG_NOTICE([Compiler: $CC])
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_LANG([C])
PKG_PROG_PKG_CONFIG
if test "`command -v gmake`"; then
	AC_SUBST(MAKE, [gmake])
else
	AC_SUBST(MAKE, [make])
fi
if test $CC = "clang"; then
	AC_CHECK_PROGS([LLVM_PROFDATA], [llvm-profdata llvm-profdata-15], 0)
	if test $LLVM_PROFDATA = 0; then
	      	AC_MSG_ERROR([No llvm-profdata])
	fi
	AC_MSG_NOTICE([llvm-profdata: $LLVM_PROFDATA])
fi
AC_SUBST(RPATH,[$rpath])
$CC -v

# Checks for compiler flags
compiler=$CC

# Checking -march=native compiler flag
if test $win = 0 && test $openbsd = 0; then
	AC_ARG_WITH([native], AS_HELP_STRING([--with-native],
		[Build using -march=native compiler flag]))
	if test "x$with_native" = "xyes"; then
		CC="$CC -march=native"
		AC_LINK_IFELSE([AC_LANG_PROGRAM([[]], [[]])], [arch=1], \
			[arch=0])
		CC=$compiler
		if test $arch = 0; then
			AC_MSG_NOTICE([checking for -march=native... no])
		else
			AC_MSG_NOTICE([checking for -march=native... yes])
			AC_SUBST(ARCH, "-march=native")
		fi
	fi
fi

# Checking -flto compiler flag
CC="$CC -flto"
AC_LINK_IFELSE([AC_LANG_PROGRAM([[]], [[]])], [lto=1], [lto=0])
CC=$compiler
if test $lto = 0; then
	AC_MSG_NOTICE("checking for -flto... no")
else
	AC_MSG_NOTICE("checking for -flto... yes")
	AC_SUBST(LTO, "-flto")
fi

# Checking PGO optimiztion
AC_ARG_WITH([pgo], AS_HELP_STRING([--with-pgo], [Build using PGO]))
pgo=0
if test "x$with_pgo" = "xyes"; then
	CC="$CC -fprofile-generate"
	AC_LINK_IFELSE([AC_LANG_PROGRAM([[]], [[]])], [pgo=1])
	CC=$compiler
	if test pgo = 0; then
		CC="$CC -fprofile-instr-generate"
		AC_LINK_IFELSE([AC_LANG_PROGRAM([[]], [[]])], [pgo=1])
		CC=$compiler
	fi
fi
if test "$pgo" = 0; then
		AC_MSG_NOTICE("checking for PGO... no")
else
		AC_MSG_NOTICE("checking for PGO... yes")
fi
AC_SUBST(PGO, [$pgo])

# Checks for header files.
AC_CHECK_HEADERS_ONCE([libintl.h locale.h stdlib.h string.h unistd.h])
if test -f "src/jb/src/jb_def.h"; then
	AC_MSG_NOTICE([JB headers: OK])
else
	AC_MSG_ERROR([No JB headers])
fi

# Set precision
AC_ARG_WITH([precision],
	AS_HELP_STRING([--with-precision],
		[Build with precision degree (1-5), default=3]))
if test "x$with_precision" = "x1"; then
	precision=1
elif test "x$with_precision" = "x2"; then
	precision=2
elif test "x$with_precision" = "x4"; then
	precision=4
elif test "x$with_precision" = "x5"; then
	precision=5
else
	precision=3
fi
AC_SUBST(PRECISION, [-DJBM_PRECISION=$precision])
AC_SUBST(SUFFIX, [-$precision])

# Checks for libraries
libjb=${bindir}libjb-$precision$so
if test -x $libjb; then
	AC_MSG_NOTICE([$libjb: OK])
else
	AC_MSG_ERROR([$libjb: not installed])
fi
libjbm=${bindir}libjbm-$precision$so
if test -x $libjbm; then
	AC_MSG_NOTICE([$libjbm: OK])
else
	AC_MSG_ERROR([$libjbm: not installed])
fi
libjbbin=${bindir}libjbbin-$precision$so
if test -x $libjbbin; then
	AC_MSG_NOTICE([$libjbbin: OK])
else
	AC_MSG_ERROR([$libjbbin: not installed])
fi
AC_SEARCH_LIBS([pow], [m])
PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([GTHREAD], [gthread-2.0])
PKG_CHECK_MODULES([JSON], [json-glib-1.0])
AC_SEARCH_LIBS([textdomain], [intl])
PKG_CHECK_MODULES([GTOP], [libgtop-2.0], , AC_MSG_NOTICE([No libgtop]))

# Check for GTK
AC_ARG_WITH([gtk], AS_HELP_STRING([--with-gtk],
	[Build using a version of GTK (3 (--with-gtk=3) or 4 (--with-gtk=4))]))
gtk4=0
gtk3=0
gtk=0
if test "x$with_gtk" = "x4"; then
	PKG_CHECK_MODULES([GTK], [gtk4], [gtk4=1], [gtk4=0])
	gtk3=1
elif test "x$with_gtk" = "x3"; then
	gtk3=1
fi
if test $gtk4 = 1; then
	gtk=1
	AC_SUBST(GTK4, "-DGTK4=1")
elif test $gtk3 = 1; then
	PKG_CHECK_MODULES([GTK], [gtk+-3.0],
		[AC_MSG_NOTICE([With GTK])],
		[AC_MSG_NOTICE([Without GTK])])
	gtk=1
	AC_SUBST(GTK4, "-DGTK4=0")
fi

# Graphical interface
if test $gtk = 1; then
	libjbwin=${bindir}libjbwin-$precision$so
	if test -x $libjbbin; then
		AC_MSG_NOTICE([$libjbwin: OK])
	else
		AC_MSG_ERROR([$libjbwin: not installed])
	fi
	PKG_CHECK_MODULES([PNG], [libpng])
	PKG_CHECK_MODULES([FREETYPE], [freetype2])
	PKG_CHECK_MODULES([GLEW], [glew])
	AC_SEARCH_LIBS([glFlush], [GL opengl32])
	AC_SUBST([EXEGUI], "\$(exe)")

	# Checks for GtkGLArea, FreeGLUT, GLFW or SDL to draw the graphic window
	AC_ARG_WITH([gtkglarea], AS_HELP_STRING([--with-gtkglarea],
		[Build graphic window with GtkGLArea]))
	AC_ARG_WITH([freeglut], AS_HELP_STRING([--with-freeglut],
		[Build graphic window with FreeGLUT]))
	AC_ARG_WITH([glfw],
		AS_HELP_STRING([--with-glfw], [Build graphic window with GLFW]))
	AC_ARG_WITH([sdl],
		AS_HELP_STRING([--with-sdl], [Build graphic window with SDL]))
	if test "x$with_freeglut" = "xyes"; then
		AC_SUBST(GRAPHIC,"-DHAVE_FREEGLUT=1")
		AC_SEARCH_LIBS([glutInit], [freeglut glut])
	elif test "x$with_glfw" = "xyes"; then
		AC_SUBST(GRAPHIC,"-DHAVE_GLFW=1")
		PKG_CHECK_MODULES([GLFW], [glfw3])
	elif test "x$with_sdl" = "xyes"; then
		AC_SUBST(GRAPHIC,"-DHAVE_SDL=1")
		PKG_CHECK_MODULES([SDL], [sdl2])
	else
		AC_SUBST(GRAPHIC,"-DHAVE_GTKGLAREA=1")
	fi

	# Check for FreeMono font
	AC_MSG_NOTICE([searching FreeMono font])
	if test $win = 1; then
		dir=/$mingw
	else
		dir=/usr
	fi
	font=`find $dir -type f -iname freemono.otf`
	if test "x$font" = "x"; then
		font=`find $dir -type f -iname freemono.ttf`
		if test "x$font" = "x"; then
			AC_MSG_ERROR([font not found])
		fi
	fi
	AC_MSG_NOTICE([Font: $font])
	AC_SUBST(FONT, "-DFONT=\\\"$font\\\"")
fi

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_MKTIME
AC_CHECK_FUNCS([floor pow sqrt setlocale])

# Checks for types
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T

# Output
AC_CONFIG_SRCDIR([src/field.c])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
