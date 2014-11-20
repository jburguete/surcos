/*
SURCOS - A software tool to solve irrigation and fertigation in isolated furrows
and furrow networks.

Copyright 2011-2014, Javier Burguete Tolosa.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 
1. Redistributions of source code must retain the above copyright notice, this
	list of conditions and the following disclaimer.
 
2. Redistributions in binary form must reproduce the above copyright notice,
	this list of conditions and the following disclaimer in the documentation
	and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY Javier Burguete Tolosa ``AS IS'' AND ANY EXPRESS
OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
EVENT SHALL Javier Burguete Tolosa OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * \file surcos_gui.c
 * \brief Source file to define the main function with graphical GUI.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2011-2014, Javier Burguete Tolosa
 */
#define _GNU_SOURCE
#include <locale.h>
#include <libintl.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include "jb/jb_win.h"
#include "field.h"
#include "mainWindow.h"

/**
 * \def DEBUG_MAIN
 * \brief Macro to define the main() function.
 */
#define DEBUG_MAIN 0

/**
 * \var windowMain
 * \brief Main window.
 */
GtkWidget *windowMain;

/**
 * \def PROGRAM_NAME
 * \brief Macro to define the executable program name.
 */
#define PROGRAM_NAME "surcos_gui"

/**
 * \def LOCALE_DIR
 * \brief Macro to define the directory to search the locale language files.
 */
#ifdef G_OS_WIN32
	#define LOCALE_DIR "..\\share\\locale"
#else
	#define LOCALE_DIR "locale"
#endif

/**
 * \fn int main(int argn, char *argc[])
 * \brief Main function.
 * \param argn
 * \brief Arguments number.
 * \param argc
 * \brief Argument strings.
 */
int main(int argn, char *argc[])
{
	char *buffer, *buffer2;
	MainWindow *window;

#if DEBUG_MAIN
printf("main: start\n");
#endif

	// Initing field arrays
	field->si = NULL;
	field->input = NULL;
	field->probe = NULL;

	// LIBRARY AND LANGUAGE INITIALIZATIONS

	// Init threads
#if PARALLELIZING
	g_thread_init(NULL);
	parallel->mutex = g_mutex_new();
#endif

	// Setting locale to system default
	buffer = setlocale(LC_ALL, "");
#if DEBUG_MAIN
printf("locale = %s\n", buffer);
#endif

	// Setting numerical locale to international standard
	buffer = setlocale(LC_NUMERIC, "C");
#if DEBUG_MAIN
printf("LC_NUMERIC = %s\n", buffer);
#endif

	// Avoiding the overriding of the locale settings by gtk_init
	gtk_disable_setlocale();

	// Setting the program name and locale directory to find locale files
	buffer = g_build_filename(g_get_current_dir(), LOCALE_DIR, NULL);
	buffer2 = bindtextdomain(PROGRAM_NAME, buffer);
#if DEBUG_MAIN
printf("Locale dir = %s\n", buffer);
printf("bindtextdomain = %s\n", buffer2);
#endif

	// Setting the format of the codeset files (UTF-8)
	buffer2 = bind_textdomain_codeset(PROGRAM_NAME, "UTF-8");
#if DEBUG_MAIN
printf("codeset = %s\n", buffer2);
#endif
	
	// Loading the locale string
	buffer2 = textdomain(PROGRAM_NAME);
#if DEBUG_MAIN
printf("textdomain = %s\n", buffer2);
#endif

	// Init the GTK+ library
#if DEBUG_MAIN
printf("gtk_init\n");
#endif
	gtk_init(&argn, &argc);

	// Init the GLUT library
#if DEBUG_MAIN
printf("glutInit\n");
#endif
	glutInit(&argn, argc);

#if JBW_GRAPHIC == JBW_GRAPHIC_GLUT
	// Some FREEGLUT options
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	// Showing the main window
#if DEBUG_MAIN
printf("main window\n");
#endif
	window = main_window_new();
#else
	// Initing graphical libraries
	jbw_graphic_init(&argn, &argc);
	// Showing the main window
#if DEBUG_MAIN
printf("main window\n");
#endif
	window = main_window_new();
#endif

	// Starting the main bucle
	gtk_main();

	// Freeing memory
	gtk_widget_destroy(GTK_WIDGET(window->window));
	free(window);

	return 0;
}
