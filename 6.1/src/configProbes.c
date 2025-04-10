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
 * \file configProbes.c
 * \brief Source file to define the configuration of the probes data.
 * \authors Javier Burguete Tolosa, Asier Lacasta Soto.
 * \copyright Copyright 2011-2023, Javier Burguete Tolosa.
 */
#include "config.h"
#include "jb/src/win.h"
#include "jb/src/json.h"
#include "furrow.h"
#include "parameters.h"
#include "field.h"
#include "configProbes.h"

#define DEBUG_CONFIG_PROBES_UPDATE 0
///< Macro to debug the config_probes_update() function.

#define DEBUG_CONFIG_PROBES_READ 0
///< Macro to debug the config_probes_read() function.

/**
 * Function to insert probes.
 */
static void
config_probes_insert (ConfigProbes * w, ///< Probes configuration structure.
                      unsigned int nProbesOld,  ///< Old probes number.
                      unsigned int nProbes)     ///< New probes number.
{
  char buffer[32];
  GtkWidget *widget;
  unsigned int i;
  for (i = nProbesOld; i < nProbes; ++i)
    {
      snprintf (buffer, 32, "%d", i + 1);
      widget = gtk_button_new_with_label (buffer);
      gtk_widget_set_can_focus (widget, FALSE);
      gtk_widget_show (widget);
      gtk_grid_attach (w->grid, widget, 0, i + 1, 1, 1);
      w->list = g_list_append (w->list, widget);
      widget = gtk_spin_button_new_with_range (-1e6, 1e6, 0.01);
      gtk_widget_show (widget);
      gtk_grid_attach (w->grid, widget, 1, i + 1, 1, 1);
      w->list = g_list_append (w->list, widget);
      widget = gtk_spin_button_new_with_range (-1e6, 1e6, 0.01);
      gtk_widget_show (widget);
      gtk_grid_attach (w->grid, widget, 2, i + 1, 1, 1);
      w->list = g_list_append (w->list, widget);
    }
}

/**
 * Function to remove probes.
 */
static void
config_probes_remove (ConfigProbes * w, ///< Probes configuration structure.
                      unsigned int nProbesOld,  ///< Old probes number.
                      unsigned int nProbes)     ///< New probes number.
{
  GList *element, *next;
  unsigned int i, j;
  element = g_list_nth (w->list, 3 * nProbes);
  for (i = nProbes; i < nProbesOld; ++i)
    {
      for (j = 0; j < 3; ++j)
        {
          next = element->next;
          gtk_grid_remove (w->grid, GTK_WIDGET (element->data));
          w->list = g_list_remove_link (w->list, element);
          element = next;
        }
    }
}

/**
 * Function to update the probes number.
 */
static void
config_probes_update (GtkSpinButton * spin,
///< GtkSpinButton configuring the probes number.
                      ConfigProbes * w) ///< Probes configuration structure.
{
  unsigned int nProbesOld;
  nProbesOld = w->nProbes;
#if DEBUG_CONFIG_PROBES_UPDATE
  printf ("config_probes_update: start\n");
#endif
  w->nProbes = gtk_spin_button_get_value_as_int (spin);
#if DEBUG_CONFIG_PROBES_UPDATE
  printf ("nProbes = %d\n", w->nProbes);
  printf ("Inserting rows\n");
#endif
  config_probes_insert (w, nProbesOld, w->nProbes);
#if DEBUG_CONFIG_PROBES_UPDATE
  printf ("Removing extra rows\n");
#endif
  config_probes_remove (w, nProbesOld, w->nProbes);
#if DEBUG_CONFIG_PROBES_UPDATE
  printf ("config_probes_update: end\n");
#endif
}

/**
 * Function to read the probes configuration.
 */
void
config_probes_read (ConfigProbes * w)   ///< Probes configuration structure.
{
  Probe *probe;
  GList *list;
  unsigned int i;
#if DEBUG_CONFIG_PROBES_READ
  printf ("config_probes_read: start\n");
  printf ("nProbes = %d\n", w->nProbes);
#endif
  gtk_spin_button_set_value (w->spin, w->nProbes);
  config_probes_insert (w, 0, w->nProbes);
  list = w->list;
  if (w->nProbes)
    {
      w->probe = (Probe *) realloc (w->probe, w->nProbes * sizeof (Probe));
      probe = w->probe;
      for (i = 0; i < w->nProbes; i++)
        {
#if DEBUG_CONFIG_PROBES_READ
          printf ("Probe %i: x=%lg y=%lg\n",
                  i, (double) probe[i].x, (double) probe[i].y);
#endif
          list = list->next;
          gtk_spin_button_set_value (GTK_SPIN_BUTTON (list->data), probe[i].x);
          list = list->next;
          gtk_spin_button_set_value (GTK_SPIN_BUTTON (list->data), probe[i].y);
          list = list->next;
        }
    }
  else
    {
      free (w->probe);
      w->probe = NULL;
    }
#if DEBUG_CONFIG_PROBES_READ
  printf ("config_probes_read: end\n");
#endif
}

/**
 * Function to write the probes configuration in a file.
 */
void
config_probes_write (ConfigProbes * w)  ///< Probes configuration structure.
{
  Probe *probe;
  unsigned int i;
  GList *list;
  list = w->list;
  probe = w->probe;
  for (i = 0; i < w->nProbes; i++)
    {
      list = list->next;
      probe[i].x = gtk_spin_button_get_value (GTK_SPIN_BUTTON (list->data));
      list = list->next;
      probe[i].y = gtk_spin_button_get_value (GTK_SPIN_BUTTON (list->data));
      list = list->next;
    }
}

/**
 * Function to free the memory used by a Probes configuration structure.
 */
static void
config_probes_destroy (ConfigProbes * w)
                       ///< Probes configuration structure.
{
  g_list_free (w->list);
}

/**
 * Function to open the probes configuration.
 */
void
config_probes_new (ConfigProbes * w)    ///< Probes configuration structure.
{
  const char *label[] = { _("Probe"), "x (m)", "y (m)" };
  unsigned int i;
  w->list = NULL;
  w->label = (GtkLabel *) gtk_label_new (_("Number of probes"));
  w->spin = (GtkSpinButton *) gtk_spin_button_new_with_range (0., 100., 1.);
  w->hbox = (GtkGrid *) gtk_grid_new ();
  gtk_grid_attach (w->hbox, GTK_WIDGET (w->label), 0, 0, 1, 1);
  gtk_grid_attach (w->hbox, GTK_WIDGET (w->spin), 1, 0, 1, 1);
  w->grid = (GtkGrid *) gtk_grid_new ();
  for (i = 0; i < 3; ++i)
    {
      w->button[i] = (GtkButton *) gtk_button_new_with_label (label[i]);
      gtk_widget_set_can_focus (GTK_WIDGET (w->button[i]), FALSE);
      gtk_grid_attach (w->grid, GTK_WIDGET (w->button[i]), i, 0, 1, 1);
    }
  w->vbox = (GtkGrid *) gtk_grid_new ();
  gtk_grid_attach (w->vbox, GTK_WIDGET (w->hbox), 0, 0, 1, 1);
  gtk_grid_attach (w->vbox, GTK_WIDGET (w->grid), 0, 1, 1, 1);
  g_signal_connect_after (w->spin, "value-changed",
                          (GCallback) config_probes_update, w);
  g_signal_connect_swapped (w->vbox, "destroy",
                            (GCallback) config_probes_destroy, w);
}
