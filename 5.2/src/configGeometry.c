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
 * \file configGeometry.c
 * \brief Source file to define the configuration of the geometry data.
 * \authors Javier Burguete Tolosa, Asier Lacasta Soto.
 * \copyright Copyright 2011-2013, Javier Burguete Tolosa.
 */
#define _GNU_SOURCE
#include <libintl.h>
#include "configGeometry.h"

/**
 * \def DEBUG_CONFIG_GEOMETRY_NEW
 * \brief Macro to debug the config_geometry_new() function
 */
#define DEBUG_CONFIG_GEOMETRY_NEW 0

extern int plot_show;
extern char *input_dir;
extern GtkWidget *windowMain;

/**
 * \fn void config_point_read(ConfigPoint *w, FILE *file)
 * \brief Function to read a point configuration.
 * \param w
 * \brief Point configuration structure.
 * \param file
 * \brief File.
 */
void config_point_read(ConfigPoint *w, FILE *file)
{
	int i;
	double x[3];
	fscanf(file, "%lf%lf%lf", x, x+1, x+2);
	for (i=0; i<3; ++i) gtk_spin_button_set_value(w->spin[i], x[i]);
}

/**
 * \fn void config_point_write(ConfigPoint *w, FILE *file)
 * \brief Function to write a point configuration.
 * \param w
 * \brief Point configuration structure.
 * \param file
 * \brief File.
 */
void config_point_write(ConfigPoint *w, FILE *file)
{
	int i;
	double x[3];
	for (i=0; i<3; ++i) x[i] = gtk_spin_button_get_value(w->spin[i]);
	fprintf(file,"%g %g %g\n", x[0], x[1], x[2]);
}

/**
 * \fn void config_point_new(ConfigPoint *w, char *label)
 * \brief Function to open a point configuration.
 * \param w
 * \brief Point configuration structure.
 * \param label
 * \brief Label string.
 */
void config_point_new(ConfigPoint *w, char *label)
{
	int i;
	w->button = (GtkButton*)gtk_button_new_with_label(label);
	gtk_widget_set_can_focus(GTK_WIDGET(w->button), FALSE);
	for (i=0; i<2; ++i)
		w->spin[i] = (GtkSpinButton*)gtk_spin_button_new_with_range
			(-1e6, 1e6, 0.01);
	w->spin[2] = (GtkSpinButton*)gtk_spin_button_new_with_range
		(-1e4, 1e4, 0.001);
}

/**
 * \fn void config_geometry_new(ConfigGeometry *w)
 * \brief Function to open a geometry configuration.
 * \param w
 * \brief Geometry configuration structure.
 */
void config_geometry_new(ConfigGeometry *w)
{
	int i, j;
	char *label_id[4]={"1","2","3","4"};
	char *label_point[4]={gettext("Point"),"x (m)","y (m)","z (m)"};
#if DEBUG_CONFIG_GEOMETRY_NEW
printf("config_geometry_new: start\n");
#endif
	for (i = 0; i < 4; ++i) config_point_new(w->point + i, label_id[i]);
	w->table = (GtkGrid*)gtk_grid_new();
	for (i = 0; i < 4; ++i)
	{
		w->button[i] = (GtkButton*)gtk_button_new_with_label(label_point[i]);
		gtk_widget_set_can_focus(GTK_WIDGET(w->button[i]), FALSE);
		gtk_grid_attach(w->table, GTK_WIDGET(w->button[i]), i, 0, 1, 1);
	}
	for (j = 0; j < 4; ++j)
	{
		gtk_grid_attach(w->table, GTK_WIDGET(w->point[j].button),
			0, j + 1, 1, 1);
		for (i = 0; i < 3; ++i)
			gtk_grid_attach(w->table, GTK_WIDGET(w->point[j].spin[i]),
				i + 1, j + 1, 1, 1);
	}
	w->image = (GtkImage*)gtk_image_new_from_file
		(gettext("diagram_geometry_en.png"));
	gtk_grid_attach(w->table, GTK_WIDGET(w->image), 4, 0, 1, 1);
	gtk_grid_attach(w->table, gtk_label_new(NULL), 0, 5, 4, 1);
	gtk_grid_attach(w->table, gtk_label_new(NULL), 5, 0, 1, 6);
	gtk_grid_attach(w->table, gtk_label_new(NULL), 0, 6, 6, 1);
#if DEBUG_CONFIG_GEOMETRY_NEW
printf("config_geometry_new: end\n");
#endif
}
