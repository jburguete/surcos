SURCOS (5.2 version)
====================

A program to solve irrigation and fertigation in isolated furrows and furrow
networks.

REQUIRED
--------

* gcc or clang (to compile the surce code)
* autoconf (to generate the Makefile in different systems)
* pkg-config (to find the libraries to compile)
* gettext (to work with different locales)
* gthreads (to use multicores in shared memory machines)
* glib (extended utilities of C to work with data, lists, random numbers, ...)
* png (to work with PNG files)
* gtk+ (to make the interface windows)
* freeglut (interaction with OpenGL to draw graphics)
* doxygen (standard comments format to generate documentation)
* latex (to build the PDF manuals)
* jb (utility library of J. Burguete)

FILES
-----

* configure.ac: configure generator.
* Makefile.in: Makefile generator.
* TODO: List of tasks TO DO (in spanish)
* src/config.h.in: config header generator.
* src/*.h: Header files.
* src/*.c: Source files.
* *.png: Diagram and logo files.
* graph/*.tex: Latex files to generate the diagrams.
* Doxyfile: configuration file to generate doxygen documentation.
* locale/es/LC_MESSAGES/*.po: spanish language files.
* locale/fr/LC_MESSAGES/*.po: french language files.
* locale/it/LC_MESSAGES/*.po: italian language files.
* examples/*/*.in: example input files.
* manual/*: Manual files.

BUILDING INSTRUCTIONS
---------------------

Debian Linux 7.0
................
Debian kFreeBSD 7.0
...................
Debian Hurd 7.0
...............
FreeBSD 9.1
...........

* download the latest JB library (github.com:jburguete/jb.git)
* cd 5.2/src
* link the latest JB library to jb (i.e. ln -s PATH_TO_THE_JB_LIBRARY/1.4.2 jb)
* cd ..
* aclocal
* autoconf
* ./configure
* make
* strip surcos surcos_gui (optional: to make a final version)

Microsoft Windows 7 32 bits
...........................
Microsoft Windows XP 32 bits
............................

* install MSYS/MinGW (http://www.mingw.org) and the required libraries and
utilities
* download the latest JB library (github.com:jburguete/jb.git)
* cd 5.2/src
* link the latest JB library to jb (i.e. ln -s PATH_TO_THE_JB_LIBRARY/1.4.2 jb)
* cd ..
* aclocal
* autoconf
* LDFLAGS='-lglut -lopengl32' configure --host=mingw32
* make
* strip win32/bin/*.exe (optional: to make a final version)

MAKING REFERENCE MANUAL INSTRUCTIONS
------------------------------------

* cd 5.2
* doxygen
* cd doc/latex
* make
