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
 * \file graphics.c
 * \brief Source file to define the result graphics.
 * \authors Javier Burguete Tolosa, Asier Lacasta Soto.
 * \copyright Copyright 2011-2018, Javier Burguete Tolosa.
 */
#define _GNU_SOURCE
#include <stdlib.h>
#include <time.h>
#include <png.h>
#include "field.h"
#include "graphics.h"

#define DEBUG_PROBES_NUMBER 0
///< Macro to debug the probes_number() function.

#define DEBUG_FURROWS_NUMBER 0
///< Macro to debug the furrows_number() function.

#define DEBUG_GRAPHIC_MAP_DRAW 0
///< Macro to debug the graphic_map_draw() function.

#define DEBUG_GRAPHIC_FURROWS_DRAW 0
///< Macro to debug the graphic_furrows_draw() function.

#define DEBUG_GRAPHIC_FURROWS_UPDATE 0
///< Macro to debug the graphic_furrows_update() function.

#define DEBUG_GRAPHIC_FURROWS_NEW 0
///< Macro to debug the graphic_furrows_new() function.

#define DEBUG_GRAPHIC_PROBES_UPDATE 0
///< Macro to debug the graphic_probes_update() function.

#define DEBUG_GRAPHIC_PROBES_DRAW 0
///< Macro to debug the graphic_probes_draw() function.

#define DEBUG_GRAPHIC_PROBES_NEW 0
///< Macro to debug the graphic_probes_new() function.

#define DEBUG_WINDOW_PLOT_DRAW 0
///< Macro to debug the window_plot_draw() function.

#define DEBUG_WINDOW_PLOT_UPDATE 0
///< Macro to debug the window_plot_update() function.

#define DEBUG_WINDOW_PLOT_NEW 0
///< Macro to debug the window_plot_new() function.

extern GtkWidget *windowMain;
extern char *input_dir;

int nfurrows = -1;
///< Irrigation furrows number.
int recirculation = 1;
///< 1 on presence of recirculation furrow, 0 else.
int numProbes = 0;
///< Probes number.
int ny = 0;
///< Number of time steps with dumped data.
double time_step;
///< Time interval to dump results data.
WindowPlot window_plot[1];
///< Results plot structure.

/**
 * \fn void probes_number()
 * \brief Function to read the probes number.
 */
void
probes_number ()
{
  char buffer[512];
  FILE *f;
#if DEBUG_PROBES_NUMBER
  printf ("probes_number: start\n");
#endif
  snprintf (buffer, 512, "%s/probe.in", input_dir);
  f = g_fopen (buffer, "r");
  fscanf (f, "%d", &numProbes);
  fclose (f);
#if DEBUG_PROBES_NUMBER
  printf ("probes: %d\n", numProbes);
  printf ("probes_number: end\n");
#endif
}

/**
 * \fn void furrows_number()
 * \brief Function to read the furrows number.
 */
void
furrows_number ()
{
  char buffer[512];
  FILE *file;
#if DEBUG_FURROWS_NUMBER
  printf ("furrows_number: start\n");
  printf ("input_dir = %s\n", input_dir);
#endif
  nfurrows = -1;
  snprintf (buffer, 512, "%s/00b.out", input_dir);
  file = g_fopen (buffer, "r");
  if (file)
    {
      fclose (file);
      nfurrows++;
      do
        {
          snprintf (buffer, 512, "%s/%03d.out", input_dir, nfurrows);
          file = g_fopen (buffer, "r");
          if (!file)
            break;
          fclose (file);
          nfurrows++;
        }
      while (1);
    }
  snprintf (buffer, 512, "%s/00c.out", input_dir);
  file = g_fopen (buffer, "r");
  if (!file)
    recirculation = 0;
  else
    {
      recirculation = 1;
      fclose (file);
    }
  ny = 0;
  do
    {
      snprintf (buffer, 512, "%s/00b-%03d.out", input_dir, ny);
      file = g_fopen (buffer, "r");
      if (!file)
        break;
      fclose (file);
      ny++;
    }
  while (1);
#if DEBUG_FURROWS_NUMBER
  printf ("nfurrows=%d ny=%d recirculation=%d\n", nfurrows, ny,
          recirculation);
  printf ("furrows_number: start\n");
#endif
}

/**
 * \fn void read_time_step(double *t)
 * \brief Function to read the time interval between file solution dumps.
 * \param t
 * \brief Pointer to the time interval between file solution dumps.
 */
void
read_time_step (double *t)
{
  double x;
  char buffer[512];
  FILE *file;
  snprintf (buffer, 512, "%s/times.in", input_dir);
  file = g_fopen (buffer, "r");
  fscanf (file, "%lf%lf%lf", &x, &x, t);
  fclose (file);
}

/**
 * \fn void furrow_output_destroy(FurrowOutput *fo)
 * \brief Function to free the memory used by a furrow mesh geometry structure.
 * \param fo
 * \brief Furrow mesh geometry structure.
 */
void
furrow_output_destroy (FurrowOutput * fo)
{
  free (fo->x);
  free (fo->y);
}

/**
 * \fn void furrow_output_print(FurrowOutput *fo)
 * \brief Function to print the variables of a furrow mesh geometry.
 * \param fo
 * \brief Furrow mesh geometry structure.
 */
void
furrow_output_print (FurrowOutput * fo)
{
  printf ("x1=%g y1=%g\n", fo->x1, fo->y1);
  printf ("x2=%g y2=%g\n", fo->x2, fo->y2);
  printf ("x3=%g y3=%g\n", fo->x3, fo->y3);
  printf ("x4=%g y4=%g\n", fo->x4, fo->y4);
}

/**
 * \fn void furrow_output_vertex(FurrowOutput *fo, int i, float *x, float *y)
 * \brief Function to read the corner coordinates of a furrow mesh node.
 * \param fo
 * \brief Furrow mesh geometry structure.
 * \param i
 * \brief Node number.
 * \param x
 * \brief Array of node corner x-coordinates.
 * \param y
 * \brief Array of node corner y-coordinates.
 */
void
furrow_output_vertex (FurrowOutput * fo, int i, float *x, float *y)
{
  double imax, imin, dx1, dy1, dx2, dy2;
  dx1 = (fo->x2 - fo->x1) / (fo->n - 1.);
  dy1 = (fo->y2 - fo->y1) / (fo->n - 1.);
  dx2 = (fo->x4 - fo->x3) / (fo->n - 1.);
  dy2 = (fo->y4 - fo->y3) / (fo->n - 1.);
  imax = fmin (i + 0.5, fo->n - 1.);
  imin = fmax (i - 0.5, 0.);
  x[0] = fo->x1 + imin * dx1;
  y[0] = fo->y1 + imin * dy1;
  x[1] = fo->x1 + imax * dx1;
  y[1] = fo->y1 + imax * dy1;
  x[2] = fo->x3 + imin * dx2;
  y[2] = fo->y3 + imin * dy2;
  x[3] = fo->x3 + imax * dx2;
  y[3] = fo->y3 + imax * dy2;
}

/**
 * \fn void furrow_output_read(FurrowOutput *fo, char *name, int variable)
 * \brief Function to read a variable of a furrow results file.
 * \param fo
 * \brief Furrow mesh geometry structure.
 * \param name
 * \brief Furrow results file name.
 * \param variable
 * \brief Variable type.
 */
void
furrow_output_read (FurrowOutput * fo, char *name, int variable)
{
  double x[12];
  FILE *file;
  file = g_fopen (name, "r");
  fo->n = 0;
  fo->x = fo->y = NULL;
  while (fscanf (file, "%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf",
                 x, x + 1, x + 2, x + 3, x + 4, x + 5, x + 6, x + 7, x + 8,
                 x + 9, x + 10, x + 11) == 12)
    {
      fo->x = (double *) realloc (fo->x, (fo->n + 1) * sizeof (double));
      fo->y = (double *) realloc (fo->y, (fo->n + 1) * sizeof (double));
      fo->x[fo->n] = x[0];
      switch (variable)
        {
        case 0:
          fo->y[fo->n] = x[1];
          break;
        case 1:
          fo->y[fo->n] = x[7];
          break;
        case 2:
          fo->y[fo->n] = -x[6] - x[9];
          break;
        default:
          fo->y[fo->n] = -x[8] - x[10];
        }
      ++fo->n;
    }
  fclose (file);
}

#if JBW_GRAPHIC == JBW_GRAPHIC_GLUT
void
graphic_resize (int width, int height)
{
  if (width < 640)
    width = 640;
  if (height < 400)
    height = 400;
  glutReshapeWindow (width, height);
}
#endif

/**
 * \def COLOR_SCHEME
 * \brief Macro to set the color scheme of the furrows map.
 * \def RCOLOR1
 * \brief Red of the 1st color.
 * \def GCOLOR1
 * \brief Green of the 1st color.
 * \def BCOLOR1
 * \brief Blue of the 1st color.
 * \def RCOLOR2
 * \brief Red of the 2nd color.
 * \def GCOLOR2
 * \brief Green of the 2nd color.
 * \def BCOLOR2
 * \brief Blue of the 2nd color.
 * \def RCOLOR3
 * \brief Red of the 3th color.
 * \def GCOLOR3
 * \brief Green of the 3th color.
 * \def BCOLOR3
 * \brief Blue of the 3th color.
 */
#define COLOR_SCHEME 2
#if COLOR_SCHEME == 1
#define RCOLOR1 1.
#define GCOLOR1 0.
#define BCOLOR1 0.
#define RCOLOR3 0.
#define GCOLOR3 0.
#define BCOLOR3 1.
#elif COLOR_SCHEME == 2
#define RCOLOR1 0.75
#define GCOLOR1 0.25
#define BCOLOR1 0.
#define RCOLOR3 0.
#define GCOLOR3 0.25
#define BCOLOR3 0.75
#elif COLOR_SCHEME == 3
#define RCOLOR1 0.5
#define GCOLOR1 0.5
#define BCOLOR1 0.
#define RCOLOR3 0.
#define GCOLOR3 0.5
#define BCOLOR3 0.5
#elif COLOR_SCHEME == 4
#define RCOLOR1 0.75
#define GCOLOR1 0.
#define BCOLOR1 0.25
#define RCOLOR3 0.25
#define GCOLOR3 0.
#define BCOLOR3 0.75
#else
#error "Unknown color scheme"
#endif
#define RCOLOR2 (1. - RCOLOR1 - RCOLOR3)
#define GCOLOR2 (1. - GCOLOR1 - GCOLOR3)
#define BCOLOR2 (1. - BCOLOR1 - BCOLOR3)

/**
 * \fn give_rainbow_color(double position)
 * \brief Function to set a color of the scheme.
 * \param position
 * \brief A number between [0,1] to set the color.
 */
void
give_rainbow_color (double position)
{
  float c[3];
  if (position < 0. || position > 1.)
    {
      c[0] = RCOLOR1;
      c[1] = GCOLOR1;
      c[2] = BCOLOR1;
    }
  else if (position < 0.5)
    {
      position *= 2;
      c[0] = RCOLOR1 + (RCOLOR2 - RCOLOR1) * position;
      c[1] = GCOLOR1 + (GCOLOR2 - GCOLOR1) * position;
      c[2] = BCOLOR1 + (BCOLOR2 - BCOLOR1) * position;
    }
  else
    {
      position -= 0.5;
      position *= 2;
      c[0] = RCOLOR2 + (RCOLOR3 - RCOLOR2) * position;
      c[1] = GCOLOR2 + (GCOLOR3 - GCOLOR2) * position;
      c[2] = BCOLOR2 + (BCOLOR3 - BCOLOR2) * position;
    }
  glColor3fv (c);
}

/**
 * \fn void graphic_map_draw()
 * \brief Function to draw the furrows map.
 */
void
graphic_map_draw ()
{
  int i, j, x0, y0, xf, yf, n = 1 + nfurrows + recirculation;
  double z, zmax, zmin, dif, initialY, deltaX1, deltaY1, deltaX2, deltaY2,
    width, distance, widthX1, widthY1, widthX2, widthY2;
  float x, xx[4], yy[4], xmin, ymin, xmax, ymax;
  char buffer[512];
  char *label[] = {
    _("Water depth (m)"),
    _("Fertilizer concentration (kg/m³)"),
    _("Infiltrated water mass per unit length (kg/m)"),
    _("Infiltrated fertilizer mass per unit length (kg/m)")
  };
  GraphicMap *g;
  FurrowOutput fo[n];
  JBWGraphic *graphic;

#if DEBUG_GRAPHIC_MAP_DRAW
  printf ("debug_graphic_map_draw: start\n");
#endif
  g = window_plot->graphic_map;
  graphic = g->graphic;

  // Setting title
  g->variable = gtk_combo_box_get_active (GTK_COMBO_BOX (g->combo));
  g->step = gtk_range_get_value (GTK_RANGE (g->scale));
  snprintf (buffer, 128, "%s (t=%g s)",
            label[g->variable], g->step * time_step);
  jbw_graphic_set_title (graphic, buffer);

  // Reading the output files
  snprintf (buffer, 512, "%s/00b-%03d.out", input_dir, g->step);
  furrow_output_read (fo, buffer, g->variable);
  for (i = 0; i < nfurrows; ++i)
    {
      snprintf (buffer, 512, "%s/%03d-%03d.out", input_dir, i, g->step);
      furrow_output_read (fo + 1 + i, buffer, g->variable);
    }
  if (recirculation)
    {
      snprintf (buffer, 512, "%s/00c-%03d.out", input_dir, g->step);
      furrow_output_read (fo + n - 1, buffer, g->variable);
    }

  // Getting extreme values
  zmin = INFINITY;
  zmax = -INFINITY;
  for (i = 0; i < n; ++i)
    {
      for (j = 0; j < fo[i].n; ++j)
        {
          z = fo[i].y[j];
          if (z < zmin)
            zmin = z;
          if (z > zmax)
            zmax = z;
        }
    }
#if DEBUG_GRAPHIC_MAP_DRAW
  printf ("zmin=%g zmax=%g\n", zmin, zmax);
#endif

  // Viewport
  graphic->x1 = graphic->y1 = 0;
#if JBW_GRAPHIC == JBW_GRAPHIC_GLUT
  graphic->x2 = glutGet (GLUT_WINDOW_WIDTH);
  graphic->y2 = glutGet (GLUT_WINDOW_HEIGHT);
#endif

  // Clearing the screen
  jbw_draw_clear (1., 1., 1., 0.);

  // Plotting the legend
  xf = graphic->x2 - 15 * graphic->wchar;
  yf = graphic->y2 - 2 * graphic->hchar;
  x0 = MAX (64, 2 * graphic->wchar);
  y0 = MAX (21, graphic->hchar);
  glViewport (x0, yf, xf - x0, graphic->y2 - yf);
  glLoadIdentity ();
  glColor3f (0.f, 0.f, 0.f);
  x = -((float) strlen (graphic->str_title)) * graphic->wchar / (xf - x0);
  jbw_graphic_draw_string (graphic, graphic->str_title, x, -0.5);
  glViewport (xf, y0, graphic->x2 - xf, yf - y0);
  glLoadIdentity ();
  glOrtho (-(graphic->x2 - xf), graphic->x2 - xf, 0., yf - y0, -1., 1.);
  snprintf (buffer, 32, "%.6g", zmax);
  x = -((float) strlen (buffer)) * graphic->wchar;
  jbw_graphic_draw_string (graphic, buffer, x, yf - y0 - graphic->hchar);
  snprintf (buffer, 32, "%.6g", zmin);
  x = -((float) strlen (buffer)) * graphic->wchar;
  jbw_graphic_draw_string (graphic, buffer, x, 0.f);
  initialY = 2 * graphic->hchar;
  deltaY1 = 0.01 * (yf - y0 - 4 * graphic->hchar);
  glBegin (GL_QUADS);
  for (i = 0; i < 100; i++)
    {
      give_rainbow_color (0.01f * i);
      glVertex2f (-graphic->wchar, initialY);
      glVertex2f (graphic->wchar, initialY);
      initialY += deltaY1;
      glVertex2f (graphic->wchar, initialY);
      glVertex2f (-graphic->wchar, initialY);
    }
  glEnd ();

  // Extreme map values
  xmax = xmin = field->x[0];
  ymax = ymin = field->y[0];
  for (i = 0; ++i < 4;)
    {
      if (field->x[i] > xmax)
        xmax = field->x[i];
      else if (field->x[i] < xmin)
        xmin = field->x[i];
      if (field->y[i] > ymax)
        ymax = field->y[i];
      else if (field->y[i] < ymin)
        ymin = field->y[i];
    }
#if DEBUG_GRAPHIC_MAP_DRAW
  printf ("xmin=%g xmax=%g\n", xmin, xmax);
  printf ("ymin=%g ymax=%g\n", ymin, ymax);
#endif
  deltaX1 = xmax - xmin;
  deltaY1 = ymax - ymin;
  xf -= x0;
  yf -= y0;
  if (deltaX1 * yf > deltaY1 * xf)
    {
      dif = ((deltaX1 * yf) / xf - deltaY1) / 2;
      ymax += dif;
      ymin -= dif;
    }
  else
    {
      dif = ((deltaY1 * xf) / yf - deltaX1) / 2;
      xmax += dif;
      xmin -= dif;
    }
#if DEBUG_GRAPHIC_MAP_DRAW
  printf ("xmin=%g xmax=%g\n", xmin, xmax);
  printf ("ymin=%g ymax=%g\n", ymin, ymax);
#endif

  dif = zmax - zmin;
  dif = fmax (dif, 1e-10);

  glViewport (x0, y0, xf, yf);
  glLoadIdentity ();
  glOrtho (xmin, xmax, ymin, ymax, -1., 1.);

  glBegin (GL_QUADS);

  // Drawing the distribution furrow
  fo[0].x1 = field->x[0];
  fo[0].y1 = field->y[0];
  fo[0].x2 = field->x[1];
  fo[0].y2 = field->y[1];
  fo[0].x3 = field->x[4];
  fo[0].y3 = field->y[4];
  fo[0].x4 = field->x[5];
  fo[0].y4 = field->y[5];
#if DEBUG_GRAPHIC_MAP_DRAW
  printf ("Distribution furrow\n");
  furrow_output_print (fo);
  printf ("n=%d\n", fo[0].n);
#endif
  for (i = 0; i < fo[0].n; i++)
    {
      give_rainbow_color ((fo[0].y[i] - zmin) / dif);
      furrow_output_vertex (fo, i, xx, yy);
      glVertex2f (xx[0], yy[0]);
      glVertex2f (xx[1], yy[1]);
      glVertex2f (xx[3], yy[3]);
      glVertex2f (xx[2], yy[2]);
    }

  // Drawing the irrigation furrows
  if (field->nfurrows > 0)
    {
      deltaX1 = field->x[5] - field->x[4];
      deltaY1 = field->y[5] - field->y[4];
      distance = sqrt (deltaX1 * deltaX1 + deltaY1 * deltaY1);
      width = fmin (field->si->D, 0.5 * distance / (nfurrows - 0.5));
      widthX1 = deltaX1 * width / distance;
      widthY1 = deltaY1 * width / distance;
      z = (1. - width / distance) / (nfurrows - 1);
      deltaX1 *= z;
      deltaY1 *= z;
      deltaX2 = field->x[7] - field->x[6];
      deltaY2 = field->y[7] - field->y[6];
      distance = sqrt (deltaX2 * deltaX2 + deltaY2 * deltaY2);
      width = fmin (field->si->D, 0.5 * distance / (nfurrows - 0.5));
      widthX2 = deltaX2 * width / distance;
      widthY2 = deltaY2 * width / distance;
      z = (1. - width / distance) / (nfurrows - 1);
      deltaX2 *= z;
      deltaY2 *= z;
      for (j = 0; j < nfurrows; j++)
        {
          fo[j + 1].x1 = field->x[4] + j * deltaX1;
          fo[j + 1].y1 = field->y[4] + j * deltaY1;
          fo[j + 1].x3 = fo[j + 1].x1 + widthX1;
          fo[j + 1].y3 = fo[j + 1].y1 + widthY1;
          fo[j + 1].x2 = field->x[6] + j * deltaX2;
          fo[j + 1].y2 = field->y[6] + j * deltaY2;
          fo[j + 1].x4 = fo[j + 1].x2 + widthX2;
          fo[j + 1].y4 = fo[j + 1].y2 + widthY2;
#if DEBUG_GRAPHIC_MAP_DRAW
          printf ("Irrigation furrow %d\n", j + 1);
          furrow_output_print (fo + j + 1);
#endif
          for (i = 0; i < fo[j + 1].n; i++)
            {
              give_rainbow_color ((fo[j + 1].y[i] - zmin) / dif);
              furrow_output_vertex (fo + j + 1, i, xx, yy);
              glVertex2f (xx[0], yy[0]);
              glVertex2f (xx[1], yy[1]);
              glVertex2f (xx[3], yy[3]);
              glVertex2f (xx[2], yy[2]);
            }
        }
    }

  // Drawing the recirculation furrow
  if (recirculation)
    {
      fo[n - 1].x1 = field->x[6];
      fo[n - 1].y1 = field->y[6];
      fo[n - 1].x2 = field->x[7];
      fo[n - 1].y2 = field->y[7];
      fo[n - 1].x3 = field->x[2];
      fo[n - 1].y3 = field->y[2];
      fo[n - 1].x4 = field->x[3];
      fo[n - 1].y4 = field->y[3];
#if DEBUG_GRAPHIC_MAP_DRAW
      printf ("Recirculation furrow\n");
      furrow_output_print (fo + n - 1);
#endif
      for (i = 0; i < fo[n - 1].n; i++)
        {
          give_rainbow_color ((fo[n - 1].y[i] - zmin) / dif);
          furrow_output_vertex (fo + n - 1, i, xx, yy);
          glVertex2f (xx[0], yy[0]);
          glVertex2f (xx[1], yy[1]);
          glVertex2f (xx[3], yy[3]);
          glVertex2f (xx[2], yy[2]);
        }
    }
  glEnd ();

  // Freeing the variables
  for (i = 0; i < n; ++i)
    furrow_output_destroy (fo + i);

#if DEBUG_GRAPHIC_MAP_DRAW
  printf ("debug_graphic_map_draw: end\n");
#endif
}

/**
 * \fn void graphic_map_update(GraphicMap *g)
 * \brief Function to update the furrows map.
 * \param g
 * \brief Furrows map structure.
 */
void
graphic_map_update (GraphicMap * g)
{
#if JBW_GRAPHIC == JBW_GRAPHIC_GLUT
  glutPostRedisplay ();
#endif
}

/**
 * \fn void graphic_map_new(GraphicMap *g, JBWGraphic *graphic)
 * \brief Function to open the furrows map plot.
 * \param g
 * \brief Furrows map structure.
 * \param graphic
 * \brief Graphic structure.
 */
void
graphic_map_new (GraphicMap * g, JBWGraphic * graphic)
{
  g->graphic = graphic;

  g->label = (GtkLabel *) gtk_label_new (_("Time step"));

  g->scale = (GtkScale *) gtk_scale_new_with_range
    (GTK_ORIENTATION_HORIZONTAL, 0., ny - 1, 1.);
#if JBW_GRAPHIC == JBW_GRAPHIC_GLUT
  gtk_widget_set_size_request (GTK_WIDGET (g->scale), 200, -1);
#endif
  g_signal_connect_swapped (g->scale, "value-changed",
                            (GCallback) graphic_map_update, g);

  g->combo = (GtkComboBoxText *) gtk_combo_box_text_new ();
  gtk_combo_box_text_append_text (g->combo, _("Water depth (m)"));
  gtk_combo_box_text_append_text (g->combo,
                                  _("Fertilizer concentration (kg/m³)"));
  gtk_combo_box_text_append_text (g->combo,
                                  _
                                  ("Infiltrated water mass per unit length (kg/m)"));
  gtk_combo_box_text_append_text (g->combo,
                                  _
                                  ("Infiltrated fertilizer mass per unit length (kg/m)"));
  gtk_combo_box_set_active (GTK_COMBO_BOX (g->combo), 0);
  g_signal_connect_swapped (g->combo, "changed",
                            (GCallback) graphic_map_update, g);

  g->box = (GtkGrid *) gtk_grid_new ();
  gtk_grid_attach (g->box, GTK_WIDGET (g->label), 0, 0, 1, 1);
  gtk_grid_attach (g->box, GTK_WIDGET (g->scale), 1, 0, 1, 1);
  gtk_grid_attach (g->box, GTK_WIDGET (g->combo), 2, 0, 1, 1);

  graphic_map_update (g);
}

/**
 * \fn void graphic_furrows_draw()
 * \brief Function to draw the longitudinal profile of a variable in a furrow.
 */
void
graphic_furrows_draw ()
{
  int n;
  double D;
  JBFLOAT *x = NULL, *y = NULL, *y2 = NULL, *z = NULL, *z2 = NULL, p[12];
  char buffer[512], buffer2[32];
  char *label_y[6] = {
    _("Superficial depth (m)"),
    _("Discharge (m³/s)"),
    _("Superficial level (m)"),
    _("Fertilizer concentration (kg/m³)"),
    _("Superficial water area (m²)"),
    _("Advance time (s)")
  };
  char *label_yy[6] = {
    _("Infiltrated depth (m)"),
    NULL,
    _("Bottom level (m)"),
    NULL,
    _("Infiltrated water area (m²)"),
    _("Recession time (s)")
  };
  char *label_z[6] = {
    NULL,
    NULL,
    NULL,
    NULL,
    _("Superficial fertilizer mass per unit length (kg/m)"),
    NULL
  };
  char *label_zz[6] = {
    NULL,
    NULL,
    NULL,
    NULL,
    _("Infiltrated fertilizer mass per unit length (kg/m)"),
    NULL
  };
  FILE *file;
  GraphicFurrows *g = window_plot->graphic_furrows;
#if DEBUG_GRAPHIC_FURROWS_DRAW
  printf ("graphic_furrows_draw: start\n");
  printf ("furrow=%d variable=%d step=%d\n", g->furrow, g->variable, g->step);
#endif
  g->furrow = gtk_combo_box_get_active (GTK_COMBO_BOX (g->combo_furrow));
  g->variable = gtk_combo_box_get_active (GTK_COMBO_BOX (g->combo_variable));
  g->step = gtk_range_get_value (GTK_RANGE (g->scale));
  if (g->furrow == 0)
    snprintf (buffer2, 32, _("Distribution furrow"));
  else if (recirculation)
    {
      if (g->furrow == 1)
        snprintf (buffer2, 32, _("Recirculation furrow"));
      else
        snprintf (buffer2, 32, _("Furrow %d"), g->furrow - 1);
    }
  else
    snprintf (buffer2, 32, _("Furrow %d"), g->furrow);
  snprintf (buffer, 512, "%s (t=%g s)", buffer2, g->step * time_step);
  jbw_graphic_set_title (g->graphic, buffer);
  jbw_graphic_set_xlabel (g->graphic, _("Position (m)"));
  jbw_graphic_set_ylabel (g->graphic, label_y[g->variable]);
  jbw_graphic_set_yylabel (g->graphic, label_yy[g->variable]);
  jbw_graphic_set_zlabel (g->graphic, label_z[g->variable]);
  jbw_graphic_set_zzlabel (g->graphic, label_zz[g->variable]);
  if (recirculation)
    {
      switch (g->furrow)
        {
        case 0:
          snprintf (buffer2, 32, "00b");
          D = field->sb->D;
          break;
        case 1:
          snprintf (buffer2, 32, "00c");
          D = field->sc->D;
          break;
        default:
          D = field->si->D;
          snprintf (buffer2, 32, "%03d", g->furrow - 2);
        }
    }
  else
    {
      switch (g->furrow)
        {
        case 0:
          D = field->sb->D;
          snprintf (buffer2, 32, "00b");
          break;
        default:
          D = field->si->D;
          snprintf (buffer2, 32, "%03d", g->furrow - 1);
        }
    }
  if (g->variable == 5)
    {
      snprintf (buffer, 512, "%s/%s.out", input_dir, buffer2);
      file = g_fopen (buffer, "r");
      for (n = 0; fscanf (file, FRF FRF FRF, p, p + 1, p + 2) == 3; ++n)
        {
          x = (JBFLOAT *) realloc (x, (n + 1) * sizeof (JBFLOAT));
          y = (JBFLOAT *) realloc (y, (n + 1) * sizeof (JBFLOAT));
          y2 = (JBFLOAT *) realloc (y2, (n + 1) * sizeof (JBFLOAT));
          x[n] = p[0];
          y[n] = p[1];
          y2[n] = p[2];
        }
    }
  else
    {
      snprintf (buffer, 512, "%s/%s-%03d.out", input_dir, buffer2, g->step);
#if DEBUG_GRAPHIC_FURROWS_DRAW
      printf ("filename=%s\n", buffer);
#endif
      file = g_fopen (buffer, "r");
      for (n = 0;
           fscanf (file, FRF FRF FRF FRF FRF FRF FRF FRF FRF FRF FRF FRF,
                   p, p + 1, p + 2, p + 3, p + 4, p + 5, p + 6, p + 7, p + 8,
                   p + 9, p + 10, p + 11) == 12; ++n)
        {
          x = (JBFLOAT *) realloc (x, (n + 1) * sizeof (JBFLOAT));
          y = (JBFLOAT *) realloc (y, (n + 1) * sizeof (JBFLOAT));
          x[n] = p[0];
          switch (g->variable)
            {
            case 0:
              y[n] = p[1];
              y2 = (JBFLOAT *) realloc (y2, (n + 1) * sizeof (JBFLOAT));
              y2[n] = (p[6] + p[9]) / D;
              break;
            case 1:
              y[n] = p[3];
              break;
            case 2:
              y2 = (JBFLOAT *) realloc (y2, (n + 1) * sizeof (JBFLOAT));
              y[n] = p[4];
              y2[n] = p[5];
              break;
            case 3:
              y[n] = p[7];
              break;
            case 4:
              y2 = (JBFLOAT *) realloc (y2, (n + 1) * sizeof (JBFLOAT));
              z = (JBFLOAT *) realloc (z, (n + 1) * sizeof (JBFLOAT));
              z2 = (JBFLOAT *) realloc (z2, (n + 1) * sizeof (JBFLOAT));
              y[n] = p[2];
              y2[n] = p[6] + p[9];
              z[n] = p[2] * p[7];
              z2[n] = p[8] + p[10];
              break;
            }
        }
    }
#if DEBUG_GRAPHIC_FURROWS_DRAW
  printf ("plotting\n");
#endif
  jbw_graphic_draw_lines (g->graphic, x, y, y2, z, z2, n);
#if DEBUG_GRAPHIC_FURROWS_DRAW
  printf ("freeing\n");
#endif
  free (x);
  free (y);
  free (y2);
  free (z);
  free (z2);
  fclose (file);
#if DEBUG_GRAPHIC_FURROWS_DRAW
  printf ("graphic_furrows_draw: end\n");
#endif
}

/**
 * \fn void graphic_furrows_update(GraphicFurrows *g)
 * \brief Function to read the longitudinal profile of a variable in a furrow.
 * \param g
 * \brief Furrow longitudinal profile structure.
 */
void
graphic_furrows_update (GraphicFurrows * g)
{
#if DEBUG_GRAPHIC_FURROWS_UPDATE
  printf ("graphic_furrows_update: start\n");
#endif
#if JBW_GRAPHIC == JBW_GRAPHIC_GLUT
  glutPostRedisplay ();
#endif
#if DEBUG_GRAPHIC_FURROWS_UPDATE
  printf ("graphic_furrows_update: end\n");
#endif
}

/**
 * \fn void graphic_furrows_new(GraphicFurrows *g, JBWGraphic *graphic)
 * \brief Function to open a furrow longitudinal profile of a variable.
 * \param g
 * \brief Furrow longitudinal profile structure.
 * \param graphic
 * \brief Graphic structure.
 */
void
graphic_furrows_new (GraphicFurrows * g, JBWGraphic * graphic)
{
  int i;
  char buffer[32];

#if DEBUG_GRAPHIC_FURROWS_NEW
  printf ("graphic_furrows_new: start\n");
  printf ("nfurrows=%d nsteps=%d\n", nfurrows, ny);
#endif

  g->graphic = graphic;

  g->label = (GtkLabel *) gtk_label_new (_("Time step"));

  g->scale = (GtkScale *) gtk_scale_new_with_range
    (GTK_ORIENTATION_HORIZONTAL, 0., ny - 1, 1.);
#if JBW_GRAPHIC == JBW_GRAPHIC_GLUT
  gtk_widget_set_size_request (GTK_WIDGET (g->scale), 200, -1);
#endif
  g_signal_connect_swapped (g->scale, "value-changed",
                            (GCallback) graphic_furrows_update, g);

  g->combo_furrow = (GtkComboBoxText *) gtk_combo_box_text_new ();
  gtk_combo_box_text_append_text (g->combo_furrow, _("Distribution furrow"));
  if (recirculation)
    gtk_combo_box_text_append_text (g->combo_furrow,
                                    _("Recirculation furrow"));
  for (i = 0; i < nfurrows; ++i)
    {
      snprintf (buffer, 32, _("Furrow %d"), i + 1);
      gtk_combo_box_text_append_text (g->combo_furrow, buffer);
    }
  gtk_combo_box_set_active (GTK_COMBO_BOX (g->combo_furrow), 0);
  g_signal_connect_swapped (g->combo_furrow, "changed",
                            (GCallback) graphic_furrows_update, g);

  g->combo_variable = (GtkComboBoxText *) gtk_combo_box_text_new ();
  gtk_combo_box_text_append_text (g->combo_variable, _("Water depth (m)"));
  gtk_combo_box_text_append_text (g->combo_variable,
                                  _("Water discharge (m³/s)"));
  gtk_combo_box_text_append_text (g->combo_variable, _("Level (m)"));
  gtk_combo_box_text_append_text (g->combo_variable,
                                  _("Fertilizer concentration (kg/m³)"));
  gtk_combo_box_text_append_text (g->combo_variable,
                                  _("Mass per unit length (kg/m)"));
  gtk_combo_box_text_append_text (g->combo_variable,
                                  _("Advance and recession times (s)"));
  gtk_combo_box_set_active (GTK_COMBO_BOX (g->combo_variable), 0);
  g_signal_connect_swapped (g->combo_variable, "changed",
                            (GCallback) graphic_furrows_update, g);

  g->box = (GtkGrid *) gtk_grid_new ();
  gtk_grid_attach (g->box, GTK_WIDGET (g->label), 0, 0, 1, 1);
  gtk_grid_attach (g->box, GTK_WIDGET (g->scale), 1, 0, 1, 1);
  gtk_grid_attach (g->box, GTK_WIDGET (g->combo_furrow), 2, 0, 1, 1);
  gtk_grid_attach (g->box, GTK_WIDGET (g->combo_variable), 3, 0, 1, 1);

  graphic_furrows_update (g);
#if DEBUG_GRAPHIC_FURROWS_NEW
  printf ("graphic_furrows_new: end\n");
#endif
}

/**
 * \fn void graphic_probes_draw()
 * \brief Function to draw the results of a probe.
 */
void
graphic_probes_draw ()
{
  int n, j, r, probe;
  char buffer[512];
  JBFLOAT tt, hh[numProbes], cc[numProbes], *t = NULL, *h = NULL, *c = NULL;
  FILE *file;
#if DEBUG_GRAPHIC_PROBES_DRAW
  printf ("graphic_probes_draw: start\n");
  printf ("numProbes=%d\n", numProbes);
#endif
  if (numProbes <= 0)
    return;
  probe = gtk_combo_box_get_active
    (GTK_COMBO_BOX (window_plot->graphic_probes->combo));
#if DEBUG_GRAPHIC_PROBES_DRAW
  printf ("probe=%d\n", probe);
#endif
  snprintf (buffer, 512, "%s %d", _("Probe"), probe + 1);
  jbw_graphic_set_title (window_plot->graphic, buffer);
  jbw_graphic_set_xlabel (window_plot->graphic, _("Time (s)"));
  jbw_graphic_set_ylabel (window_plot->graphic, _("Depth (m)"));
  jbw_graphic_set_zlabel (window_plot->graphic,
                          _("Fertilizer concentration (kg/m³)"));
  jbw_graphic_set_yylabel (window_plot->graphic, NULL);
  jbw_graphic_set_zzlabel (window_plot->graphic, NULL);
  snprintf (buffer, 512, "%s/probes.out", input_dir);
  file = g_fopen (buffer, "r");
  n = 0;
  do
    {
      if (fscanf (file, FRF, &tt) != 1)
        break;
      for (j = 0; j < numProbes; ++j)
        {
          r = fscanf (file, FRF FRF, hh + j, cc + j);
          if (r != 2)
            break;
        }
      ++n;
      t = (JBFLOAT *) realloc (t, n * sizeof (JBFLOAT));
      h = (JBFLOAT *) realloc (h, n * sizeof (JBFLOAT));
      c = (JBFLOAT *) realloc (c, n * sizeof (JBFLOAT));
      t[n - 1] = tt;
      h[n - 1] = hh[probe];
      c[n - 1] = cc[probe];
    }
  while (r == 2);
  fclose (file);
#if DEBUG_GRAPHIC_PROBES_DRAW
  printf ("npoints=%d\n", n);
#endif
  jbw_graphic_draw_lines (window_plot->graphic_probes->graphic,
                          t, h, NULL, c, NULL, n);
  free (t);
  free (h);
  free (c);
#if DEBUG_GRAPHIC_PROBES_DRAW
  printf ("graphic_probes_draw: end\n");
#endif
}

/**
 * \fn void graphic_probes_update(GraphicProbes *g)
 * \brief Function to update the probe results plot.
 * \param g
 * \brief Probe results plot structure.
 */
void
graphic_probes_update (GraphicProbes * g)
{
#if DEBUG_GRAPHIC_PROBES_UPDATE
  printf ("graphic_probes_update: start\n");
#endif
#if JBW_GRAPHIC == JBW_GRAPHIC_GLUT
  glutPostRedisplay ();
#endif
#if DEBUG_GRAPHIC_PROBES_UPDATE
  printf ("graphic_probes_update: end\n");
#endif
}

/**
 * \fn void graphic_probes_new(GraphicProbes *g, JBWGraphic *graphic)
 * \brief Function to open a probe results plot.
 * \param g
 * \brief Probe results plot structure.
 * \param graphic
 * \brief Graphic structure.
 */
void
graphic_probes_new (GraphicProbes * g, JBWGraphic * graphic)
{
  int i;
  char buffer[32];

#if DEBUG_GRAPHIC_PROBES_NEW
  printf ("graphic_probes_new: start\n");
  printf ("numProbes=%d\n", numProbes);
  printf ("defining graphic\n");
#endif
  g->graphic = graphic;

#if DEBUG_GRAPHIC_PROBES_NEW
  printf ("defining combo\n");
#endif
  g->combo = (GtkComboBoxText *) gtk_combo_box_text_new ();
  for (i = 0; i < numProbes; i++)
    {
      snprintf (buffer, 32, _("Probe %d"), i + 1);
      gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (g->combo), buffer);
    }
  gtk_combo_box_set_active (GTK_COMBO_BOX (g->combo), 0);
  g_signal_connect_swapped (g->combo, "changed",
                            (GCallback) graphic_probes_update, g);

  graphic_probes_update (g);
#if DEBUG_GRAPHIC_PROBES_NEW
  printf ("graphic_probes_new: end\n");
#endif
}

/**
 * \fn void window_plot_draw()
 * \brief Function to draw the results.
 */
void
window_plot_draw ()
{
#if DEBUG_WINDOW_PLOT_DRAW
  printf ("window_plot_draw: start\n");
#endif
  switch (gtk_notebook_get_current_page (window_plot->notebook))
    {
    case 0:
      graphic_map_draw ();
      break;
    case 1:
      graphic_furrows_draw ();
      break;
    default:
      graphic_probes_draw ();
    }
  jbw_graphic_draw_logo (window_plot->graphic);
#if DEBUG_WINDOW_PLOT_DRAW
  printf ("window_plot_draw: end\n");
#endif
}

/**
 * \fn void window_plot_update()
 * \brief Function to update the results plot.
 */
void
window_plot_update ()
{
#if DEBUG_WINDOW_PLOT_UPDATE
  printf ("window_plot_update: start\n");
#endif
#if JBW_GRAPHIC == JBW_GRAPHIC_GLUT
  glutPostRedisplay ();
#endif
#if DEBUG_WINDOW_PLOT_UPDATE
  printf ("window_plot_update: end\n");
#endif
}

#if JBW_GRAPHIC == JBW_GRAPHIC_GLUT
void
window_plot_response (GtkDialog * window, gint response)
{
  switch (response)
    {
    case GTK_RESPONSE_ACCEPT:
      jbw_graphic_dialog_save (window_plot->graphic);
      break;
    default:
      glutLeaveMainLoop ();
    }
}
#endif

/**
 * \fn void window_plot_new()
 * \brief Function to open a results plot.
 */
void
window_plot_new ()
{
  int i;
  WindowPlot *w = window_plot;
#if DEBUG_WINDOW_PLOT_NEW
  printf ("window_plot_new: start\n");
#endif

#if DEBUG_WINDOW_PLOT_NEW
  printf ("starting the graphic\n");
#endif
#if JBW_GRAPHIC == JBW_GRAPHIC_GLUT
  w->graphic = jbw_graphic_new (0, 6, 6, 6, window_plot_draw);
#endif
  w->graphic->logo = jbw_logo_new ("logo3.png");
  w->graphic->resize = 1;
  w->graphic->grid = 1;

#if DEBUG_WINDOW_PLOT_NEW
  printf ("trying probes_number\n");
#endif
  probes_number ();

#if DEBUG_WINDOW_PLOT_NEW
  printf ("trying furrows_number\n");
#endif
  furrows_number ();

#if DEBUG_WINDOW_PLOT_NEW
  printf ("reading time step\n");
#endif
  read_time_step (&time_step);

#if DEBUG_WINDOW_PLOT_NEW
  printf ("time_step=%g\n", time_step);
  printf ("defining map graphic\n");
#endif
  graphic_map_new (w->graphic_map, w->graphic);

#if DEBUG_WINDOW_PLOT_NEW
  printf ("defining furrows graphic\n");
#endif
  graphic_furrows_new (w->graphic_furrows, w->graphic);

#if DEBUG_WINDOW_PLOT_NEW
  printf ("defining notebook\n");
#endif
  w->notebook = (GtkNotebook *) gtk_notebook_new ();
  gtk_notebook_append_page (w->notebook,
                            GTK_WIDGET (w->graphic_map->box),
                            gtk_label_new (_("Map Furrows Representation")));
  gtk_notebook_append_page (w->notebook, GTK_WIDGET (w->graphic_furrows->box),
                            gtk_label_new (_("Furrow Representation")));

#if DEBUG_WINDOW_PLOT_NEW
  printf ("defining probes graphic\n");
#endif
  if (numProbes > 0)
    {
      graphic_probes_new (w->graphic_probes, w->graphic);
      gtk_notebook_append_page (w->notebook,
                                GTK_WIDGET (w->graphic_probes->combo),
                                gtk_label_new (_("Probes Time Evolution")));
    }
  gtk_widget_show_all (GTK_WIDGET (w->notebook));

#if DEBUG_WINDOW_PLOT_NEW
  printf ("defining dialog\n");
#endif
  w->dialog =
    (GtkDialog *) gtk_dialog_new_with_buttons (_("Plotting results"),
                                               GTK_WINDOW (windowMain),
                                               GTK_DIALOG_MODAL,
                                               _("_Save"),
                                               GTK_RESPONSE_ACCEPT,
                                               _("_Close"),
                                               GTK_RESPONSE_CLOSE, NULL);
  gtk_container_add (GTK_CONTAINER (gtk_dialog_get_content_area (w->dialog)),
                     GTK_WIDGET (w->notebook));

  g_signal_connect_after (w->notebook, "switch-page", window_plot_update,
                          NULL);

#if DEBUG_WINDOW_PLOT_NEW
  printf ("waitting signal\n");
#endif
  window_parent = (GtkWindow *) w->dialog;
#if JBW_GRAPHIC == JBW_GRAPHIC_GLUT
  gtk_widget_show_all (GTK_WIDGET (w->dialog));
  g_signal_connect (w->dialog, "response", (GCallback) window_plot_response,
                    NULL);
  glutReshapeFunc (graphic_resize);
  glutIdleFunc ((void (*)) gtk_main_iteration);
#if DEBUG_WINDOW_PLOT_NEW
  printf ("glut main loop\n");
#endif
  glutMainLoop ();
  i = 1;
#if DEBUG_WINDOW_PLOT_NEW
  printf ("glut init\n");
#endif
  glutInit (&i, NULL);
  glutInitDisplayMode (GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
#endif
#if DEBUG_WINDOW_PLOT_NEW
  printf ("destroying graphic\n");
#endif
  jbw_graphic_destroy (w->graphic);
#if DEBUG_WINDOW_PLOT_NEW
  printf ("destroying dialog\n");
#endif
  gtk_widget_destroy (GTK_WIDGET (w->dialog));

}
