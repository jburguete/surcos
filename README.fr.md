SURCOS (version 6.1)
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

* configure.ac: générateur de configuration.
* Makefile.in: générateur du Makefile.
* TODO: liste de tâches à faire.
* src/config.h.in: générateur du fichier d'en tête de configuration.
* src/\*.h: fichiers d'en tête.
* src/\*.c: fichiers source.
* \*.png: fichiers de diagramme et logo.
* graph/\*.tex: fichiers LaTeX pour générer les diagrammes.
* Doxyfile: fichier de configuration pour générer la documentation doxygen.
* locale/es/LC\_MESSAGES/\*.po: fichiers de langue espagnole.
* locale/fr/LC\_MESSAGES/\*.po: fichiers de langue française.
* locale/it/LC\_MESSAGES/\*.po: fichiers de langue italienne.
* examples/\*.json: fichiers d'example d'entrée.
* check-errors/\*.json: fichiers d'entrée pour prouver les messages d'erreur.
* manual/\*: fichiers de manuel.

INSTRUCTIONS DE CONSTRUCTION
----------------------------

BIBLIOTHÈQUES ET OUTILS REQUIS
______________________________

Obligatoire:
* [gcc](https://gcc.gnu.org) ou [clang](https://clang.llvm.org) pour compiler le
  code source.
* [make](https://www.gnu.org/software/make) pour construire le fichier
  executable.
* [autoconf](https://www.gnu.org/software/autoconf) pour générer le Makefile en
  differents systèmes d'exploitation.
* [automake](https://www.gnu.org/software/automake) pour détecter le système
  d'exploitation.
* [pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config) pour
  trouver les bibliothèques pour compiler.
* [glib](https://developer.gnome.org/glib) outils étendues de C pour travailler
  avec données, listes, fichiers mappés, expressions régulières, utiliser
  multiprocesseurs en machines à mémoire partagée, ...
* [json-glib](https://gitlab.gnome.org/GNOME/json-glib) pour travailler avec
  fichiers JSON.
* [gettext](https://www.gnu.org/software/gettext) pour travailler avec
  differents codes et langues internationaux.
* [jb](https://github.com/jburguete/jb.git) bibliothèque d'outils de
  J. Burguete.

Optionnel pour détecter les characteristiques du processeur:
* [libgtop](https://github.com/GNOME/libgtop) pour obtenir le nombre de
  processeurs.

Optionnel: obligatoire seulement pour construire le programme GUI.
* [png](http://libpng.sourceforge.net) pour travailler avec fichiers PNG.
* [gtk](https://www.gtk.org) pour travailler avec fenêtres interactives.
* [glew](https://glew.sourceforge.net) functions OpenGL de haut niveau.

On peut utiliser les bibliothèques graphiques suivantes comme alternative au
widget GtkGLArea de la bibliothèque GTK pour interagir avec OpenGL pour dessiner
graphiques:
* [freeglut](https://freeglut.sourceforge.net)
* [sdl2](https://www.libsdl.org)
* [glfw](https://www.glfw.org)

Optionnel pour construire la documentation:
* [doxygen](https://www.doxygen.nl) format de commentaires standard pour générer
  documentation.
* [latex](https://www.latex-project.org/) pour construire les manuels PDF.

SISTÈMES D'EXPLOITATION
_______________________

On peut installer tous les bibliothèques et les outils requis en utilisant les
instructions en [install-unix](https://github.com/jburguete/install-unix).

Ce programme a été construit et verifié avec les suivants systèmes
d'exploitation:
* Arch Linux
* Debian Linux 12
* Devuan Linux 5
* Dragonfly BSD 6.4.1
* Fedora Linux 42
* FreeBSD 14.2
* Gentoo Linux
* Linux Mint DE 6
* MacOS Ventura + Homebrew
* Manjaro Linux
* Microsoft Windows 11 + MSYS2
* NetBSD 10.1
* OpenBSD 7.7
* OpenInidiana Hipster
* OpenSUSE Linux 15.6
* Ubuntu Linux 25.04

Avec systèmes Microsoft Windows on doit installer
[MSYS2](http://sourceforge.net/projects/msys2) et en plus les bibliothèques et
les outils requis. On peut suivre les instructions détaillées dans le
didacticiel
[install-unix](https://github.com/jburguete/install-unix/blob/master/tutorial.pdf).

Avec NetBSD 10.1, pour utiliser le derniére version du compilateur GCC, il faut
faire avant dans le terminal:
> $ export PATH="/usr/pkg/gcc12/bin:$PATH"

Pour faire le changement permanent on peut ajouter à la dernière ligne du
fichier ".profile" dans le répertoire racine du utilisateur:
> $ PATH="/usr/pkg/gcc12/bin:$PATH"

Avec OpenBSD 7.7 on doit faire avant dans le terminal:
> $ export AUTOCONF\_VERSION=2.69 AUTOMAKE\_VERSION=1.16

INSTRUCTIONS DE CONSTRUCTION
____________________________

1. Télécharger la dernière version de la
  [bibliotèque JB](https://github.com/jburguete/jb):
> $ git clone https://github.com/jburguete/jb.git

2. Construire la bibliothèque JB:
> $ cd jb/11.3.9
>
> $ ./build.sh
>
> $ cd ../..

3. Télécharge ce dépôt:
> $ git clone https://github.com/jburguete/surcos.git

4. Lier la dernière version de la bibliothèque JB dans le dossier source à jb:
> $ cd surcos/6.1/src
>
> $ ln -s ../../../jb/11.3.9 jb
>
> $ cd ..

5. Construire SURCOS an faisant dans le terminal:
> $ ./build.sh
