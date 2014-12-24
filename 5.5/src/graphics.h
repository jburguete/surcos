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
 * \file graphics.h
 * \brief Header file to define the result graphics.
 * \authors Javier Burguete Tolosa, Asier Lacasta Soto.
 * \copyright Copyright 2011-2014, Javier Burguete Tolosa.
 */
#ifndef GRAPHICS__H
#define GRAPHICS__H 1

#include "jb/jb_win.h"

/**
 * \struct _FurrowOutput
 * \brief Structure to define the geometry of a furrow mesh.
 */
struct _FurrowOutput
{
/**
 * \var x1
 * \brief x-coordinate of the 1st corner point limiting the furrow.
 * \var y1
 * \brief y-coordinate of the 1st corner point limiting the furrow.
 * \var x2
 * \brief x-coordinate of the 2nd corner point limiting the furrow.
 * \var y2
 * \brief y-coordinate of the 2nd corner point limiting the furrow.
 * \var x3
 * \brief x-coordinate of the 3th corner point limiting the furrow.
 * \var y3
 * \brief y-coordinate of the 3th corner point limiting the furrow.
 * \var x4
 * \brief x-coordinate of the 4th corner point limiting the furrow.
 * \var y4
 * \brief y-coordinate of the 4th corner point limiting the furrow.
 * \var x
 * \brief Array of x-coordinates of the furrow mesh nodes.
 * \var y
 * \brief Array of y-coordinates of the furrow mesh nodes.
 * \var n
 * \brief Number of furrow mesh nodes.
 */
	double x1, y1, x2, y2, x3, y3, x4, y4, *x, *y;
	int n;
};

/**
 * \typedef FurrowOutput
 */
typedef struct _FurrowOutput FurrowOutput;

/**
 * \struct _GraphicMap
 * \brief Structure to draw a furrow map.
 */
struct _GraphicMap
{
/**
 * \var variable
 * \brief Type of variable to plot.
 * \var step
 * \brief Time step to plot.
 * \var graphic
 * \brief Graphic structure.
 * \var label
 * \brief GtkLabel to show the time step label.
 * \var scale
 * \brief GtkScale to configure the time step to plot.
 * \var combo
 * \brief GtkComboBox to configure the variable to plot.
 * \var box
 * \brief GtkGrid containing the configuration widgets.
 */
	int variable, step;
	JBWGraphic *graphic;
	GtkLabel *label;
	GtkScale *scale;
	GtkComboBoxText *combo;
	GtkGrid *box;
};

/**
 * \typedef GraphicMap
 */
typedef struct _GraphicMap GraphicMap;

/**
 * \struct _GraphicFurrows
 * \brief Structure to plot a longitudinal profile of a variable in a furrow.
 */
struct _GraphicFurrows
{
/**
 * \var furrow
 * \brief Furrow number.
 * \var variable
 * \brief Variable type.
 * \var step
 * \brief Time step number.
 * \var graphic
 * \brief Graphic structure.
 * \var label
 * \brief GtkLabel to show the time step label.
 * \var scale
 * \brief GtkScale to configure the time step to plot.
 * \var combo_furrow
 * \brief GtkComboBox to configure the furrow to plot.
 * \var combo_variable
 * \brief GtkComboBox to configure the variable to plot.
 * \var box
 * \brief GtkGrid containing the configuration widgets.
 */
	int furrow, variable, step;
	JBWGraphic *graphic;
	GtkLabel *label;
	GtkScale *scale;
	GtkComboBoxText *combo_furrow, *combo_variable;
	GtkGrid *box;
};

/**
 * \typedef GraphicFurrows
 */
typedef struct _GraphicFurrows GraphicFurrows;

/**
 * \struct _GraphicProbes
 * \brief Structure to plot probe results.
 */
struct _GraphicProbes
{
/**
 * \var combo
 * \brief GtkComboBoxText to select the probe results to plot.
 * \var graphic
 * \brief Graphic structure.
 */
	GtkComboBoxText *combo;
	JBWGraphic *graphic;
};

/**
 * \typedef GraphicProbes
 */
typedef struct _GraphicProbes GraphicProbes;

/**
 * \struct _WindowPlot
 * \brief Structure to plot the results. 
 */
struct _WindowPlot
{
/**
 * \var graphic_map
 * \brief Structure to draw a furrow map.
 * \var graphic_furrows
 * \brief Structure to plot a longitudinal profile of a variable in a furrow.
 * \var graphic_probes
 * \brief Structure to plot probe results.
 * \var notebook
 * \brief GtkNotebook to select the plot type.
 * \var graphic
 * \brief Graphic structure.
 * \var box
 * \brief GtkGrid containing the widgets.
 * \var dialog
 * \brief GtkDialog to show the plot configuration.
 */
	GraphicMap graphic_map[1];
	GraphicFurrows graphic_furrows[1];
	GraphicProbes graphic_probes[1];
	GtkNotebook *notebook;
	JBWGraphic *graphic;
	GtkDialog *dialog;
};

/**
 * \typedef WindowPlot
 */
typedef struct _WindowPlot WindowPlot;

void window_plot_new();

#endif
