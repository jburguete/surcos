SURCOS (5.6 version)
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

This software has been built and tested in the following operative systems.
Probably, it can be built in other systems, distributions, or versions but it
has not been tested.

Debian 8 (Hurd, Linux and kFreeBSD)
___________________________________
DragonFly BSD 4.6
___________________
Dyson
_____
FreeBSD 10.3
____________
NetBSD 7.0 (with modular xorg)
______________________________

1. Download the latest [JB library](https://github.com/jburguete/jb)
> $ git clone https://github.com/jburguete/jb.git

2. Download this repository
> $ git clone https://github.com/jburguete/surcos.git

3. Link the JB library on the source directory to jb doing on a terminal:
> $ cd surcos/5.6/src
>
> $ ln -s ../../../jb/1.14.2 jb

4. Build SURCOS doing on a terminal:
> $ cd ..
>
> $ ./build

OpenBSD 5.9
___________

1. Select adequate versions:
> $ export AUTOCONF_VERSION=2.69 AUTOMAKE_VERSION=1.15

2. Then, in a terminal, follow steps 1 to 4 of the previous subsection.

Microsoft Windows 7 (with MSYS2)
________________________________
Microsoft Windows 8.1 (with MSYS2)
__________________________________

1. Install [MSYS2](http://sourceforge.net/projects/msys2) and the required
libraries and utilities. You can follow detailed instructions in
[install-unix]
(https://github.com/jburguete/install-unix/blob/master/tutorial.pdf)

2. Then, in a MSYS2 terminal, follow steps 1 to 4 of the previous Debian Linux
8.2 section.

MAKING REFERENCE MANUAL INSTRUCTIONS (doc/latex/refman.pdf file)
----------------------------------------------------------------

Execute on a terminal:
> $ cd surcos/5.6
>
> $ doxygen
>
> $ cd doc/latex
>
> $ make
