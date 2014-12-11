SURCOS (5.5 version)
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

This software has been built and tested in the following operative systems:

Debian Linux 7.7
________________
Debian kFreeBSD 7.7
___________________
Debian Hurd 7.7
_______________
DragonFly BSD 4.0.1
___________________
FreeBSD 10.0
____________
NetBSD 6.1.5 (with modular xorg)
________________________________

1. Download the latest [JB library](https://github.com/jburguete/jb)

2. Download this repository

3. Link the JB library on the source directory to jb doing on a terminal:
> $ cd PATH_TO_SURCOS/5.5/src
>
> $ ln -s PATH_TO_THE_JB_LIBRARY/1.8.4 jb

4. Build SURCOS doing on a terminal:
> $ cd PATH_TO_SURCOS/5.5
>
> $ aclocal
>
> $ autoconf
>
> $ automake --add-missing
>
> $ ./configure
>
> $ make
>
> $ strip surcos surcos_gui (optional: to make a final version)

Microsoft Windows 7 32 bits
___________________________
Microsoft Windows XP 32 bits
____________________________

1. Install [MSYS/MinGW](http://www.mingw.org) and the required libraries and
utilities. You can follow detailed instructions in
[MinGW-64-Make](https://github.com/jburguete/MinGW-64-Make)

2. Download the latest [JB library](https://github.com/jburguete/jb)

3. Download this repository

4. Link the JB library on the source directory to jb doing on a terminal:
> $ cd PATH_TO_SURCOS/5.5/src
>
> $ ln -s PATH_TO_THE_JB_LIBRARY/1.8.4 jb

5. Build SURCOS doing on a terminal:
> $ cd PATH_TO_SURCOS/5.5
>
> $ aclocal
>
> $ autoconf
>
> $ automake --add-missing
>
> $ configure
>
> $ make
>
> $ strip win32/bin/*.exe (optional: to make a final version)

Microsoft Windows 7 64 bits
___________________________
Microsoft Windows XP 64 bits
____________________________

1. Install [MSYS/MinGW](http://www.mingw.org) and the required libraries and
utilities. You can follow detailed instructions in
[MinGW-64-Make](https://github.com/jburguete/MinGW-64-Make)

2. Download the latest [JB library](https://github.com/jburguete/jb)

3. Download this repository

4. Link the JB library on the source directory to jb doing on a terminal:
> $ cd PATH_TO_SURCOS/5.5/src
>
> $ ln -s PATH_TO_THE_JB_LIBRARY/1.8.4 jb

5. Build SURCOS doing on a terminal:
> $ cd PATH_TO_SURCOS/5.5
>
> $ aclocal
>
> $ autoconf
>
> $ automake --add-missing
>
> $ configure --host=x86_64-w64-mingw32
>
> $ make
>
> $ strip win64/bin/*.exe (optional: to make a final version)

OpenBSD 5.6
___________

1. Download the latest [JB library](https://github.com/jburguete/jb)

2. Download this repository

3. Link the JB library on the source directory to jb doing on a terminal:
> $ cd PATH_TO_SURCOS/5.5/src
>
> $ ln -s PATH_TO_THE_JB_LIBRARY/1.8.4 jb

4. Build SURCOS doing on a terminal:
> $ export AUTOCONF_VERSION=2.69 AUTOMAKE_VERSION=1.14
>
> $ cd PATH_TO_SURCOS/5.5
>
> $ aclocal
>
> $ autoconf
>
> $ automake --add-missing
>
> $ ./configure
>
> $ make
>
> $ strip surcos surcos_gui (optional: to make a final version)

MAKING REFERENCE MANUAL INSTRUCTIONS (doc/latex/refman.pdf file)
----------------------------------------------------------------

Execute on a terminal:
> $ cd PATH_TO_SURCOS/5.5
>
> $ doxygen
>
> $ cd doc/latex
>
> $ make
