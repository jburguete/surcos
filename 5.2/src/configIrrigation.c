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
 * \file configIrrigation.c
 * \brief Source file to define the configuration of the irrigation.
 * \authors Javier Burguete Tolosa, Asier Lacasta Soto.
 * \copyright Copyright 2011-2013, Javier Burguete Tolosa.
 */
#define _GNU_SOURCE
#include <libintl.h>
#include <glib/gstdio.h>
#include <gtk/gtk.h>
#include "configIrrigation.h"

/**
 * \def DEBUG_INPUT_UPDATE
 * \brief Macro to debug the config_irrigation_new() function
 */
#define DEBUG_INPUT_UPDATE 0

/**
 * \def DEBUG_CONFIG_IRRIGATION_READ
 * \brief Macro to debug the config_irrigation_read() function
 */
#define DEBUG_CONFIG_IRRIGATION_READ 0

/**
 * \def DEBUG_CONFIG_IRRIGATION_NEW
 * \brief Macro to debug the config_irrigation_new() function
 */
#define DEBUG_CONFIG_IRRIGATION_NEW 0

extern char *input_dir;

/**
 * \fn void input_insert\
 * 	(GList **list, int nInputsOld, int nInputs, GtkGrid *table)
 * \brief Function to insert inputs.
 * \param list
 * \brief List of inputs.
 * \param nInputsOld
 * \brief Old inputs number.
 * \param nInputs
 * \brief New inputs number.
 * \param table
 * \brief configuration GtkGrid.
 */
void input_insert(GList **list, int nInputsOld, int nInputs, GtkGrid *table)
{
	int i, j;
	char buffer[32];
	double min[5]={-1e6, -1e6, 0., 0., 0.};
	double max[5]={1e6, 1e6, 1e6, 1e6, 1.};
	double step[5]={0.01, 0.01, 1., 1., 1e-6};
	GtkWidget *widget;
	for (i = nInputsOld; i < nInputs; ++i)
	{
		snprintf(buffer, 32, "%d", i+1); 
		widget = gtk_button_new_with_label(buffer);
		gtk_widget_set_can_focus(widget, FALSE);
		gtk_widget_show(widget);
		gtk_grid_attach(table, widget, 0, i + 1, 1, 1);
		*list = g_list_append(*list, widget);
		for (j = 0; j < 5; ++j)
		{
			widget = gtk_spin_button_new_with_range(min[j], max[j], step[j]);
			gtk_widget_show(widget);
			gtk_grid_attach(table, widget, j + 1, i + 1, 1, 1);
			*list = g_list_append(*list, widget);
		}
	}
}

/**
 * \fn void input_remove(GList **list, int nInputsOld, int nInputs)
 * \brief Function to remove inputs.
 * \param list
 * \brief List of inputs.
 * \param nInputsOld
 * \brief Old inputs number.
 * \param nInputs
 * \brief New inputs number.
 */
void input_remove(GList **list, int nInputsOld, int nInputs)
{
	int i, j;
	GList *element, *next;
	element = g_list_nth(*list, 6 * nInputs);
	for (i = nInputs; i < nInputsOld; ++i)
	{
		for (j=0; j<6; ++j)
		{
			next = element->next;
			gtk_widget_destroy(GTK_WIDGET(element->data));
			*list = g_list_remove_link(*list, element);
			element = next;
		}
	}
}

/**
 * \fn void input_update\
 * 	(GList **list, int *nInputs, GtkSpinButton *spin, GtkGrid *table)
 * \brief Function to update the inputs number.
 * \param list
 * \brief List of inputs.
 * \param nInputs
 * \brief New inputs number.
 * \param spin
 * \brief GtkSpinButton defining the inputs number.
 * \param table
 * \brief configuration GtkGrid.
 */
void input_update
(GList **list, int *nInputs, GtkSpinButton *spin, GtkGrid *table)
{
	int nInputsOld;
	nInputsOld = *nInputs;
#if DEBUG_INPUT_UPDATE
printf("input_update: start\n");
#endif
	*nInputs = gtk_spin_button_get_value_as_int(spin);
#if DEBUG_INPUT_UPDATE
printf("nInputs = %d\n", *nInputs);
printf("Inserting rows\n");
#endif
	input_insert(list, nInputsOld, *nInputs, table);
#if DEBUG_INPUT_UPDATE
printf("Removing extra rows\n");
#endif
	input_remove(list, nInputsOld, *nInputs);
#if DEBUG_INPUT_UPDATE
printf("input_update: end\n");
#endif
}

/**
 * \fn void config_irrigation_write(ConfigIrrigation *w)
 * \brief Function to save in a file the irrigation configuration.
 * \param w
 * \brief Structure of irrigation configuration.
 */
void config_irrigation_write(ConfigIrrigation *w)
{
	int i, j, k, n[2];
	double x[5];
	char buffer[512];
	FILE* f;
	GList *list;
	snprintf(buffer, 512, "%s/input.in", input_dir);
	f = g_fopen(buffer, "w");
	n[0] = gtk_spin_button_get_value_as_int(w->spin[0]);
	n[1] = gtk_spin_button_get_value_as_int(w->spin[1]);
	fprintf(f, "%d %d\n", n[0], n[1]);
	for (j = 0; j < 2; ++j)
	{
		list = w->list[j];
		for (i = 0; i < n[j]; ++i)
		{
			list = list->next;
			for (k = 0; k < 5; ++k)
			{
				x[k] = gtk_spin_button_get_value(GTK_SPIN_BUTTON(list->data));
				list = list->next;
			}
			fprintf(f, "%g %g %g %g %g\n", x[0], x[1], x[2], x[3], x[4]);
		}
	}
	fclose(f);
}

/**
 * \fn void config_irrigation_read(ConfigIrrigation *w)
 * \brief Function to read in a file the irrigation configuration.
 * \param w
 * \brief Structure of irrigation configuration.
 */
void config_irrigation_read(ConfigIrrigation *w)
{
	int i, j, k;
	double x[5];
	char buffer[512];
	FILE* f;
	GList *list;
#if DEBUG_CONFIG_IRRIGATION_READ
printf("config_irrigation_read: start\n");
#endif
	snprintf(buffer, 512,"%s/input.in", input_dir);
#if DEBUG_CONFIG_IRRIGATION_READ
printf("file = %s\n", buffer);
#endif
	f = g_fopen(buffer, "r");
	if (!f)
	{
//		jbw_show_error(gettext("Unable to open the inputs file"));
#if DEBUG_CONFIG_IRRIGATION_READ
printf("config_irrigation_read: end\n");
#endif
	}
	fscanf(f, "%d%d", w->n, w->n + 1);
	for (j = 0; j < 2; ++j)
	{
#if DEBUG_CONFIG_IRRIGATION_READ
printf("j=%d n=%d\n", j, w->n[j]);
#endif
		gtk_spin_button_set_value(w->spin[j], w->n[j]);
#if DEBUG_CONFIG_IRRIGATION_READ
printf("inserting row\n");
#endif
		input_insert(&w->list[j], 0, w->n[j], w->table[j]);
		list = w->list[j];
		for (i = 0; i < w->n[j]; ++i)
		{
			list = list->next;
			fscanf(f, "%lg%lg%lg%lg%lg", x, x+1, x+2, x+3, x+4);
#if DEBUG_CONFIG_IRRIGATION_READ
printf("x0=%g x1=%g x2=%g x3=%g x4=%g\n", x[0], x[1], x[2], x[3], x[4]);
#endif
			for (k = 0; k < 5; ++k)
			{
				gtk_spin_button_set_value(GTK_SPIN_BUTTON(list->data), x[k]);
				list = list->next;
			}
		}
	}
	fclose(f);
#if DEBUG_CONFIG_IRRIGATION_READ
printf("config_irrigation_read: end\n");
#endif
}

/**
 * \fn void config_irrigation_water(ConfigIrrigation *w)
 * \brief Function to update the irrigation configuration water inputs.
 * \param w
 * \brief Structure of irrigation configuration.
 */
void config_irrigation_water(ConfigIrrigation *w)
{
	input_update(&w->list[0], &w->n[0], w->spin[0], w->table[0]);
}

/**
 * \fn void config_irrigation_fertilizer(ConfigIrrigation *w)
 * \brief Function to update the irrigation configuration fertilizer inputs.
 * \param w
 * \brief Structure of irrigation configuration.
 */
void config_irrigation_fertilizer(ConfigIrrigation *w)
{
	input_update(&w->list[1], &w->n[1], w->spin[1], w->table[1]);
}

/**
 * \fn void config_irrigation_new(ConfigIrrigation *w)
 * \brief Function to open the irrigation configuration table.
 * \param w
 * \brief Structure of irrigation configuration.
 */
void config_irrigation_new(ConfigIrrigation *w)
{
	int i, j;
	char buffer[512];
	char *label1[]=
	{
		gettext("Input"),
		"x (m)",
		"y (m)",
		gettext("Initial time (s)"),
		gettext("Final time (s)"),
		gettext("Discharge (m³/s)")
	};
	char *label2[]=
	{
		gettext("Input"),
		"x (m)",
		"y (m)",
		gettext("Initial time (s)"),
		gettext("Final time (s)"),
		gettext("Discharge (kg/s)")
	};
	char *label_number[2]=
	{
		gettext("Number of water inputs"),
		gettext("Number of fertilizer inputs")
	};
	char *label_frame[2]={gettext("Water"), gettext("Fertilizer")};
	char **label_input[2]={label1, label2};
	double min[2]={1., 0.};
	double max[2]={10., 10.};
	double step[2]={1., 1.};

#if DEBUG_CONFIG_IRRIGATION_NEW
printf("config_irrigation_new: start\n");
printf("creating the boxes\n");
#endif
	w->box = (GtkHBox*)gtk_hbox_new(FALSE, 0);

	w->box_image = (GtkVBox*)gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(w->box), GTK_WIDGET(w->box_image),
		FALSE, FALSE, 0);
	w->image = (GtkImage*)gtk_image_new_from_file("diagram_input.png");
	gtk_box_pack_start(GTK_BOX(w->box_image), GTK_WIDGET(w->image),
		FALSE, FALSE, 0);
	snprintf(buffer, 512, "q: %s\nt0: %s\ntf: %s", gettext("Discharge"),
		gettext("Initial time"), gettext("Final time"));
	w->label_image = (GtkLabel*)gtk_label_new(buffer);
	gtk_box_pack_start(GTK_BOX(w->box_image), GTK_WIDGET(w->label_image),
		FALSE, FALSE, 0);

	w->box_input = (GtkVBox*)gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(w->box), GTK_WIDGET(w->box_input),
		FALSE, FALSE, 0);
	w->box_number = (GtkHBox*)gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(w->box_input), GTK_WIDGET(w->box_number),
		FALSE, FALSE, 0);

#if DEBUG_CONFIG_IRRIGATION_NEW
printf("adding frames\n");
#endif
	for (i = 0; i < 2; ++i)
	{
		w->label[i] = (GtkLabel*)gtk_label_new(label_number[i]);
		gtk_box_pack_start(GTK_BOX(w->box_number), GTK_WIDGET(w->label[i]),
			FALSE, FALSE, 0);
		w->spin[i] = (GtkSpinButton*)gtk_spin_button_new_with_range
			(min[i], max[i], step[i]);
		gtk_box_pack_start(GTK_BOX(w->box_number), GTK_WIDGET(w->spin[i]),
			FALSE, FALSE, 10);
		w->frame[i] = (GtkFrame*)gtk_frame_new(label_frame[i]);
		gtk_box_pack_start(GTK_BOX(w->box_input), GTK_WIDGET(w->frame[i]),
			FALSE, FALSE, 0);
		w->table[i] = (GtkGrid*)gtk_grid_new();
		gtk_container_add(GTK_CONTAINER(w->frame[i]), GTK_WIDGET(w->table[i]));
		for (j = 0; j < 6; ++j)
		{
			w->button[j][i] = (GtkButton*)gtk_button_new_with_label
				(label_input[i][j]);
			gtk_widget_set_can_focus(GTK_WIDGET(w->button[j][i]), FALSE);
			gtk_grid_attach(w->table[i], GTK_WIDGET(w->button[j][i]),
				j, 0, 1, 1);
		}
		w->list[i] = NULL;
	}

	gtk_container_add(GTK_CONTAINER(w->box_input), gtk_label_new(NULL));
	gtk_container_add(GTK_CONTAINER(w->box), gtk_label_new(NULL));

#if DEBUG_CONFIG_IRRIGATION_NEW
printf("setting values\n");
#endif
	config_irrigation_read(w);
	g_signal_connect_swapped(w->spin[0], "value-changed",
		(GCallback)config_irrigation_water, w);
	g_signal_connect_swapped(w->spin[1], "value-changed",
		(GCallback)config_irrigation_fertilizer, w);

#if DEBUG_CONFIG_IRRIGATION_NEW
printf("config_irrigation_new: end\n");
#endif
}
