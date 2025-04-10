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
 * \file surcos_gui.c
 * \brief Source file to define the main function with graphical GUI.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2011-2023, Javier Burguete Tolosa
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <locale.h>
#include "config.h"
#include "jb/src/win.h"
#include "jb/src/json.h"
#include "furrow.h"
#include "parameters.h"
#include "scheme.h"
#include "field.h"
#include "kernel.h"
#include "graphics.h"
#include "mainWindow.h"

#define DEBUG_MAIN 0            ///< Macro to debug the main() function.

#define PROGRAM_NAME "surcos"
///< Macro to define the executable program name.

/**
 * \def LOCALE_DIR
 * \brief Macro to define the directory to search the locale language files.
 */
#ifdef G_OS_WIN32
#define LOCALE_DIR "..\\share\\locale"
#else
#define LOCALE_DIR "locale"
#endif

MainWindow *main_window;        ///< Main window.

#if HAVE_FREEGLUT

#elif HAVE_GLFW

/**
 * Idle function in the main loop.
 *
 * \return Always 1.
 */
int
main_window_idle ()
{
  if (main_window->plotted
      && glfwWindowShouldClose (main_window->plot->graphic->window))
    jbw_graphic_loop_exit = 1;
  return 1;
}

#endif

#if HAVE_FREEGLUT || HAVE_SDL

/**
 * Function to resize the plot.
 */
void
main_window_resize (int width,  ///< Width.
                    int height) ///< Height.
{
  if (main_window->plotted)
    jbw_graphic_resize (width, height);
}

#endif

#if HAVE_FREEGLUT || HAVE_SDL || HAVE_GLFW

/**
 * Function to render the plot.
 */
void
main_window_render ()
{
  if (main_window->plotted)
    jbw_graphic_render ();
}

#endif

/**
 * Main function.
 */
int
main (int argn,                 ///< Arguments number.
      char *argc[])             ///< Argument strings.
{
  MainWindow *window;

#if DEBUG_MAIN
  printf ("main: start\n");
#endif

  // Initing JB windows library
  jbw_init (&argn, &argc);

  // Initing field arrays
  field->si = NULL;
  field->input = NULL;
  field->probe = NULL;

  // LIBRARY AND LANGUAGE INITIALIZATIONS

  // Init threads
#if PARALLELIZING
  g_thread_init (NULL);
  parallel->mutex = g_mutex_new ();
#endif

  // Setting locale to system default, numerical locale to international
  // standard, and the program name and locale directory to find locale files
  jb_set_locales (PROGRAM_NAME, LOCALE_DIR, "", "C");

  // Showing the main window
#if DEBUG_MAIN
  printf ("main window\n");
#endif
  main_window = window = main_window_new ();

  // Opening a case if specified
  if (argn >= 2)
    {
      input_file = g_strdup (argc[argn - 1]);
      kernel_read (input_file);
      main_window_update (window);
    }

  // Starting the main bucle
#if HAVE_GTKGLAREA
  jbw_graphic_loop_pointer = g_main_loop_new (NULL, 0);
#elif HAVE_FREEGLUT
  jbw_graphic_loop_idle = jbw_process_pending;
  jbw_graphic_loop_resize = main_window_resize;
  jbw_graphic_loop_render = main_window_render;
#elif HAVE_SDL
  jbw_graphic_loop_resize = main_window_resize;
  jbw_graphic_loop_render = main_window_render;
#elif HAVE_GLFW
  jbw_graphic_loop_idle = main_window_idle;
  jbw_graphic_loop_render = main_window_render;
#endif
  jbw_graphic_loop ();

  // Freeing memory
  free (window);
  g_free (input_file);

  return 0;
}
