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
 * \file graphics.h
 * \brief Header file to define the result graphics.
 * \authors Javier Burguete Tolosa, Asier Lacasta Soto.
 * \copyright Copyright 2011-2023, Javier Burguete Tolosa.
 */
#ifndef GRAPHICS__H
#define GRAPHICS__H 1

#include "jb/src/jb_win.h"

/**
 * \struct FurrowOutput
 * \brief Structure to define the geometry of a furrow mesh.
 */
typedef struct
{
  double *x;
  ///< Array of x-coordinates of the furrow mesh nodes.
  double *y;
  ///< Array of y-coordinates of the furrow mesh nodes.
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
  unsigned int n;               ///< Number of furrow mesh nodes.
} FurrowOutput;

/**
 * \struct GraphicMap
 * \brief Structure to draw a furrow map.
 */
typedef struct
{
  JBWGraphic *graphic;          ///< Graphic structure.
  GtkLabel *label;              ///< GtkLabel to show the time step label.
  GtkScale *scale;
  ///< GtkScale to configure the time step to plot.
  GtkComboBoxText *combo;
  ///< GtkComboBox to configure the variable to plot.
  GtkBox *box;
  ///< GtkBox containing the configuration widgets.
  unsigned int variable;        ///< Type of variable to plot.
  unsigned int step;            ///< Time step to plot.
} GraphicMap;

/**
 * \struct GraphicFurrows
 * \brief Structure to plot a longitudinal profile of a variable in a furrow.
 */
typedef struct
{
  JBWGraphic *graphic;          ///< Graphic structure.
  GtkLabel *label;              ///< GtkLabel to show the time step label.
  GtkScale *scale;
  ///< GtkScale to configure the time step to plot.
  GtkComboBoxText *combo_furrow;
  ///< GtkComboBox to configure the furrow to plot.
  GtkComboBoxText *combo_variable;
  ///< GtkComboBox to configure the variable to plot.
  GtkBox *box;
  ///< GtkBox containing the configuration widgets.
  unsigned int furrow;          ///< Furrow number.
  unsigned int variable;        ///< Variable type.
  unsigned int step;            ///< Time step number.
} GraphicFurrows;

/**
 * \struct GraphicProbes
 * \brief Structure to plot probe results.
 */
typedef struct
{
  JBWGraphic *graphic;          ///< Graphic structure.
  GtkComboBoxText *combo;
  ///< GtkComboBoxText to select the probe results to plot.
} GraphicProbes;

/**
 * \struct WindowPlot
 * \brief Structure to plot the results.
 */
typedef struct
{
  GraphicMap graphic_map[1];    ///< Structure to draw a furrow map.
  GraphicFurrows graphic_furrows[1];
  ///< Structure to plot a longitudinal profile of a variable in a furrow.
  GraphicProbes graphic_probes[1];      ///< Structure to plot probe results.
  GtkButton *button_save;       ///< GtkButton to save the plot in a file.
  GtkNotebook *notebook;        ///< GtkNotebook to select the plot type.
  GtkBox *box;                  ///< GtkBox to pack the widgets.
  GtkImage *logo;               ///< GtkImage to show the program logo.
  GtkHeaderBar *bar;            ///< GtkHeaderBar to show the window title.
  GtkWindow *window;
  ///< GtkWindow containing the configuration widgets.
  JBWGraphic *graphic;          ///< Graphic structure.
  gulong id_destroy_graphic;    ///< Destroy signal identifier.
  gulong id_destroy_window;     ///< Destroy signal identifier.
} WindowPlot;

#if HAVE_FREEGLUT
extern JBWGraphic *graphic;
#endif

extern WindowPlot *window_plot;

void window_plot_destroy (WindowPlot * w);
void window_plot_new (WindowPlot * w);

#endif
