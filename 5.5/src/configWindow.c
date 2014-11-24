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
 * \file configWindow.c
 * \brief Source file to define the configuration of the problem.
 * \authors Javier Burguete Tolosa, Asier Lacasta Soto.
 * \copyright Copyright 2011-2014, Javier Burguete Tolosa.
 */
#define _GNU_SOURCE
#include <libintl.h>
#include <glib/gstdio.h>
#include <gtk/gtk.h>
#include "config.h"
#include "configIrrigation.h"
#include "configGeometry.h"
#include "configFurrows.h"
#include "configProbes.h"
#include "configTimes.h"
#include "configWindow.h"

/**
 * \def DEBUG_WINDOW_CONFIG_SET_READ
 * \brief Macro to debug the window_config_set_read() function.
 */
#define DEBUG_WINDOW_CONFIG_SET_READ 0

/**
 * \def DEBUG_WINDOW_CONFIG_SET_WRITE
 * \brief Macro to debug the window_config_set_write() function.
 */
#define DEBUG_WINDOW_CONFIG_SET_WRITE 0

/**
 * \def DEBUG_WINDOW_CONFIG_NEW
 * \brief Macro to debug the window_config_new() function.
 */
#define DEBUG_WINDOW_CONFIG_NEW 0

extern int plot_show;
extern char *input_dir;
extern GtkWidget *windowMain;

/**
 * \fn void config_fertilizer_new(ConfigFertilizer *cf)
 * \brief Function to open the fertilizer configuration.
 * \param cf
 * \brief Fertilizer configuration structure.
 */
void config_fertilizer_new(ConfigFertilizer *cf)
{
	cf->box = (GtkGrid*)gtk_grid_new();
	cf->label = (GtkLabel*)gtk_label_new(gettext("Solubility (kg/m³)"));
	gtk_grid_attach(cf->box, GTK_WIDGET(cf->label), 0, 0, 1, 1);
	cf->spin = (GtkSpinButton*)gtk_spin_button_new_with_range
		(FIELD_SOLUBILITY_MIN, FIELD_SOLUBILITY_MAX, 0.001);
	gtk_grid_attach(cf->box, GTK_WIDGET(cf->spin), 1, 0, 1, 1);
}

/**
 * \fn void window_config_set_read\
 * 	(ConfigGeometry *wg, TableConfigFurrows *wf, ConfigFertilizer *cf)
 * \brief Function to read the configuration.
 * \param wg
 * \brief Field geometry configuration structure.
 * \param wf
 * \brief Field furrows configuration structure.
 * \param cf
 * \brief Fertilizer configuration structure.
 */
void window_config_set_read
	(ConfigGeometry *wg, TableConfigFurrows *wf, ConfigFertilizer *cf)
{
	int i, nfurrows, recirculation;
	double solubility;
	char buffer[512];
	FILE *f;
#if DEBUG_WINDOW_CONFIG_SET_READ
printf("window_config_set_read: start\n");
#endif
	snprintf(buffer, 512, "%s/field.in", input_dir);
	f = g_fopen(buffer, "r");
	fscanf(f, "%d%d%lg", &recirculation, &nfurrows, &solubility);
#if DEBUG_WINDOW_CONFIG_SET_READ
printf("nfurrows=%d recirculation=%d\n", nfurrows, recirculation);
#endif
	gtk_spin_button_set_value(wf->spin, nfurrows);
	gtk_spin_button_set_value(cf->spin, solubility);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wf->button_recirculation),
		recirculation);
#if DEBUG_WINDOW_CONFIG_SET_READ
printf("reading points\n");
#endif
	for (i=0; i<4; ++i) config_point_read(wg->point + i, f);
#if DEBUG_WINDOW_CONFIG_SET_READ
printf("reading furrows\n");
#endif
	for (i=0; i<3; ++i) config_furrow_read(wf->furrow + i, f);
	fclose(f);
#if DEBUG_WINDOW_CONFIG_SET_READ
printf("window_config_set_read: end\n");
#endif
}

/**
 * \fn void window_config_set_write\
 * 	(ConfigGeometry *wg, TableConfigFurrows *wf, ConfigFertilizer *cf)
 * \brief Function to write the configuration.
 * \param wg
 * \brief Field geometry configuration structure.
 * \param wf
 * \brief Field furrows configuration structure.
 * \param cf
 * \brief Fertilizer configuration structure.
 */
void window_config_set_write
	(ConfigGeometry *wg, TableConfigFurrows *wf, ConfigFertilizer *cf)
{
	int i, nfurrows, recirculation;
	double solubility;
	char buffer[512];
	FILE *f;
#if DEBUG_WINDOW_CONFIG_SET_WRITE
printf("window_config_set_write: start\n");
#endif
	snprintf(buffer, 512, "%s/field.in", input_dir);
	f = g_fopen(buffer, "w");
	nfurrows = gtk_spin_button_get_value_as_int(wf->spin);
	solubility = gtk_spin_button_get_value(cf->spin);
	recirculation = gtk_toggle_button_get_active
		(GTK_TOGGLE_BUTTON(wf->button_recirculation));
#if DEBUG_WINDOW_CONFIG_SET_WRITE
printf("nfurrows=%d recirculation=%d\n", nfurrows, recirculation);
#endif
	fprintf(f, "%d\n%d\n%g\n", recirculation, nfurrows, solubility);
#if DEBUG_WINDOW_CONFIG_SET_WRITE
printf("writing points\n");
#endif
	for (i=0; i<4; ++i) config_point_write(wg->point + i, f);
#if DEBUG_WINDOW_CONFIG_SET_WRITE
printf("writing furrows\n");
#endif
	for (i=0; i<3; ++i) config_furrow_write(wf->furrow + i, f);
	fclose(f);
#if DEBUG_WINDOW_CONFIG_SET_WRITE
printf("window_config_set_write: end\n");
#endif
}

/**
 * \fn void window_config_new()
 * \brief Function to open the problem configuration dialog.
 */
void window_config_new()
{
	WindowConfig w[1];

#if DEBUG_WINDOW_CONFIG_NEW
printf("window_config_new: start\n");
printf("creating the irrigation table\n");
#endif
	config_irrigation_new(w->irrigation);

#if DEBUG_WINDOW_CONFIG_NEW
printf("creating the geometry table\n");
#endif
	config_geometry_new(w->geometry);

#if DEBUG_WINDOW_CONFIG_NEW
printf("creating the furrows table\n");
#endif
	table_config_furrows_new(w->furrows);

#if DEBUG_WINDOW_CONFIG_NEW
printf("creating the times table\n");
#endif
	config_times_new(w->times);

#if DEBUG_WINDOW_CONFIG_NEW
printf("creating the fertilizer table\n");
#endif
	config_fertilizer_new(w->fertilizer);

#if DEBUG_WINDOW_CONFIG_NEW
printf("creating the probes table\n");
#endif
	config_probes_new(w->probes);

#if DEBUG_WINDOW_CONFIG_NEW
printf("creating the notebook\n");
#endif
	w->notebook = (GtkNotebook*)gtk_notebook_new();
	gtk_notebook_append_page(w->notebook, GTK_WIDGET(w->geometry->table),
		gtk_label_new(gettext("Geometry")));
	gtk_notebook_append_page(w->notebook, GTK_WIDGET(w->furrows->table),
		gtk_label_new(gettext("Furrows")));
	gtk_notebook_append_page(w->notebook, GTK_WIDGET(w->irrigation->box),
		gtk_label_new(gettext("Inputs")));
	gtk_notebook_append_page(w->notebook, GTK_WIDGET(w->fertilizer->box),
		gtk_label_new(gettext("Fertilizer")));
	gtk_notebook_append_page(w->notebook, GTK_WIDGET(w->probes->vbox),
		gtk_label_new(gettext("Probes")));
	gtk_notebook_append_page(w->notebook, GTK_WIDGET(w->times->table),
		gtk_label_new(gettext("Advanced parameters")));

#if DEBUG_WINDOW_CONFIG_NEW
printf("creating the dialog\n");
#endif
	w->dialog = (GtkDialog*)gtk_dialog_new_with_buttons(
		gettext("Configure irrigation"),
		GTK_WINDOW(windowMain),
		GTK_DIALOG_MODAL,
		gettext("_Cancel"), GTK_RESPONSE_CANCEL,
		gettext("_OK"), GTK_RESPONSE_OK,
		NULL);
	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(w->dialog)),
		GTK_WIDGET(w->notebook));
	gtk_widget_show_all(GTK_WIDGET(w->dialog));

#if DEBUG_WINDOW_CONFIG_NEW
printf("reading\n");
#endif
	window_config_set_read(w->geometry, w->furrows, w->fertilizer);

#if DEBUG_WINDOW_CONFIG_NEW
printf("dialog bucle\n");
#endif
	if (gtk_dialog_run(w->dialog) == GTK_RESPONSE_OK)
	{
#if DEBUG_WINDOW_CONFIG_NEW
printf("if OK saving the values\n");
#endif
		config_irrigation_write(w->irrigation);
		window_config_set_write(w->geometry, w->furrows, w->fertilizer);
		config_times_write(w->times);
		config_probes_write(w->probes);
		plot_show = 0;
	}

#if DEBUG_WINDOW_CONFIG_NEW
printf("destroying dialog\n");
#endif
	gtk_widget_destroy(GTK_WIDGET(w->dialog));

#if DEBUG_WINDOW_CONFIG_NEW
printf("window_config_new: end\n");
#endif
}
