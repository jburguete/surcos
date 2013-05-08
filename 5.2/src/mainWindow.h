/*
SURCOS - A program to simulate furrow fertigation.

Copyright 2011-2013, Javier Burguete Tolosa.

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
 * \file mainWindow.h
 * \brief Header file to define the main window.
 * \authors Javier Burguete Tolosa, Asier Lacasta Soto.
 * \copyright Copyright 2011-2013, Javier Burguete Tolosa.
 */
#ifndef MAIN_WINDOW__H
#define MAIN_WINDOW__H 1

#include "jb/jb_win.h"

/**
 * \struct _Summary
 * \brief Structure to display the summary.
 */
struct _Summary
{
/**
 * \var scrolled_input
 * \brief GtkScrolledWindow to scroll the summary input window.
 * \var scrolled_output
 * \brief GtkScrolledWindow to scroll the summary output window.
 * \var view_input
 * \brief GtkTextView to view the summary input data.
 * \var view_output
 * \brief GtkTextView to view the summary output data.
 * \var text_input
 * \brief GtkTextBuffer to write the summary input data.
 * \var text_output
 * \brief GtkTextBuffer to write the summary output data.
 * \var notebook
 * \brief GtkNotebook containing the summary widgets.
 * \var dialog
 * \brief GtkDialog to display the summary.
 */
	GtkScrolledWindow *scrolled_input, *scrolled_output;
	GtkTextView *view_input, *view_output;
	GtkTextBuffer *text_input, *text_output;
	GtkNotebook *notebook;
	GtkDialog *dialog;
};

/**
 * \typedef Summary
 */
typedef struct _Summary Summary;

/**
 * \struct _MainWindow
 * \brief Main window structure.
 */
struct _MainWindow
{
/**
 * \var button_exit
 * \brief GtkToolButton to exit the program.
 * \var button_open
 * \brief GtkToolButton to open a fertigation problem.
 * \var button_config
 * \brief GtkToolButton to configure the fertigation problem.
 * \var button_run
 * \brief GtkToolButton to run the simulation.
 * \var button_plot
 * \brief GtkToolButton to plot the simulation results.
 * \var button_summary
 * \brief GtkToolButton to display a summary with the input and results data.
 * \var button_help
 * \brief GtkToolButton to display the program help.
 * \var toolbar
 * \brief GtkToolBar containing the tool buttons.
 * \var label
 * \brief GtkLabel used to pack the toolbar.
 * \var box
 * \brief GtkVBox used to pack the toolbar
 * \var pixbuf
 * \brief GdkPixbuf of the logo.
 * \var window
 * \brief Main GtkWindow.
 */
	GtkToolButton *button_exit, *button_open, *button_config, *button_run,
		*button_plot, *button_summary, *button_help;
	GtkToolbar *toolbar;
	GtkLabel *label;
	GtkVBox *box;
	GdkPixbuf *pixbuf;
	GtkWindow *window;
};

/**
 * \typedef MainWindow
 */
typedef struct _MainWindow MainWindow;

#if JBW_GRAPHIC == JBW_GRAPHIC_GTKGLEXT
void main_window_new(GdkGLConfig *glconfig);
#else
void main_window_new();
#endif

#endif
