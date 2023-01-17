SURCOS (version 5.11)
====================

:gb:[english](README.md) :es:[español](README.es.md)
:fr:[français](README.fr.md)

Un outil informatique pour resolver irrigation et fertirrigation en sillons
isolés ou en réseaux de sillons.

AUTEURS
-------

* Javier Burguete Tolosa (jburguete@eead.csic.es)
* Asier Lacasta Soto (asierlacasta@gmail.com)
* Pilar García Navarro (pigar@unizar.es)

FICHIERS EXÉCUTABLES DE WINDOWS
-------------------------------

Ce référentiel contient fichiers source et d'example avec la dernière version de
SURCOS. On peut télécharger versions stables avec fichiers exécutables et
manuels pour systèmes Microsoft Windows en
[digital.csic](http://hdl.handle.net/10261/75830)

FICHIERS
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

REQUIS
------

* [gcc](https://gcc.gnu.org) or [clang](http://clang.llvm.org) pour compiler le
  code source
* [make](http://www.gnu.org/software/make) pour construire le fichier exécutable
* [autoconf](http://www.gnu.org/software/autoconf) pour générer le Makefile en
  differents systèmes d'exploitation
* [automake](http://www.gnu.org/software/automake) pour détecter le système
  d'exploitation
* [pkg-config](http://www.freedesktop.org/wiki/Software/pkg-config) pour trouver
  les bibliothèqes pour compiler
* [glib](https://developer.gnome.org/glib) outils étendues de C pour travailler
  avec données, listes, fichiers mappés, expressions régulières, utiliser
  multiprocesseurs en machines à mémoire partagée, ...
* [gettext](http://www.gnu.org/software/gettext) pour travailler avec differents
  codes et langues internationaux
* [jb](https://github.com/jburguete/jb.git) (utility library of J. Burguete)

OPTIONEL
________

* [png](http://libpng.sourceforge.net) (to work with PNG files)
* [gtk+3](http://www.gtk.org) (to create the interactive GUI tool)
* [freeglut](http://freeglut.sourceforge.net) (interaction with OpenGL to draw 
  graphics)
* [doxygen](http://www.stack.nl/~dimitri/doxygen) (optional: standard comments
  format to generate documentation)
* [latex](https://www.latex-project.org/) (optional: to build the PDF manuals)

BUILDING INSTRUCTIONS
---------------------

This software has been built and tested in the following operative systems.
Probably, it can be built in other systems, distributions, or versions but it
has not been tested.

Arch Linux
__________
Debian 10 (Linux)
_________________
Devuan Linux 3
_________________
DragonFly BSD 5.11
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
> $ cd surcos/5.11/src
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
> $ cd surcos/5.11
>
> $ doxygen
>
> $ cd doc/latex
>
> $ make
