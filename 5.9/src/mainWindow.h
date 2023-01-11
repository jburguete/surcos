/*
SURCOS - A software tool to solve irrigation and fertigation in isolated furrows
and furrow networks.

Copyright 2011-2023, Javier Burguete Tolosa.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
	list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
	this list of conditions and the following disclaimer in the documentation
	and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY Javier Burguete Tolosa ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL Javier Burguete Tolosa OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * \file mainWindow.h
 * \brief Header file to define the main window.
 * \authors Javier Burguete Tolosa, Asier Lacasta Soto.
 * \copyright Copyright 2011-2023, Javier Burguete Tolosa.
 */
#ifndef MAIN_WINDOW__H
#define MAIN_WINDOW__H 1

/**
 * \struct _Summary
 * \brief Structure to display the summary.
 */
typedef struct
{
  GtkNotebook *notebook;
  ///< GtkNotebook containing the summary widgets.
  GtkScrolledWindow *scrolled_input;
  ///< GtkScrolledWindow to scroll the summary input window.
  GtkScrolledWindow *scrolled_output;
  ///< GtkScrolledWindow to scroll the summary output window.
  GtkTextView *view_input;
  ///< GtkTextView to view the summary input data.
  GtkTextView *view_output;
  ///< GtkTextView to view the summary output data.
  GtkTextBuffer *text_input;
  ///< GtkTextBuffer to write the summary input data.
  GtkTextBuffer *text_output;
  ///< GtkTextBuffer to write the summary output data.
} Summary;

/**
 * \struct _MainWindow
 * \brief Main window structure.
 */
typedef struct
{
  WindowPlot plot[1];
  ///< WindowPlot widget to plot the graphics.
  Summary summary[1];
  ///< Summary widget.
  GtkWindow *window;
  ///< Main GtkWindow.
  GtkButton *button_exit;
  ///< GtkButton to exit the program.
  GtkButton *button_new;
  ///< GtkButton to create a new fertigation problem.
  GtkButton *button_open;
  ///< GtkButton to open a fertigation problem.
  GtkButton *button_config;
  ///< GtkButton to configure the fertigation problem.
  GtkButton *button_run;
  ///< GtkButton to run the simulation.
  GtkButton *button_help;
  ///< GtkButton to display the program help.
  GtkButton *button_about;
  ///< GtkButton to display the about dialog.
  GtkGrid *grid;
  ///< GtkGrid to pack the widgets.
  GdkPixbuf *pixbuf;
  ///< GdkPixbuf of the logo.
  GMainLoop *loop;
  ///< GMainLoop main loop.
  unsigned int plotted;
  ///< 1 on plotted graphics, 0 otherwise.
  unsigned int open;
  ///< 1 on open case, 0 otherwise.
} MainWindow;

extern char *input_dir;
extern MainWindow *main_window;

void main_window_update (MainWindow * w);
MainWindow *main_window_new ();

#endif
