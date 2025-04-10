SURCOS (versión 6.1)
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

* configure.ac: generador de configuración.
* Makefile.in: generador del Makefile.
* TODO: lista de tareas pendientes de realizar.
* src/config.h.in: generador del fichero de cabecera de configuración.
* src/\*.h: ficheros de cabecera.
* src/\*.c: ficheros fuente.
* \*.png: ficheros de diagrama y logo.
* graph/\*.tex: ficheros LaTeX para generar los diagramas.
* Doxyfile: fichero de configuración para generar la documentación doxygen.
* locale/es/LC\_MESSAGES/\*.po: ficheros de lenguaje español.
* locale/fr/LC\_MESSAGES/\*.po: ficheros de lenguaje francés.
* locale/it/LC\_MESSAGES/\*.po: ficheros de lenguaje italiano.
* examples/\*.json: ficheros de ejemplo de entrada.
* check-errors/\*.json: ficheros de entrada para probar los mensajes de error.
* manual/\*: ficheros de manual.

CONSTRUYENDO LA BIBLIOTECA
--------------------------

BIBLIOTECAS Y UTILIDADES REQUERIDAS
___________________________________

Obligatorio:
* [gcc](https://gcc.gnu.org) o [clang](https://clang.llvm.org) para compilar el
  código fuente.
* [make](https://www.gnu.org/software/make) para construir el fichero
  ejecutable.
* [autoconf](https://www.gnu.org/software/autoconf) para generar el Makefile en
  diferentes sistemas operativos.
* [automake](https://www.gnu.org/software/automake) para detectar el sistema
  operativo.
* [pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config) para
  encontrar las bibliotecas para compilar.
* [glib](https://developer.gnome.org/glib) utilidades extendidas de C para
  trabajar con datos, listas, ficheros mapeados, expresiones regulares, usar
  multiprocesador en máquinas de memoria compartida, ...
* [json-glib](https://gitlab.gnome.org/GNOME/json-glib) para trabajar con
  ficheros JSON.
* [gettext](https://www.gnu.org/software/gettext) para trabajar con diferentes
  códigos y lenguajes internacionales.
* [jb](https://github.com/jburguete/jb.git) bilbioteca de utilidades de
  J. Burguete.

Opcional para detectar las características del procesador:
* [libgtop](https://github.com/GNOME/libgtop) para obtener el número de
  procesadores.

Opcional: requerido únicamente para construir el programa GUI.
* [png](http://libpng.sourceforge.net) para trabajar con ficheros PNG.
* [gtk](https://www.gtk.org) para trabajar con ventanas interactivas.
* [glew](https://glew.sourceforge.net) funciones OpenGL de alto nivel.

Bibliotecas opcionales que pueden usarse como alternativa al widget GtkGLArea de
la biblioteca GTK para interactuar con OpenGL para dibujar gráficos:
* [freeglut](https://freeglut.sourceforge.net)
* [sdl2](https://www.libsdl.org)
* [glfw](https://www.glfw.org)

Opcional para construir la documentación:
* [doxygen](https://www.doxygen.nl) formato de comentarios estándar para generar
  documentación.
* [latex](https://www.latex-project.org/) para construir los manuales PDF.

SISTEMAS OPERATIVOS
___________________

Pueden instalarse todas las bibliotecas y utilidades requeridas usando las
instrucciones de [install-unix](https://github.com/jburguete/install-unix).

Este programa ha sido construido y probado en los siguientes sistemas
operativos:
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
* OpenBSD 7.6
* OpenInidiana Hipster
* OpenSUSE Linux 15.5
* Ubuntu Linux 23.04

En sistemas Microsoft Windows hay que instalar
[MSYS2](http://sourceforge.net/projects/msys2) además de las bibliotecas y
utilidades requeridas. Puede seguir instrucciones detalladas en el tutorial
[install-unix](https://github.com/jburguete/install-unix/blob/master/tutorial.pdf).

En NetBSD 9.3, para usar la última versión del compilador GCC, hay que hacer
primero en el terminal:
> $ export PATH="/usr/pkg/gcc12/bin:$PATH"

Si se quiere hacer permanente tambien se puede añadir en el fichero ".profile"
de la carpeta raíz del usuario la línea:
> $ PATH="/usr/pkg/gcc12/bin:$PATH"

En OpenBSD 7.6 hay que hacer antes en el terminal:
> $ export AUTOCONF\_VERSION=2.69 AUTOMAKE\_VERSION=1.16

INSTRUCCIONES DE CONSTRUCCIÓN
_____________________________

1. Descárguese la última versión de la
  [biblioteca JB](https://github.com/jburguete/jb)
> $ git clone https://github.com/jburguete/jb.git

2. Construya la biblioteca JB:
> $ cd jb/7.0.1
>
> $ ./build.sh
>
> $ cd ../..

3. Descárguese este repositorio:
> $ git clone https://github.com/jburguete/surcos.git

4. Enlace la carpeta de la última versión de la biblioteca Jb en la carpeta
  fuente de este programa a jb:
> $ cd surcos/6.1/src
>
> $ ln -s ../../../jb/7.0.1 jb
>
> $ cd ..

5. Construya el programa surcos haciendo en el terminal:
> $ ./build.sh
