/*
SURCOS - A program to simulate furrow fertigation.

Copyright 2011-2012, Javier Burguete Tolosa.

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
 * \file mainWindow.c
 * \brief Source file to define the main window.
 * \authors Javier Burguete Tolosa, Asier Lacasta Soto.
 * \copyright Copyright 2011-2012, Javier Burguete Tolosa.
 */

#include "kernel.h"
#include "configWindow.h"
#include "graphics.h"
#include "mainWindow.h"

/**
 * \def DEBUG_PLOT_CREATE
 * \brief Macro to debug the plot_create() function.
 */
#define DEBUG_PLOT_CREATE 0

/**
 * \def DEBUG_MAIN_WINDOW_NEW
 * \brief Macro to debug the main_window_new() function.
 */
#define DEBUG_MAIN_WINDOW_NEW 0

extern GtkWidget *windowMain;

/**
 * \var input_dir
 * \brief Directory where the input data files are located.
 * \var plot_show
 * \brief 1 on showing the results plot, 0 on else.
 */
char *input_dir = NULL;
int plot_show = 0;

/**
 * \fn void text_corner(GtkTextBuffer *text, GtkTextIter *iter, int corner,\
 * 	double x, double y, double z)
 * \brief Function to show field corner point characteristics.
 * \param text
 * \brief GtkTextBuffer text to write the field corner point characteristics.
 * \param iter
 * \brief GtkTextIter to write the field corner point characteristics.
 * \param corner
 * \brief Field corner point number.
 * \param x
 * \brief Field corner point x-coordinate.
 * \param y
 * \brief Field corner point y-coordinate.
 * \param z
 * \brief Field corner point z-coordinate.
 */
void text_corner(GtkTextBuffer *text, GtkTextIter *iter, int corner,
	double x, double y, double z)
{
	char buffer[512];
	snprintf(buffer, 512, "\t%s %d\n", gettext("Coordinates of corner"),
		corner);
	gtk_text_buffer_insert_with_tags_by_name(text, iter, buffer, -1,
		"bold", NULL);
	snprintf(buffer, 512, "\t\tx = %lg m\n", x);
	gtk_text_buffer_insert(text, iter, buffer, -1);
	snprintf(buffer, 512, "\t\ty = %lg m\n", y);
	gtk_text_buffer_insert(text, iter, buffer, -1);
	snprintf(buffer, 512, "\t\tz = %lg m\n", z);
	gtk_text_buffer_insert(text, iter, buffer, -1);
}

/**
 * \fn void text_furrow\
 * 	(GtkTextBuffer *text, GtkTextIter *iter, char *label, Furrow *furrow)
 * \brief Function to show furrow characteristics.
 * \param text
 * \brief GtkTextBuffer text to write the furrow characteristics.
 * \param iter
 * \brief GtkTextIter to write the furrow characteristics.
 * \param label
 * \brief Furrow label.
 * \param furrow
 * \brief Furrow structure.
 */
void text_furrow
	(GtkTextBuffer *text, GtkTextIter *iter, char *label, Furrow *furrow)
{
	char buffer[512];
	snprintf(buffer, 512, "\t%s\n", label);
	gtk_text_buffer_insert_with_tags_by_name(text, iter, buffer, -1,
		"bold", NULL);
	snprintf(buffer, 512, "\t\t%s = %lg m\n", gettext("Base width"), furrow->b);
	gtk_text_buffer_insert(text, iter, buffer, -1);
	snprintf(buffer, 512, "\t\t%s = %lg\n", gettext("Lateral slope"),
		furrow->z);
	gtk_text_buffer_insert(text, iter, buffer, -1);
	snprintf(buffer, 512, "\t\t%s = %lg m\n", gettext("Depth"), furrow->h);
	gtk_text_buffer_insert(text, iter, buffer, -1);
	snprintf(buffer, 512, "\t\t%s = %lg m\n", gettext("Distance"), furrow->D);
	gtk_text_buffer_insert(text, iter, buffer, -1);
	snprintf(buffer, 512, "\t\t%s = %lg m\n",
		gettext("Soil retention capacity"), furrow->H);
	gtk_text_buffer_insert(text, iter, buffer, -1);
	if (furrow->epsilon > 0.)
	{
		snprintf(buffer, 512, "\t\t%s = %s\n", gettext("Friction model"),
			"Burguete");
		gtk_text_buffer_insert(text, iter, buffer, -1);
		snprintf(buffer, 512, "\t\t%s = %lg\n",
			gettext("Aerodynamical coefficient"), furrow->epsilon);
		gtk_text_buffer_insert(text, iter, buffer, -1);
		snprintf(buffer, 512, "\t\t%s = %lg m\n",
			gettext("Characteristic roughness length"), furrow->n);
		gtk_text_buffer_insert(text, iter, buffer, -1);
	}
	else
	{
		snprintf(buffer, 512, "\t\t%s = %s\n", gettext("Friction model"),
			"Gauckler-Manning");
		gtk_text_buffer_insert(text, iter, buffer, -1);
		snprintf(buffer, 512, "\t\t%s = %lg s m",
			gettext("Number of Gauckler-Manning"), furrow->n);
		gtk_text_buffer_insert(text, iter, buffer, -1);
		gtk_text_buffer_insert_with_tags_by_name(text, iter, "-1/3", -1,
			"superscript", NULL);
		gtk_text_buffer_insert(text, iter, "\n", -1);
	}
	snprintf(buffer, 512, "\t\t%s = %lg m·s",
		gettext("Kostiakov's coefficient"), furrow->i1);
	gtk_text_buffer_insert(text, iter, buffer, -1);
	snprintf(buffer, 512, "-%lg", furrow->i2);
	gtk_text_buffer_insert_with_tags_by_name(text, iter, buffer, -1,
		"superscript", NULL);
	gtk_text_buffer_insert(text, iter, "\n", -1);
	snprintf(buffer, 512, "\t\t%s = %lg\n", gettext("Kostiakov's exponent"),
		furrow->i2);
	gtk_text_buffer_insert(text, iter, buffer, -1);
	snprintf(buffer, 512, "\t\t%s = %lg m/s\n",
		gettext("Infiltration velocity in saturated soil"), furrow->i3);
	gtk_text_buffer_insert(text, iter, buffer, -1);
}

/**
 * \fn void text_input\
 * 	(GtkTextBuffer *text, GtkTextIter *iter, int type, int i, Input *input)
 * \brief Function to show input irrigation characteristics.
 * \param text
 * \brief GtkTextBuffer text to write the input irrigation characteristics.
 * \param iter
 * \brief GtkTextIter to write the input irrigation characteristics.
 * \param type
 * \brief Input irrigation type.
 * \param i
 * \brief Input irrigation number.
 * \param input
 * \brief Input irrigation structure.
 */
void text_input
	(GtkTextBuffer *text, GtkTextIter *iter, int type, int i, Input *input)
{
	char buffer[512], *label;
	if (type) label = gettext("Fertilizer input");
	else label = gettext("Water input");
	snprintf(buffer, 512, "\t%s %d\n", label, i);
	gtk_text_buffer_insert_with_tags_by_name(text, iter, buffer, -1,
		"bold", NULL);
	snprintf(buffer, 512, "\t\tx = %lg m\n", input->x);
	gtk_text_buffer_insert(text, iter, buffer, -1);
	snprintf(buffer, 512, "\t\ty = %lg m\n", input->y);
	gtk_text_buffer_insert(text, iter, buffer, -1);
	snprintf(buffer, 512, "\t\t%s = %lg s\n", gettext("Initial time"),
		input->t0);
	gtk_text_buffer_insert(text, iter, buffer, -1);
	snprintf(buffer, 512, "\t\t%s = %lg s\n", gettext("Final time"), input->tf);
	if (type) snprintf(buffer, 512, "\t\t%s = %lg kg/s\n", gettext("Discharge"),
		input->q);
	else snprintf(buffer, 512, "\t\t%s = %lg m\302\263/s\n",
		gettext("Discharge"), input->q);
	gtk_text_buffer_insert(text, iter, buffer, -1);
}

/**
 * \fn void text_probe\
 * 	(GtkTextBuffer *text, GtkTextIter *iter, int i, Probe *probe)
 * \brief Function to show probe characteristics.
 * \param text
 * \brief GtkTextBuffer text to write the probe characteristics.
 * \param iter
 * \brief GtkTextIter to write the probe characteristics.
 * \param i
 * \brief Probe number.
 * \param probe
 * \brief Probe structure.
 */
void text_probe
	(GtkTextBuffer *text, GtkTextIter *iter, int i, Probe *probe)
{
	char buffer[512];
	snprintf(buffer, 512, "\t%s %d\n", gettext("Probe"), i);
	gtk_text_buffer_insert_with_tags_by_name(text, iter, buffer, -1,
		"bold", NULL);
	snprintf(buffer, 512, "\t\tx = %lg m\n", probe->x);
	gtk_text_buffer_insert(text, iter, buffer, -1);
	snprintf(buffer, 512, "\t\ty = %lg m\n", probe->y);
	gtk_text_buffer_insert(text, iter, buffer, -1);
}

/**
 * \fn void text_mass\
 * 	(GtkTextBuffer *text, GtkTextIter *iter, char *label, double mass)
 * \brief Function to show the mass.
 * \param text
 * \brief GtkTextBuffer text to write the mass.
 * \param iter
 * \brief GtkTextIter to write the mass.
 * \param label
 * \brief Mass label.
 * \param mass
 * \brief Mass.
 */
void text_mass(GtkTextBuffer *text, GtkTextIter *iter, char *label, double mass)
{
	char buffer[512];
	snprintf(buffer, 512, "\t\t%s = %lg m\302\263\n", label, mass);
	gtk_text_buffer_insert(text, iter, buffer, -1);
}

/**
 * \fn void summary_new()
 * \brief Function to display the summary.
 */
void summary_new()
{
	int i, n;
	char buffer[512];
	double m1, m2;
	GtkTextIter iter[1];
	Summary s[1];

	// showing input data summary
	s->view_input = (GtkTextView*)gtk_text_view_new();
	s->scrolled_input = (GtkScrolledWindow*)gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add(GTK_CONTAINER(s->scrolled_input),
		GTK_WIDGET(s->view_input));
	s->text_input = gtk_text_view_get_buffer(s->view_input);
	gtk_text_view_set_editable(s->view_input, FALSE);
	gtk_text_buffer_get_start_iter(s->text_input, iter);
	gtk_text_buffer_create_tag(s->text_input, "bold", "weight",
		PANGO_WEIGHT_BOLD, NULL);
	gtk_text_buffer_create_tag(s->text_input, "superscript",  
		"rise", 5 * PANGO_SCALE, "size", 8 * PANGO_SCALE, NULL);  

	snprintf(buffer, 512, "%s\n", gettext("GEOMETRY"));
	gtk_text_buffer_insert_with_tags_by_name(s->text_input, iter, buffer, -1,
		"bold", NULL);
	text_corner(s->text_input, iter, 1,
		field->sb->x0, field->sb->y0, field->sb->z0);
	text_corner(s->text_input, iter, 2,
		field->sb->xf, field->sb->yf, field->sb->zf);
	if (field->nfurrows > 0)
	{
		if (field->open)
		{
			text_corner(s->text_input, iter, 3,
				field->sc->x0, field->sc->y0, field->sc->z0);
			text_corner(s->text_input, iter, 4,
				field->sc->xf, field->sc->yf, field->sc->zf);
		}
		else
		{
			text_corner(s->text_input, iter, 3,
				field->si->xf, field->si->yf, field->si->zf);
			text_corner(s->text_input, iter, 4,
				field->si[field->nfurrows - 1].xf,
				field->si[field->nfurrows - 1].yf,
				field->si[field->nfurrows - 1].zf);
		}
	}
	gtk_text_buffer_insert(s->text_input, iter, "\n", -1);

	snprintf(buffer, 512, "%s\n", gettext("FURROWS"));
	gtk_text_buffer_insert_with_tags_by_name(s->text_input, iter, buffer, -1,
		"bold", NULL);
	snprintf(buffer, 512, "\t%s = %s\n", gettext("Recirculation furrow"),
		field->open ? gettext("yes") : gettext("not"));
	gtk_text_buffer_insert(s->text_input, iter, buffer, -1);
	snprintf(buffer, 512, "\t%s = %d\n",
		gettext("Number of irrigation furrows"), field->nfurrows);
	gtk_text_buffer_insert(s->text_input, iter, buffer, -1);
	text_furrow(s->text_input, iter, gettext("Distribution furrow"), field->sb);
	if (field->nfurrows > 0)
	{
		text_furrow(s->text_input, iter, gettext("Irrigation furrows"),
			field->si);
		if (field->open)
			text_furrow(s->text_input, iter, gettext("Recirculation furrow"),
				field->sc);
	}
	gtk_text_buffer_insert(s->text_input, iter, "\n", -1);

	snprintf(buffer, 512, "%s\n", gettext("INPUTS"));
	gtk_text_buffer_insert_with_tags_by_name(s->text_input, iter, buffer, -1,
		"bold", NULL);
	snprintf(buffer, 512, "\t%s = %d\n", gettext("Number of water inputs"),
		field->nwinputs);
	gtk_text_buffer_insert(s->text_input, iter, buffer, -1);
	snprintf(buffer, 512, "\t%s = %d\n", gettext("Number of fertilizer inputs"),
		field->nsinputs);
	gtk_text_buffer_insert(s->text_input, iter, buffer, -1);
	for (i = 0; i < field->nwinputs; ++i)
		text_input(s->text_input, iter, 0, i + 1, field->input + i);
	for (i = 0; i < field->nsinputs; ++i)
		text_input(s->text_input, iter, 1, i + 1,
			field->input + field->nwinputs + i);
	gtk_text_buffer_insert(s->text_input, iter, "\n", -1);

	snprintf(buffer, 512, "%s\n", gettext("FERTILIZER"));
	gtk_text_buffer_insert_with_tags_by_name(s->text_input, iter, buffer, -1,
		"bold", NULL);
	snprintf(buffer, 512, "\t%s = %lg kg/m\302\263\n\n", gettext("Solubility"),
		solubility);
	gtk_text_buffer_insert(s->text_input, iter, buffer, -1);

	snprintf(buffer, 512, "%s\n", gettext("PROBES"));
	gtk_text_buffer_insert_with_tags_by_name(s->text_input, iter, buffer, -1,
		"bold", NULL);
	snprintf(buffer, 512, "\t%s = %d\n", gettext("Number of probes"),
		field->nprobes);
	gtk_text_buffer_insert(s->text_input, iter, buffer, -1);
	for (i = 0; i < field->nprobes; ++i)
		text_probe(s->text_input, iter, i + 1, field->probe + i);
	gtk_text_buffer_insert(s->text_input, iter, "\n", -1);

	snprintf(buffer, 512, "%s\n", gettext("ADVANCED OPTIONS"));
	gtk_text_buffer_insert_with_tags_by_name(s->text_input, iter, buffer, -1,
		"bold", NULL);
	snprintf(buffer, 512, "\t%s = %lg s\n", gettext("Maximum simulation time"),
		field->tf);
	gtk_text_buffer_insert(s->text_input, iter, buffer, -1);
	snprintf(buffer, 512, "\t%s = %lg\n", gettext("CFL number"), field->cfl);
	gtk_text_buffer_insert(s->text_input, iter, buffer, -1);
	snprintf(buffer, 512, "\t%s = %lg s\n", gettext("Data dump period"),
		field->tm);
	gtk_text_buffer_insert(s->text_input, iter, buffer, -1);
	snprintf(buffer, 512, "\t%s = %d\n",
		gettext("Cells number for distribution channel (between furrows)"),
		field->nbi);
	gtk_text_buffer_insert(s->text_input, iter, buffer, -1);
	snprintf(buffer, 512, "\t%s = %d\n",
		gettext("Cells number for irrigation channels"), field->ni);
	gtk_text_buffer_insert(s->text_input, iter, buffer, -1);

	// showing output data
	s->view_output = (GtkTextView*)gtk_text_view_new();
	s->scrolled_output =
		(GtkScrolledWindow*)gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add(GTK_CONTAINER(s->scrolled_output),
		GTK_WIDGET(s->view_output));
	s->text_output = gtk_text_view_get_buffer(s->view_output);
	gtk_text_view_set_editable(s->view_output, FALSE);
	gtk_text_buffer_get_start_iter(s->text_output, iter);
	gtk_text_buffer_create_tag(s->text_output, "bold", "weight",
		PANGO_WEIGHT_BOLD, NULL);

	snprintf(buffer, 512, "%s = %lg s\n", gettext("Irrigation life time"), t);
	gtk_text_buffer_insert(s->text_output, iter, buffer, -1);
	snprintf(buffer, 512, "%s = %lg s\n\n", gettext("Calculation time"),
		field->cpu);
	gtk_text_buffer_insert(s->text_output, iter, buffer, -1);

	snprintf(buffer, 512, "%s\n", gettext("IRRIGATION QUALITY"));
	gtk_text_buffer_insert_with_tags_by_name(s->text_output, iter, buffer, -1,
		"bold", NULL);
	snprintf(buffer, 512, "\t%s\n", gettext("Water"));
	gtk_text_buffer_insert_with_tags_by_name(s->text_output, iter, buffer, -1,
		"bold", NULL);
	snprintf(buffer, 512, "\t\t%s = %lg %%\n", gettext("Uniformity"),
		field_uniformity_water(field));
	gtk_text_buffer_insert(s->text_output, iter, buffer, -1);
	snprintf(buffer, 512, "\t\t%s = %lg %%\n", gettext("Efficiency"),
		field_efficiency_water(field));
	gtk_text_buffer_insert(s->text_output, iter, buffer, -1);
	snprintf(buffer, 512, "\t%s\n", gettext("Fertilizer"));
	gtk_text_buffer_insert_with_tags_by_name(s->text_output, iter, buffer, -1,
		"bold", NULL);
	snprintf(buffer, 512, "\t\t%s = %lg %%\n", gettext("Uniformity"),
		field_uniformity_fertilizer(field));
	gtk_text_buffer_insert(s->text_output, iter, buffer, -1);
	snprintf(buffer, 512, "\t\t%s = %lg %%\n\n", gettext("Efficiency"),
		field_efficiency_fertilizer(field));
	gtk_text_buffer_insert(s->text_output, iter, buffer, -1);

	snprintf(buffer, 512, "%s\n", gettext("WATER VOLUME"));
	gtk_text_buffer_insert_with_tags_by_name(s->text_output, iter, buffer, -1,
		"bold", NULL);
	snprintf(buffer, 512, "\t%s\n", gettext("Total"));
	gtk_text_buffer_insert_with_tags_by_name(s->text_output, iter, buffer, -1,
		"bold", NULL);
	m1 = field_input_water(field);
	text_mass(s->text_output, iter, gettext("Input"), m1);
	m2 = volume_water_total(field->p, field->n);
	text_mass(s->text_output, iter, gettext("Calculated"), m2);
	snprintf(buffer, 512, "\t\t%s = %lg %%\n",
		gettext("Conservation error"), 100. * fabs(m2 - m1) / m1);
	gtk_text_buffer_insert(s->text_output, iter, buffer, -1);
	text_mass(s->text_output, iter, gettext("Superficial"),
		volume_water_surface(field->p, field->n));
	text_mass(s->text_output, iter, gettext("Infiltrated"),
		volume_water_soil(field->p, field->n));
	text_mass(s->text_output, iter, gettext("Percolated"),
		volume_water_loss(field->p, field->n));
	if (field->nfurrows > 0)
	{
		n = field->nfurrows * field->ni;
		snprintf(buffer, 512, "\t%s\n", gettext("Irrigation furrows"));
		gtk_text_buffer_insert_with_tags_by_name(s->text_output, iter,
			buffer, -1, "bold", NULL);
		text_mass(s->text_output, iter, gettext("Total"),
			volume_water_total(field->p, n));
		text_mass(s->text_output, iter, gettext("Superficial"),
			volume_water_surface(field->p, n));
		text_mass(s->text_output, iter, gettext("Infiltrated"),
			volume_water_soil(field->p, n));
		text_mass(s->text_output, iter, gettext("Percolated"),
			volume_water_loss(field->p, n));
		snprintf(buffer, 512, "\t%s\n", gettext("Distribution furrow"));
		gtk_text_buffer_insert_with_tags_by_name(s->text_output, iter,
			buffer, -1, "bold", NULL);
		text_mass(s->text_output, iter, gettext("Total"),
			volume_water_total(field->p + n, field->nb));
		text_mass(s->text_output, iter, gettext("Superficial"),
			volume_water_surface(field->p + n, field->nb));
		text_mass(s->text_output, iter, gettext("Infiltrated"),
			volume_water_soil(field->p + n, field->nb));
		text_mass(s->text_output, iter, gettext("Percolated"),
			volume_water_loss(field->p + n, field->nb));
		if (field->open)
		{
			n += field->nb;
			snprintf(buffer, 512, "\t%s\n", gettext("Recirculation furrow"));
			gtk_text_buffer_insert_with_tags_by_name(s->text_output, iter,
				buffer, -1, "bold", NULL);
			text_mass(s->text_output, iter, gettext("Total"),
				volume_water_total(field->p + n, field->nb));
			text_mass(s->text_output, iter, gettext("Superficial"),
				volume_water_surface(field->p + n, field->nb));
			text_mass(s->text_output, iter, gettext("Infiltrated"),
				volume_water_soil(field->p + n, field->nb));
			text_mass(s->text_output, iter, gettext("Percolated"),
				volume_water_loss(field->p + n, field->nb));
		}
	}
	gtk_text_buffer_insert(s->text_output, iter, "\n", -1);

	snprintf(buffer, 512, "%s\n", gettext("FERTILIZER MASS"));
	gtk_text_buffer_insert_with_tags_by_name(s->text_output, iter, buffer, -1,
		"bold", NULL);
	snprintf(buffer, 512, "\t%s\n", gettext("Total"));
	gtk_text_buffer_insert_with_tags_by_name(s->text_output, iter, buffer, -1,
		"bold", NULL);
	m1 = field_input_fertilizer(field);
	snprintf(buffer, 512, "\t\t%s = %lg kg\n", gettext("Input"),
		m1);
	gtk_text_buffer_insert(s->text_output, iter, buffer, -1);
	m2 = mass_fertilizer_total(field->p, field->n);
	snprintf(buffer, 512, "\t\t%s = %lg kg\n", gettext("Calculated"),
		m2);
	gtk_text_buffer_insert(s->text_output, iter, buffer, -1);
	snprintf(buffer, 512, "\t\t%s = %lg %%\n", gettext("Conservation error"),
		100. * fabs(m2 - m1) / m1);
	gtk_text_buffer_insert(s->text_output, iter, buffer, -1);
	snprintf(buffer, 512, "\t\t%s = %lg kg\n", gettext("Superficial"),
		mass_fertilizer_surface(field->p, field->n));
	gtk_text_buffer_insert(s->text_output, iter, buffer, -1);
	snprintf(buffer, 512, "\t\t%s = %lg kg\n", gettext("Infiltrated"),
		mass_fertilizer_soil(field->p, field->n));
	gtk_text_buffer_insert(s->text_output, iter, buffer, -1);
	snprintf(buffer, 512, "\t\t%s = %lg kg\n", gettext("Percolated"),
		mass_fertilizer_loss(field->p, field->n));
	gtk_text_buffer_insert(s->text_output, iter, buffer, -1);
	snprintf(buffer, 512, "\t\t%s = %lg kg\n", gettext("Solid"),
		mass_fertilizer_solid(field->p, field->n));
	gtk_text_buffer_insert(s->text_output, iter, buffer, -1);
	if (field->nfurrows > 0)
	{
		n = field->nfurrows * field->ni;
		snprintf(buffer, 512, "\t%s\n", gettext("Irrigation furrows"));
		gtk_text_buffer_insert_with_tags_by_name(s->text_output, iter,
			buffer, -1, "bold", NULL);
		snprintf(buffer, 512, "\t\t%s = %lg kg\n", gettext("Total"),
			mass_fertilizer_total(field->p, n));
		gtk_text_buffer_insert(s->text_output, iter, buffer, -1);
		snprintf(buffer, 512, "\t\t%s = %lg kg\n", gettext("Superficial"),
			mass_fertilizer_surface(field->p, n));
		gtk_text_buffer_insert(s->text_output, iter, buffer, -1);
		snprintf(buffer, 512, "\t\t%s = %lg kg\n", gettext("Infiltrated"),
			mass_fertilizer_soil(field->p, n));
		gtk_text_buffer_insert(s->text_output, iter, buffer, -1);
		snprintf(buffer, 512, "\t\t%s = %lg kg\n", gettext("Percolated"),
			mass_fertilizer_loss(field->p, n));
		gtk_text_buffer_insert(s->text_output, iter, buffer, -1);
		snprintf(buffer, 512, "\t\t%s = %lg kg\n", gettext("Solid"),
			mass_fertilizer_solid(field->p, n));
		gtk_text_buffer_insert(s->text_output, iter, buffer, -1);
		snprintf(buffer, 512, "\t%s\n", gettext("Distribution furrow"));
		gtk_text_buffer_insert_with_tags_by_name(s->text_output, iter,
			buffer, -1, "bold", NULL);
		snprintf(buffer, 512, "\t\t%s = %lg kg\n", gettext("Total"),
			mass_fertilizer_total(field->p + n, field->nb));
		gtk_text_buffer_insert(s->text_output, iter, buffer, -1);
		snprintf(buffer, 512, "\t\t%s = %lg kg\n", gettext("Superficial"),
			mass_fertilizer_surface(field->p + n, field->nb));
		gtk_text_buffer_insert(s->text_output, iter, buffer, -1);
		snprintf(buffer, 512, "\t\t%s = %lg kg\n", gettext("Infiltrated"),
			mass_fertilizer_soil(field->p + n, field->nb));
		gtk_text_buffer_insert(s->text_output, iter, buffer, -1);
		snprintf(buffer, 512, "\t\t%s = %lg kg\n", gettext("Percolated"),
			mass_fertilizer_loss(field->p + n, field->nb));
		gtk_text_buffer_insert(s->text_output, iter, buffer, -1);
		snprintf(buffer, 512, "\t\t%s = %lg kg\n", gettext("Solid"),
			mass_fertilizer_solid(field->p + n, field->nb));
		gtk_text_buffer_insert(s->text_output, iter, buffer, -1);
		if (field->open)
		{
			n += field->nb;
			snprintf(buffer, 512, "\t%s\n", gettext("Recirculation furrow"));
			gtk_text_buffer_insert_with_tags_by_name(s->text_output, iter,
				buffer, -1, "bold", NULL);
			snprintf(buffer, 512, "\t\t%s = %lg kg\n", gettext("Total"),
				mass_fertilizer_total(field->p + n, field->nb));
			gtk_text_buffer_insert(s->text_output, iter, buffer, -1);
			snprintf(buffer, 512, "\t\t%s = %lg kg\n", gettext("Superficial"),
				mass_fertilizer_surface(field->p + n, field->nb));
			gtk_text_buffer_insert(s->text_output, iter, buffer, -1);
			snprintf(buffer, 512, "\t\t%s = %lg kg\n", gettext("Infiltrated"),
				mass_fertilizer_soil(field->p + n, field->nb));
			gtk_text_buffer_insert(s->text_output, iter, buffer, -1);
			snprintf(buffer, 512, "\t\t%s = %lg kg\n", gettext("Percolated"),
				mass_fertilizer_loss(field->p + n, field->nb));
			gtk_text_buffer_insert(s->text_output, iter, buffer, -1);
			snprintf(buffer, 512, "\t\t%s = %lg kg\n", gettext("Solid"),
				mass_fertilizer_solid(field->p + n, field->nb));
			gtk_text_buffer_insert(s->text_output, iter, buffer, -1);
		}
	}

	// packing input and output views in a notebook
	s->notebook = (GtkNotebook*)gtk_notebook_new();
	gtk_notebook_append_page(s->notebook, GTK_WIDGET(s->scrolled_input),
		gtk_label_new(gettext("Input")));
	gtk_notebook_append_page(s->notebook, GTK_WIDGET(s->scrolled_output),
		gtk_label_new(gettext("Results")));
	gtk_widget_set_size_request(GTK_WIDGET(s->notebook), 600, 400);

	// showing the dialog
	gtk_widget_show_all(GTK_WIDGET(s->notebook));
	s->dialog = (GtkDialog*)gtk_dialog_new_with_buttons(
		gettext("Summary"),
		GTK_WINDOW(windowMain),
		GTK_DIALOG_MODAL,
		GTK_STOCK_CLOSE, GTK_RESPONSE_CLOSE,
		NULL);
	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(s->dialog)),
		GTK_WIDGET(s->notebook));
	gtk_dialog_run(s->dialog);

	// closing the dialog
	gtk_widget_destroy(GTK_WIDGET(s->dialog));
}

/**
 * \fn int intro_window_destroy(GtkWidget *widget)
 * \brief Function to destroy the introduction window.
 * \param widget
 * \brief Introduction window widget.
 * \return FALSE.
 */
int intro_window_destroy(GtkWidget *widget)
{
	gtk_widget_destroy(widget);
	return FALSE;
}

/**
 * \fn void main_window_update(MainWindow *w)
 * \brief Function to make sensitive/insensitive some tool buttons of the main
 * 	window.
 * \param w
 * \brief Main window structure.
 */
void main_window_update(MainWindow *w)
{
	int i;
	if (input_dir) i=1; else i=0;
	gtk_widget_set_sensitive(GTK_WIDGET(w->button_config), i);
	gtk_widget_set_sensitive(GTK_WIDGET(w->button_run), i);
	gtk_widget_set_sensitive(GTK_WIDGET(w->button_plot), plot_show);
	gtk_widget_set_sensitive(GTK_WIDGET(w->button_summary), plot_show);
}

/**
 * \fn void main_window_open(MainWindow *w)
 * \brief Function to open a new fertigation problem.
 * \param w
 * \brief Main window structure.
 */
void main_window_open(MainWindow *w)
{
	GtkFileChooserDialog *dlg;
	dlg = (GtkFileChooserDialog*)gtk_file_chooser_dialog_new(
		"Case Directory",
		GTK_WINDOW(windowMain),
		GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
		GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
		GTK_STOCK_OK, GTK_RESPONSE_OK,
		NULL);
	if (gtk_dialog_run(GTK_DIALOG(dlg)) == GTK_RESPONSE_OK)
		input_dir = (char*)gtk_file_chooser_get_filename
			(GTK_FILE_CHOOSER(dlg));
	gtk_widget_destroy(GTK_WIDGET(dlg));
	if (input_dir && !kernel_open(input_dir, 1, NULL))
	{
		g_free(input_dir);
		input_dir = NULL;
	}
	main_window_update(w);
}

/**
 * \fn void main_window_config(MainWindow *w)
 * \brief Function to configure a fertigation problem.
 * \param w
 * \brief Main window structure.
 */
void main_window_config(MainWindow *w)
{
	window_config_new();
	main_window_update(w);
}

/**
 * \fn void main_window_run(MainWindow *w)
 * \brief Function to run the simulation.
 * \param w
 * \brief Main window structure.
 */
void main_window_run(MainWindow *w)
{
	char buffer[512];
	#ifdef G_OS_WIN32
		snprintf(buffer, 512, "del \"%s\\*.out\"", input_dir);
	#else
		snprintf(buffer, 512, "rm %s/*.out", input_dir);
	#endif
	system(buffer);
	plot_show = kernel(input_dir, 1, NULL);
	main_window_update(w);
}

/**
 * \fn void main_window_about(MainWindow *w)
 * \brief Function to open an about help dialog.
 * \param w
 * \brief Main window structure.
 */
void main_window_about(MainWindow *w)
{
	gchar *authors[] =
	{
		"Javier Burguete Tolosa (jburguete@eead.csic.es)",
		"Asier Lacasta Soto (alacasta@unizar.es)",
		NULL
	};
	gchar *artists[] = {"Carlos Aliaga Badal (caliaga.87@gmail.com)", NULL};
	gtk_show_about_dialog(GTK_WINDOW(windowMain),
		"program_name",
		"surcos",
		"authors",
		authors,
		"artists",
		artists,
		"version",
		"5.2",
		"copyright",
		gettext("Copyright 2011-2012 Javier Burguete Tolosa."),
		"logo",
		w->pixbuf,
		"website-label",
		"Website",
		"website",
		"http://www.eead.csic.es",
		NULL);
}

/**
 * \fn void main_window_new()
 * \brief Function to open the main window.
 */
#if JBW_GRAPHIC == JBW_GRAPHIC_GTKGLEXT
void main_window_new(GdkGLConfig *glconfig)
#else
void main_window_new()
#endif
{
	GtkWindow *window;
	MainWindow *w;

#if DEBUG_MAIN_WINDOW_NEW
printf("main_window_new: start\n");
#endif

	w = (MainWindow*)malloc(sizeof(MainWindow));

	w->button_exit = (GtkToolButton*)gtk_tool_button_new_from_stock(
		GTK_STOCK_QUIT);
	g_signal_connect(w->button_exit, "clicked", gtk_main_quit, NULL);

	w->button_open = (GtkToolButton*)gtk_tool_button_new_from_stock(
		GTK_STOCK_OPEN);
	g_signal_connect_swapped(w->button_open, "clicked",
		(GCallback)main_window_open, w);

	w->button_config = (GtkToolButton*)gtk_tool_button_new(
 		gtk_image_new_from_stock(GTK_STOCK_PREFERENCES,
			GTK_ICON_SIZE_LARGE_TOOLBAR), // icon
		gettext("Configure"));
	gtk_widget_set_tooltip_text(GTK_WIDGET(w->button_config),
		gettext("Simulation configure"));
	g_signal_connect_swapped(w->button_config, "clicked",
		(GCallback)main_window_config, w);

	w->button_run = (GtkToolButton*)gtk_tool_button_new_from_stock(
		GTK_STOCK_EXECUTE);
	g_signal_connect_swapped(w->button_run, "clicked",
		(GCallback)main_window_run, w);

	w->button_plot = (GtkToolButton*)gtk_tool_button_new(
 		gtk_image_new_from_file("curve32.png"), // icon
		gettext("Plot"));
	gtk_widget_set_tooltip_text(GTK_WIDGET(w->button_plot),
		gettext("Plot results"));
#if JBW_GRAPHIC == JBW_GRAPHIC_GTKGLEXT
	g_signal_connect_swapped(w->button_plot, "clicked",
		(GCallback)window_plot_new, glconfig);
#else
	g_signal_connect(w->button_plot, "clicked", window_plot_new, NULL);
#endif

	w->button_summary = (GtkToolButton*)gtk_tool_button_new(
 		gtk_image_new_from_stock(GTK_STOCK_EDIT,
			GTK_ICON_SIZE_LARGE_TOOLBAR), // icon
		gettext("Summary"));
	gtk_widget_set_tooltip_text(GTK_WIDGET(w->button_summary),
		gettext("Show summary"));
	g_signal_connect(w->button_summary, "clicked", summary_new, NULL);

	w->button_help = (GtkToolButton*)gtk_tool_button_new_from_stock(
		GTK_STOCK_HELP);

	w->toolbar = (GtkToolbar*)gtk_toolbar_new ();
//	gtk_toolbar_set_orientation(w->toolbar, GTK_ORIENTATION_HORIZONTAL);
	gtk_toolbar_set_style(w->toolbar, GTK_TOOLBAR_BOTH);
	gtk_toolbar_insert(w->toolbar, GTK_TOOL_ITEM(w->button_exit), -1);
	gtk_toolbar_insert(w->toolbar, GTK_TOOL_ITEM(w->button_open), -1);
	gtk_toolbar_insert(w->toolbar, GTK_TOOL_ITEM(w->button_config), -1);
	gtk_toolbar_insert(w->toolbar, GTK_TOOL_ITEM(w->button_run), -1);
	gtk_toolbar_insert(w->toolbar, GTK_TOOL_ITEM(w->button_plot), -1);
	gtk_toolbar_insert(w->toolbar, GTK_TOOL_ITEM(w->button_summary), -1);
	gtk_toolbar_insert(w->toolbar, GTK_TOOL_ITEM(w->button_help), -1);

	w->label = (GtkLabel*)gtk_label_new("");

	w->box = (GtkVBox*)gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(w->box), GTK_WIDGET(w->toolbar), 0, 1, 0);
	gtk_container_add(GTK_CONTAINER(w->box), GTK_WIDGET(w->label));

	w->pixbuf = gtk_image_get_pixbuf
		(GTK_IMAGE(gtk_image_new_from_file("logo2.png")));

	windowMain = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	w->window = (GtkWindow*)windowMain;
	gtk_container_add(GTK_CONTAINER(w->window), GTK_WIDGET(w->box));
	gtk_window_set_icon(w->window, w->pixbuf);
	gtk_window_set_title (w->window, ":: Surcos");
	gtk_widget_set_size_request(GTK_WIDGET(w->window), 640, 70);
	gtk_widget_show_all(GTK_WIDGET(w->window));
 
	g_signal_connect(w->window, "destroy", gtk_main_quit, NULL);
	g_signal_connect_swapped(w->button_help, "clicked",
		(GCallback)main_window_about, w);

	main_window_update(w);

	// Window Intro 
	window = (GtkWindow*)gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_container_add(GTK_CONTAINER(window),
		gtk_image_new_from_file("fondo6.png"));
	gtk_window_set_opacity(window, 0.9);
	gtk_window_set_decorated(window, FALSE);
   	gtk_widget_show_all(GTK_WIDGET(window));
	gtk_window_set_position(window, GTK_WIN_POS_CENTER_ALWAYS);
	g_timeout_add_seconds(3, (GSourceFunc)intro_window_destroy, window);

#if DEBUG_MAIN_WINDOW_NEW
printf("main_window_new: end\n");
#endif
}
