# To work in Debian (Linux, kFreeBSD or Hurd) or in FreeBSD:
# aclocal
# autoconf
# ./configure
# make

# To work in Microsoft Windows (64 bits) with MinGW/MSYS:
# aclocal
# autoconf
# CPPFLAGS='-I/mingw/include -I/mingw/x86_64-w64-mingw32/include' configure --host=x86_64-w64-mingw32
# make

# To work in Microsoft Windows (32 bits) with MinGW/MSYS:
# aclocal
# autoconf
# LDFLAGS='-lglut -lopengl32' CPPFLAGS='-I/mingw/include -I/mingw/i686-w64-mingw32/include' configure --host=i686-w64-mingw32
# make

# To work in NetBSD:
# ln -s /usr/X11R7/include/GL/glu.h /usr/pkg/include/GL
# ln -s /usr/X11R7/lib/libGL.so.2 /usr/pkg/lib
# aclocal
# autoconf
# CPPFLAGS=-I/usr/pkg/include LDFLAGS=-L/usr/pkg/lib ./configure --host=x86_64--netbsd
# make

# To work in OpenBSD:
# AUTOCONF_VERSION=2.69 aclocal-1.11
# autoconf-2.69
# CPPFLAGS='-I/usr/local/include -I/usr/X11R6/include' LDFLAGS='-L/usr/local/lib -L/usr/X11R6/lib' ./configure
# make

# To work in DragonflyBSD:
# setenv CPPFLAGS '-I/usr/pkg/include -I/usr/pkg/include/gettext'
# setenv CFLAGS '-I/usr/pkg/include -I/usr/pkg/include/gettext'
# setenv LDFLAGS '-L/usr/pkg/lib'
# aclocal
# autoconf
# ./configure
# make

AC_PREREQ([2.67])
AC_INIT(surcos, [5.2])

# Checks for operative systems
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE
AC_MSG_NOTICE([System: $build])
AC_MSG_NOTICE([System: $host])
if test $host = "x86_64-w64-mingw32"; then
	AC_MSG_NOTICE([Microsoft Windows 64 bits])
	AC_SUBST(srcdir, [src/])
	AC_SUBST(objdir, [win64/obj/])
	AC_SUBST(bindir, [win64/bin/])
	AC_SUBST(etcdir, [win64/etc/])
	AC_SUBST(libdir, [win64/lib/])
	AC_SUBST(sharedir, [win64/share/])
	AC_SUBST(sysdir, [/mingw/])
	AC_SUBST(basedir, [win64/])
	AC_SUBST(icon, [win64/obj/logo.o])
	AC_SUBST(exegui, winsurcos)
	AC_SUBST(ccflags, "-march=corei7 -mfpmath=sse -msse3 -mwindows")
	AC_SUBST(libgcc, "libgcc_s_seh-1")
	win=1
elif test $host = "i686-pc-mingw32"; then
	AC_MSG_NOTICE([Microsoft Windows 32 bits])
	AC_SUBST(srcdir, [src/])
	AC_SUBST(objdir, [win32/obj/])
	AC_SUBST(bindir, [win32/bin/])
	AC_SUBST(etcdir, [win32/etc/])
	AC_SUBST(libdir, [win32/lib/])
	AC_SUBST(sharedir, [win32/share/])
	AC_SUBST(sysdir, [/mingw/])
	AC_SUBST(basedir, [win32/])
	AC_SUBST(icon, [win32/obj/logo.o])
	AC_SUBST(exegui, winsurcos)
	AC_SUBST(ccflags, "-march=pentium4 -mfpmath=sse -msse2 -mwindows")
	AC_SUBST(libgcc, "libgcc_s_sjlj-1")
	win=1
else
	AC_MSG_NOTICE([Unix type system])
	AC_SUBST(srcdir, [src/])
	AC_SUBST(objdir, [src/])
	AC_SUBST(bindir, [./])
	AC_SUBST(localdir, [locale/])
	AC_SUBST(exegui, surcos_gui)
	AC_SUBST(ccflags, [-march=native])
	win=0
fi
if test $win = 0; then
	AC_SUBST(latex, "pdflatex -shell-escape")
else
	AC_CHECK_TOOL(WINDRES, windres)
	AC_SUBST(latex, "pdflatex -enable-write18")
fi

# Checks for programs
AC_PROG_CC([gcc48 gcc47 gcc46 egcc gcc clang])
AC_PROG_MAKE_SET
AC_LANG([C])
PKG_PROG_PKG_CONFIG
AC_MSG_NOTICE([Compiler: $CC])

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

# Checks for libraries.
AC_SEARCH_LIBS([pow], [m])
PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([GTHREAD], [gthread-2.0])
AC_SEARCH_LIBS([textdomain], [intl])
PKG_CHECK_MODULES([PNG], [libpng])
AC_MSG_NOTICE([GTK+ version 3])
PKG_CHECK_MODULES([GTK], [gtk+-3.0], gtk3=1, gtk3=0)
if test $gtk3 = 0; then
	AC_MSG_NOTICE([GTK+ version 2])
	PKG_CHECK_MODULES([GTK], [gtk+-2.0])
fi
AC_SEARCH_LIBS([glEnd], [GL opengl32])
AC_SEARCH_LIBS([glutInit], [freeglut glut])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h locale.h sys/param.h libintl.h GL/freeglut.h])

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
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
