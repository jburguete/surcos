SURCOS (versión 5.7)
====================

:gb:[english](README.md) :es:[español](README.es.md)
:fr:[français](README.fr.md)

Un programa informático para resolver riego y fertirriego en surcos aislados o
redes de surcos.

AUTORES
-------

* Javier Burguete Tolosa (jburguete@eead.csic.es)
* Asier Lacasta Soto (asierlacasta@gmail.com)
* Pilar García Navarro (pigar@unizar.es)

FICHEROS EJECUTABLES DE WINDOWS
-------------------------------

Este repositorio contiene ficheros fuente y de ejemplo con la última versión de
SURCOS. Versiones estables con ficheros ejecutables y manuales para sistemas
Microsoft Windows se pueden descargar en
[digital.csic](http://hdl.handle.net/10261/75830)

FICHEROS
--------

* configure.ac: configure generator.
* Makefile.in: Makefile generator.
* TODO: List of tasks TO DO
* src/config.h.in: config header generator.
* src/\*.h: Header files.
* src/\*.c: Source files.
* \*.png: Diagram and logo files.
* graph/\*.tex: Latex files to generate the diagrams.
* Doxyfile: configuration file to generate doxygen documentation.
* locale/es/LC\_MESSAGES/\*.po: spanish language files.
* locale/fr/LC\_MESSAGES/\*.po: french language files.
* locale/it/LC\_MESSAGES/\*.po: italian language files.
* examples/\*/\*.in: example input files.
* manual/\*: Manual files.

BUILDING INSTRUCTIONS
---------------------

REQUIRED
________

* [gcc](https://gcc.gnu.org) or [clang](http://clang.llvm.org) (to compile the
  source code)
* [make](http://www.gnu.org/software/make) (to build the executable file)
* [autoconf](http://www.gnu.org/software/autoconf) (to generate the Makefile in
  different operative systems)
* [automake](http://www.gnu.org/software/automake) (to check the operative
  system)
* [pkg-config](http://www.freedesktop.org/wiki/Software/pkg-config) (to find the
  libraries to compile)
* [glib](https://developer.gnome.org/glib) (extended utilities of C to work with
  data, lists, mapped files, regular expressions, using multicores in shared
  memory machines, ...)
* [gettext](http://www.gnu.org/software/gettext) (to work with different
  locales and languages)
* [png](http://libpng.sourceforge.net) (to work with PNG files)
* [gtk+3](http://www.gtk.org) (to create the interactive GUI tool)
* [freeglut](http://freeglut.sourceforge.net) (interaction with OpenGL to draw 
  graphics)
* [jb](https://github.com/jburguete/jb.git) (utility library of J. Burguete)
* [doxygen](http://www.stack.nl/~dimitri/doxygen) (optional: standard comments
  format to generate documentation)
* [latex](https://www.latex-project.org/) (optional: to build the PDF manuals)

This software has been built and tested in the following operative systems.
Probably, it can be built in other systems, distributions, or versions but it
has not been tested.

Arch Linux
__________
Debian 10 (Linux)
_________________
Devuan Linux 3
_________________
DragonFly BSD 5.8
___________________
Dyson Illumos
_____________
Fedora Linux 32
_______________
FreeBSD 12.1
____________
Linux Mint DE 3
_______________
Manjaro Linux
_____________
NetBSD 9.0
__________
OpenIndiana Hipster
___________________
OpenSUSE Linux Leap 15
______________________
Xubuntu Linux 20.04
__________________

1. Download the latest [JB library](https://github.com/jburguete/jb)
> $ git clone https://github.com/jburguete/jb.git

2. Download this repository
> $ git clone https://github.com/jburguete/surcos.git

3. Link the JB library on the source directory to jb doing on a terminal:
> $ cd surcos/5.7/src
>
> $ ln -s ../../../jb/2.4.1 jb

4. Build SURCOS doing on a terminal:
> $ cd ..
>
> $ ./build

OpenBSD 6.7
___________

1. Select adequate versions:
> $ export AUTOCONF_VERSION=2.69 AUTOMAKE_VERSION=1.16

2. Then, in a terminal, follow steps 1 to 4 of the previous subsection.

Microsoft Windows 10 (with MSYS2)
_________________________________

1. Install [MSYS2](http://sourceforge.net/projects/msys2) and the required
libraries and utilities. You can follow detailed instructions in
[install-unix](https://github.com/jburguete/install-unix/blob/master/tutorial.pdf)

2. Then, in a MSYS2 terminal, follow steps 1 to 4 of the previous Debian Linux
8 section.

MAKING REFERENCE MANUAL INSTRUCTIONS (doc/latex/refman.pdf file)
----------------------------------------------------------------

Execute on a terminal:
> $ cd surcos/5.7
>
> $ doxygen
>
> $ cd doc/latex
>
> $ make
