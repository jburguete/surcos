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
 * \file kernel.c
 * \brief Source file to define the simulation.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2011-2020, Javier Burguete Tolosa.
 */
#include "config.h"
#include "jb/jb_win.h"
#include "furrow.h"
#include "parameters.h"
#include "scheme.h"
#include "field.h"
#include "kernel.h"
#if JBW != JBW_NO
#include "graphics.h"
#include "mainWindow.h"
#endif

#define DEBUG_KERNEL_DESTROY 0
///< Macro to debug the kernel_destroy() function.
#define DEBUG_KERNEL_STEP 0
///< Macro to debug the kernel_step() function.
#define DEBUG_KERNEL_OPEN 0
///< Macro to debug the kernel_open() function.
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

#if JBW != JBW_NO
WindowRun windowRun[1];
///< Structure to show a simulation progression dialog.
#endif

char *message = NULL;
///< Pointer to a message string.

/**
 * Function to print an error message.
 */
static void
print_error (const char *msg)   ///< Error message string.
{
  printf ("Error!\n%s\n", msg);
}

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
results_save (char *dir)        //< Directory.
{
  char buffer[512];
  FILE *file;
  JBDOUBLE m1, m2;
  int n;

  snprintf (buffer, 512, "%s/summary.out", dir);
  file = g_fopen (buffer, "w");

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
  snprintf (buffer, 512, "\t\t%s = " FPL " %%\n", _("Efficiency"),
            field_efficiency_water (field));
  fprintf (file, buffer);
  snprintf (buffer, 512, "\t%s\n", _("Fertilizer"));
  fprintf (file, buffer);
  snprintf (buffer, 512, "\t\t%s = " FPL " %%\n", _("Uniformity"),
            field_uniformity_fertilizer (field));
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

#if JBW != JBW_NO

/**
 * Function to close a simulation running.
 */
static void
window_run_close ()
{
  simulating = 0;
}

/**
 * Function to close a simulation running dialog.
 */
static void
window_run_destroy ()
{
  GMainContext *context = g_main_context_default ();
  while (g_main_context_pending (context))
    g_main_context_iteration (context, 0);
  gtk_widget_destroy (GTK_WIDGET (windowRun->dialog));
}

/**
 * Function to update a simulation running dialog.
 */
static void
window_run_update (Field * T)   //< Field data structure.
{
  char buffer[32];
  GMainContext *context = g_main_context_default ();
  gtk_progress_bar_set_fraction (windowRun->progress, t / T->tf);
  snprintf (buffer, 32, "(%.2lf/%.2lf)", (double) t, (double) T->tf);
  gtk_progress_bar_set_text (windowRun->progress, buffer);
  while (g_main_context_pending (context))
    g_main_context_iteration (context, 0);
}

/**
 * Function to open a simulation running dialog.
 */
static void
window_run_new ()
{
  char buffer[32];
  windowRun->progress = (GtkProgressBar *) gtk_progress_bar_new ();

  windowRun->dialog =
    (GtkDialog *)
    gtk_dialog_new_with_buttons (_("Running the numerical simulation ..."),
                                 main_window->window, GTK_DIALOG_MODAL,
                                 _("_Stop"), GTK_RESPONSE_CLOSE, NULL);
  gtk_window_set_position (GTK_WINDOW (windowRun->dialog),
                           GTK_WIN_POS_CENTER_ALWAYS);
  gtk_widget_set_size_request (GTK_WIDGET (windowRun->dialog), 500, 80);
  gtk_container_add (GTK_CONTAINER
                     (gtk_dialog_get_content_area (windowRun->dialog)),
                     GTK_WIDGET (windowRun->progress));
  g_signal_connect_after (windowRun->dialog, "response",
                          G_CALLBACK (window_run_destroy), NULL);
  g_signal_connect_after (windowRun->dialog, "destroy", window_run_close, NULL);
  gtk_progress_bar_set_fraction (windowRun->progress, 0.);
  snprintf (buffer, 32, "(0.0/0.0)");
  gtk_progress_bar_set_text (windowRun->progress, buffer);
  gtk_widget_show_all (GTK_WIDGET (windowRun->dialog));
  gtk_widget_show_all (gtk_dialog_get_content_area (windowRun->dialog));
}

#endif

/**
 * Function to open a simulation.
 *
 * \return 0 on error, 1 on success.
 */
unsigned int
kernel_open (char *dir,         ///< Directory where the input data files are located.
             unsigned int gui)  ///< 1 on graphical GUI, 0 on else.
{
  void (*show_error) (const char *msg);
#if DEBUG_KERNEL_OPEN
  printf ("kernel_open: start\n");
#endif
  if (gui)
    show_error = jbw_show_error;
  else
    show_error = print_error;
  g_free (message);
  message = NULL;
#if DEBUG_KERNEL_OPEN
  printf ("Reading field\n");
#endif
  if (!field_open (dir))
    goto exit_error;

#if PARALLELIZING
  parallel_open ();
#endif

#if DEBUG_KERNEL_OPEN
  printf ("kernel_open: end\n");
#endif
  return 1;

exit_error:
#if DEBUG_KERNEL_OPEN
  printf ("message=%s\n", message);
#endif
  show_error (message);
#if DEBUG_KERNEL_OPEN
  printf ("kernel_open: end\n");
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
#if JBW != JBW_NO
  GMainContext *context = g_main_context_default ();
#endif
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
#if JBW != JBW_NO
      while (gui && g_main_context_pending (context))
        g_main_context_iteration (context, 0);
#endif
    }
#if DEBUG_KERNEL_STEP
  printf ("kernel_step: end\n");
#endif
  return 1;
}

/**
 * Function to perform a simulation.
 *
 * \return 0 on error, 1 on success.
 */
unsigned int
kernel (char *dir,
        ///< Directory where the input data files are located.
        unsigned int gui,       ///< 1 on graphical GUI, 0 on else.
        unsigned int msg)       ///< 1 on periodic messages, 0 on else.
{
  char buffer[512];
  FILE *file_probes;
  unsigned long int i;
  unsigned int j;

#if DEBUG_KERNEL
  JBDOUBLE ud, uds, ef, efs;
  printf ("kernel: start\n");
  printf ("gui=%d\n", gui);
  printf ("Opening kernel\n");
#endif
  field_destroy (field);
  field->cpu = clock ();
  if (!kernel_open (dir, gui))
    return 0;

  simulating = 1;
#if JBW != JBW_NO
  if (gui)
    window_run_new ();
#endif

#if DEBUG_KERNEL
  printf ("Openning field parameters\n");
#endif
  field_parameters ();
#if DEBUG_KERNEL
  printf ("Saving parameters\n");
#endif
  field_save_parameters (0, dir);
#if DEBUG_KERNEL
  printf ("Opening files\n");
#endif
  snprintf (buffer, 512, "%s/probes.out", dir);
  file_probes = g_fopen (buffer, "w");
#if DEBUG_KERNEL
  printf ("Calculating\n");
#endif
  for (t = 0., i = 0l; t < field->tf;)
    {
      j = kernel_step (file_probes, gui);
#if DEBUG_KERNEL
      printf ("Saving parameters\n");
#endif
      field_save_parameters (++i, dir);
#ifndef G_OS_WIN32
      if (msg)
        fprintf (stdout, "t=" FPL " m=" FPL " mc=" FPL "\n",
                 t,
                 volume_water_total (field->p, field->n),
                 mass_fertilizer_total (field->p, field->n));
#endif
#if JBW != JBW_NO
      if (gui && simulating)
        window_run_update (field);
#endif
      if (!j)
        break;
    }
#if JBW != JBW_NO
  if (gui && simulating)
    window_run_destroy ();
#endif
  fclose (file_probes);
#if DEBUG_KERNEL
  ud = field_uniformity_water (field);
  uds = field_uniformity_fertilizer (field);
  ef = field_efficiency_water (field);
  efs = field_efficiency_fertilizer (field);
  printf ("Water uniformity: " FGL "%%\nFertilizer uniformity: " FGL "%%\n"
          "Water efficiency: " FGL "%%\nFertilizer efficiency: " FGL "%%\n",
          ud, uds, ef, efs);
#endif
  field_save_advances (dir);
  results_save (dir);
  field->cpu = (clock () - field->cpu) / CLOCKS_PER_SEC;
#if DEBUG_KERNEL
  printf ("kernel: end\n");
#endif
  return 1;
}
