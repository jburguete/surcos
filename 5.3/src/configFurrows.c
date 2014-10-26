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
 * \file configFurrows.c
 * \brief Source file to define the configuration of the furrows data.
 * \authors Javier Burguete Tolosa, Asier Lacasta Soto.
 * \copyright Copyright 2011-2014, Javier Burguete Tolosa.
 */
#define _GNU_SOURCE
#include <libintl.h>
#include <glib/gstdio.h>
#include <gtk/gtk.h>
#include "configFurrows.h"

/**
 * \fn void config_furrow_set_sensitive(ConfigFurrow *w, int sensitive)
 * \brief Function to set configurable a furrow.
 * \param w
 * \brief Furrow configuration structure.
 * \param sensitive
 * \brief 1 on configurable, 0 on no configurable.
 */
void config_furrow_set_sensitive(ConfigFurrow *w, int sensitive)
{
	int i;
	gtk_widget_set_sensitive(GTK_WIDGET(w->button), sensitive);
	for (i=0; i<10; ++i)
		gtk_widget_set_sensitive(GTK_WIDGET(w->spin[i]), sensitive);
}

/**
 * \fn void config_furrow_read(ConfigFurrow *w, FILE *file)
 * \brief Function to read a furrow configuration in a file.
 * \param w
 * \brief Furrow configuration structure.
 * \param file
 * \brief File.
 */
void config_furrow_read(ConfigFurrow *w, FILE *file)
{
	int i;
	double x[11];
	fscanf(file,"%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%d%d%d",
		x, x+1, x+2, x+3, x+4, x+5, x+6, x+7, x+8, x+9, x+10, &i, &i, &i);
	for (i=0; i<10; ++i) gtk_spin_button_set_value(w->spin[i], x[i]);
}

/**
 * \fn void config_furrow_write(ConfigFurrow *w, FILE *file)
 * \brief Function to write a furrow configuration of a file.
 * \param w
 * \brief Furrow configuration structure.
 * \param file
 * \brief File.
 */
void config_furrow_write(ConfigFurrow *w, FILE *file)
{
	int i;
	double x[10];
	for (i=0; i<10; ++i) x[i] = gtk_spin_button_get_value(w->spin[i]);
	fprintf(file,"%g %g %g %g %g %g %g %g %g %g 0 0 0 0\n",
		x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7], x[8], x[9]);
}

/**
 * \fn void config_furrow_new(ConfigFurrow *w, char *label)
 * \brief Function to open a furrow configuration.
 * \param w
 * \brief Furrow configuration structure.
 * \param label
 * \brief Furrow label.
 */
void config_furrow_new(ConfigFurrow *w, char *label)
{
	int i;
	double min[10]={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
	double max[10]={100.,100.,10.,100.,10.,1.,1.,1e6,1.,1.};
	double step[10]={0.01,0.01,0.01,0.01,0.001,0.001,0.001,1e-5,0.01,1e-7};

	w->button = (GtkButton*)gtk_button_new_with_label(label);
	gtk_widget_set_can_focus(GTK_WIDGET(w->button), FALSE);
	for (i=0; i<10; ++i)
		w->spin[i] = (GtkSpinButton*)gtk_spin_button_new_with_range
			(min[i], max[i], step[i]);
}

/**
 * \fn void table_config_furrows_update(TableConfigFurrows *w)
 * \brief Function to update a field furrows configuration.
 * \param w
 * \brief Field furrows configuration structure.
 */
void table_config_furrows_update(TableConfigFurrows *w)
{
	int i, nfurrows;
	nfurrows = gtk_spin_button_get_value_as_int(w->spin);
	// Sensitive if they are recirculation furrow
	i=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w->button_recirculation));
	config_furrow_set_sensitive(w->furrow + 2, i);
	// Sensitive if they are irrigation furrows
	config_furrow_set_sensitive(w->furrow + 1, (nfurrows > 0)? 1 : 0);
}

/**
 * \fn void table_config_furrows_new(TableConfigFurrows *w)
 * \brief Function to open a field furrows configuration.
 * \param w
 * \brief Field furrows configuration structure.
 */
void table_config_furrows_new(TableConfigFurrows *w)
{
	int i, j;
	char *label_channel[3]=
	{
		gettext("Distribution"),
		gettext("Irrigation"),
		gettext("Recirculation")
	};
	char *label_coefficient[10]=
	{
		"B (m)",
		"Z",
		"H (m)",
		"W (m)",
		"R (m)",
		"e",
		"n",
		"K (m/s^a)",
		"a",
		"f0 (m/s)",
	};
	for (i=0; i<3; ++i) config_furrow_new(w->furrow + i, label_channel[i]);
	w->table = (GtkGrid*)gtk_grid_new();
	w->box_image = (GtkGrid*)gtk_grid_new();
	w->image = (GtkImage*)gtk_image_new_from_file("diagram_furrow.png");
	gtk_grid_attach(w->box_image, GTK_WIDGET(w->image), 0, 0, 1, 1);
	w->label_models = (GtkLabel*)gtk_label_new
		(gettext("R: water retention capacity\n"
			"e=0: Gauckler-Manning's model\n"
			"e>0: Burguete's aerodynamical coefficient\n"
			"n(e=0): Gauckler-Manning's number\n"
			"n(e>0): Burguete's characteristic roughness length\n"
			"K: Kostiakov's constant\n"
			"a: Kostiakov's exponent\n"
			"f0: infiltration velocity in saturated soil"));
	gtk_grid_attach(w->box_image, GTK_WIDGET(w->label_models), 1, 0, 1, 1);
	gtk_grid_attach(w->table, GTK_WIDGET(w->box_image), 0, 0, 4, 1);
	w->label_furrow = (GtkLabel*)gtk_label_new
		(gettext("Number of irrigation furrows"));
	w->spin = (GtkSpinButton*)gtk_spin_button_new_with_range(0., 1000., 1.);
	w->button_recirculation = (GtkCheckButton*)gtk_check_button_new_with_label
		(gettext("Recirculation"));
	w->box_furrows = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach(w->box_furrows, GTK_WIDGET(w->label_furrow), 0, 0, 1, 1);
	gtk_grid_attach(w->box_furrows, GTK_WIDGET(w->spin), 1, 0, 1, 1);
	gtk_grid_attach(w->box_furrows, GTK_WIDGET(w->button_recirculation),
		2, 0, 1, 1);
	gtk_grid_attach(w->table, GTK_WIDGET(w->box_furrows), 0, 1, 4, 1);
	w->button_furrow = (GtkButton*)gtk_button_new_with_label(gettext("Furrow"));
	gtk_widget_set_can_focus(GTK_WIDGET(w->button_furrow), FALSE);
	gtk_grid_attach(w->table, GTK_WIDGET(w->button_furrow), 2, 2, 3, 1);
	w->button_geometry = (GtkButton*)gtk_button_new_with_label
		(gettext("Geometry"));
	gtk_widget_set_can_focus(GTK_WIDGET(w->button_geometry), FALSE);
	gtk_grid_attach(w->table, GTK_WIDGET(w->button_geometry), 0, 4, 1, 4);
	w->button_soil = (GtkButton*)gtk_button_new_with_label(gettext("Soil"));
	gtk_widget_set_can_focus(GTK_WIDGET(w->button_soil), FALSE);
	gtk_grid_attach(w->table, GTK_WIDGET(w->button_soil), 0, 8, 1, 1);
	w->button_friction = (GtkButton*)gtk_button_new_with_label
		(gettext("Friction model"));
	gtk_widget_set_can_focus(GTK_WIDGET(w->button_friction), FALSE);
	gtk_grid_attach(w->table, GTK_WIDGET(w->button_friction), 0, 9, 1, 2);
	w->button_infiltration = (GtkButton*)gtk_button_new_with_label
		(gettext("Infiltration model"));
	gtk_widget_set_can_focus(GTK_WIDGET(w->button_infiltration), FALSE);
	gtk_grid_attach(w->table, GTK_WIDGET(w->button_infiltration), 0, 11, 1, 3);
	for (i = 0; i < 10; ++i)
	{
		w->button_coefficient[i] =
			(GtkButton*)gtk_button_new_with_label(label_coefficient[i]);
		gtk_widget_set_can_focus(GTK_WIDGET(w->button_coefficient[i]), FALSE);
		gtk_grid_attach(w->table, GTK_WIDGET(w->button_coefficient[i]),
			1, 4 + i, 1, 1);
	}
	for (j = 0; j < 3; ++j)
	{
		gtk_grid_attach(w->table, GTK_WIDGET(w->furrow[j].button),
			2 + j, 3, 1, 1);
		for (i = 0; i < 10; ++i)
			gtk_grid_attach(w->table, GTK_WIDGET(w->furrow[j].spin[i]),
				j + 2, i + 4, 1, 1);
	}
	gtk_grid_attach(w->table, gtk_label_new(NULL), 0, 7, 11, 1);
	g_signal_connect_swapped(w->spin, "changed",
		(GCallback)table_config_furrows_update, w);
	g_signal_connect_swapped(w->button_recirculation, "clicked",
		(GCallback)table_config_furrows_update, w);
}
