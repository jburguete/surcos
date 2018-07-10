/*
SURCOS - A software tool to solve irrigation and fertigation in isolated furrows
and furrow networks.

Copyright 2011-2018, Javier Burguete Tolosa.

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
 * \file configTimes.c
 * \brief Source file to define the configuration of the advanced options.
 * \authors Javier Burguete Tolosa, Asier Lacasta Soto.
 * \copyright Copyright 2011-2018, Javier Burguete Tolosa.
 */
#define _GNU_SOURCE
#include <libintl.h>
#include <glib/gstdio.h>
#include <gtk/gtk.h>
#include "jb/jb_def.h"
#include "configTimes.h"

#define DEBUG_CONFIG_TIMES_NEW 0
///< Macro to debug the config_times_new() function.

extern char *input_dir;

/**
 * \fn void config_times_write(ConfigTimes *w)
 * \brief Function to write the advanced options in a file.
 * \param w
 * \brief Advanced options configuration structure.
 */
void
config_times_write (ConfigTimes * w)
{
  int c1, c2;
  double t, cfl, tvolc;
  char buffer[512];
  FILE *f;
  t = gtk_spin_button_get_value (w->spin[0]);
  cfl = gtk_spin_button_get_value (w->spin[1]);
  tvolc = gtk_spin_button_get_value (w->spin[2]);
  c1 = gtk_spin_button_get_value_as_int (w->spin[3]);
  c2 = gtk_spin_button_get_value_as_int (w->spin[4]);
  snprintf (buffer, 512, "%s/times.in", input_dir);
  f = g_fopen (buffer, "w");
  fprintf (f, "%g %g %g\n", t, cfl, tvolc);
  fclose (f);
  snprintf (buffer, 512, "%s/mesh.in", input_dir);
  f = g_fopen (buffer, "w");
  fprintf (f, "%d %d\n", c1, c2);
  fclose (f);
}

/**
 * \fn void config_times_read(ConfigTimes *w)
 * \brief Function to read the advanced options in a file.
 * \param w
 * \brief Advanced options configuration structure.
 */
void
config_times_read (ConfigTimes * w)
{
  int c1, c2;
  double t, cfl, tvolc;
  char buffer[512];
  FILE *f;
  snprintf (buffer, 512, "%s/times.in", input_dir);
  f = g_fopen (buffer, "r");
  fscanf (f, "%lf %lf %lf", &t, &cfl, &tvolc);
  fclose (f);
  snprintf (buffer, 512, "%s/mesh.in", input_dir);
  f = g_fopen (buffer, "r");
  fscanf (f, "%d %d", &c1, &c2);
  fclose (f);
  gtk_spin_button_set_value (w->spin[0], t);
  gtk_spin_button_set_value (w->spin[1], cfl);
  gtk_spin_button_set_value (w->spin[2], tvolc);
  gtk_spin_button_set_value (w->spin[3], c1);
  gtk_spin_button_set_value (w->spin[4], c2);
}

/**
 * \fn void config_times_new(ConfigTimes *w)
 * \brief Function to open the advanced options configuration.
 * \param w
 * \brief Advanced options configuration structure.
 */
void
config_times_new (ConfigTimes * w)
{
  int i;

#if DEBUG_CONFIG_TIMES_NEW
  printf ("config_times_new: start\n");
  printf ("creating the labels\n");
#endif
  // Creating the labels
  w->label[0] = (GtkLabel *) gtk_label_new (_("Maximum simulation time (s)"));
  w->label[1] = (GtkLabel *) gtk_label_new ("CFL");
  w->label[2] = (GtkLabel *) gtk_label_new (_("Data saving cycle (s)"));
  w->label[3] = (GtkLabel *) gtk_label_new
    (_("Cells number for distribution channel (between furrows)"));
  w->label[4] = (GtkLabel *) gtk_label_new
    (_("Cells number for irrigation channels"));

#if DEBUG_CONFIG_TIMES_NEW
  printf ("creating the spin buttons\n");
#endif
  // Creating the spin buttons
  w->spin[0] = (GtkSpinButton *) gtk_spin_button_new_with_range (0., 1e6, 1.);
  w->spin[1] =
    (GtkSpinButton *) gtk_spin_button_new_with_range (0., 1., 0.01);
  w->spin[2] = (GtkSpinButton *) gtk_spin_button_new_with_range (0., 1e6, 1.);
  w->spin[3] = (GtkSpinButton *) gtk_spin_button_new_with_range (2., 1e5, 1.);
  w->spin[4] = (GtkSpinButton *) gtk_spin_button_new_with_range (2., 1e5, 1.);

#if DEBUG_CONFIG_TIMES_NEW
  printf ("creating the table\n");
#endif
  // Creating a table and adding to the dialog
  w->table = (GtkGrid *) gtk_grid_new ();

#if DEBUG_CONFIG_TIMES_NEW
  printf ("adding labels and spin buttons\n");
#endif
  // Adding labels and spin buttons
  for (i = 0; i < 5; ++i)
    {
      gtk_grid_attach (w->table, GTK_WIDGET (w->label[i]), 0, i, 1, 1);
      gtk_grid_attach (w->table, GTK_WIDGET (w->spin[i]), 1, i, 1, 1);
    }
  gtk_grid_attach (w->table, gtk_label_new (NULL), 0, 5, 3, 1);

#if DEBUG_CONFIG_TIMES_NEW
  printf ("reading the data\n");
#endif
  // Reading the data
  config_times_read (w);

#if DEBUG_CONFIG_TIMES_NEW
  printf ("config_times_new: end\n");
#endif
}
