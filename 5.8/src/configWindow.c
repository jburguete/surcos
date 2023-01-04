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
 * \file configWindow.c
 * \brief Source file to define the configuration of the problem.
 * \authors Javier Burguete Tolosa, Asier Lacasta Soto.
 * \copyright Copyright 2011-2023, Javier Burguete Tolosa.
 */
#include "config.h"
#include "jb/jb_win.h"
#include "furrow.h"
#include "parameters.h"
#include "scheme.h"
#include "field.h"
#include "kernel.h"
#include "configIrrigation.h"
#include "configGeometry.h"
#include "configFurrows.h"
#include "configProbes.h"
#include "configTimes.h"
#include "configWindow.h"
#include "graphics.h"
#include "mainWindow.h"

#define DEBUG_WINDOW_CONFIG_SET_READ 0
///< Macro to debug the window_config_set_read() function.

#define DEBUG_WINDOW_CONFIG_SET_WRITE 0
///< Macro to debug the window_config_set_write() function.

#define DEBUG_WINDOW_CONFIG_NEW 0
///< Macro to debug the window_config_new() function.

extern char *input_dir;

/**
 * Function to open the fertilizer configuration.
 */
static void
config_fertilizer_new (ConfigFertilizer * cf)
///< Fertilizer configuration structure.
{
  cf->box = (GtkGrid *) gtk_grid_new ();
  cf->label = (GtkLabel *) gtk_label_new (_("Solubility (kg/m³)"));
  gtk_grid_attach (cf->box, GTK_WIDGET (cf->label), 0, 0, 1, 1);
  cf->spin = (GtkSpinButton *) gtk_spin_button_new_with_range
    (FIELD_SOLUBILITY_MIN, FIELD_SOLUBILITY_MAX, 0.001);
  gtk_grid_attach (cf->box, GTK_WIDGET (cf->spin), 1, 0, 1, 1);
}

/**
 * Function to read the configuration.
 */
static void
window_config_set_read (ConfigGeometry * wg,
///< Field geometry configuration structure.
                        TableConfigFurrows * wf,
///< Field furrows configuration structure.
                        ConfigFertilizer * cf)
///< Fertilizer configuration structure.
{
  char buffer[512];
  FILE *f;
  double solubility;
  unsigned int i, nfurrows, recirculation;
#if DEBUG_WINDOW_CONFIG_SET_READ
  printf ("window_config_set_read: start\n");
#endif
  snprintf (buffer, 512, "%s/field.in", input_dir);
  f = g_fopen (buffer, "r");
  fscanf (f, "%u%u%lg", &recirculation, &nfurrows, &solubility);
#if DEBUG_WINDOW_CONFIG_SET_READ
  printf ("nfurrows=%u recirculation=%u\n", nfurrows, recirculation);
#endif
  gtk_spin_button_set_value (wf->spin, nfurrows);
  gtk_spin_button_set_value (cf->spin, solubility);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (wf->button_recirculation),
                                recirculation);
#if DEBUG_WINDOW_CONFIG_SET_READ
  printf ("reading points\n");
#endif
  for (i = 0; i < 4; ++i)
    config_point_read (wg->point + i, f);
#if DEBUG_WINDOW_CONFIG_SET_READ
  printf ("reading furrows\n");
#endif
  for (i = 0; i < 3; ++i)
    config_furrow_read (wf->furrow + i, f);
  fclose (f);
#if DEBUG_WINDOW_CONFIG_SET_READ
  printf ("window_config_set_read: end\n");
#endif
}

/**
 * Function to write the configuration.
 */
static void
window_config_set_write (ConfigGeometry * wg,
///< Field geometry configuration structure.
                         TableConfigFurrows * wf,
///< Field furrows configuration structure.
                         ConfigFertilizer * cf)
///< Fertilizer configuration structure.
{
  char buffer[512];
  FILE *f;
  double solubility;
  unsigned int i, nfurrows, recirculation;
#if DEBUG_WINDOW_CONFIG_SET_WRITE
  printf ("window_config_set_write: start\n");
#endif
  snprintf (buffer, 512, "%s/field.in", input_dir);
  f = g_fopen (buffer, "w");
  nfurrows = gtk_spin_button_get_value_as_int (wf->spin);
  solubility = gtk_spin_button_get_value (cf->spin);
  recirculation = gtk_toggle_button_get_active
    (GTK_TOGGLE_BUTTON (wf->button_recirculation));
#if DEBUG_WINDOW_CONFIG_SET_WRITE
  printf ("nfurrows=%u recirculation=%u\n", nfurrows, recirculation);
#endif
  fprintf (f, "%u\n%u\n%lg\n", recirculation, nfurrows, solubility);
#if DEBUG_WINDOW_CONFIG_SET_WRITE
  printf ("writing points\n");
#endif
  for (i = 0; i < 4; ++i)
    config_point_write (wg->point + i, f);
#if DEBUG_WINDOW_CONFIG_SET_WRITE
  printf ("writing furrows\n");
#endif
  for (i = 0; i < 3; ++i)
    config_furrow_write (wf->furrow + i, f);
  fclose (f);
#if DEBUG_WINDOW_CONFIG_SET_WRITE
  printf ("window_config_set_write: end\n");
#endif
}

/**
 * Function to open the problem configuration dialog.
 *
 * \return 1 on new problem, 0 otherwise.
 */
unsigned int
window_config_new ()
{
  WindowConfig w[1];
  unsigned int i;

#if DEBUG_WINDOW_CONFIG_NEW
  printf ("window_config_new: start\n");
  printf ("creating the irrigation table\n");
#endif
  config_irrigation_new (w->irrigation);

#if DEBUG_WINDOW_CONFIG_NEW
  printf ("creating the geometry table\n");
#endif
  config_geometry_new (w->geometry);

#if DEBUG_WINDOW_CONFIG_NEW
  printf ("creating the furrows table\n");
#endif
  table_config_furrows_new (w->furrows);

#if DEBUG_WINDOW_CONFIG_NEW
  printf ("creating the times table\n");
#endif
  config_times_new (w->times);

#if DEBUG_WINDOW_CONFIG_NEW
  printf ("creating the fertilizer table\n");
#endif
  config_fertilizer_new (w->fertilizer);

#if DEBUG_WINDOW_CONFIG_NEW
  printf ("creating the probes table\n");
#endif
  config_probes_new (w->probes);

#if DEBUG_WINDOW_CONFIG_NEW
  printf ("creating the notebook\n");
#endif
  w->notebook = (GtkNotebook *) gtk_notebook_new ();
  gtk_notebook_append_page (w->notebook, GTK_WIDGET (w->geometry->table),
                            gtk_label_new (_("Geometry")));
  gtk_notebook_append_page (w->notebook, GTK_WIDGET (w->furrows->table),
                            gtk_label_new (_("Furrows")));
  gtk_notebook_append_page (w->notebook, GTK_WIDGET (w->irrigation->box),
                            gtk_label_new (_("Inputs")));
  gtk_notebook_append_page (w->notebook, GTK_WIDGET (w->fertilizer->box),
                            gtk_label_new (_("Fertilizer")));
  gtk_notebook_append_page (w->notebook, GTK_WIDGET (w->probes->vbox),
                            gtk_label_new (_("Probes")));
  gtk_notebook_append_page (w->notebook, GTK_WIDGET (w->times->table),
                            gtk_label_new (_("Advanced parameters")));

#if DEBUG_WINDOW_CONFIG_NEW
  printf ("creating the title\n");
#endif
  w->bar = (GtkHeaderBar *) gtk_header_bar_new ();
  gtk_header_bar_set_title (w->bar, _("Configure irrigation"));
  gtk_header_bar_set_subtitle (w->bar,
                               _("Set the irrigation problem configuration"));
  gtk_header_bar_set_show_close_button (w->bar, 1);
  w->logo = (GtkImage *) gtk_image_new_from_file ("logo3.png");
  gtk_header_bar_pack_start (w->bar, GTK_WIDGET (w->logo));

#if DEBUG_WINDOW_CONFIG_NEW
  printf ("creating the dialog\n");
#endif
  w->dialog =
    (GtkDialog *) gtk_dialog_new_with_buttons (NULL,
                                               main_window->window,
                                               GTK_DIALOG_MODAL,
                                               _("_Cancel"),
                                               GTK_RESPONSE_CANCEL,
                                               _("_OK"), GTK_RESPONSE_OK, NULL);
  gtk_window_set_titlebar (GTK_WINDOW (w->dialog), GTK_WIDGET (w->bar));
  gtk_container_add (GTK_CONTAINER (gtk_dialog_get_content_area (w->dialog)),
                     GTK_WIDGET (w->notebook));
  gtk_widget_show_all (GTK_WIDGET (w->dialog));

#if DEBUG_WINDOW_CONFIG_NEW
  printf ("reading\n");
#endif
  window_config_set_read (w->geometry, w->furrows, w->fertilizer);

#if DEBUG_WINDOW_CONFIG_NEW
  printf ("dialog bucle\n");
#endif
  i = 0;
  if (gtk_dialog_run (w->dialog) == GTK_RESPONSE_OK)
    {
#if DEBUG_WINDOW_CONFIG_NEW
      printf ("if OK saving the values\n");
#endif
      config_irrigation_write (w->irrigation);
      window_config_set_write (w->geometry, w->furrows, w->fertilizer);
      config_times_write (w->times);
      config_probes_write (w->probes);
      i = 1;
    }

#if DEBUG_WINDOW_CONFIG_NEW
  printf ("destroying dialog\n");
#endif
  gtk_widget_destroy (GTK_WIDGET (w->dialog));

#if DEBUG_WINDOW_CONFIG_NEW
  printf ("window_config_new: end\n");
#endif
  return i;
}
