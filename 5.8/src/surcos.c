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
 * \file surcos.c
 * \brief Source file to define the main simulation function.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2011-2023, Javier Burguete Tolosa.
 */
#include "config.h"
#include "jb/jb_win.h"
#include "furrow.h"
#include "parameters.h"
#include "scheme.h"
#include "field.h"
#include "kernel.h"

#define PROGRAM_NAME "surcos"
///< Macro to define the executable program name.

/**
 * \def LOCALE_DIR
 * \brief Macro to define the directory to search the locale language files.
 */
#ifdef G_OS_WIN32
#define LOCALE_DIR "..\\share\\locale"
#else
#define LOCALE_DIR "./locale"
#endif

#define DEBUG_SURCOS 0
///< Macro to debug the main surcos function.

/**
 * Main library function.
 * \return 0 on success, error code number on error.
 */
int
surcos (int argn,               ///< Arguments number.
        char *argc[])           ///< Argument strings.
{
  char *buffer, *buffer2;

#if DEBUG_SURCOS
  printf ("main: start\n");
#endif

  // Initing field arrays
  field->si = NULL;
  field->p = NULL;
  field->input = NULL;
  field->probe = NULL;

  // LIBRARY AND LANGUAGE INITIALIZATIONS
#if PARALLELIZING
  g_thread_init (NULL);
  parallel->mutex = g_mutex_new ();
#endif

  // Setting locale to system default
  buffer = setlocale (LC_ALL, "");
#if DEBUG_SURCOS
  printf ("locale = %s\n", buffer);
#endif

  // Setting numerical locale to international standard
  buffer = setlocale (LC_NUMERIC, "C");
#if DEBUG_SURCOS
  printf ("LC_NUMERIC = %s\n", buffer);
#endif

  // Setting the program name and locale directory to find locale files
  buffer2 = g_get_current_dir ();
  buffer = g_build_filename (buffer2, LOCALE_DIR, NULL);
  g_free (buffer2);
  buffer2 = bindtextdomain (PROGRAM_NAME, buffer);
#if DEBUG_SURCOS
  printf ("Locale dir = %s\n", buffer);
  printf ("bindtextdomain = %s\n", buffer2);
#endif
  g_free (buffer);

  // Setting the format of the codeset files (UTF-8)
  buffer2 = bind_textdomain_codeset (PROGRAM_NAME, "UTF-8");
#if DEBUG_SURCOS
  printf ("codeset = %s\n", buffer2);
#endif

  // Loading the locale strings
  buffer2 = textdomain (PROGRAM_NAME);
#if DEBUG_SURCOS
  printf ("textdomain = %s\n", buffer2);
#endif

  switch (argn)
    {
    case 2:
      kernel (argc[1], 0, 0);
      break;
    case 3:
      if (!strcmp (argc[2], "-v"))
        {
          kernel (argc[1], 0, 1);
          break;
        }
      //fallthrough
    default:
      printf ("%s:\n%s\n", _("Usage is"), _("surcos [-v] input_directory"));
      return 4;
    }
  field_destroy (field);

#if DEBUG_SURCOS
  printf ("surcos: end\n");
#endif
  return 0;
}
