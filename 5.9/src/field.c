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
 * \file field.c
 * \brief Source file to define the field.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2011-2023, Javier Burguete Tolosa.
 */
#include "config.h"
#include "jb/src/jb_win.h"
#include "furrow.h"
#include "parameters.h"
#include "scheme.h"
#include "field.h"

#define FIELD_SHOW 0            ///< Macro to compile the field_show function.

#define DEBUG_PARALLEL_OPEN 0
///< Macro to debug the parallel_open function.
#define DEBUG_FIELD_READ_DATA 0
///< Macro to debug the field_read_data function.
#define DEBUG_FIELD_READ_INPUT 0
///< Macro to debug the field_read_input function.
#define DEBUG_FIELD_READ_MESH 0
///< Macro to debug the field_read_mesh function.
#define DEBUG_FIELD_MESH 0
///< Macro to debug the field_mesh function.
#define DEBUG_FIELD_DTMAX 0
///< Macro to debug the field_dtmax function.
#define DEBUG_JUNCTION_LATERAL 0
///< Macro to debug the junction_lateral function.
#define DEBUG_FIELD_JUNCTIONS 0
///< Macro to debug the field_junctions function.
#define DEBUG_FIELD_STEP 0
///< Macro to debug the field_step function.
#define DEBUG_FIELD_PARAMETERS 0
///< Macro to debug the field_parameters function.
#define DEBUG_FIELD_DESTROY 0
///< Macro to debug the field_destroy function.
#define DEBUG_FIELD_OPEN 0
///< Macro to debug the field_open function.

JBDOUBLE t;                     ///< Time.
JBDOUBLE solubility;            ///< Fertilizer solubility.
Field field[1];
///< Field struct with the field, mesh and scheme data needed to simulate.

static void travel_furrows (void (*function) (Parameters *, unsigned int));
static void travel_mesh (void (*function) (Parameters *));

/**
 * Function to read the data of an input.
 *
 * \return 0 on error, 1 on success.
 */
static int
input_read (Input * i,          ///< Input data structure.
            FILE * file)        ///< Input data file.
{
  if (fscanf (file, FRL FRL FRL FRL FRL, &i->x, &i->y, &i->t0, &i->tf, &i->q)
      != 5)
    {
      message = g_strdup (_("Bad input data"));
      return 0;
    }
  if (JB_PROTECT (i->x, INPUT_X_MIN, INPUT_X_MAX))
    {
      message = g_strconcat (_("Bad input"), " x", NULL);
      return 0;
    }
  if (JB_PROTECT (i->y, INPUT_Y_MIN, INPUT_Y_MAX))
    {
      message = g_strconcat (_("Bad input"), " y", NULL);
      return 0;
    }
  if (JB_PROTECT (i->t0, INPUT_T0_MIN, INPUT_T0_MAX))
    {
      message = g_strconcat (_("Bad input"), ": ", _("Initial time"), NULL);
      return 0;
    }
  if (JB_PROTECT (i->tf, INPUT_TF_MIN, INPUT_TF_MAX) || i->tf < i->t0)
    {
      message = g_strconcat (_("Bad input"), ": ", _("Final time"), NULL);
      return 0;
    }
  if (JB_PROTECT (i->x, INPUT_Q_MIN, INPUT_Q_MAX))
    {
      message = g_strconcat (_("Bad input"), ": ", _("Discharge"), NULL);
      return 0;
    }
  return 1;
}

/**
 * Debug function to show the characteristic of a furrows network.
 *
 * \return 0 on error, 1 on success.
 */
#if FIELD_SHOW
void
field_show (Field * T,          ///< Field structure.
            FILE * file)        ///< Input data file.
{
  unsigned int i;
  for (i = 0; i < T->nfurrows; ++i)
    {
      fprintf (file, "Furrow %d\n", i);
      furrow_show (T->si + i, file);
    }
  fprintf (file, "Furrow b\n");
  furrow_show (T->sb, file);
  if (T->nfurrows <= 0)
    return;
  if (T->open)
    {
      fprintf (file, "Furrow c\n");
      furrow_show (T->sc, file);
    }
}
#endif

#if DEBUG_FIELD_STEP
/**
 * Debug function to detect NAN numbers.
 *
 * \return 0 no NAN numbers, 1 detected a NAN number.
 */
static int
field_isnan (Field * T)         ///< Field structure.
{
  Parameters *p;
  unsigned int i;
  printf ("Field: checking if any number is NaN\nCells number=%d\n", T->n);
  for (i = 0; i < T->n; ++i)
    {
      p = T->p + i;
      if (isnan (p->A) || isnan (p->Q) || isnan (p->Ac) || isnan (p->Ai))
        {
          printf ("i=%d A=" FWF " Q=" FWF " Ac=" FWF " Ai=" FWF "\n", i, p->A,
                  p->Q, p->Ac, p->Ai);
          exit (0);
          return 1;
        }
    }
  printf ("Field: OK\n");
  return 0;
}
#endif

/**
 * Function to read a furrows network.
 *
 * \return 0 on error, 1 on success.
 */
static int
field_read_data (char *directory)       ///< Directory locating the input files.
{
  JBDOUBLE v[3];
  FILE *file;
  char *buffer, *buffer2;
  unsigned int i, j = 0;
#if DEBUG_FIELD_READ_DATA
  printf ("field_read_data: start\n");
#endif
  buffer = g_build_filename (directory, "field.in", NULL);
#if DEBUG_FIELD_READ_DATA
  printf ("file=%s\n", buffer);
#endif
  file = g_fopen (buffer, "r");
  if (!file)
    {
      message = g_strconcat (_("Unable to open the geometry data file"),
                             "\n", buffer, NULL);
      g_free (buffer);
#if DEBUG_FIELD_READ_DATA
      printf ("message=%s\n", message);
      printf ("field_read_data: end\n");
#endif
      return 0;
    }
  if (fscanf (file,
              "%u%u" FRL FRL FRL FRL FRL FRL FRL FRL FRL FRL FRL FRL FRL,
              &field->open, &field->nfurrows, &solubility,
              &field->x[0], &field->y[0], &field->z[0],
              &field->x[1], &field->y[1], &field->z[1],
              &field->x[2], &field->y[2], &field->z[2],
              &field->x[3], &field->y[3], &field->z[3]) != 15)
    {
      message = g_strdup (_("Bad data"));
      goto exit0;
    }
  if (JB_PROTECT (solubility, FIELD_SOLUBILITY_MIN, FIELD_SOLUBILITY_MAX))
    {
      message = g_strconcat (_("Bad data"), " ", _("solubility"), NULL);
      goto exit0;
    }
  if (JB_PROTECT (field->x[0], FIELD_X_MIN, FIELD_X_MAX))
    {
      message = g_strconcat (_("Bad data"), " x1", NULL);
      goto exit0;
    }
  if (JB_PROTECT (field->y[0], FIELD_Y_MIN, FIELD_Y_MAX))
    {
      message = g_strconcat (_("Bad data"), " y1", NULL);
      goto exit0;
    }
  if (JB_PROTECT (field->z[0], FIELD_Z_MIN, FIELD_Z_MAX))
    {
      message = g_strconcat (_("Bad data"), " z1", NULL);
      goto exit0;
    }
  if (JB_PROTECT (field->x[1], FIELD_X_MIN, FIELD_X_MAX))
    {
      message = g_strconcat (_("Bad data"), " x2", NULL);
      goto exit0;
    }
  if (JB_PROTECT (field->y[1], FIELD_Y_MIN, FIELD_Y_MAX))
    {
      message = g_strconcat (_("Bad data"), " y2", NULL);
      goto exit0;
    }
  if (JB_PROTECT (field->z[1], FIELD_Z_MIN, FIELD_Z_MAX))
    {
      message = g_strconcat (_("Bad data"), " z2", NULL);
      goto exit0;
    }
  if (JB_PROTECT (field->x[2], FIELD_X_MIN, FIELD_X_MAX))
    {
      message = g_strconcat (_("Bad data"), " x3", NULL);
      goto exit0;
    }
  if (JB_PROTECT (field->y[2], FIELD_Y_MIN, FIELD_Y_MAX))
    {
      message = g_strconcat (_("Bad data"), " y3", NULL);
      goto exit0;
    }
  if (JB_PROTECT (field->z[2], FIELD_Z_MIN, FIELD_Z_MAX))
    {
      message = g_strconcat (_("Bad data"), " z3", NULL);
      goto exit0;
    }
  if (JB_PROTECT (field->x[3], FIELD_X_MIN, FIELD_X_MAX))
    {
      message = g_strconcat (_("Bad data"), " x4", NULL);
      goto exit0;
    }
  if (JB_PROTECT (field->y[3], FIELD_Y_MIN, FIELD_Y_MAX))
    {
      message = g_strconcat (_("Bad data"), " y4", NULL);
      goto exit0;
    }
  if (JB_PROTECT (field->z[3], FIELD_Z_MIN, FIELD_Z_MAX))
    {
      message = g_strconcat (_("Bad data"), " z4", NULL);
      goto exit0;
    }
#if DEBUG_FIELD_READ_DATA
  printf ("nfurrows=%u\n", field->nfurrows);
  printf ("Reading distribution furrow\n");
#endif
  if (!furrow_read (field->sb, field->cib, file))
    {
      buffer2 = message;
      message = g_strconcat (_("Distribution furrow"), "\n", message, NULL);
      g_free (buffer2);
      goto exit0;
    }
  furrow_position (field->sb, field->x[0], field->y[0], field->z[0],
                   field->x[1], field->y[1], field->z[1]);
  if (field->nfurrows > 0)
    {
#if DEBUG_FIELD_READ_DATA
      printf ("Reading irrigation furrow\n");
#endif
      field->si = (Furrow *) malloc (field->nfurrows * sizeof (Furrow));
      if (!field->si)
        {
          message = g_strdup
            (_("Not enough memory to open the irrigation furrows"));
          goto exit0;
        }
      if (!furrow_read (field->si, field->cii, file))
        {
          buffer2 = message;
          message = g_strconcat (_("Irrigation furrows"), "\n", message, NULL);
          g_free (buffer2);
          goto exit0;
        }
      for (i = 0; ++i < field->nfurrows;)
        furrow_copy (field->si + i, field->si);
      if (field->open)
        {
#if DEBUG_FIELD_READ_DATA
          printf ("Reading recirculation furrow\n");
#endif
          if (!furrow_read (field->sc, field->cic, file))
            {
              buffer2 = message;
              message = g_strconcat (_("Recirculation furrow"), "\n",
                                     message, NULL);
              g_free (buffer2);
              goto exit0;
            }
          furrow_position (field->sc, field->x[2], field->y[2], field->z[2],
                           field->x[3], field->y[3], field->z[3]);
        }
    }
  v[0] = field->x[2] - field->x[0];
  v[1] = field->y[2] - field->y[0];
  v[2] = sqrt (v[0] * v[0] + v[1] * v[1]);
  v[0] /= v[2];
  v[1] /= v[2];
  field->x[4] = field->x[0] + v[0] * field->sb->D;
  field->y[4] = field->y[0] + v[1] * field->sb->D;
  if (FABSL (v[0]) > FABSL (v[1]))
    field->z[4] = field->z[0] + (field->z[2] - field->z[0])
      * (field->x[4] - field->x[0]) / (field->x[2] - field->x[0]);
  else
    field->z[4] = field->z[0] + (field->z[2] - field->z[0])
      * (field->y[4] - field->y[0]) / (field->y[2] - field->y[0]);
  if (field->nfurrows && field->open)
    {
      field->x[6] = field->x[2] - v[0] * field->sc->D;
      field->y[6] = field->y[2] - v[1] * field->sc->D;
      if (FABSL (v[0]) > FABSL (v[1]))
        field->z[6] = field->z[0] + (field->z[2] - field->z[0])
          * (field->x[6] - field->x[0]) / (field->x[2] - field->x[0]);
      else
        field->z[6] = field->z[0] + (field->z[2] - field->z[0])
          * (field->y[6] - field->y[0]) / (field->y[2] - field->y[0]);
    }
  else
    {
      field->x[6] = field->x[2];
      field->y[6] = field->y[2];
      field->z[6] = field->z[2];
    }
  v[0] = field->x[3] - field->x[1];
  v[1] = field->y[3] - field->y[1];
  v[2] = SQRTL (v[0] * v[0] + v[1] * v[1]);
  v[0] /= v[2];
  v[1] /= v[2];
  field->x[5] = field->x[1] + v[0] * field->sb->D;
  field->y[5] = field->y[1] + v[1] * field->sb->D;
  if (FABSL (v[0]) > FABSL (v[1]))
    field->z[5] = field->z[1] + (field->z[3] - field->z[1])
      * (field->x[5] - field->x[1]) / (field->x[3] - field->x[1]);
  else
    field->z[5] = field->z[1] + (field->z[3] - field->z[1])
      * (field->y[5] - field->y[1]) / (field->y[3] - field->y[1]);
  if (field->nfurrows && field->open)
    {
      field->x[7] = field->x[3] - v[0] * field->sc->D;
      field->y[7] = field->y[3] - v[1] * field->sc->D;
      if (FABSL (v[0]) > FABSL (v[1]))
        field->z[7] = field->z[1] + (field->z[3] - field->z[1])
          * (field->x[7] - field->x[1]) / (field->x[3] - field->x[1]);
      else
        field->z[7] = field->z[1] + (field->z[3] - field->z[1])
          * (field->y[7] - field->y[1]) / (field->y[3] - field->y[1]);
    }
  else
    {
      field->x[7] = field->x[3];
      field->y[7] = field->y[3];
      field->z[7] = field->z[3];
    }
#if DEBUG_FIELD_READ_DATA
  printf ("x0=%g y0=%g z0=%g\n", field->x[0], field->y[0], field->z[0]);
  printf ("x1=%g y1=%g z1=%g\n", field->x[1], field->y[1], field->z[1]);
  printf ("x2=%g y2=%g z2=%g\n", field->x[2], field->y[2], field->z[2]);
  printf ("x3=%g y3=%g z3=%g\n", field->x[3], field->y[3], field->z[3]);
  printf ("x4=%g y4=%g z4=%g\n", field->x[4], field->y[4], field->z[4]);
  printf ("x5=%g y5=%g z5=%g\n", field->x[5], field->y[5], field->z[5]);
  printf ("x6=%g y6=%g z6=%g\n", field->x[6], field->y[6], field->z[6]);
  printf ("x7=%g y7=%g z7=%g\n", field->x[7], field->y[7], field->z[7]);
#endif
  for (i = 0; i < field->nfurrows; ++i)
    furrow_position (field->si + i,
                     field->x[4] + i * (field->x[5] - field->x[4])
                     / (field->nfurrows - 1),
                     field->y[4] + i * (field->y[5] - field->y[4])
                     / (field->nfurrows - 1),
                     field->z[4] + i * (field->z[5] - field->z[4])
                     / (field->nfurrows - 1),
                     field->x[6] + i * (field->x[7] - field->x[6])
                     / (field->nfurrows - 1),
                     field->y[6] + i * (field->y[7] - field->y[6])
                     / (field->nfurrows - 1),
                     field->z[6] + i * (field->z[7] - field->z[6])
                     / (field->nfurrows - 1));
  j = 1;
exit0:
  fclose (file);
  if (!j)
    {
      buffer2 = message;
      message = g_strconcat (_("Bad data set"), "\n", message, "\n",
                             buffer, NULL);
      g_free (buffer2);
#if DEBUG_FIELD_READ_DATA
      printf ("message=%s\n", message);
#endif
    }
  g_free (buffer);
#if DEBUG_FIELD_READ_DATA
  printf ("field_read_data: end\n");
#endif
  return j;
}

/**
 * Function to read the water and fertilizer inputs.
 *
 * \return 0 on error, 1 on success.
 */
static int
field_read_input (char *directory)      ///< Directory locating the input files.
{
  FILE *file;
  char *buffer;
  unsigned int i, n;
#if DEBUG_FIELD_READ_INPUT
  printf ("field_read_input: start\n");
#endif
  buffer = g_build_filename (directory, "input.in", NULL);
  file = g_fopen (buffer, "r");
  if (!file)
    {
      message = g_strconcat (_("Unable to open the input file"), "\n",
                             buffer, NULL);
      g_free (buffer);
#if DEBUG_FIELD_READ_INPUT
      printf ("field_read_input: end\n");
#endif
      return 0;
    }
  if (fscanf (file, "%u%u", &field->nwinputs, &field->nsinputs) != 2)
    {
      message = g_strconcat (_("Bad input data"), "\n", buffer, NULL);
      goto error1;
    }
  n = field->nwinputs + field->nsinputs;
  field->input = (Input *) malloc (n * sizeof (Input));
  if (!field->input)
    {
      message =
        g_strconcat (_("Not enough memory to open the input data"), "\n",
                     buffer, NULL);
      goto error1;
    }
  for (i = 0; i < n; ++i)
    {
      if (!input_read (field->input + i, file))
        {
          message = g_strconcat (_("Bad input data"), "\n", buffer, NULL);
          goto error1;
        }
      field->input[i].node = scheme_search_node (field->p, field->n,
                                                 field->input[i].x,
                                                 field->input[i].y);
    }
  fclose (file);
  g_free (buffer);
#if DEBUG_FIELD_READ_INPUT
  printf ("field_read_input: end\n");
#endif
  return 1;

error1:
  fclose (file);
  g_free (buffer);
#if DEBUG_FIELD_READ_INPUT
  printf ("field_read_input: end\n");
#endif
  return 0;
}

/**
 * Function to read the water and fertilizer inputs.
 *
 * \return 0 on error, 1 on success.
 */
static int
field_read_mesh (char *directory)       ///< Directory locating the mesh file.
{
  FILE *file;
  char *buffer;
  unsigned int i = 0;
#if DEBUG_FIELD_READ_MESH
  printf ("field_read_mesh: start\n");
#endif
  buffer = g_build_filename (directory, "mesh.in", NULL);
  file = g_fopen (buffer, "r");
  if (!file)
    {
      message = g_strconcat (_("Unable to open the mesh data file"),
                             "\n", buffer, NULL);
      g_free (buffer);
      return 0;
    }
  if (fscanf (file, "%u", &field->nbi) != 1)
    goto exit0;
  if (field->nfurrows > 0 && fscanf (file, "%u", &field->ni) != 1)
    goto exit0;
  field->nb = jbm_max (3, jbm_max (1, field->nfurrows - 1) * field->nbi + 1);
  if (!field->nfurrows)
    field->ni = field->nb;
  i = 1;
exit0:
  fclose (file);
  if (!i)
    message = g_strconcat (_("Bad mesh data"), "\n", buffer, NULL);
  g_free (buffer);
#if DEBUG_FIELD_READ_MESH
  printf ("nfurrows=%d nb=%d ni=%d\n", field->nfurrows, field->nb, field->ni);
  printf ("field_read_mesh: end\n");
#endif
  return i;
}

/**
 * Function to read the simulation times.
 *
 * \return 0 on error, 1 on success.
 */
static int
field_read_time (char *directory)
///< Directory locating the simulation time file.
{
  FILE *file;
  char *buffer;
  unsigned int i;
  buffer = g_build_filename (directory, "times.in", NULL);
  file = g_fopen (buffer, "r");
  if (!file)
    {
      message = g_strconcat (_("Unable to open the times data file"),
                             "\n", buffer, NULL);
      g_free (buffer);
      return 0;
    }
  if (fscanf (file, FRL FRL FRL, &field->tf, &field->cfl, &field->tm) != 3)
    {
      message = g_strconcat (_("Bad times data"), "\n", buffer, NULL);
      i = 0;
    }
  else
    i = 1;
  fclose (file);
  g_free (buffer);
  return i;
}

/**
 * Function to read the models to apply.
 *
 * \return 0 on error, 1 on success.
 */
static int
field_read_model (char *directory)      ///< Directory locating the models file.
{
  FILE *file;
  char *buffer;
  unsigned int i;
  buffer = g_build_filename (directory, "model.in", NULL);
  file = g_fopen (buffer, "r");
  if (!file)
    {
      message = g_strconcat (_("Unable to open the model data file"),
                             "\n", buffer, NULL);
      g_free (buffer);
      return 0;
    }
  if (fscanf (file, "%u%u%u%u", &type_fertilizer, &type_infiltration,
              &type_diffusion_soil, &type_beta) != 4)
    {
      message = g_strconcat (_("Bad model data"), "\n", buffer, NULL);
      i = 0;
    }
  else
    i = 1;
  fclose (file);
  g_free (buffer);
  return i;
}

/**
 * Function to read the probes.
 *
 * \return 0 on error, 1 on success.
 */
static int
field_read_probe (char *directory)      ///< Directory locating the probes file.
{
  FILE *file;
  char *buffer;
  JBDOUBLE x, y;
  unsigned int i, j;
  buffer = g_build_filename (directory, "probe.in", NULL);
  file = g_fopen (buffer, "r");
  if (!file)
    {
      message = g_strconcat (_("Unable to open the probes data file"),
                             "\n", buffer, NULL);
      g_free (buffer);
      return 0;
    }
  if (fscanf (file, "%u", &field->nprobes) != 1)
    {
      j = 0;
      message = g_strconcat (_("Bad probes data"), "\n", buffer, NULL);
      goto exit0;
    }
  if (!field->nprobes)
    {
      j = 1;
      goto exit0;
    }
  field->probe = (Probe *) malloc (field->nprobes * sizeof (Probe));
  for (i = 0; i < field->nprobes; ++i)
    {
      if (fscanf (file, FRL FRL, &x, &y) != 2)
        {
          j = 0;
          message = g_strconcat (_("Bad probes data"), "\n", buffer, NULL);
          goto exit0;
        }
      field->probe[i].x = x;
      field->probe[i].y = y;
      field->probe[i].node = scheme_search_node (field->p, field->n, x, y);
    }
  j = 1;
exit0:
  fclose (file);
  g_free (buffer);
  return j;
}

/**
 * Function to save the furrow longitudinal profiles at a step time.
 */
void
field_save_parameters (unsigned long int t,     ///< Step time number.
                       char *directory) ///< Directory to save the file.
{
  char buffer[512];
  Parameters *p;
  unsigned int i;
  for (i = 0, p = field->p; i < field->nfurrows; ++i, p += field->ni)
    {
      snprintf (buffer, 512, "%s/%03u-%03lu.out", directory, i, t);
      scheme_save (p, field->ni, buffer);
    }
  snprintf (buffer, 512, "%s/00b-%03lu.out", directory, t);
  scheme_save (p, field->nb, buffer);
  if (field->nfurrows <= 0)
    return;
  if (field->open)
    {
      snprintf (buffer, 512, "%s/00c-%03lu.out", directory, t);
      scheme_save (p += field->nb, field->nb, buffer);
    }
}

/**
 * Function to save the probes data.
 */
void
field_save_probes (FILE * file) ///< File to save the probes data.
{
  unsigned int i, j;
  if (!field->nprobes)
    return;
  fprintf (file, FPL, t);
  for (i = 0; i < field->nprobes; ++i)
    {
      j = field->probe[i].node;
      if (field->p[j].A > field->p[j].Amin)
        fprintf (file, " " FWF " " FWF, field->p[j].h, field->p[j].c);
      else
        fprintf (file, " " FWF " " FWF, (JBFLOAT) 0., (JBFLOAT) 0.);
    }
  fprintf (file, "\n");
}

/**
 * Function to save the furrow advance and recesion times.
 */
void
field_save_advances (char *dir) ///< Directory to save the file.
{
  char buffer[512];
  Parameters *p;
  FILE *file;
  unsigned int i;
  for (i = 0, p = field->p; i < field->nfurrows; ++i, p += field->ni)
    {
      snprintf (buffer, 512, "%s/%03d.out", dir, i);
      file = g_fopen (buffer, "w");
      scheme_save_advance (p, field->ni, file);
      fclose (file);
    }
  snprintf (buffer, 512, "%s/00b.out", dir);
  file = g_fopen (buffer, "w");
  scheme_save_advance (p, field->nb, file);
  fclose (file);
  if (field->open)
    {
      snprintf (buffer, 512, "%s/00c.out", dir);
      file = g_fopen (buffer, "w");
      scheme_save_advance (p + field->nb, field->nb, file);
      fclose (file);
    }
}

/**
 * Function to create the mesh.
 */
static void
field_mesh ()
{
  Parameters *p;
  unsigned int i;
#if DEBUG_FIELD_MESH
  printf ("field_mesh: start\n");
  printf ("nfurrows=%d nb=%d ni=%d\n", field->nfurrows, field->nb, field->ni);
#endif
  field->n = field->nb + field->nfurrows * field->ni;
  if (field->open)
    field->n += field->nb;
#if DEBUG_FIELD_MESH
  printf ("reallocating n=%d\n", field->n);
#endif
  field->p =
    p = (Parameters *) g_realloc (field->p, field->n * sizeof (Parameters));
#if DEBUG_FIELD_MESH
  printf ("irrigation furrows\n");
#endif
  for (i = 0; i < field->nfurrows; ++i, p += field->ni)
    scheme_mesh (p, field->ni, field->si + i, field->cii);
#if DEBUG_FIELD_MESH
  printf ("distribution furrow\n");
#endif
  scheme_mesh (p, field->nb, field->sb, field->cib);
#if DEBUG_FIELD_MESH
  printf ("recirculation furrow\n");
#endif
  if (field->open)
    scheme_mesh (p += field->nb, field->nb, field->sc, field->cic);
#if DEBUG_FIELD_MESH
  printf ("ix\n");
#endif
  travel_furrows (scheme_ix);
#if DEBUG_FIELD_MESH
  printf ("field_mesh: end\n");
#endif
}

/**
 * Function to calculate the inverse of the maximum time step size in a mesh
 * node.
 */
static void
dtmax_point (Parameters * p)    ///< Mesh node parameters structure.
{
  register JBDOUBLE dt;
  dt = DTMAX (p->u, p->uc, p->dx);
  if (dt > dtmax)
    {
#if PARALLELIZING
      g_mutex_lock (parallel->mutex);
#endif
      dtmax = dt;
#if PARALLELIZING
      g_mutex_unlock (parallel->mutex);
#endif
    }
}

/**
 * Function to caluculate the maximum time step size.
 */
void
field_dtmax ()
{
  Input *input;
  Parameters *p;
  JBDOUBLE u, h;
  unsigned int i;
#if DEBUG_FIELD_DTMAX
  printf ("field_dtmax: start\n");
#endif
  dtmax = 0.;
  travel_mesh (dtmax_point);
#if DEBUG_FIELD_DTMAX
  printf ("dtmax=" FPL "\n", dtmax);
#endif
  for (i = 0; i < field->nwinputs; ++i)
    {
      input = field->input + i;
      if (input->t0 <= t && t <= input->tf && input->q > 0.)
        {
          p = field->p + input->node;
          h = parameters_depth_critical (p, input->q);
          u = input->q / parameters_area (p, h);
          dtmax = fmaxl (dtmax, DTMAX (u, u, p->dx));
        }
    }
  dtmax = field->cfl / dtmax;
#if DEBUG_FIELD_DTMAX
  printf ("dtmax=" FPL "\n", dtmax);
  printf ("field_dtmax: end\n");
#endif
}

/**
 * Function to caluculate the boundary conditions.
 */
static void
field_boundary (JBDOUBLE dt)    ///< Time step size.
{
  JBDOUBLE to, m;
  Parameters *p;
  Input *input;
  unsigned int i, n;
  to = t - dt;
  n = field->nwinputs;
  for (i = 0; i < n; ++i)
    {
      input = field->input + i;
      if (to <= input->t0)
        {
          if (t <= input->t0)
            m = 0.;
          else if (t <= input->tf)
            m = (t - input->t0) * input->q;
          else
            m = (input->tf - input->t0) * input->q;
        }
      else if (to <= input->tf)
        {
          if (t <= input->tf)
            m = dt * input->q;
          else
            m = (input->tf - to) * input->q;
        }
      else
        m = 0.;
      p = field->p + input->node;
      p->A += m / p->dx;
      parameters_depth (p);
    }
  n += field->nsinputs;
  for (; i < n; ++i)
    {
      input = field->input + i;
      if (to <= input->t0)
        {
          if (t <= input->t0)
            m = 0.;
          else if (t <= input->tf)
            m = (t - input->t0) * input->q;
          else
            m = (input->tf - input->t0) * input->q;
        }
      else if (to <= input->tf)
        {
          if (t <= input->tf)
            m = dt * input->q;
          else
            m = (input->tf - to) * input->q;
        }
      else
        m = 0.;
      p = field->p + input->node;
      p->Ac += m / p->dx;
      p->c = p->Ac / p->A;
      parameters_depth (p);
    }
  p = field->p + field->nfurrows * field->ni;
  p->Q = 0.;
  p += field->nb - 1;
  p->Q = 0.;
  if (field->open)
    {
      ++p;
      p->Q = 0.;
      p += field->nb - 1;
      p->Q = 0.;
    }
}

/**
 * Function to calculate a lateral junction.
 */
INLINE_VOID
junction_lateral (Parameters * p1,
///< Pointer to the mesh node of the main furrow.
                  Parameters * p2,
///< Pointer to the mesh node of the lateral furrow.
                  JBDOUBLE contribution,
///< Numerical contributions to the junction.
                  JBDOUBLE c)   ///< Fertilizer concentration.
{
  JBDOUBLE V, Vc, Vt, h, dh, bdx1, zdx1, bdx2, zdx2, A1, A2;
#if DEBUG_JUNCTION_LATERAL
  printf ("debug_junction_lateral: start\n");
#endif
#if JUNCTION == JUNCTION_CONSERVATIVE
  A1 = p1->A;
  A2 = p2->A;
  V = p1->A * p1->dx + p2->A * p2->dx - contribution;
  Vc = p1->Ac * p1->dx + p2->Ac * p2->dx - contribution * c;
#if DEBUG_JUNCTION_LATERAL
  printf ("V=" FPL " Vc=" FPL "\n", V, Vc);
#endif
  bdx1 = p1->s->b * p1->dx;
  zdx1 = p1->s->z * p1->dx;
  bdx2 = p2->s->b * p2->dx;
  zdx2 = p2->s->z * p2->dx;
#if DEBUG_JUNCTION_LATERAL
  printf ("h1=" FPL " h2=" FPL "\n", p1->s->h, p2->s->h);
  printf ("bdx1=" FPL " zdx1=" FPL "\n", bdx1, zdx1);
  printf ("bdx2=" FPL " zdx2=" FPL "\n", bdx2, zdx2);
#endif
  if (p1->s->h > p2->s->h)
    {
      dh = p1->s->h - p2->s->h;
      Vt = AREA (dh, bdx1, zdx1);
      if (V <= Vt)
        {
          p1->A = V / p1->dx;
          p2->A = 0.;
        }
      else
        {
          h = jbm_solve_quadratic (zdx1 + zdx2, bdx1 + bdx2 -
                                   jbm_fdbl (zdx2 * dh),
                                   dh * (zdx2 * dh - bdx2) - V, 0., INFINITY);
#if DEBUG_JUNCTION_LATERAL
          printf ("h=" FPL "\n", h);
#endif
          p1->A = AREA (h, p1->s->b, p1->s->z);
          h -= dh;
          p2->A = AREA (h, p2->s->b, p2->s->z);
        }
    }
  else
    {
      dh = p2->s->h - p1->s->h;
      Vt = AREA (dh, bdx2, zdx2);
      if (V <= Vt)
        {
          p2->A = V / p2->dx;
          p1->A = 0.;
        }
      else
        {
          h = jbm_solve_quadratic (zdx1 + zdx2, bdx1 + bdx2 -
                                   jbm_fdbl (zdx1 * dh),
                                   dh * (zdx1 * dh - bdx1) - V, 0., INFINITY);
#if DEBUG_JUNCTION_LATERAL
          printf ("h=" FPL "\n", h);
#endif
          p2->A = AREA (h, p2->s->b, p2->s->z);
          h -= dh;
          p1->A = AREA (h, p1->s->b, p1->s->z);
        }
    }
  V = p1->A * p1->dx + p2->A * p2->dx;
#if DEBUG_JUNCTION_LATERAL
  printf ("V=" FPL "\n", V);
#endif
  if (V > 0.)
    {
      Vc /= V;
      p1->Ac = p1->A * Vc;
      p2->Ac = p2->A * Vc;
    }
  /*
     if (p2->zs > p1->zs)
     {
     if (A2 > 0.) p2->Q *= p2->A / A2;
     }
     else
     {
     if (A1 > 0.) p1->Q *= p1->A / A1;
     }
   */
  if (A2 > 0.)
    p2->Q *= p2->A / A2;
  if (A1 > 0.)
    p1->Q *= p1->A / A1;
#elif JUNCTION == JUNCTION_SIMPLE
  p1->A = p2->A;
  p1->Ac = p2->Ac;
#else
#error "Unknown junction type"
#endif
#if DEBUG_JUNCTION_LATERAL
  printf ("debug_junction_lateral: end\n");
#endif
}

/**
 * Function to calculate the furrow junctions.
 */
void
field_junctions ()
{
  Parameters *p;
  unsigned int i, j, k, open, nfurrows, nb, ni, nbi;
#if DEBUG_JUNCTIONS
  printf ("field_junctions: start\n");
  printf ("nfurrows=%u\n", field->nfurrows);
#endif
  open = field->open;
  nfurrows = field->nfurrows;
  nb = field->nb;
  ni = field->ni;
  nbi = field->nbi;
  p = field->p;
  if (nfurrows == 0)
    {
      p[nb - 1].Q = 0.;
      return;
    }
  for (i = 0, k = nfurrows * ni, j = 0; j < nfurrows; i += ni, k += nbi, ++j)
    {
#if DEBUG_JUNCTIONS
      printf ("i=%u k=%u a1=" FPL " c=" FPL "\n", i, k, p[i].Qv * dt, p[i].cv);
      printf ("A1=" FPL " Ac1=" FPL " dx1=" FPL "\n", p[i].A, p[i].Ac, p[i].dx);
      printf ("A2=" FPL " Ac2=" FPL " dx2=" FPL "\n", p[k].A, p[k].Ac, p[k].dx);
#endif
      junction_lateral (p + i, p + k, p[i].Qv * dt, p[i].cv);
#if DEBUG_JUNCTIONS
      printf ("A1=" FPL " Ac1=" FPL " dx1=" FPL "\n", p[i].A, p[i].Ac, p[i].dx);
      printf ("A2=" FPL " Ac2=" FPL " dx2=" FPL "\n", p[k].A, p[k].Ac, p[k].dx);
#endif
    }
  i = nfurrows * ni + nb - 1;
  p[i].Q = 0.;
  if (!open)
    {
      for (i = ni - 1, j = 0; j < nfurrows; i += ni, ++j)
        p[i].Q = 0.;
      return;
    }
  p[++i].Q = 0.;
  p[i + nb - 1].Q = 0.;
  for (i = ni - 1, k = nfurrows * ni + nb, j = 0; j < nfurrows;
       i += ni, k += nbi, ++j)
    {
#if DEBUG_JUNCTIONS
      printf ("i=%u k=%u a1=" FPL " c=" FPL "\n", i, k, -p[i].Qv * dt, p[i].cv);
      printf ("A1=" FPL " Ac1=" FPL " dx1=" FPL "\n", p[i].A, p[i].Ac, p[i].dx);
      printf ("A2=" FPL " Ac2=" FPL " dx2=" FPL "\n", p[k].A, p[k].Ac, p[k].dx);
#endif
      junction_lateral (p + i, p + k, -p[i].Qv * dt, p[i].cv);
#if DEBUG_JUNCTIONS
      printf ("A1=" FPL " Ac1=" FPL " dx1=" FPL "\n", p[i].A, p[i].Ac, p[i].dx);
      printf ("A2=" FPL " Ac2=" FPL " dx2=" FPL "\n", p[k].A, p[k].Ac, p[k].dx);
#endif
    }
#if DEBUG_JUNCTIONS
  printf ("nfurrows=%u\n", nfurrows);
  printf ("field_junctions: end\n");
#endif
}

/**
 * Function to calculate a numerical time step.
 */
void
field_step (JBDOUBLE dt)        ///< Time step size.
{
#if DEBUG_FIELD_STEP
  printf ("field_step: start\n");
  printf ("dt=" FPL "\n", dt);
  field_isnan (field);
  printf ("mw=%g mf=%g\n", volume_water_total (field->p, field->n),
          mass_fertilizer_total (field->p, field->n));
  printf ("scheme\n");
#endif
  travel_furrows (scheme_step);
#if DEBUG_FIELD_STEP
  field_isnan (field);
  printf ("mw=%g mf=%g\n", volume_water_total (field->p, field->n),
          mass_fertilizer_total (field->p, field->n));
  printf ("junctions\n");
#endif
  field_junctions ();
#if DEBUG_FIELD_STEP
  field_isnan (field);
  printf ("mw=%g mf=%g\n", volume_water_total (field->p, field->n),
          mass_fertilizer_total (field->p, field->n));
  printf ("boundaries\n");
#endif
  field_boundary (dt);
#if DEBUG_FIELD_STEP
  field_isnan (field);
  printf ("mw=%g mf=%g\n", volume_water_total (field->p, field->n),
          mass_fertilizer_total (field->p, field->n));
  printf ("parameters\n");
#endif
}

/**
 * Function to calculate the numerical parameters needed in a time step.
 */
void
field_parameters ()
{
#if DEBUG_FIELD_PARAMETERS
  printf ("field_parameters: start\n");
  printf ("n=%d\n", field->n);
  field_isnan (field);
  printf ("mw=%g mf=%g\n", volume_water_total (field->p, field->n),
          mass_fertilizer_total (field->p, field->n));
#endif
  travel_mesh (parameters_open);
#if DEBUG_FIELD_PARAMETERS
  field_isnan (field);
  printf ("mw=%g mf=%g\n", volume_water_total (field->p, field->n),
          mass_fertilizer_total (field->p, field->n));
  printf ("field_parameters: end\n");
#endif
}

/**
 * Function to calculate the amount of input water in a time step.
 *
 * \return Amount of input water in a time step.
 */
JBDOUBLE
field_input_water ()
{
  Input *input;
  register JBDOUBLE k;
  register unsigned int i;
  k = 0.;
  input = field->input;
  for (i = 0; i < field->nwinputs; ++i, ++input)
    {
      if (t <= input->t0);
      else if (t <= input->tf)
        k += (t - input->t0) * input->q;
      else
        k += (input->tf - input->t0) * input->q;
    }
  return k;
}

/**
 * Function to calculate the amount of input fertilizer in a time step.
 *
 * \return Amount of input ferilizer in a time step.
 */
JBDOUBLE
field_input_fertilizer ()
{
  Input *input;
  register JBDOUBLE k;
  register unsigned int i;
  k = 0.;
  input = field->input + field->nwinputs;
  for (i = 0; i < field->nsinputs; ++i, ++input)
    {
      if (t <= input->t0);
      else if (t <= input->tf)
        k += (t - input->t0) * input->q;
      else
        k += (input->tf - input->t0) * input->q;
    }
  return k;
}

/**
 * Function to calculate the uniformity of the applied water.
 *
 * \return Uniformity of the applied water.
 */
JBDOUBLE
field_uniformity_water ()
{
  unsigned int n;
  if (!field->nfurrows)
    n = field->n;
  else
    n = field->ni * field->nfurrows;
  return uniformity_water (field->p, n);
}

/**
 * Function to calculate the uniformity of the applied fertilizer.
 *
 * \return Uniformity of the applied fertilizer.
 */
JBDOUBLE
field_uniformity_fertilizer ()
{
  unsigned int n;
  if (!field->nfurrows)
    n = field->n;
  else
    n = field->ni * field->nfurrows;
  return uniformity_fertilizer (field->p, n);
}

/**
 * Function to calculate the uniformity of the applied water.
 *
 * \return Efficiency of the applied water.
 */
JBDOUBLE
field_efficiency_water ()
{
  JBDOUBLE m;
  unsigned int n;
  m = volume_water_total (field->p, field->n);
  if (m == 0.)
    return 0.;
  if (!field->nfurrows)
    n = field->n;
  else
    n = field->ni * field->nfurrows;
  return 100. * volume_water_soil (field->p, n) / m;
}

/**
 * Function to calculate the uniformity of the applied fertilizer.
 *
 * \return Efficiency of the applied fertilizer.
 */
JBDOUBLE
field_efficiency_fertilizer ()
{
  JBDOUBLE m;
  unsigned int n;
  m = mass_fertilizer_total (field->p, field->n);
  if (m == 0.)
    return 0.;
  if (!field->nfurrows)
    n = field->n;
  else
    n = field->ni * field->nfurrows;
  return 100. * mass_fertilizer_soil (field->p, n) / m;
}

/**
 * Function to free the memory used by a field structure.
 */
void
field_destroy ()
{
#if DEBUG_FIELD_DESTROY
  printf ("field_destroy: start\n");
#endif
  free (field->si);
  g_free (field->p);
  free (field->input);
  free (field->probe);
#if DEBUG_FIELD_DESTROY
  printf ("field_destroy: end\n");
#endif
}

/**
 * Function to open a furrows network.
 *
 * \return 0 on error, 1 on success.
 */
int
field_open (char *dir)          ///< Directory locating the input files.
{
#if DEBUG_FIELD_OPEN
  printf ("field_open: start\n");
#endif
  field->si = NULL;
  field->p = NULL;
  field->input = NULL;
  field->probe = NULL;
#if DEBUG_FIELD_OPEN
  printf ("Reading set\n");
#endif
  if (!field_read_data (dir))
    goto exit_error;
#if DEBUG_FIELD_OPEN
  printf ("Reading time\n");
#endif
  if (!field_read_time (dir))
    goto exit_error;
#if DEBUG_FIELD_OPEN
  printf ("Reading mesh\n");
#endif
  if (!field_read_mesh (dir))
    goto exit_error;
#if DEBUG_FIELD_OPEN
  printf ("Reading model\n");
#endif
  if (!field_read_model (dir))
    goto exit_error;
#if DEBUG_FIELD_OPEN
  printf ("Creating mesh\n");
#endif
  field_mesh ();
#if DEBUG_FIELD_OPEN
  printf ("Reading input\n");
#endif
  if (!field_read_input (dir))
    goto exit_error;
#if DEBUG_FIELD_OPEN
  printf ("Reading probe\n");
#endif
  if (!field_read_probe (dir))
    goto exit_error;
#if DEBUG_FIELD_OPEN
  printf ("field_open: end\n");
#endif
  return 1;

exit_error:
#if DEBUG_FIELD_OPEN
  printf ("Destroying field\n");
#endif
  field_destroy ();
#if DEBUG_FIELD_OPEN
  printf ("field_open: end\n");
#endif
  return 0;
}

#if PARALLELIZING

Parallel parallel[1];
///< Parallel struct to save the parallelization data.

/**
 * Function to open the data needed to parallelize the simulation.
 */
void
parallel_open ()
{
  unsigned int i;
#if DEBUG_PARALLEL_OPEN
  printf ("parallel_open: start\n");
#endif
  parallel->nthreads = jb_cores ();
#if DEBUG_PARALLEL_OPEN
  printf ("nthreads=%d\n", parallel->nthreads);
#endif
  parallel->node =
    (unsigned int *) malloc ((parallel->nthreads + 1) * sizeof (unsigned int));
  parallel->furrow =
    (unsigned int *) malloc ((parallel->nthreads + 1) * sizeof (unsigned int));
  parallel->nfurrows = field->nfurrows + 1;
  if (field->open)
    ++parallel->nfurrows;
#if DEBUG_PARALLEL_OPEN
  printf ("nfurrows=%d\n", parallel->nfurrows);
#endif
  for (i = 0; i <= parallel->nthreads; ++i)
    {
      parallel->node[i] = (field->n * i) / parallel->nthreads;
      parallel->furrow[i] = jbm_min (parallel->nfurrows,
                                     (field->n * i) / (parallel->nthreads *
                                                       field->ni));
#if DEBUG_PARALLEL_OPEN
      printf ("i=%d node=%d furrow=%d\n", i, parallel->node[i],
              parallel->furrow[i]);
#endif
    }
#if DEBUG_PARALLEL_OPEN
  printf ("parallel_open: end\n");
#endif
}

/**
 * Function to perform a parallelized thread in mesh nodes.
 */
static void
travel_thread (void *data)      ///< Pointer to the data needed by the thread.
{
  unsigned int thread, i;
  thread = (unsigned int) (size_t) data;
  for (i = parallel->node[thread]; i < parallel->node[thread + 1]; ++i)
    parallel->function_node (field->p + i);
}

/**
 * Function to perform a parallelized thread in some furrows.
 */
static void
travel_furrows_thread (void *data)
///< Pointer to the data needed by the thread.
{
  Parameters *p;
  unsigned int thread, i;
  thread = (unsigned int) (size_t) data;
  for (i = parallel->furrow[thread]; i < parallel->furrow[thread + 1]; ++i)
    {
      if (i < field->nfurrows)
        {
          p = field->p + i * field->ni;
          parallel->function_furrow (p, field->ni);
        }
      else
        {
          p = field->p + field->nfurrows * field->ni
            + (i - field->nfurrows) * field->nb;
          parallel->function_furrow (p, field->nb);
        }
    }
}

/**
 * Function to perform parallelized threads in a mesh.
 */
static void
travel_mesh (void (*function) (Parameters *))
///< Pointer to the function to perform parallelized in the mesh.
{
  GThread *thread[parallel->nthreads];
  GError *error;
  unsigned int i;
  parallel->function_node = function;
  for (i = 0; i < parallel->nthreads; ++i)
    thread[i] = g_thread_create
      ((void (*)) travel_thread, (void *) (size_t) i, TRUE, &error);
  for (i = 0; i < parallel->nthreads; ++i)
    g_thread_join (thread[i]);
}

/**
 * Function to perform parallelized threads in furrows.
 */
static void
travel_furrows (void (*function) (Parameters *, unsigned int))
///< Pointer to the function to perform parallelized in every furrow.
{
  GThread *thread[parallel->nthreads];
  GError *error;
  unsigned int i;
  parallel->function_furrow = function;
  for (i = 0; i < parallel->nthreads; ++i)
    thread[i] = g_thread_create
      ((void (*)) travel_furrows_thread, (void *) (size_t) i, TRUE, &error);
  for (i = 0; i < parallel->nthreads; ++i)
    g_thread_join (thread[i]);
}

/**
 * Function to calculate parallelized the mass in a thread.
 */
static void
mass_thread (void *data)        ///< Pointer to the data needed by the thread.
{
  Parameters *p;
  register JBDOUBLE mass;
  unsigned int i, thread;
  thread = (unsigned int) (size_t) data;
  p = parallel->parameter;
  mass = 0.;
  for (i = parallel->thread[thread]; i < parallel->thread[thread + 1]; ++i)
    mass += parallel->function_mass (p + i);
  g_mutex_lock (parallel->mutex);
  parallel->mass += mass;
  g_mutex_unlock (parallel->mutex);
}

/**
 * Function to calculate parallelized the mass in a mesh.
 *
 * \return Total mass.
 */
JBDOUBLE
mass_parallel (Parameters * p,  ///< Array of the node parameters data.
               unsigned int n,  ///< Number of nodes.
               JBDOUBLE (*function) (Parameters *))
///< Pointer to the function to calculate the mass in a node.
{
  GThread *thread[parallel->nthreads];
  unsigned int nthread[parallel->nthreads];
  GError *error;
  unsigned int i;
  parallel->mass = 0;
  parallel->function_mass = function;
  parallel->parameter = p;
  parallel->thread = nthread;
  for (i = 0; i <= parallel->nthreads; ++i)
    nthread[i] = (n * i) / parallel->nthreads;
  for (i = 0; i < parallel->nthreads; ++i)
    thread[i] = g_thread_create
      ((void (*)) mass_thread, (void *) (size_t) i, TRUE, &error);
  for (i = 0; i < parallel->nthreads; ++i)
    g_thread_join (thread[i]);
  return parallel->mass;
}

#else

/**
 * Function to perform a function in the furrows.
 */
static void
travel_furrows (void (*function) (Parameters *, unsigned int))
///< Pointer to the function to perform in every furrow.
{
  Parameters *p;
  unsigned int i;
  p = field->p;
  for (i = 0; i < field->nfurrows; ++i, p += field->ni)
    function (p, field->ni);
  function (p, field->nb);
  if (field->nfurrows == 0)
    return;
  if (field->open)
    function (p += field->nb, field->nb);
}

/**
 * Function to perform a function in a mesh.
 */
static void
travel_mesh (void (*function) (Parameters *))
///< Pointer to the function to perform at every mesh node.
{
  Parameters *p;
  unsigned int i, n;
  p = field->p;
  n = field->n;
  for (i = 0; i < n; ++i)
    function (p++);
}

/**
 * Function to calculate the total water volume in a mesh.
 *
 * \return Total water volume.
 */
JBDOUBLE
volume_water_total (Parameters * p,     ///< Array of the node parameters data.
                    unsigned int n)     ///< Number of nodes.
{
  register JBDOUBLE k;
  register unsigned int i;
  for (i = 0, k = 0.; i < n; ++i, ++p)
    k += parameters_volume_water_total (p);
  return k;
}

/**
 * Function to calculate the surface water volume in a mesh.
 *
 * \return Surface water volume.
 */
JBDOUBLE
volume_water_surface (Parameters * p,   ///< Array of the node parameters data.
                      unsigned int n)   ///< Number of nodes.
{
  register JBDOUBLE k;
  register unsigned int i;
  for (i = 0, k = 0.; i < n; ++i, ++p)
    k += parameters_volume_water_surface (p);
  return k;
}

/**
 * Function to calculate the soil water volume in a mesh.
 *
 * \return Soil water volume.
 */
JBDOUBLE
volume_water_soil (Parameters * p,      ///< Array of the node parameters data.
                   unsigned int n)      ///< Number of nodes.
{
  register JBDOUBLE k;
  register unsigned int i;
  for (i = 0, k = 0.; i < n; ++i, ++p)
    k += parameters_volume_water_soil (p);
  return k;
}

/**
 * Function to calculate the losses of water volume in a mesh.
 *
 * \return Losses of water volume.
 */
JBDOUBLE
volume_water_loss (Parameters * p,      ///< Array of the node parameters data.
                   unsigned int n)      ///< Number of nodes.
{
  register JBDOUBLE k;
  register unsigned int i;
  for (i = 0, k = 0.; i < n; ++i, ++p)
    k += parameters_volume_water_loss (p);
  return k;
}

/**
 * Function to calculate the total fertilizer mass in a mesh.
 *
 * \return Total fertilizer mass.
 */
JBDOUBLE
mass_fertilizer_total (Parameters * p,  ///< Array of the node parameters data.
                       unsigned int n)  ///< Number of nodes.
{
  register JBDOUBLE k;
  register unsigned int i;
  for (i = 0, k = 0.; i < n; ++i, ++p)
    k += parameters_mass_fertilizer_total (p);
  return k;
}

/**
 * Function to calculate the surface fertilizer mass in a mesh.
 *
 * \return Surface fertilizer mass.
 */
JBDOUBLE
mass_fertilizer_surface (Parameters * p,        ///< Array of the node parameters data.
                         unsigned int n)        ///< Number of nodes.
{
  register JBDOUBLE k;
  register unsigned int i;
  for (i = 0, k = 0.; i < n; ++i, ++p)
    k += parameters_mass_fertilizer_surface (p);
  return k;
}

/**
 * Function to calculate the soil fertilizer mass in a mesh.
 *
 * \return Soil fertilizer mass.
 */
JBDOUBLE
mass_fertilizer_soil (Parameters * p,   ///< Array of the node parameters data.
                      unsigned int n)   ///< Number of nodes.
{
  register unsigned int i;
  register JBDOUBLE k;
  for (i = 0, k = 0.; i < n; ++i, ++p)
    k += parameters_mass_fertilizer_soil (p);
  return k;
}

/**
 * Function to calculate the losses of fertilizer mass in a mesh.
 *
 * \return Losses of fertilizer mass.
 */
JBDOUBLE
mass_fertilizer_loss (Parameters * p,   ///< Array of the node parameters data.
                      unsigned int n)   ///< Number of nodes.
{
  register unsigned int i;
  register JBDOUBLE k;
  for (i = 0, k = 0.; i < n; ++i, ++p)
    k += parameters_mass_fertilizer_loss (p);
  return k;
}

/**
 * Function to calculate the solid fertilizer mass in a mesh.
 *
 * \return Solid fertilizer mass.
 */
JBDOUBLE
mass_fertilizer_solid (Parameters * p,  ///< Array of the node parameters data.
                       unsigned int n)  ///< Number of nodes.
{
  register JBDOUBLE k;
  register unsigned int i;
  for (i = 0, k = 0.; i < n; ++i, ++p)
    k += parameters_mass_fertilizer_solid (p);
  return k;
}

#endif
