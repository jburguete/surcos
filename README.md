SURCOS (6.0 version)
====================

:gb:[english](README.md) :es:[español](README.es.md)
:fr:[français](README.fr.md)

A software tool to solve irrigation and fertigation in isolated furrows and
furrow networks.

AUTHORS
-------

* Javier Burguete Tolosa (jburguete@eead.csic.es)
* Asier Lacasta Soto (asierlacasta@gmail.com)
* Pilar García Navarro (pigar@unizar.es)

WINDOWS EXECUTABLE FILES
------------------------

This repository contains source and example files with the latest version of 
SURCOS. Stable versions with executable files and manuals for Microsoft Windows 
systems can be downloaded in 
[digital.csic](http://hdl.handle.net/10261/75830)

FILES
-----

* configure.ac: configure generator.
* Makefile.in: Makefile generator.
* TODO: list of tasks TO DO.
* src/config.h.in: config header generator.
* src/\*.h: header files.
* src/\*.c: source files.
* \*.png: diagram and logo files.
* graph/\*.tex: LaTeX files to generate the diagrams.
* Doxyfile: configuration file to generate doxygen documentation.
* locale/es/LC\_MESSAGES/\*.po: spanish language files.
* locale/fr/LC\_MESSAGES/\*.po: french language files.
* locale/it/LC\_MESSAGES/\*.po: italian language files.
* examples/\*.json: example input files.
* check-errors/\*.json: input files to check error messages.
* manual/\*: manual files.

BUILDING INSTRUCTIONS
---------------------

REQUIRED LIBRARIES AND TOOLS
____________________________

Mandatory:
* [gcc](https://gcc.gnu.org) or [clang](https://clang.llvm.org) to compile the
  source code.
* [make](https://www.gnu.org/software/make) to build the executable file.
* [autoconf](https://www.gnu.org/software/autoconf) to generate the Makefile in
  different operative systems.
* [automake](https://www.gnu.org/software/automake) to check the operative
  system.
* [pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config) to find the
  libraries to compile.
* [glib](https://developer.gnome.org/glib) extended utilities of C to work with
  data, lists, mapped files, regular expressions, using multicores in shared
  memory machines, ...
* [json-glib](https://gitlab.gnome.org/GNOME/json-glib) to deal with JSON files.
* [gettext](https://www.gnu.org/software/gettext) to work with different
  international locales and languages.
* [jb](https://github.com/jburguete/jb.git) tools library of J. Burguete.

Optional to get the processor properties:
* [libgtop](https://github.com/GNOME/libgtop) to get the processors number.

Optional: required only to build the GUI program:
* [png](http://libpng.sourceforge.net) to work with PNG files.
* [gtk](https://www.gtk.org) to create the interactive GUI tool.
* [glew](https://glew.sourceforge.net) high level OpenGL functions.

The following optional libraries can be used as alternative to the GtkGLArea
widget of the GTK library to interact with OpenGL to draw graphs.
* [freeglut](https://freeglut.sourceforge.net)
* [sdl2](https://www.libsdl.org)
* [glfw](https://www.glfw.org)

Optional to build the documentation:
* [doxygen](https://www.doxygen.nl) standard comments format to
  generate documentation)
* [latex](https://www.latex-project.org) to build the PDF manuals.

OPERATIVE SYSTEMS
_________________

You can install all required utilities and libraries using the instructions of
[install-unix](https://github.com/jburguete/install-unix).

This software has been built and tested in the following operative systems:
* Arch Linux
* Debian Linux 12
* Devuan Linux 4
* Dragonfly BSD 6.4.0
* Fedora Linux 38
* FreeBSD 13.2
* Gentoo Linux
* Linux Mint DE 5
* MacOS Ventura + Homebrew
* Manjaro Linux
* Microsoft Windows 10 + MSYS2
* NetBSD 9.3
* OpenBSD 7.3
* OpenInidiana Hipster
* OpenSUSE Linux 15.5
* Ubuntu Linux 23.04

On Microsoft Windows systems you have to install
[MSYS2](http://sourceforge.net/projects/msys2) and the required
libraries and utilities. You can follow detailed instructions in
[install-unix](https://github.com/jburguete/install-unix/blob/master/tutorial.pdf)
tutorial.

On NetBSD 9.3, to use the last GCC version, you have to do first on the
building terminal:
> $ export PATH="/usr/pkg/gcc12/bin:$PATH"

To do permanent this change the following line can be added to the ".profile"
file in the user root directory:
> $ PATH="/usr/pkg/gcc12/bin:$PATH"

On OpenBSD 7.3 you have to do first on the building terminal:
> $ export AUTOCONF\_VERSION=2.69 AUTOMAKE\_VERSION=1.16

BUILDING INSTRUCTIONS
_____________________

1. Download the latest version of the
  [JB library](https://github.com/jburguete/jb):
> $ git clone https://github.com/jburguete/jb.git

2. Build the JB library:
> $ cd jb/7.0.1
>
> $ ./build.sh
>
> $ cd ../..

3. Download this repository:
> $ git clone https://github.com/jburguete/surcos.git

4. Link the latest version of the JB library on the source directory to jb:
> $ cd surcos/6.0/src
>
> $ ln -s ../../../jb/7.0.1 jb
>
> $ cd ..

5. Build SURCOS doing on the terminal:
> $ ./build.sh
