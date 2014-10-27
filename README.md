SURCOS (5.3 version)
====================

A software tool to solve irrigation and fertigation in isolated furrows and
furrow networks.

AUTHORS
-------

* Javier Burguete Tolosa (jburguete@eead.csic.es)
* Asier Lacasta Soto (asierlacasta@gmail.com)
* Pilar García Navarro (pigar@unizar.es)

REQUIRED
--------

* gcc or clang (to compile the source code)
* autoconf (to generate the Makefile in different systems)
* automake (to detect the system)
* pkg-config (to find the libraries to compile)
* gettext (to work with different international locales and languages)
* gthreads (to use multicores in shared memory machines)
* glib (extended utilities of C to work with data, lists, random numbers, ...)
* png (to work with PNG files)
* gtk+ (to make the interface windows)
* freeglut (interaction with OpenGL to draw graphics)
* [jb](https://github.com/jburguete/jb.git) (utility library of J. Burguete)
* doxygen (optional: standard comments format to generate documentation)
* latex (optional: to build the PDF manuals)

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

Debian Linux 7.6
________________
Debian kFreeBSD 7.6
___________________
Debian Hurd 7.6
_______________
DragonFly BSD 3.8.2
___________________
FreeBSD 10.0
____________
NetBSD 6.1.5 (with modular xorg)
________________________________

* download the latest [JB library](https://github.com/jburguete/jb)
* cd 5.3/src
* link the latest JB library to jb (i.e. ln -s PATH_TO_THE_JB_LIBRARY/1.8.3 jb)
* cd ..
* aclocal
* autoconf
* automake --add-missing
* ./configure
* make
* strip surcos surcos_gui (optional: to make a final version)

Microsoft Windows 7 32 bits
___________________________
Microsoft Windows XP 32 bits
____________________________

* install [MSYS/MinGW](http://www.mingw.org) and the required libraries and
utilities. You can follow detailed instructions in
[MinGW-64-Make](https://github.com/jburguete/MinGW-64-Make)
* download the latest [JB library](https://github.com/jburguete/jb)
* cd 5.3/src
* link the latest JB library to jb (i.e. ln -s PATH_TO_THE_JB_LIBRARY/1.8.3 jb)
* cd ..
* aclocal
* autoconf
* automake --add-missing
* configure
* make
* strip win32/bin/*.exe (optional: to make a final version)

Microsoft Windows 7 64 bits
___________________________
Microsoft Windows XP 64 bits
____________________________

* install [MSYS/MinGW](http://www.mingw.org) and the required libraries and
utilities. You can follow detailed instructions in
[MinGW-64-Make](https://github.com/jburguete/MinGW-64-Make)
* download the latest [JB library](https://github.com/jburguete/jb)
* cd 5.3/src
* link the latest JB library to jb (i.e. ln -s PATH_TO_THE_JB_LIBRARY/1.8.3 jb)
* cd ..
* aclocal
* autoconf
* automake --add-missing
* configure --host=x86_64-w64-mingw32
* make
* strip win64/bin/*.exe (optional: to make a final version)

OpenBSD 5.5
___________

* download the latest [JB library](https://github.com/jburguete/jb)
* cd 5.3/src
* link the latest JB library to jb (i.e. ln -s PATH_TO_THE_JB_LIBRARY/1.8.3 jb)
* cd ..
* export AUTOCONF_VERSION=2.69 AUTOMAKE_VERSION=1.14
* aclocal
* autoconf
* automake --add-missing
* ./configure
* make
* strip surcos surcos_gui (optional: to make a final version)

MAKING REFERENCE MANUAL INSTRUCTIONS
------------------------------------

* cd 5.3
* doxygen
* cd doc/latex
* make
