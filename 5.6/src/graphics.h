/*
SURCOS - A software tool to solve irrigation and fertigation in isolated furrows
and furrow networks.

Copyright 2011-2018, Javier Burguete Tolosa.

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
 * \copyright Copyright 2011-2018, Javier Burguete Tolosa.
 */
#ifndef GRAPHICS__H
#define GRAPHICS__H 1

#include "jb/jb_win.h"

/**
 * \struct FurrowOutput
 * \brief Structure to define the geometry of a furrow mesh.
 */
typedef struct
{
  double x1;
  ///< x-coordinate of the 1st corner point limiting the furrow.
  double y1;
  ///< y-coordinate of the 1st corner point limiting the furrow.
  double x2;
  ///< x-coordinate of the 2nd corner point limiting the furrow.
  double y2;
  ///< y-coordinate of the 2nd corner point limiting the furrow.
  double x3;
  ///< x-coordinate of the 3th corner point limiting the furrow.
  double y3;
  ///< y-coordinate of the 3th corner point limiting the furrow.
  double x4;
  ///< x-coordinate of the 4th corner point limiting the furrow.
  double y4;
  ///< y-coordinate of the 4th corner point limiting the furrow.
  double *x;
  ///< Array of x-coordinates of the furrow mesh nodes.
  double *y;
  ///< Array of y-coordinates of the furrow mesh nodes.
  int n;
  ///< Number of furrow mesh nodes.
} FurrowOutput;

/**
 * \struct GraphicMap
 * \brief Structure to draw a furrow map.
 */
typedef struct
{
  int variable;
  ///< Type of variable to plot.
 	int step;
  ///< Time step to plot.
  JBWGraphic *graphic;
  ///< Graphic structure.
  GtkLabel *label;
  ///< GtkLabel to show the time step label.
  GtkScale *scale;
  ///< GtkScale to configure the time step to plot.
  GtkComboBoxText *combo;
  ///< GtkComboBox to configure the variable to plot.
  GtkGrid *box;
  ///< GtkGrid containing the configuration widgets.
} GraphicMap;

/**
 * \struct GraphicFurrows
 * \brief Structure to plot a longitudinal profile of a variable in a furrow.
 */
typedef struct
{
  int furrow;
  ///< Furrow number.
  int variable;
  ///< Variable type.
  int step;
  ///< Time step number.
  JBWGraphic *graphic;
  ///< Graphic structure.
  GtkLabel *label;
  ///< GtkLabel to show the time step label.
  GtkScale *scale;
  ///< GtkScale to configure the time step to plot.
  GtkComboBoxText *combo_furrow;
  ///< GtkComboBox to configure the furrow to plot.
  GtkComboBoxText *combo_variable;
  ///< GtkComboBox to configure the variable to plot.
  GtkGrid *box;
  ///< GtkGrid containing the configuration widgets.
} GraphicFurrows;

/**
 * \struct GraphicProbes
 * \brief Structure to plot probe results.
 */
typedef struct
{
  GtkComboBoxText *combo;
  ///< GtkComboBoxText to select the probe results to plot.
  JBWGraphic *graphic;
  ///< Graphic structure.
} GraphicProbes;

/**
 * \struct WindowPlot
 * \brief Structure to plot the results.
 */
typedef struct
{
  GraphicMap graphic_map[1];
  ///< Structure to draw a furrow map.
  GraphicFurrows graphic_furrows[1];
  ///< Structure to plot a longitudinal profile of a variable in a furrow.
  GraphicProbes graphic_probes[1];
  ///< Structure to plot probe results.
  GtkNotebook *notebook;
  ///< GtkNotebook to select the plot type.
  JBWGraphic *graphic;
  ///< Graphic structure.
  GtkDialog *dialog;
  ///< GtkDialog to show the plot configuration.
} WindowPlot;

void window_plot_new ();

#endif
