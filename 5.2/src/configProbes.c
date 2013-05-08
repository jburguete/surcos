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
 * \file configProbes.c
 * \brief Source file to define the configuration of the probes data.
 * \authors Javier Burguete Tolosa, Asier Lacasta Soto.
 * \copyright Copyright 2011-2013, Javier Burguete Tolosa.
 */
#define _GNU_SOURCE
#include <libintl.h>
#include <glib/gstdio.h>
#include <gtk/gtk.h>
#include "configProbes.h"

/**
 * \def DEBUG_CONFIG_PROBES_UPDATE
 * \brief Macro to debug the config_probes_update() function.
 */
#define DEBUG_CONFIG_PROBES_UPDATE 0

/**
 * \def DEBUG_CONFIG_PROBES_READ
 * \brief Macro to debug the config_probes_read() function.
 */
#define DEBUG_CONFIG_PROBES_READ 0

extern char *input_dir;

/**
 * \var nProbes
 * \brief Probes number.
 */
int nProbes;

/**
 * \fn void config_probes_insert(ConfigProbes *w, int nProbesOld, int nProbes)
 * \brief Function to insert probes.
 * \param w
 * \brief Probes configuration structure.
 * \param nProbesOld
 * \brief Old probes number.
 * \param nProbes
 * \brief New probes number.
 */
void config_probes_insert(ConfigProbes *w, int nProbesOld, int nProbes)
{
	int i;
	char buffer[32];
	GtkWidget *widget;
	for (i = nProbesOld; i < nProbes; ++i)
	{
		snprintf(buffer, 32, "%d", i+1); 
		widget = gtk_button_new_with_label(buffer);
		gtk_widget_set_can_focus(widget, FALSE);
		gtk_widget_show(widget);
		gtk_table_attach(w->table, widget, 0, 1, i+1, i+2,
			GTK_FILL, 0, 0, 0);
		w->list = g_list_append(w->list, widget);
		widget = gtk_spin_button_new_with_range(-1e6, 1e6, 0.01);
		gtk_widget_show(widget);
		gtk_table_attach(w->table, widget, 1, 2, i+1, i+2,
			GTK_FILL, 0, 0, 0);
		w->list = g_list_append(w->list, widget);
		widget = gtk_spin_button_new_with_range(-1e6, 1e6, 0.01);
		gtk_widget_show(widget);
		gtk_table_attach(w->table, widget, 2, 3, i+1, i+2,
			GTK_FILL, 0, 0, 0);
		w->list = g_list_append(w->list, widget);
	}
}

/**
 * \fn void config_probes_remove(ConfigProbes *w, int nProbesOld, int nProbes)
 * \brief Function to remove probes.
 * \param w
 * \brief Probes configuration structure.
 * \param nProbesOld
 * \brief Old probes number.
 * \param nProbes
 * \brief New probes number.
 */
void config_probes_remove(ConfigProbes *w, int nProbesOld, int nProbes)
{
	int i, j;
	GList *element, *next;
	element = g_list_nth(w->list, 3 * nProbes);
	for (i=nProbes; i<nProbesOld; ++i)
	{
		for (j=0; j<3; ++j)
		{
			next = element->next;
			gtk_widget_destroy(GTK_WIDGET(element->data));
			w->list = g_list_remove_link(w->list, element);
			element = next;
		}
	}
}

/**
 * \fn void config_probes_update(GtkSpinButton *spin, ConfigProbes *w)
 * \brief Function to update the probes number.
 * \param spin
 * \brief GtkSpinButton configuring the probes number.
 * \param w
 * \brief Probes configuration structure.
 */
void config_probes_update(GtkSpinButton *spin, ConfigProbes *w)
{
	int nProbesOld;
	nProbesOld = nProbes;
#if DEBUG_CONFIG_PROBES_UPDATE
printf("config_probes_update: start\n");
#endif
	nProbes = gtk_spin_button_get_value_as_int(spin);
#if DEBUG_CONFIG_PROBES_UPDATE
printf("nProbes = %d\n", nProbes);
printf("Inserting rows\n");
#endif
	config_probes_insert(w, nProbesOld, nProbes);
#if DEBUG_CONFIG_PROBES_UPDATE
printf("Removing extra rows\n");
#endif
	config_probes_remove(w, nProbesOld, nProbes);
#if DEBUG_CONFIG_PROBES_UPDATE
printf("config_probes_update: end\n");
#endif
}

/**
 * \fn void config_probes_read(ConfigProbes *w)
 * \brief Function to read the probes configuration in a file.
 * \param w
 * \brief Probes configuration structure.
 */
void config_probes_read(ConfigProbes *w)
{
	int i;
	double x, y;
	char buffer[512];
	FILE* f;
	GList *list;
#if DEBUG_CONFIG_PROBES_READ
printf("config_probes_read: start\n");
#endif
	snprintf(buffer, 512, "%s/probe.in", input_dir);
	f = g_fopen(buffer, "r");
	fscanf(f, "%d", &nProbes);
#if DEBUG_CONFIG_PROBES_READ
printf("nProbes = %d\n", nProbes);
#endif
	gtk_spin_button_set_value(w->spin, nProbes);
	config_probes_insert(w, 0, nProbes);
	list = w->list;
	for (i=0; i<nProbes; i++)
	{
#if DEBUG_CONFIG_PROBES_READ
printf("i = %d\n", i);
#endif
		fscanf(f, "%lf%lf", &x, &y);
#if DEBUG_CONFIG_PROBES_READ
printf("Probe %i: x=%f y=%f\n", i, x, y);
#endif
		list = list->next;
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(list->data), x);
		list = list->next;
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(list->data), y);
		list = list->next;
	}
	fclose(f);
#if DEBUG_CONFIG_PROBES_READ
printf("config_probes_read: end\n");
#endif
}

/**
 * \fn void config_probes_write(ConfigProbes *w)
 * \brief Function to write the probes configuration in a file.
 * \param w
 * \brief Probes configuration structure.
 */
void config_probes_write(ConfigProbes *w)
{
	int i;
	double x, y;
	char buffer[512];
	FILE* f;
	GList *list;
	snprintf(buffer, 512, "%s/probe.in", input_dir);
	f = g_fopen(buffer, "w");
	fprintf(f, "%d\n", nProbes);
	list = w->list;
	for(i=0;i<nProbes;i++)
	{
		list = list->next;
		x = gtk_spin_button_get_value(GTK_SPIN_BUTTON(list->data));
		list = list->next;
		y = gtk_spin_button_get_value(GTK_SPIN_BUTTON(list->data));
		list = list->next;
		fprintf(f, "%g %g\n", x, y);
	}
	fclose(f);
}

/**
 * \fn void config_probes_new(ConfigProbes *w)
 * \brief Function to open the probes configuration.
 * \param w
 * \brief Probes configuration structure.
 */
void config_probes_new(ConfigProbes *w)
{
	int i;
	char *label[]={gettext("Probe"), "x (m)", "y (m)"};
	w->list = NULL;
	w->label = (GtkLabel*)gtk_label_new(gettext("Number of probes"));
	w->spin = (GtkSpinButton*)gtk_spin_button_new_with_range(0., 100., 1.);
	w->hbox = (GtkHBox*)gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(w->hbox), GTK_WIDGET(w->label), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(w->hbox), GTK_WIDGET(w->spin), FALSE, FALSE, 0);
	w->table = (GtkTable*)gtk_table_new(0, 0, FALSE);
	for (i=0; i<3; ++i)
	{
		w->button[i] = (GtkButton*)gtk_button_new_with_label(label[i]);
		gtk_widget_set_can_focus(GTK_WIDGET(w->button[i]), FALSE);
		gtk_table_attach(w->table, GTK_WIDGET(w->button[i]),
			i, i+1, 0, 1, GTK_FILL, 0, 0, 0);
	}
	w->vbox = (GtkVBox*)gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(w->vbox), GTK_WIDGET(w->hbox), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(w->vbox), GTK_WIDGET(w->table), FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(w->vbox), gtk_label_new(NULL));

	config_probes_read(w);
	g_signal_connect_after(w->spin, "value-changed",
		(GCallback)config_probes_update, w);
}
