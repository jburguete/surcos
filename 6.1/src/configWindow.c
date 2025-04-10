/*
SURCOS - A software tool to solve irrigation and fertigation in isolated furrows
and furrow networks.

Copyright 2011-2025, Javier Burguete Tolosa.

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
 * \copyright Copyright 2011-2025, Javier Burguete Tolosa.
 */
#include "config.h"
#include "jb/src/win.h"
#include "jb/src/json.h"
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

#define DEBUG_CONFIG_WINDOW_READ 0
///< Macro to debug the config_window_read() function.

#define DEBUG_CONFIG_WINDOW_WRITE 0
///< Macro to debug the config_window_write() function.

#define DEBUG_CONFIG_WINDOW_NEW_CLOSE 0
///< Macro to debug the config_window_new_close() function.

#define DEBUG_CONFIG_WINDOW_NEW 0
///< Macro to debug the config_window_new() function.

ConfigWindow config_window[1];
///< Structure to configurate the fertigation problem.

/**
 * Function to copy 2 Field structures.
 */
void
field_copy (Field * field_new,  ///< New Field structure.
            Field * field_model)        ///< Model Field structure.
{
  size_t size;
  memcpy (field_new, field_model, sizeof (Field));
  if (field_new->nfurrows)
    {
      size = field_new->nfurrows * sizeof (Furrow);
      field_new->si = (Furrow *) malloc (size);
      memcpy (field_new->si, field_model->si, size);
    }
  else
    field_new->si = NULL;
  size = (field_new->nwinputs + field_new->nsinputs) * sizeof (Input);
  field_new->input = (Input *) malloc (size);
  memcpy (field_new->input, field_model->input, size);
  if (field_new->nprobes)
    {
      size = field_new->nprobes * sizeof (Probe);
      field_new->probe = (Probe *) malloc (size);
      memcpy (field_new->probe, field_model->probe, size);
    }
  else
    field_new->probe = NULL;
}

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
config_window_read (ConfigWindow * w)
                        ///< Field configuration structure.
{
  ConfigIrrigation *ci;
  ConfigGeometry *wg;
  ConfigPoint *point;
  TableConfigFurrows *wf;
  ConfigFurrow *furrow;
  ConfigTimes *ct;
  ConfigFertilizer *cf;
  ConfigProbes *cp;
  Field *field;
  size_t size;
#if DEBUG_CONFIG_WINDOW_READ
  printf ("config_window_read: start\n");
#endif
  field = w->field;
  ci = w->irrigation;
  wg = w->geometry;
  wf = w->furrows;
  cf = w->fertilizer;
  ct = w->times;
  cp = w->probes;
#if DEBUG_CONFIG_WINDOW_READ
  printf ("reading inputs\n");
#endif
  ci->n[0] = field->nwinputs;
  size = ci->n[0] * sizeof (Input);
  ci->input[0] = (Input *) realloc (ci->input[0], size);
  memcpy (ci->input[0], field->input, size);
  ci->n[1] = field->nsinputs;
  if (ci->n[1])
    {
      size = ci->n[1] * sizeof (Input);
      ci->input[1] = (Input *) realloc (ci->input[1], size);
      memcpy (ci->input[1], field->input + field->nwinputs, size);
    }
  else
    {
      free (ci->input[1]);
      ci->input[1] = NULL;
    }
  config_irrigation_read (ci);
#if DEBUG_CONFIG_WINDOW_READ
  printf ("reading points\n");
#endif
  point = wg->point;
  point->r[0] = field->x[0];
  point->r[1] = field->y[0];
  point->r[2] = field->z[0];
  ++point;
  point->r[0] = field->x[1];
  point->r[1] = field->y[1];
  point->r[2] = field->z[1];
  ++point;
  point->r[0] = field->x[2];
  point->r[1] = field->y[2];
  point->r[2] = field->z[2];
  ++point;
  point->r[0] = field->x[3];
  point->r[1] = field->y[3];
  point->r[2] = field->z[3];
  config_geometry_read (wg);
#if DEBUG_CONFIG_WINDOW_READ
  printf ("reading furrows\n");
#endif
  wf->nfurrows = field->nfurrows;
  wf->recirculation = field->open;
  wf->initial_conditions = field->initial_conditions;
  furrow = wf->furrow;
  memcpy (furrow->furrow, field->sb, sizeof (Furrow));
  memcpy (furrow->initial_conditions, field->cib, sizeof (InitialConditions));
  if (wf->nfurrows)
    {
      ++furrow;
      memcpy (furrow->furrow, field->si, sizeof (Furrow));
      memcpy (furrow->initial_conditions, field->cii,
              sizeof (InitialConditions));
      if (wf->recirculation)
        {
          ++furrow;
          memcpy (furrow->furrow, field->sc, sizeof (Furrow));
          memcpy (furrow->initial_conditions, field->cic,
                  sizeof (InitialConditions));
        }
    }
  table_config_furrows_read (wf);
#if DEBUG_CONFIG_WINDOW_READ
  printf ("reading times\n");
#endif
  ct->ts = field->tf;
  ct->tm = field->tm;
  ct->cfl = field->cfl;
  ct->c1 = field->nbi;
  ct->c2 = field->ni;
  config_times_read (ct);
#if DEBUG_CONFIG_WINDOW_READ
  printf ("reading fertilizer\n");
#endif
  gtk_spin_button_set_value (cf->spin, w->solubility);
#if DEBUG_CONFIG_WINDOW_WRITE
  printf ("writing probes\n");
#endif
  cp->nProbes = field->nprobes;
  if (field->nprobes)
    {
      size = cp->nProbes * sizeof (Probe);
      cp->probe = (Probe *) realloc (cp->probe, size);
      memcpy (cp->probe, field->probe, size);
    }
  else
    {
      free (cp->probe);
      cp->probe = NULL;
    }
  config_probes_read (cp);
#if DEBUG_CONFIG_WINDOW_READ
  printf ("config_window_read: end\n");
#endif
}

/**
 * Function to write the configuration.
 */
static void
config_window_write (ConfigWindow * w)
                         ///< Field configuration structure.
{
  ConfigIrrigation *ci;
  ConfigGeometry *wg;
  ConfigPoint *point;
  TableConfigFurrows *wf;
  ConfigFurrow *furrow;
  ConfigTimes *ct;
  ConfigFertilizer *cf;
  ConfigProbes *cp;
  Field *field;
  size_t size;
#if DEBUG_CONFIG_WINDOW_WRITE
  printf ("config_window_write: start\n");
#endif
  field = w->field;
  ci = w->irrigation;
  wg = w->geometry;
  wf = w->furrows;
  cf = w->fertilizer;
  ct = w->times;
  cp = w->probes;
#if DEBUG_CONFIG_WINDOW_WRITE
  printf ("writing inputs\n");
#endif
  config_irrigation_write (ci);
  field->nwinputs = ci->n[0];
  field->nsinputs = ci->n[1];
  size = (ci->n[0] + ci->n[1]) * sizeof (Input);
  field->input = (Input *) realloc (field->input, size);
  size = ci->n[0] * sizeof (Input);
  memcpy (field->input, ci->input[0], size);
  size = ci->n[1] * sizeof (Input);
  memcpy (field->input + field->nwinputs, ci->input[1], size);
#if DEBUG_CONFIG_WINDOW_WRITE
  printf ("nfurrows=%u recirculation=%u\n", field->nfurrows, field->open);
  printf ("writing points\n");
#endif
  config_geometry_write (wg);
  point = wg->point;
  field->x[0] = point->r[0];
  field->y[0] = point->r[1];
  field->z[0] = point->r[2];
  ++point;
  field->x[1] = point->r[0];
  field->y[1] = point->r[1];
  field->z[1] = point->r[2];
  ++point;
  field->x[2] = point->r[0];
  field->y[2] = point->r[1];
  field->z[2] = point->r[2];
  ++point;
  field->x[3] = point->r[0];
  field->y[3] = point->r[1];
  field->z[3] = point->r[2];
#if DEBUG_CONFIG_WINDOW_WRITE
  printf ("writing furrows\n");
#endif
  table_config_furrows_write (wf);
  field->nfurrows = wf->nfurrows;
  field->open = wf->recirculation;
  field->initial_conditions = wf->initial_conditions;
  furrow = wf->furrow;
  memcpy (field->sb, furrow->furrow, sizeof (Furrow));
  memcpy (field->cib, furrow->initial_conditions, sizeof (InitialConditions));
  if (wf->nfurrows)
    {
      ++furrow;
      field->si
        = (Furrow *) realloc (field->si, wf->nfurrows * sizeof (Furrow));
      memcpy (field->si, furrow->furrow, sizeof (Furrow));
      memcpy (field->cii, furrow->initial_conditions,
              sizeof (InitialConditions));
      if (wf->recirculation)
        {
          ++furrow;
          memcpy (furrow->furrow, field->sc, sizeof (Furrow));
          memcpy (field->cic, furrow->initial_conditions,
                  sizeof (InitialConditions));
        }
    }
#if DEBUG_CONFIG_WINDOW_WRITE
  printf ("writing times\n");
#endif
  config_times_write (ct);
  field->tf = ct->ts;
  field->tm = ct->tm;
  field->cfl = ct->cfl;
  field->nbi = ct->c1;
  field->ni = ct->c2;
#if DEBUG_CONFIG_WINDOW_WRITE
  printf ("writing fertilizer\n");
#endif
  w->solubility = gtk_spin_button_get_value (cf->spin);
#if DEBUG_CONFIG_WINDOW_WRITE
  printf ("writing probes\n");
#endif
  config_probes_write (cp);
  field->nprobes = cp->nProbes;
  if (field->nprobes)
    {
      size = cp->nProbes * sizeof (Probe);
      field->probe = (Probe *) realloc (field->probe, size);
      memcpy (field->probe, cp->probe, size);
    }
  else
    {
      free (field->probe);
      field->probe = NULL;
    }
#if DEBUG_CONFIG_WINDOW_WRITE
  printf ("config_window_write: end\n");
#endif
}

/**
 * Function to close the dialog to open the problem configuration.
 */
static void
config_window_new_close (GtkDialog * dlg,
                         ///< Dialog to open the problem configuration. 
                         int response_id)       ///< Response identifier.
{
#if DEBUG_CONFIG_WINDOW_NEW_CLOSE
  printf ("check response\n");
#endif
  ConfigWindow *w;
  w = config_window;
  if (response_id == GTK_RESPONSE_OK)
    {
#if DEBUG_CONFIG_WINDOW_NEW_CLOSE
      printf ("if OK saving the values\n");
#endif
      config_window_write (w);
      solubility = w->solubility;
      field_destroy (field);
      field_copy (field, w->field);
      main_window->open = 1;
    }
#if DEBUG_CONFIG_WINDOW_NEW_CLOSE
  printf ("destroying dialog\n");
#endif
  gtk_window_destroy (GTK_WINDOW (dlg));
}

/**
 * Function to open the problem configuration dialog.
 */
void
config_window_new ()
{
  ConfigWindow *w;
#if GTK4
  char buffer[512];
  GtkLabel *label;
#endif

#if DEBUG_CONFIG_WINDOW_NEW
  printf ("config_window_new: start\n");
  printf ("creating the irrigation table\n");
#endif

  w = config_window;
  config_irrigation_new (w->irrigation);

#if DEBUG_CONFIG_WINDOW_NEW
  printf ("creating the geometry table\n");
#endif
  config_geometry_new (w->geometry);

#if DEBUG_CONFIG_WINDOW_NEW
  printf ("creating the furrows table\n");
#endif
  table_config_furrows_new (w->furrows);

#if DEBUG_CONFIG_WINDOW_NEW
  printf ("creating the times table\n");
#endif
  config_times_new (w->times);

#if DEBUG_CONFIG_WINDOW_NEW
  printf ("creating the fertilizer table\n");
#endif
  config_fertilizer_new (w->fertilizer);

#if DEBUG_CONFIG_WINDOW_NEW
  printf ("creating the probes table\n");
#endif
  config_probes_new (w->probes);

#if DEBUG_CONFIG_WINDOW_NEW
  printf ("creating the notebook\n");
#endif
  w->notebook = (GtkNotebook *) gtk_notebook_new ();
  gtk_notebook_append_page (w->notebook, GTK_WIDGET (w->geometry->grid),
                            gtk_label_new (_("Geometry")));
  gtk_notebook_append_page (w->notebook, GTK_WIDGET (w->furrows->grid),
                            gtk_label_new (_("Furrows")));
  gtk_notebook_append_page (w->notebook, GTK_WIDGET (w->irrigation->box),
                            gtk_label_new (_("Inputs")));
  gtk_notebook_append_page (w->notebook, GTK_WIDGET (w->fertilizer->box),
                            gtk_label_new (_("Fertilizer")));
  gtk_notebook_append_page (w->notebook, GTK_WIDGET (w->probes->vbox),
                            gtk_label_new (_("Probes")));
  gtk_notebook_append_page (w->notebook, GTK_WIDGET (w->times->grid),
                            gtk_label_new (_("Advanced parameters")));

#if DEBUG_CONFIG_WINDOW_NEW
  printf ("creating the title\n");
#endif
  w->bar = (GtkHeaderBar *) gtk_header_bar_new ();
#if GTK4
  label = (GtkLabel *) gtk_label_new (NULL);
  snprintf (buffer, 512, "<b>%s</b>\n%s", _("Configure irrigation"),
            _("Set the irrigation problem configuration"));
  gtk_label_set_markup (label, buffer);
  gtk_header_bar_set_title_widget (w->bar, GTK_WIDGET (label));
  gtk_header_bar_set_show_title_buttons (w->bar, 1);
#else
  gtk_header_bar_set_title (w->bar, _("Configure irrigation"));
  gtk_header_bar_set_subtitle (w->bar,
                               _("Set the irrigation problem configuration"));
  gtk_header_bar_set_show_close_button (w->bar, 1);
#endif
  w->logo = (GtkPicture *) gtk_picture_new_for_filename ("logo3.png");
  gtk_header_bar_pack_start (w->bar, GTK_WIDGET (w->logo));

#if DEBUG_CONFIG_WINDOW_NEW
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
  g_signal_connect (w->dialog, "response", G_CALLBACK (config_window_new_close),
                    NULL);
#if GTK4
  gtk_box_append (GTK_BOX (gtk_dialog_get_content_area (w->dialog)),
                  GTK_WIDGET (w->notebook));
  gtk_window_present (GTK_WINDOW (w->dialog));
#else
  gtk_container_add (GTK_CONTAINER (gtk_dialog_get_content_area (w->dialog)),
                     GTK_WIDGET (w->notebook));
  gtk_widget_show_all (GTK_WIDGET (w->dialog));
#endif

#if DEBUG_CONFIG_WINDOW_NEW
  printf ("reading\n");
#endif
  w->solubility = solubility;
  field_copy (w->field, field);
  config_window_read (w);
  table_config_furrows_update (w->furrows);

#if DEBUG_CONFIG_WINDOW_NEW
  printf ("config_window_new: end\n");
#endif
}
