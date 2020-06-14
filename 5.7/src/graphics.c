/*
SURCOS - A software tool to solve irrigation and fertigation in isolated furrows
and furrow networks.

Copyright 2011-2020, Javier Burguete Tolosa.

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
 * \file graphics.c
 * \brief Source file to define the result graphics.
 * \authors Javier Burguete Tolosa, Asier Lacasta Soto.
 * \copyright Copyright 2011-2020, Javier Burguete Tolosa.
 */
#include "config.h"
#include "jb/jb_win.h"
#include "furrow.h"
#include "parameters.h"
#include "scheme.h"
#include "field.h"
#include "graphics.h"
#include "mainWindow.h"

#define DEBUG_PROBES_NUMBER 0
///< Macro to debug the probes_number() function.
#define DEBUG_FURROWS_NUMBER 0
///< Macro to debug the furrows_number() function.
#define DEBUG_GRAPHIC_MAP_DRAW 0
///< Macro to debug the graphic_map_draw() function.
#define DEBUG_GRAPHIC_FURROWS_DRAW 0
///< Macro to debug the graphic_furrows_draw() function.
#define DEBUG_GRAPHIC_FURROWS_NEW 0
///< Macro to debug the graphic_furrows_new() function.
#define DEBUG_GRAPHIC_PROBES_DRAW 0
///< Macro to debug the graphic_probes_draw() function.
#define DEBUG_GRAPHIC_PROBES_NEW 0
///< Macro to debug the graphic_probes_new() function.
#define DEBUG_WINDOW_PLOT_DRAW 0
///< Macro to debug the window_plot_draw() function.
#define DEBUG_WINDOW_PLOT_DELETE 0
///< Macro to debug the window_plot_delete() function.
#define DEBUG_WINDOW_PLOT_NEW 0
///< Macro to debug the window_plot_new() function.

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

extern char *input_dir;

WindowPlot *window_plot;
///< current results WindowPlot struct pointer.
int nfurrows = -1;
///< Irrigation furrows number.
unsigned int recirculation = 1;
///< 1 on presence of recirculation furrow, 0 else.
unsigned int numProbes = 0;
///< Probes number.
unsigned int ny = 0;
///< Number of time steps with dumped data.
double time_step;
///< Time interval to dump results data.

#if HAVE_FREEGLUT

JBWGraphic *graphic;            ///< Graphic structure.

void
jbw_freeglut_draw_resize (int width, int height)
{
  jbw_graphic_resize (graphic, width, height);
  jbw_graphic_render (graphic);
}

void
jbw_freeglut_draw ()
{
  jbw_graphic_render (graphic);
}

#elif HAVE_GLFW

static void
jbw_glfw_window_close (JBWGraphic * graphic)
{
  glfwSetWindowShouldClose (graphic->window, GL_TRUE);
}

#endif

/**
 * Function to read the probes number.
 */
static void
probes_number ()
{
  char buffer[512];
  FILE *f;
#if DEBUG_PROBES_NUMBER
  printf ("probes_number: start\n");
#endif
  snprintf (buffer, 512, "%s/probe.in", input_dir);
  f = g_fopen (buffer, "r");
  fscanf (f, "%u", &numProbes);
  fclose (f);
#if DEBUG_PROBES_NUMBER
  printf ("probes: %d\n", numProbes);
  printf ("probes_number: end\n");
#endif
}

/**
 * Function to read the furrows number.
 */
static void
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
  printf ("nfurrows=%d ny=%u recirculation=%u\n", nfurrows, ny, recirculation);
  printf ("furrows_number: start\n");
#endif
}

/**
 * Function to read the time interval between file solution dumps.
 */
static void
read_time_step (double *t)
///< Pointer to the time interval between file solution dumps.
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
 * Function to set a color of the scheme.
 */
static void
give_rainbow_color (double position,
                    ///< A number between [0,1] to set the color.
                    GLfloat * c)        ///< RGB color array.
{
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
}

/**
 * Function to free the memory used by a furrow mesh geometry structure.
 */
static void
furrow_output_destroy (FurrowOutput * fo)
                       ///< Furrow mesh geometry structure.
{
  free (fo->x);
  free (fo->y);
}

/**
 * Function to print the variables of a furrow mesh geometry.
 */
#if DEBUG_GRAPHIC_MAP_DRAW
static void
furrow_output_print (FurrowOutput * fo) ///< Furrow mesh geometry structure.
{
  printf ("x1=%g y1=%g\n", fo->x1, fo->y1);
  printf ("x2=%g y2=%g\n", fo->x2, fo->y2);
  printf ("x3=%g y3=%g\n", fo->x3, fo->y3);
  printf ("x4=%g y4=%g\n", fo->x4, fo->y4);
}
#endif

/**
 * Function to read the corner coordinates of a furrow mesh node.
 */
static void
furrow_output_vertex (FurrowOutput * fo,
                      ///< Furrow mesh geometry structure.
                      double zmin,      ///< Minimum variable value.
                      double dif,       ///< Variable range.
                      GLfloat * vertex, ///< Vertex array.
                      GLushort * index) ///< Element index array.
{
  GLfloat x[4], y[4];
  GLfloat c[3];
  double imax, imin, dx1, dy1, dx2, dy2;
  unsigned int i, j, k;
  dx1 = (fo->x2 - fo->x1) / (fo->n - 1.);
  dy1 = (fo->y2 - fo->y1) / (fo->n - 1.);
  dx2 = (fo->x4 - fo->x3) / (fo->n - 1.);
  dy2 = (fo->y4 - fo->y3) / (fo->n - 1.);
  for (i = j = k = 0; i < fo->n; i++, j += 5)
    {
      give_rainbow_color ((fo->y[i] - zmin) / dif, c);
      imax = fmin (i + 0.5, fo->n - 1.);
      imin = fmax (i - 0.5, 0.);
      x[0] = fo->x1 + imin * dx1;
      y[0] = fo->y1 + imin * dy1;
      x[1] = fo->x1 + imax * dx1;
      y[1] = fo->y1 + imax * dy1;
      x[3] = fo->x3 + imin * dx2;
      y[3] = fo->y3 + imin * dy2;
      x[2] = fo->x3 + imax * dx2;
      y[2] = fo->y3 + imax * dy2;
      vertex[j] = x[0];
      vertex[j + 1] = y[0];
      memcpy (vertex + j + 2, c, 3 * sizeof (GLfloat));
      j += 5;
      vertex[j] = x[1];
      vertex[j + 1] = y[1];
      memcpy (vertex + j + 2, c, 3 * sizeof (GLfloat));
      j += 5;
      vertex[j] = x[2];
      vertex[j + 1] = y[2];
      memcpy (vertex + j + 2, c, 3 * sizeof (GLfloat));
      j += 5;
      vertex[j] = x[3];
      vertex[j + 1] = y[3];
      memcpy (vertex + j + 2, c, 3 * sizeof (GLfloat));
      index[k++] = 4 * i;
      index[k++] = 4 * i + 1;
      index[k++] = 4 * i + 2;
      index[k++] = 4 * i;
      index[k++] = 4 * i + 3;
      index[k++] = 4 * i + 2;
    }
}

/**
 * Function to read a variable of a furrow results file.
 */
static void
furrow_output_read (FurrowOutput * fo,  ///< Furrow mesh geometry structure.
                    char *name, ///< Furrow results file name.
                    unsigned int variable)      ///< Variable type.
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

/**
 * Function to draw the furrows map.
 */
static void
graphic_map_draw (WindowPlot * w)       ///< WindowPlot structure.
{
  unsigned int n = 1 + nfurrows + recirculation;
  FurrowOutput fo[n];
  char buffer[512];
  const char title[128];
  const char *label[] = {
    _("Water depth (m)"),
    _("Fertilizer concentration (kg/m³)"),
    _("Infiltrated water mass per unit length (kg/m)"),
    _("Infiltrated fertilizer mass per unit length (kg/m)")
  };
  GLfloat c[3];
  GraphicMap *g;
  JBWGraphic *graphic;
  GLfloat *vertex;
  double z, zmax, zmin, dif, initialY, deltaX1, deltaY1, deltaX2, deltaY2,
    width, distance, widthX1, widthY1, widthX2, widthY2;
  float x, y, xmin, ymin, xmax, ymax;
  GLushort *index;
  int i;
  unsigned int j, k, l, x0, y0, xf, yf, np;

#if DEBUG_GRAPHIC_MAP_DRAW
  printf ("graphic_map_draw: start\n");
#endif
  g = w->graphic_map;
  graphic = g->graphic;

  // Setting title
  g->variable = gtk_combo_box_get_active (GTK_COMBO_BOX (g->combo));
  g->step = gtk_range_get_value (GTK_RANGE (g->scale));
  snprintf ((char *) title, 128, "%s (t=%g s)",
            label[g->variable], g->step * time_step);
  jbw_graphic_set_title (graphic, title);

  // Reading the output files
  snprintf (buffer, 512, "%s/00b-%03d.out", input_dir, g->step);
  furrow_output_read (fo, buffer, g->variable);
  np = fo->n;
  for (l = 0; (int) l < nfurrows; ++l)
    {
      snprintf (buffer, 512, "%s/%03d-%03d.out", input_dir, l, g->step);
      furrow_output_read (fo + 1 + l, buffer, g->variable);
      np = jbm_max (np, fo[l + 1].n);
    }
  if (recirculation)
    {
      snprintf (buffer, 512, "%s/00c-%03d.out", input_dir, g->step);
      furrow_output_read (fo + l + 1, buffer, g->variable);
      np = jbm_max (np, fo[l + 1].n);
    }

  // Getting extreme values
  zmin = INFINITY;
  zmax = -INFINITY;
  for (l = 0; l < n; ++l)
    {
      for (j = 0; j < fo[l].n; ++j)
        {
          z = fo[l].y[j];
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
  jbw_graphic_get_display_size (graphic);
  graphic->x1 = graphic->y1 = 0;
  graphic->x2 = graphic->width;
  graphic->y2 = graphic->height;
  glViewport (0, 0, graphic->width, graphic->height);

  // Clearing the screen
  jbw_draw_clear (1.f, 1.f, 1.f, 0.f);

  // Plotting the legend
  xf = graphic->x2 - 15 * graphic->char_width;
  yf = graphic->y2 - 2 * graphic->char_height;
  x0 = MAX (64, 2 * graphic->char_width);
  y0 = MAX (21, graphic->char_height);

  x = 1.f - (2.f * strlen (graphic->str_title) * graphic->char_width)
    / graphic->width;
  y = 1.f - (2.f * graphic->char_height) / graphic->height;
  jbw_graphic_draw_text (graphic, graphic->str_title, x, y, jbw_black);
  snprintf (buffer, 32, "%.6g", zmax);
  x = ((float) xf + graphic->x2 - strlen (buffer) * graphic->char_width)
    / graphic->width - 1.f;
  y = 1.f - (4.f * graphic->char_height) / graphic->height;
  jbw_graphic_draw_text (graphic, buffer, x, y, jbw_black);
  snprintf (buffer, 32, "%.6g", zmin);
  x = ((float) xf + graphic->x2 - strlen (buffer) * graphic->char_width)
    / graphic->width - 1.f;
  y = (2.f * (y0 - graphic->char_height)) / graphic->height - 1.f,
    jbw_graphic_draw_text (graphic, buffer, x, y, jbw_black);
  glViewport (xf, y0, graphic->x2 - xf, yf - y0);
  x = (2.f * graphic->char_width) / (graphic->x2 - xf);
  j = jbm_max (np, 101);
  vertex = (GLfloat *) g_malloc (4 * 5 * sizeof (GLfloat) * j);
  index = (GLushort *) g_malloc (6 * sizeof (GLushort) * j);
  initialY = -1.;
  deltaY1 = 2. / 101.;
  for (l = j = k = 0; l < 101; l++, j += 5)
    {
      give_rainbow_color (0.01f * l, c);
      vertex[j] = -x;
      vertex[j + 1] = initialY;
      memcpy (vertex + j + 2, c, 3 * sizeof (GLfloat));
      j += 5;
      vertex[j] = x;
      vertex[j + 1] = initialY;
      memcpy (vertex + j + 2, c, 3 * sizeof (GLfloat));
      initialY += deltaY1;
      j += 5;
      vertex[j] = x;
      vertex[j + 1] = initialY;
      memcpy (vertex + j + 2, c, 3 * sizeof (GLfloat));
      j += 5;
      vertex[j] = -x;
      vertex[j + 1] = initialY;
      memcpy (vertex + j + 2, c, 3 * sizeof (GLfloat));
      index[k++] = 4 * l;
      index[k++] = 4 * l + 1;
      index[k++] = 4 * l + 2;
      index[k++] = 4 * l;
      index[k++] = 4 * l + 3;
      index[k++] = 4 * l + 2;
    }
  glUseProgram (graphic->program_2Dc);
  glUniformMatrix4fv (graphic->uniform_2Dc_matrix, 1, GL_FALSE, jbw_identity);
  jbw_graphic_draw_rectangles_color (graphic, vertex, index, 101);

  // Extreme map values
  xmax = xmin = field->x[0];
  ymax = ymin = field->y[0];
  for (l = 0; ++l < 4;)
    {
      if (field->x[l] > xmax)
        xmax = field->x[l];
      else if (field->x[l] < xmin)
        xmin = field->x[l];
      if (field->y[l] > ymax)
        ymax = field->y[l];
      else if (field->y[l] < ymin)
        ymin = field->y[l];
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
  dif = fmax (zmax - zmin, 1e-10);

  // Setting view and projection
  glViewport (x0, y0, xf, yf);
  jbw_draw_orthogonal_matrixl (graphic->uniform_2Dc_matrix, xmin, ymin,
                               xmax - xmin, ymax - ymin);

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
  furrow_output_vertex (fo, zmin, dif, vertex, index);
  jbw_graphic_draw_rectangles_color (graphic, vertex, index, fo[0].n);

  // Drawing the irrigation furrows
  if (nfurrows > 0)
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
      for (i = 1; i <= (int) nfurrows; i++)
        {
          fo[i].x1 = field->x[4] + (i - 1) * deltaX1;
          fo[i].y1 = field->y[4] + (i - 1) * deltaY1;
          fo[i].x3 = fo[i].x1 + widthX1;
          fo[i].y3 = fo[i].y1 + widthY1;
          fo[i].x2 = field->x[6] + (i - 1) * deltaX2;
          fo[i].y2 = field->y[6] + (i - 1) * deltaY2;
          fo[i].x4 = fo[i].x2 + widthX2;
          fo[i].y4 = fo[i].y2 + widthY2;
#if DEBUG_GRAPHIC_MAP_DRAW
          printf ("Irrigation furrow %d\n", i);
          furrow_output_print (fo + i);
#endif
          furrow_output_vertex (fo + i, zmin, dif, vertex, index);
          jbw_graphic_draw_rectangles_color (graphic, vertex, index, fo[i].n);
        }
    }

  // Drawing the recirculation furrow
  if (recirculation)
    {
      fo[i].x1 = field->x[6];
      fo[i].y1 = field->y[6];
      fo[i].x2 = field->x[7];
      fo[i].y2 = field->y[7];
      fo[i].x3 = field->x[2];
      fo[i].y3 = field->y[2];
      fo[i].x4 = field->x[3];
      fo[i].y4 = field->y[3];
#if DEBUG_GRAPHIC_MAP_DRAW
      printf ("Recirculation furrow\n");
      furrow_output_print (fo + i);
#endif
      furrow_output_vertex (fo + i, zmin, dif, vertex, index);
      jbw_graphic_draw_rectangles_color (graphic, vertex, index, fo[i].n);
    }

  // Freeing the variables
  for (j = 0; j < n; ++j)
    furrow_output_destroy (fo + j);
  g_free (index);
  g_free (vertex);

#if DEBUG_GRAPHIC_MAP_DRAW
  printf ("graphic_map_draw: end\n");
#endif
}

/**
 * Function to open the furrows map plot.
 */
static void
graphic_map_new (GraphicMap * g,        ///< Furrows map structure.
                 JBWGraphic * graphic)  ///< Graphic structure.
{
  g->graphic = graphic;

  g->label = (GtkLabel *) gtk_label_new (_("Time step"));

  g->scale = (GtkScale *) gtk_scale_new_with_range
    (GTK_ORIENTATION_HORIZONTAL, 0., ny - 1, 1.);
  gtk_widget_set_size_request (GTK_WIDGET (g->scale), 100, 0);
  g_signal_connect_swapped (g->scale, "value-changed",
                            (GCallback) jbw_graphic_render, g->graphic);

  g->combo = (GtkComboBoxText *) gtk_combo_box_text_new ();
  gtk_combo_box_text_append_text (g->combo, _("Water depth (m)"));
  gtk_combo_box_text_append_text (g->combo,
                                  _("Fertilizer concentration (kg/m³)"));
  gtk_combo_box_text_append_text
    (g->combo, _("Infiltrated water mass per unit length (kg/m)"));
  gtk_combo_box_text_append_text
    (g->combo, _("Infiltrated fertilizer mass per unit length (kg/m)"));
  gtk_combo_box_set_active (GTK_COMBO_BOX (g->combo), 0);
  g_signal_connect_swapped (g->combo, "changed",
                            (GCallback) jbw_graphic_render, g->graphic);

  g->box = (GtkBox *) gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_box_pack_start (g->box, GTK_WIDGET (g->label), 0, 1, 0);
  gtk_box_pack_start (g->box, GTK_WIDGET (g->scale), 1, 1, 0);
  gtk_box_pack_start (g->box, GTK_WIDGET (g->combo), 0, 1, 0);
}

/**
 * Function to draw the longitudinal profile of a variable in a furrow.
 */
static void
graphic_furrows_draw (WindowPlot * w)   ///< WindowPlot structure.
{
  char buffer[512], buffer2[32];
  const char title[512];
  const char *label_y[6] = {
    _("Superficial depth (m)"),
    _("Discharge (m³/s)"),
    _("Superficial level (m)"),
    _("Fertilizer concentration (kg/m³)"),
    _("Superficial water area (m²)"),
    _("Advance time (s)")
  };
  const char *label_yy[6] = {
    _("Infiltrated depth (m)"),
    NULL,
    _("Bottom level (m)"),
    NULL,
    _("Infiltrated water area (m²)"),
    _("Recession time (s)")
  };
  const char *label_z[6] = {
    NULL,
    NULL,
    NULL,
    NULL,
    _("Superficial fertilizer mass per unit length (kg/m)"),
    NULL
  };
  const char *label_zz[6] = {
    NULL,
    NULL,
    NULL,
    NULL,
    _("Infiltrated fertilizer mass per unit length (kg/m)"),
    NULL
  };
  JBFLOAT p[12];
  GraphicFurrows *g = w->graphic_furrows;
  JBFLOAT *x = NULL, *y = NULL, *y2 = NULL, *z = NULL, *z2 = NULL;
  FILE *file;
  double D;
  unsigned int n;
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
  snprintf ((char *) title, 512, "%s (t=%g s)", buffer2,
            (double) g->step * time_step);
  jbw_graphic_set_title (g->graphic, title);
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
  jbw_graphic_draw_lines (g->graphic, x, y, y2, z, z2, n - 1);
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
 * Function to open a furrow longitudinal profile of a variable.
 */
static void
graphic_furrows_new (GraphicFurrows * g,
///< Furrow longitudinal profile structure.
                     JBWGraphic * graphic)      ///< Graphic structure.
{
  char buffer[32];
  int i;

#if DEBUG_GRAPHIC_FURROWS_NEW
  printf ("graphic_furrows_new: start\n");
  printf ("nfurrows=%d nsteps=%u\n", nfurrows, ny);
#endif

  g->graphic = graphic;

  g->label = (GtkLabel *) gtk_label_new (_("Time step"));

  g->scale = (GtkScale *) gtk_scale_new_with_range
    (GTK_ORIENTATION_HORIZONTAL, 0., ny - 1, 1.);
  gtk_widget_set_size_request (GTK_WIDGET (g->scale), 100, 0);
  g_signal_connect_swapped (g->scale, "value-changed",
                            (GCallback) jbw_graphic_render, g->graphic);

  g->combo_furrow = (GtkComboBoxText *) gtk_combo_box_text_new ();
  gtk_combo_box_text_append_text (g->combo_furrow, _("Distribution furrow"));
  if (recirculation)
    gtk_combo_box_text_append_text (g->combo_furrow, _("Recirculation furrow"));
  for (i = 0; i < nfurrows; ++i)
    {
      snprintf (buffer, 32, _("Furrow %d"), i + 1);
      gtk_combo_box_text_append_text (g->combo_furrow, buffer);
    }
  gtk_combo_box_set_active (GTK_COMBO_BOX (g->combo_furrow), 0);
  g_signal_connect_swapped (g->combo_furrow, "changed",
                            (GCallback) jbw_graphic_render, g->graphic);

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
                            (GCallback) jbw_graphic_render, g->graphic);

  g->box = (GtkBox *) gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_box_pack_start (g->box, GTK_WIDGET (g->label), 0, 1, 0);
  gtk_box_pack_start (g->box, GTK_WIDGET (g->scale), 1, 1, 0);
  gtk_box_pack_start (g->box, GTK_WIDGET (g->combo_furrow), 0, 1, 0);
  gtk_box_pack_start (g->box, GTK_WIDGET (g->combo_variable), 0, 1, 0);

#if DEBUG_GRAPHIC_FURROWS_NEW
  printf ("graphic_furrows_new: end\n");
#endif
}

/**
 * Function to draw the results of a probe.
 */
static void
graphic_probes_draw (WindowPlot * w)    ///< WindowPlot structure.
{
  JBFLOAT hh[numProbes], cc[numProbes];
  char buffer[512];
  const char title[512];
  JBFLOAT *t = NULL, *h = NULL, *c = NULL;
  FILE *file;
  JBFLOAT tt;
  unsigned int n, j, r, probe;
#if DEBUG_GRAPHIC_PROBES_DRAW
  printf ("graphic_probes_draw: start\n");
  printf ("numProbes=%d\n", numProbes);
#endif
  if (numProbes <= 0)
    goto end_draw;
  probe = gtk_combo_box_get_active (GTK_COMBO_BOX (w->graphic_probes->combo));
#if DEBUG_GRAPHIC_PROBES_DRAW
  printf ("probe=%u\n", probe);
#endif
  snprintf ((char *) title, 512, "%s %u", _("Probe"), probe + 1);
  jbw_graphic_set_title (w->graphic, title);
  jbw_graphic_set_xlabel (w->graphic, _("Time (s)"));
  jbw_graphic_set_ylabel (w->graphic, _("Depth (m)"));
  jbw_graphic_set_zlabel (w->graphic, _("Fertilizer concentration (kg/m³)"));
  jbw_graphic_set_yylabel (w->graphic, NULL);
  jbw_graphic_set_zzlabel (w->graphic, NULL);
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
  jbw_graphic_draw_lines (w->graphic_probes->graphic,
                          t, h, NULL, c, NULL, n - 1);
  free (t);
  free (h);
  free (c);
end_draw:
#if DEBUG_GRAPHIC_PROBES_DRAW
  printf ("graphic_probes_draw: end\n");
#endif
  return;
}

/**
 * Function to open a probe results plot.
 */
static void
graphic_probes_new (GraphicProbes * g,  ///< Probe results plot structure.
                    JBWGraphic * graphic)       ///< Graphic structure.
{
  char buffer[32];
  unsigned int i;

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
                            (GCallback) jbw_graphic_render, g->graphic);
#if DEBUG_GRAPHIC_PROBES_NEW
  printf ("graphic_probes_new: end\n");
#endif
}

/**
 * Function to draw the results.
 */
static void
window_plot_draw ()             ///< WindowPlot structure.
{
#if DEBUG_WINDOW_PLOT_DRAW
  printf ("window_plot_draw: start\n");
#endif
  switch (gtk_notebook_get_current_page (window_plot->notebook))
    {
    case 0:
#if DEBUG_WINDOW_PLOT_DRAW
      printf ("graphic 0\n");
#endif
      graphic_furrows_draw (window_plot);
      break;
    case 1:
#if DEBUG_WINDOW_PLOT_DRAW
      printf ("graphic 1\n");
#endif
      graphic_map_draw (window_plot);
      break;
    default:
#if DEBUG_WINDOW_PLOT_DRAW
      printf ("graphic 2\n");
#endif
      graphic_probes_draw (window_plot);
    }
#if DEBUG_WINDOW_PLOT_DRAW
  printf ("draw logo\n");
#endif
  jbw_graphic_draw_logo (window_plot->graphic);
#if DEBUG_WINDOW_PLOT_DRAW
  printf ("window_plot_draw: end\n");
#endif
}

/**
 * Function to free the memory used by a resuslts plot.
 */
void
window_plot_destroy (WindowPlot * w)    ///< Results plot structure.
{
  gtk_widget_destroy (GTK_WIDGET (w->window));
}

/**
 * Function to exit when deleting the window plot.
 */
static void
window_plot_delete (MainWindow * w)     ///< Main window structure.
{
#if DEBUG_WINDOW_PLOT_DELETE
  printf ("window_plot_delete: start\n");
  printf ("qutting graphic main loop\n");
#endif
  jbw_main_loop_quit ();
#if DEBUG_WINDOW_PLOT_DELETE
  printf ("destroying main window\n");
#endif
  if (w->plotted)
    jbw_graphic_delete (w->plot->graphic);
  w->plotted = 0;
  gtk_widget_destroy (GTK_WIDGET (w->window));
#if DEBUG_WINDOW_PLOT_DELETE
  printf ("window_plot_delete: end\n");
#endif
}

/**
 * Function to open a results plot.
 */
void
window_plot_new (WindowPlot * w)        ///< Results plot structure.
{
#if DEBUG_WINDOW_PLOT_NEW
  printf ("window_plot_new: start\n");
#endif

#if DEBUG_WINDOW_PLOT_NEW
  printf ("starting the graphic\n");
#endif
  window_plot = w;
  w->graphic = jbw_graphic_new (6, 6, 6, window_plot_draw, "SURCOS");
  jbw_graphic_set_logo (w->graphic, "logo3.png");
#if HAVE_GTKGLAREA
  jbw_graphic_show (w->graphic);
  w->id_destroy_graphic
    = g_signal_connect_swapped (w->graphic->window, "destroy",
                                (GCallback) window_plot_delete, main_window);
#else
  jbw_graphic_init (w->graphic);
#if HAVE_FREEGLUT
  graphic = w->graphic;
#endif
#endif

#if DEBUG_WINDOW_PLOT_UPDATE
  printf ("trying probes_number\n");
#endif
  probes_number ();

#if DEBUG_WINDOW_PLOT_UPDATE
  printf ("trying furrows_number\n");
#endif
  furrows_number ();

#if DEBUG_WINDOW_PLOT_UPDATE
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
  gtk_notebook_append_page (w->notebook, GTK_WIDGET (w->graphic_furrows->box),
                            gtk_label_new (_("Furrow representation")));
  gtk_notebook_append_page (w->notebook,
                            GTK_WIDGET (w->graphic_map->box),
                            gtk_label_new (_("Map furrows representation")));
  if (numProbes > 0)
    {
#if DEBUG_WINDOW_PLOT_NEW
      printf ("defining probes graphic\n");
#endif
      graphic_probes_new (w->graphic_probes, w->graphic);
      gtk_notebook_append_page (w->notebook,
                                GTK_WIDGET (w->graphic_probes->combo),
                                gtk_label_new (_("Probes time evolution")));
    }
  jbw_signal_connect_swapped_after (w->notebook, "switch-page",
                                    (GCallback) jbw_graphic_render, w->graphic);

#if DEBUG_WINDOW_PLOT_NEW
  printf ("defining save button\n");
#endif
  w->button_save = (GtkButton *) gtk_button_new_with_label (_("Save"));
  gtk_button_set_image (w->button_save,
                        gtk_image_new_from_icon_name
                        ("document-save", GTK_ICON_SIZE_LARGE_TOOLBAR));
  gtk_button_set_image_position (w->button_save, GTK_POS_TOP);
  gtk_widget_set_tooltip_text (GTK_WIDGET (w->button_save), _("Save graph"));
  g_signal_connect_swapped (w->button_save, "clicked",
                            (GCallback) jbw_graphic_dialog_save, w->graphic);

#if DEBUG_WINDOW_PLOT_NEW
  printf ("defining box\n");
#endif
  w->box = (GtkBox *) gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_box_pack_start (w->box, GTK_WIDGET (w->notebook), 1, 1, 0);
  gtk_box_pack_start (w->box, GTK_WIDGET (w->button_save), 0, 1, 0);

#if DEBUG_WINDOW_PLOT_NEW
  printf ("defining header bar\n");
#endif
  w->bar = (GtkHeaderBar *) gtk_header_bar_new ();
  gtk_header_bar_set_title (w->bar, _("Graph"));
  gtk_header_bar_set_subtitle (w->bar, _("Set the result graph options"));
  gtk_header_bar_set_show_close_button (w->bar, 1);
  w->logo = (GtkImage *) gtk_image_new_from_file ("logo3.png");
  gtk_header_bar_pack_start (w->bar, GTK_WIDGET (w->logo));

#if DEBUG_WINDOW_PLOT_NEW
  printf ("defining window\n");
#endif
  w->window = (GtkWindow *) gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_titlebar (w->window, GTK_WIDGET (w->bar));
  gtk_container_add (GTK_CONTAINER (w->window), GTK_WIDGET (w->box));
  w->id_destroy_window
    = g_signal_connect_swapped (w->window, "destroy",
                                (GCallback) window_plot_delete, main_window);
  gtk_widget_show_all (GTK_WIDGET (w->window));

#if DEBUG_WINDOW_PLOT_NEW
  printf ("window_plot_new: end\n");
#endif
}
