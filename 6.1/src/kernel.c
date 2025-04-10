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
 * \file kernel.c
 * \brief Source file to define the simulation.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2011-2023, Javier Burguete Tolosa.
 */
#include "config.h"
#include "jb/src/win.h"
#include "jb/src/json.h"
#include "furrow.h"
#include "parameters.h"
#include "scheme.h"
#include "field.h"
#include "kernel.h"

#define DEBUG_KERNEL_DESTROY 0
///< Macro to debug the kernel_destroy() function.
#define DEBUG_KERNEL_STEP 0
///< Macro to debug the kernel_step() function.
#define DEBUG_KERNEL_READ 0
///< Macro to debug the kernel_read() function.
#define DEBUG_KERNEL_RUN 0
///< Macro to debug the kernel_run() function.
#define DEBUG_KERNEL 0
///< Macro to debug the kernel() function.

unsigned int type_fertilizer;
///< Type of fertilizer model.
unsigned int type_infiltration;
///< Type of infiltration model.
unsigned int type_diffusion_soil;
///< Type of soil diffusion model.
unsigned int type_beta;
///< Type of advection model.
unsigned int simulating;
///< 1 on a simulation is running, 0 else.
JBDOUBLE dt;
///< Time step size.
JBDOUBLE dtmax;
///< Maximum time step size.

void (*kernel_extern_new) ();
///< Pointer to an external function to do at simulation start.
void (*kernel_extern_step) ();
///< Pointer to an external function to do every simulation step.
void (*kernel_extern_destroy) ();
///< Pointer to an external function to do at simulation end.

/**
 * Function to save a mass in a results summary file.
 */
static void
results_mass (FILE * file,      ///< Results summary file.
              char *label,      ///< Mass label.
              double mass)      ///< Mass.
{
  char buffer[512];
  snprintf (buffer, 512, "\t\t%s = %g m\302\263\n", label, mass);
  fprintf (file, buffer);
}

/**
 * Function to save the results summary file.
 */
static void
results_save (char *name)       ///< Input file name.
{
  char buffer[512];
  FILE *file;
  char *buffer2;
  JBDOUBLE m1, m2;
  int n;

  buffer2 = g_strconcat (name, ".sum", NULL);
  file = g_fopen (buffer2, "w");
  g_free (buffer2);

  snprintf (buffer, 512, "%s = " FPL " s\n", _("Irrigation life time"), t);
  fprintf (file, buffer);
  snprintf (buffer, 512, "%s = " FPL " s\n\n", _("Calculation time"),
            field->cpu);
  fprintf (file, buffer);

  snprintf (buffer, 512, "%s\n", _("IRRIGATION QUALITY"));
  fprintf (file, buffer);
  snprintf (buffer, 512, "\t%s\n", _("Water"));
  fprintf (file, buffer);
  snprintf (buffer, 512, "\t\t%s = " FPL " %%\n", _("Uniformity"),
            field_uniformity_water (field));
  fprintf (file, buffer);
  snprintf (buffer, 512, "\t\t%s = " FPL " %%\n",
            _("Christiansen's uniformity"),
            field_uniformity_Christiansen_water (field));
  fprintf (file, buffer);
  snprintf (buffer, 512, "\t\t%s = " FPL " %%\n", _("Efficiency"),
            field_efficiency_water (field));
  fprintf (file, buffer);
  snprintf (buffer, 512, "\t%s\n", _("Fertilizer"));
  fprintf (file, buffer);
  snprintf (buffer, 512, "\t\t%s = " FPL " %%\n", _("Uniformity"),
            field_uniformity_fertilizer (field));
  fprintf (file, buffer);
  snprintf (buffer, 512, "\t\t%s = " FPL " %%\n",
            _("Christiansen's uniformity"),
            field_uniformity_Christiansen_fertilizer (field));
  fprintf (file, buffer);
  snprintf (buffer, 512, "\t\t%s = " FPL " %%\n\n", _("Efficiency"),
            field_efficiency_fertilizer (field));
  fprintf (file, buffer);

  snprintf (buffer, 512, "%s\n", _("WATER VOLUME"));
  fprintf (file, buffer);
  snprintf (buffer, 512, "\t%s\n", _("Total"));
  fprintf (file, buffer);
  m1 = field_input_water (field);
  results_mass (file, _("Input"), m1);
  m2 = volume_water_total (field->p, field->n);
  results_mass (file, _("Calculated"), m2);
  snprintf (buffer, 512, "\t\t%s = " FPL " %%\n",
            _("Conservation error"), 100. * FABSL (m2 - m1) / m1);
  fprintf (file, buffer);
  results_mass (file, _("Superficial"),
                volume_water_surface (field->p, field->n));
  results_mass (file, _("Infiltrated"), volume_water_soil (field->p, field->n));
  results_mass (file, _("Percolated"), volume_water_loss (field->p, field->n));
  if (field->nfurrows > 0)
    {
      n = field->nfurrows * field->ni;
      snprintf (buffer, 512, "\t%s\n", _("Irrigation furrows"));
      fprintf (file, buffer);
      results_mass (file, _("Total"), volume_water_total (field->p, n));
      results_mass (file, _("Superficial"), volume_water_surface (field->p, n));
      results_mass (file, _("Infiltrated"), volume_water_soil (field->p, n));
      results_mass (file, _("Percolated"), volume_water_loss (field->p, n));
      snprintf (buffer, 512, "\t%s\n", _("Distribution furrow"));
      fprintf (file, buffer);
      results_mass (file, _("Total"),
                    volume_water_total (field->p + n, field->nb));
      results_mass (file, _("Superficial"),
                    volume_water_surface (field->p + n, field->nb));
      results_mass (file, _("Infiltrated"),
                    volume_water_soil (field->p + n, field->nb));
      results_mass (file, _("Percolated"),
                    volume_water_loss (field->p + n, field->nb));
      if (field->open)
        {
          n += field->nb;
          snprintf (buffer, 512, "\t%s\n", _("Recirculation furrow"));
          fprintf (file, buffer);
          results_mass (file, _("Total"),
                        volume_water_total (field->p + n, field->nb));
          results_mass (file, _("Superficial"),
                        volume_water_surface (field->p + n, field->nb));
          results_mass (file, _("Infiltrated"),
                        volume_water_soil (field->p + n, field->nb));
          results_mass (file, _("Percolated"),
                        volume_water_loss (field->p + n, field->nb));
        }
    }
  fprintf (file, "\n");

  snprintf (buffer, 512, "%s\n", _("FERTILIZER MASS"));
  fprintf (file, buffer);
  snprintf (buffer, 512, "\t%s\n", _("Total"));
  fprintf (file, buffer);
  m1 = field_input_fertilizer (field);
  snprintf (buffer, 512, "\t\t%s = " FPL " kg\n", _("Input"), m1);
  fprintf (file, buffer);
  m2 = mass_fertilizer_total (field->p, field->n);
  snprintf (buffer, 512, "\t\t%s = " FPL " kg\n", _("Calculated"), m2);
  fprintf (file, buffer);
  snprintf (buffer, 512, "\t\t%s = " FPL " %%\n", _("Conservation error"),
            100. * FABSL (m2 - m1) / m1);
  fprintf (file, buffer);
  snprintf (buffer, 512, "\t\t%s = " FPL " kg\n", _("Superficial"),
            mass_fertilizer_surface (field->p, field->n));
  fprintf (file, buffer);
  snprintf (buffer, 512, "\t\t%s = " FPL " kg\n", _("Infiltrated"),
            mass_fertilizer_soil (field->p, field->n));
  fprintf (file, buffer);
  snprintf (buffer, 512, "\t\t%s = " FPL " kg\n", _("Percolated"),
            mass_fertilizer_loss (field->p, field->n));
  fprintf (file, buffer);
  snprintf (buffer, 512, "\t\t%s = " FPL " kg\n", _("Solid"),
            mass_fertilizer_solid (field->p, field->n));
  fprintf (file, buffer);
  if (field->nfurrows > 0)
    {
      n = field->nfurrows * field->ni;
      snprintf (buffer, 512, "\t%s\n", _("Irrigation furrows"));
      fprintf (file, buffer);
      snprintf (buffer, 512, "\t\t%s = " FPL " kg\n", _("Total"),
                mass_fertilizer_total (field->p, n));
      fprintf (file, buffer);
      snprintf (buffer, 512, "\t\t%s = " FPL " kg\n", _("Superficial"),
                mass_fertilizer_surface (field->p, n));
      fprintf (file, buffer);
      snprintf (buffer, 512, "\t\t%s = " FPL " kg\n", _("Infiltrated"),
                mass_fertilizer_soil (field->p, n));
      fprintf (file, buffer);
      snprintf (buffer, 512, "\t\t%s = " FPL " kg\n", _("Percolated"),
                mass_fertilizer_loss (field->p, n));
      fprintf (file, buffer);
      snprintf (buffer, 512, "\t\t%s = " FPL " kg\n", _("Solid"),
                mass_fertilizer_solid (field->p, n));
      fprintf (file, buffer);
      snprintf (buffer, 512, "\t%s\n", _("Distribution furrow"));
      fprintf (file, buffer);
      snprintf (buffer, 512, "\t\t%s = " FPL " kg\n", _("Total"),
                mass_fertilizer_total (field->p + n, field->nb));
      fprintf (file, buffer);
      snprintf (buffer, 512, "\t\t%s = " FPL " kg\n", _("Superficial"),
                mass_fertilizer_surface (field->p + n, field->nb));
      fprintf (file, buffer);
      snprintf (buffer, 512, "\t\t%s = " FPL " kg\n", _("Infiltrated"),
                mass_fertilizer_soil (field->p + n, field->nb));
      fprintf (file, buffer);
      snprintf (buffer, 512, "\t\t%s = " FPL " kg\n", _("Percolated"),
                mass_fertilizer_loss (field->p + n, field->nb));
      fprintf (file, buffer);
      snprintf (buffer, 512, "\t\t%s = " FPL " kg\n", _("Solid"),
                mass_fertilizer_solid (field->p + n, field->nb));
      fprintf (file, buffer);
      if (field->open)
        {
          n += field->nb;
          snprintf (buffer, 512, "\t%s\n", _("Recirculation furrow"));
          fprintf (file, buffer);
          snprintf (buffer, 512, "\t\t%s = " FPL " kg\n", _("Total"),
                    mass_fertilizer_total (field->p + n, field->nb));
          fprintf (file, buffer);
          snprintf (buffer, 512, "\t\t%s = " FPL " kg\n", _("Superficial"),
                    mass_fertilizer_surface (field->p + n, field->nb));
          fprintf (file, buffer);
          snprintf (buffer, 512, "\t\t%s = " FPL " kg\n", _("Infiltrated"),
                    mass_fertilizer_soil (field->p + n, field->nb));
          fprintf (file, buffer);
          snprintf (buffer, 512, "\t\t%s = " FPL " kg\n", _("Percolated"),
                    mass_fertilizer_loss (field->p + n, field->nb));
          fprintf (file, buffer);
          snprintf (buffer, 512, "\t\t%s = " FPL " kg\n", _("Solid"),
                    mass_fertilizer_solid (field->p + n, field->nb));
          fprintf (file, buffer);
        }
    }

  fclose (file);
}

/**
 * Function to read the data of a simulation.
 *
 * \return 0 on error, 1 on success.
 */
unsigned int
kernel_read (char *name)        ///< Input file name.
{
#if DEBUG_KERNEL_READ
  printf ("kernel_read: start\n");
  printf ("Reading field\n");
#endif
  if (!field_read (name))
    goto exit_error;

#if DEBUG_KERNEL_READ
  printf ("kernel_read: end\n");
#endif
  return 1;

exit_error:
  jb_error_show ();
  jb_error_destroy ();
#if DEBUG_KERNEL_READ
  printf ("kernel_read: end\n");
#endif
  return 0;
}

/**
 * Function to perform a simulation step.
 *
 * \return 0 on ending the simulation, 1 on a simple simulation step.
 */
static unsigned int
kernel_step (FILE * file_probes,        ///< File to save the probes data.
             unsigned int gui __attribute__((unused)))
  ///< 1 on graphical GUI, 0 on else.
{
  JBDOUBLE tmax;
#if DEBUG_KERNEL_STEP
  printf ("kernel_step: start\n");
  printf ("gui=%d\n", gui);
#endif
  tmax = fmin (field->tf, t + field->tm);
  while (t < tmax)
    {
#if DEBUG_KERNEL_STEP
      printf ("t=" FPL " tmax=" FPL "\n", t, tmax);
#endif
      field_dtmax ();
#if DEBUG_KERNEL_STEP
      printf ("dtmax=" FPL "\n", dtmax);
#endif
      if (t + dtmax > tmax)
        {
          dt = tmax - t;
          t = tmax;
        }
      else
        {
          dt = dtmax;
          t = t + dt;
        }
#if DEBUG_KERNEL_STEP
      printf ("Numerical step\n");
#endif
      field_step (dt);
#if DEBUG_KERNEL_STEP
      printf ("Field parameters\n");
#endif
      field_parameters ();
#if DEBUG_KERNEL_STEP
      printf ("Saving probes\n");
#endif
      field_save_probes (file_probes);
#if DEBUG_KERNEL_STEP
      printf ("Saving advances\n");
#endif
      if (!simulating || !volume_water_surface (field->p, field->n))
        {
#if DEBUG_KERNEL_STEP
          printf ("kernel_step: end\n");
#endif
          return 0;
        }
    }
#if DEBUG_KERNEL_STEP
  printf ("kernel_step: end\n");
#endif
  return 1;
}

/**
 * Function to run a simulation.
 */
void
kernel_run (char *name,         ///< Input file name.
            unsigned int gui,   ///< 1 on graphical GUI, 0 on else.
            unsigned int msg __attribute__((unused)))
  ///< 1 on periodic messages, 0 on else.
{
  FILE *file_probes, *file_parameters;
  char *dir, *buffer2;
  unsigned int j;

#if DEBUG_KERNEL_RUN
  JBDOUBLE ud, uds, ef, efs;
  printf ("kernel_run: start\n");
  printf ("gui=%d\n", gui);
#endif

  dir = g_path_get_dirname (name);
  field->cpu = clock ();
  field_open ();
  t = 0.;
#if PARALLELIZING
  parallel_open ();
#endif
  simulating = 1;
  if (gui)
    kernel_extern_new ();

#if DEBUG_KERNEL_RUN
  printf ("Openning field parameters\n");
#endif
  field_parameters ();
#if DEBUG_KERNEL_RUN
  printf ("Saving parameters\n");
#endif
  buffer2 = g_strconcat (name, ".prm", NULL);
  file_parameters = g_fopen (buffer2, "wb");
  g_free (buffer2);
  field_save_parameters_header (file_parameters);
  field_save_parameters (file_parameters);
#if DEBUG_KERNEL_RUN
  printf ("Opening files\n");
#endif
  buffer2 = g_strconcat (name, ".prb", NULL);
  file_probes = g_fopen (buffer2, "wb");
  g_free (buffer2);
  field_save_probes_header (file_probes);
#if DEBUG_KERNEL_RUN
  printf ("Calculating\n");
#endif
  while (t < field->tf)
    {
      j = kernel_step (file_probes, gui);
#if DEBUG_KERNEL_RUN
      printf ("Saving parameters\n");
#endif
      field_save_parameters (file_parameters);
#ifndef G_OS_WIN32
      if (msg)
        fprintf (stdout, "t=" FPL " m=" FPL " mc=" FPL "\n",
                 t,
                 volume_water_total (field->p, field->n),
                 mass_fertilizer_total (field->p, field->n));
#endif
      if (gui && simulating)
        kernel_extern_step ();
      if (!j)
        break;
    }
  if (gui && simulating)
    kernel_extern_destroy ();
  fclose (file_probes);
  fclose (file_parameters);
#if DEBUG_KERNEL_RUN
  ud = field_uniformity_water (field);
  uds = field_uniformity_fertilizer (field);
  ef = field_efficiency_water (field);
  efs = field_efficiency_fertilizer (field);
  printf ("Water uniformity: " FGL "%%\nFertilizer uniformity: " FGL "%%\n"
          "Water efficiency: " FGL "%%\nFertilizer efficiency: " FGL "%%\n",
          ud, uds, ef, efs);
#endif
  field_save_advances (name);
  results_save (name);
  g_free (dir);
  field->cpu = (clock () - field->cpu) / CLOCKS_PER_SEC;
#if DEBUG_KERNEL_RUN
  printf ("kernel_run: end\n");
#endif
}

/**
 * Function to perform a simulation.
 *
 * \return 0 on error, 1 on success.
 */
unsigned int
kernel (char *name,             ///< Input file name.
        unsigned int gui,       ///< 1 on graphical GUI, 0 on else.
        unsigned int msg __attribute__((unused)))
  ///< 1 on periodic messages, 0 on else.
{
#if DEBUG_KERNEL
  JBDOUBLE ud, uds, ef, efs;
  printf ("kernel: start\n");
#endif
  field_destroy (field);
  if (!kernel_read (name))
    return 0;
  kernel_run (name, gui, msg);
#if DEBUG_KERNEL
  printf ("kernel: end\n");
#endif
  return 1;
}
