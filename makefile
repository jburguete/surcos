basedir = /usr/
incdir = -I$(basedir)include/
incldir = -I$(basedir)lib/
libdir = -L$(basedir)lib/
srcdir = src/
objdir = $(srcdir)
bindir = ./
podir = locale/
localdir = $(podir)
mandir = manual/
es = es/LC_MESSAGES/
fr = fr/LC_MESSAGES/

obj = $(objdir)jb_def.o $(objdir)jb_math.o $(objdir)jb_win.o \
	$(objdir)furrow.o $(objdir)parameters.o $(objdir)scheme.o $(objdir)field.o \
	$(objdir)kernel.o $(objdir)configIrrigation.o $(objdir)configGeometry.o \
	$(objdir)configFurrows.o $(objdir)configTimes.o $(objdir)configProbes.o \
	$(objdir)configWindow.o $(objdir)graphics.o $(objdir)mainWindow.o

src = $(srcdir)jb/jb_def.c $(srcdir)jb/jb_math.c $(srcdir)jb/jb_win.c \
	$(srcdir)furrow.c $(srcdir)parameters.c $(srcdir)scheme.c $(srcdir)field.c \
	$(srcdir)kernel.c $(srcdir)configIrrigation.c $(srcdir)configGeometry.c \
	$(srcdir)configFurrows.c $(srcdir)configTimes.c $(srcdir)configProbes.c \
	$(srcdir)configWindow.c $(srcdir)graphics.c $(srcdir)mainWindow.c

man = $(mandir)ppal.tex $(mandir)bib/references.bib \
	$(mandir)capitulos/Capitulo1.tex \
	$(mandir)capitulos/Capitulo2.tex \
	$(mandir)capitulos/Capitulo3.tex \
	$(mandir)capitulos/Capitulo3.tex

man_en = $(mandir)ppal_EN.tex $(mandir)bib/references.bib \
	$(mandir)capitulos/Chapter2.tex 

png = curve32.png fondo6.png logo2.png logo3.png diagram_furrow.png \
	  diagram_geometry_en.png diagram_geometry_es.png diagram_geometry_fr.png \
	  diagram_input.png

dep = makefile $(srcdir)jb/jb_config.h
depjb = $(dep) $(srcdir)jb/jb_def.h $(srcdir)jb/jb_math.h $(srcdir)jb/jb_win.h
depkernel = $(depjb) $(srcdir)config.h \
	$(srcdir)furrow.h $(srcdir)parameters.h $(srcdir)scheme.h $(srcdir)field.h \
	$(srcdir)kernel.h
depconfig = $(depkernel) $(srcdir)configIrrigation.h $(srcdir)configGeometry.h \
	$(srcdir)configFurrows.h $(srcdir)configTimes.h $(srcdir)configProbes.h \
	$(srcdir)configWindow.h
dep_gui = $(depconfig) $(srcdir)graphics.h $(srcdir)mainWindow.h

inc = $(incdir)glib-2.0 $(incldir)glib-2.0/include \
	$(incdir)cairo $(incdir)atk-1.0 $(incdir)pango-1.0 \
	$(incdir)gdk-pixbuf-2.0 $(incdir)gtk-2.0 $(incldir)gtk-2.0/include

lib = -lm -lgobject-2.0 -lgthread-2.0 -lglib-2.0 -lgdk-x11-2.0 -lgtk-x11-2.0 -lglut

flags = $(inc) -c -O3

CC = gcc -Wall -DJBW=2 -DJBW_DRAW=2 -DJBW_GRAPHIC=3 #-pg

exe = $(bindir)surcos_gui

all: $(exe) doc/latex/refman.pdf $(mandir)ppal.pdf $(mandir)ppal_EN.pdf \
	$(podir)surcos_gui.pot \
	$(localdir)$(es)surcos_gui.mo $(localdir)$(fr)surcos_gui.mo

## Our main object (GUI)
$(exe): $(obj) $(objdir)surcos_gui.o
	$(CC) $(libdir) $(obj) $(objdir)surcos_gui.o $(lib) -o $(exe)

## Objects!
$(objdir)jb_def.o: $(dep) $(srcdir)jb/jb_def.h $(srcdir)jb/jb_def.c
	$(CC) $(flags) $(srcdir)jb/jb_def.c -o $(objdir)jb_def.o

$(objdir)jb_math.o: $(dep) $(srcdir)jb/jb_def.h $(srcdir)jb/jb_math.h \
	$(srcdir)jb/jb_math.c
	$(CC) $(flags) $(srcdir)jb/jb_math.c -o $(objdir)jb_math.o

$(objdir)jb_win.o: $(depjb) $(srcdir)jb/jb_win.c
	$(CC) $(flags) $(srcdir)jb/jb_win.c -o $(objdir)jb_win.o

$(objdir)furrow.o: $(depjb) $(srcdir)config.h \
	$(srcdir)furrow.h $(srcdir)furrow.c
	$(CC) $(flags) $(srcdir)furrow.c -o $(objdir)furrow.o

$(objdir)parameters.o: $(depjb) $(srcdir)config.h $(srcdir)furrow.h \
	$(srcdir)parameters.h $(srcdir)parameters.c
	$(CC) $(flags) $(srcdir)parameters.c -o $(objdir)parameters.o

$(objdir)scheme.o: $(depjb) $(srcdir)config.h \
	$(srcdir)furrow.h $(srcdir)parameters.h $(srcdir)scheme.h $(srcdir)scheme.c
	$(CC) $(flags) $(srcdir)scheme.c -o $(objdir)scheme.o

$(objdir)field.o: $(depjb) $(srcdir)config.h \
	$(srcdir)furrow.h $(srcdir)parameters.h $(srcdir)scheme.h $(srcdir)field.h \
	$(srcdir)field.c
	$(CC) $(flags) $(srcdir)field.c -o $(objdir)field.o

$(objdir)kernel.o: $(depkernel) $(srcdir)kernel.c
	$(CC) $(flags) $(srcdir)kernel.c -o $(objdir)kernel.o

$(objdir)configIrrigation.o: $(depkernel) $(srcdir)configIrrigation.h \
	$(srcdir)configIrrigation.c
	$(CC) $(flags) $(srcdir)configIrrigation.c \
		-o $(objdir)configIrrigation.o

$(objdir)configGeometry.o: $(depkernel) $(srcdir)configGeometry.h \
	$(srcdir)configGeometry.c
	$(CC) $(flags) $(srcdir)configGeometry.c -o $(objdir)configGeometry.o

$(objdir)configFurrows.o: $(depkernel) $(srcdir)configFurrows.h \
	$(srcdir)configFurrows.c
	$(CC) $(flags) $(srcdir)configFurrows.c -o $(objdir)configFurrows.o

$(objdir)configTimes.o: $(depkernel) $(srcdir)configTimes.h \
	$(srcdir)configTimes.c
	$(CC) $(flags) $(srcdir)configTimes.c -o $(objdir)configTimes.o

$(objdir)configProbes.o: $(depkernel) $(srcdir)configProbes.h \
	$(srcdir)configProbes.c
	$(CC) $(flags) $(srcdir)configProbes.c -o $(objdir)configProbes.o

$(objdir)configWindow.o: $(depconfig) $(srcdir)configWindow.c
	$(CC) $(flags) $(srcdir)configWindow.c -o $(objdir)configWindow.o

$(objdir)graphics.o: $(depkernel) $(srcdir)graphics.h $(srcdir)graphics.c
	$(CC) $(flags) $(srcdir)graphics.c -o $(objdir)graphics.o

$(objdir)mainWindow.o: $(dep_gui) $(srcdir)mainWindow.c
	$(CC) $(flags) $(srcdir)mainWindow.c -o $(objdir)mainWindow.o

$(objdir)surcos_gui.o: $(dep) $(srcdir)mainWindow.h $(srcdir)surcos_gui.c
	$(CC) $(flags) $(srcdir)surcos_gui.c -o $(objdir)surcos_gui.o

## Making the languages general rules (GUI)
$(podir)surcos_gui.pot: makefile $(src) $(srcdir)surcos_gui.c 
	xgettext -d $(exe) -o $(podir)surcos_gui.pot --from-code=UTF-8 \
		$(src_gui) $(srcdir)surcos_gui.c 
## THE FIRST TIME
#	msginit -l es -o $(podir)$(es)surcos_gui.po -i $(podir)surcos_gui.pot \
		--no-translator
#	msginit -l fr -o $(podir)$(fr)surcos_gui.po -i $(podir)surcos_gui.pot \
		--no-translator
## OTHER TIMES
	msgmerge -U $(podir)$(es)surcos_gui.po $(podir)surcos_gui.pot
	msgmerge -U $(podir)$(fr)surcos_gui.po $(podir)surcos_gui.pot
	gedit --encoding UTF-8 $(podir)$(es)surcos_gui.po $(podir)$(fr)surcos_gui.po

## Compiling the spanish language
$(localdir)$(es)surcos_gui.mo: $(podir)$(es)surcos_gui.po $(podir)surcos_gui.pot
	msgfmt -c -v -o $(localdir)$(es)surcos_gui.mo $(podir)$(es)surcos_gui.po

## Compiling the french language
$(localdir)$(fr)surcos_gui.mo: $(podir)$(fr)surcos_gui.po $(podir)surcos_gui.pot
	msgfmt -c -v -o $(localdir)$(fr)surcos_gui.mo $(podir)$(fr)surcos_gui.po

## Making the reference manual
doc/latex/refman.pdf: Doxyfile makefile $(dep_gui) $(src) \
	$(srcdir)surcos.c  $(srcdir)surcos_gui.c
	doxygen
	cd doc/latex; make

## Compiling the user manuals
$(mandir)ppal.pdf: makefile $(man)
	cd $(mandir); pdflatex ppal; bibtex ppal; pdflatex ppal; pdflatex ppal

$(mandir)ppal_EN.pdf: makefile $(man_en)
	cd $(mandir); pdflatex ppal_EN; bibtex ppal_EN; pdflatex ppal_EN; \
		pdflatex ppal_EN

## Clean all generated-objects
clean:
	rm $(exe) $(objdir)*.o $(mandir)ppal.pdf $(mandir)ppal_EN.pdf \
		$(localdir)$(es)surcos_gui.mo $(localdir)$(fr)surcos_gui.mo

## Rebuild option with cleaning functionallity
rebuild: clean $(exe)
