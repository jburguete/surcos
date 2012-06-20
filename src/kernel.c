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
 * \file kernel.c
 * \brief Source file to define the simulation.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2011-2012, Javier Burguete Tolosa.
 */
#define _GNU_SOURCE
#include <time.h>
#include "jb/jb_win.h"
#include "field.h"
#include "kernel.h"

/**
 * \def DEBUG_KERNEL_DESTROY
 * \brief Macro to debug the kernel_destroy() function.
 */
#define DEBUG_KERNEL_DESTROY 0
/**
 * \def DEBUG_KERNEL_STEP
 * \brief Macro to debug the kernel_step() function.
 */
#define DEBUG_KERNEL_STEP 0
/**
 * \def DEBUG_KERNEL_OPEN
 * \brief Macro to debug the kernel_open() function.
 */
#define DEBUG_KERNEL_OPEN 0
/**
 * \def DEBUG_KERNEL
 * \brief Macro to debug the kernel() function.
 */
#define DEBUG_KERNEL 0

/**
 * \var type_fertilizer
 * \brief Type of fertilizer model.
 * \var type_infiltration
 * \brief Type of infiltration model.
 * \var type_diffusion_soil
 * \brief Type of soil diffusion model.
 * \var type_beta
 * \brief Type of advection model.
 * \var simulating
 * \brief 1 on a simulation is running, 0 else.
 * \var dt
 * \brief Time step size.
 * \var dtmax
 * \brief Maximum time step size.
 */
unsigned int type_fertilizer,type_infiltration,type_diffusion_soil,
	type_beta,simulating;
JBDOUBLE dt,dtmax;

#if JBW != JBW_NO
extern GtkWidget *windowMain;
/**
 * \var windowRun
 * \brief Structure to show a simulation progression dialog.
 */
WindowRun windowRun[1];
#endif

/**
 * \var message
 * \brief Pointer to a message string.
 * \var show_error
 * \brief Pointer to the function to show the error messages.
 */
char *message = NULL;
void (*show_error)(char *msg);

/**
 * \fn void print_error(char *msg)
 * \brief Function to print an error message.
 * \param msg
 * \brief Error message string.
 */
void print_error(char *msg)
{
	printf("Error!\n%s\n", msg);
}

#if JBW != JBW_NO

/**
 * \fn void window_run_close()
 * \brief Function to close a simulation running.
 */
void window_run_close()
{
	simulating=0;
}

/**
 * \fn void window_run_destroy()
 * \brief Function to close a simulation running dialog.
 */
void window_run_destroy()
{
	while (gtk_events_pending()) gtk_main_iteration();
	gtk_widget_destroy(GTK_WIDGET(windowRun->dialog));
}

/**
 * \fn void window_run_update(Field *T)
 * \brief Function to update a simulation running dialog.
 * \param T
 * \brief Field data structure.
 */
void window_run_update(Field *T)
{
	char buffer[32];
	gtk_progress_bar_set_fraction(windowRun->progress, t / T->tf);	
	sprintf(buffer,"(%.2f/%.2f)", t, T->tf);
	gtk_progress_bar_set_text(windowRun->progress, buffer);
	while (gtk_events_pending()) gtk_main_iteration();
}

/**
 * \fn void window_run_new()
 * \brief Function to open a simulation running dialog.
 */
void window_run_new()
{
	char buffer[32];
	windowRun->progress = (GtkProgressBar*)gtk_progress_bar_new();

	windowRun->dialog = (GtkDialog*)gtk_dialog_new_with_buttons(
		gettext("Running the numerical simulation ..."),
		GTK_WINDOW(windowMain),
		GTK_DIALOG_MODAL,
		GTK_STOCK_STOP, GTK_RESPONSE_CLOSE,
		NULL);
	gtk_window_set_position(GTK_WINDOW(windowRun->dialog),
		GTK_WIN_POS_CENTER_ALWAYS);
	gtk_widget_set_size_request(GTK_WIDGET(windowRun->dialog), 500, 80);
	gtk_container_add(
		GTK_CONTAINER(gtk_dialog_get_content_area(windowRun->dialog)),
		GTK_WIDGET(windowRun->progress));
	g_signal_connect_after(windowRun->dialog, "response",
		G_CALLBACK(window_run_destroy), NULL);
	g_signal_connect_after(windowRun->dialog, "destroy", window_run_close,
		NULL);
	gtk_progress_bar_set_fraction(windowRun->progress, 0.);	
	sprintf(buffer,"(0.0/0.0)");
	gtk_progress_bar_set_text(windowRun->progress, buffer);
	gtk_widget_show_all(GTK_WIDGET(windowRun->dialog));
	gtk_widget_show_all(gtk_dialog_get_content_area(windowRun->dialog));
}

#endif

/**
 * \fn int kernel_open(char *dir, int gui, CalibrateCoefficients *calibrate)
 * \brief Function to open a simulation.
 * \param dir
 * \brief Directory where the input data files are located.
 * \param gui
 * \brief 1 on graphical GUI, 0 on else.
 * \param calibrate
 * \brief Calibrate coefficientes structure.
 * \return 0 on error, 1 on success.
 */
int kernel_open(char *dir, int gui, CalibrateCoefficients *calibrate)
{
#if DEBUG_KERNEL_OPEN
printf("kernel_open: start\n");
#endif
	if (gui) show_error = jbw_show_error; else show_error = print_error;
	g_free(message);
	message = NULL;
#if DEBUG_KERNEL_OPEN
printf("Reading field\n");
#endif
	if (!field_open(dir, calibrate)) goto exit_error;

#if PARALLELIZING
	parallel_open();
#endif

#if DEBUG_KERNEL_OPEN
printf("kernel_open: end\n");
#endif
	return 1;

exit_error:
#if DEBUG_KERNEL_OPEN
printf("message=%s\n", message);
#endif
	show_error(message);
#if DEBUG_KERNEL_OPEN
printf("kernel_open: end\n");
#endif
	return 0;
}

/**
 * \fn int kernel_step(FILE *file_probes, int gui)
 * \brief Function to perform a simulation step.
 * \param file_probes
 * \brief File to save the probes data.
 * \param gui
 * \brief 1 on graphical GUI, 0 on else.
 * \return 0 on ending the simulation, 1 on a simple simulation step.
 */
int kernel_step(FILE *file_probes, int gui)
{
	JBDOUBLE tmax;
#if DEBUG_KERNEL_STEP
printf("kernel_step: start\n");
printf("gui=%d\n", gui);
#endif
	tmax = fmin(field->tf, t + field->tm);
	while (t < tmax)
	{
#if DEBUG_KERNEL_STEP
printf("t="FWL" tmax="FWL"\n", t, tmax);
#endif
		field_dtmax();
#if DEBUG_KERNEL_STEP
printf("dtmax="FWL"\n", dtmax);
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
printf("Numerical step\n");
#endif
		field_step(dt);
#if DEBUG_KERNEL_STEP
printf("Field parameters\n");
#endif
		field_parameters();
#if DEBUG_KERNEL_STEP
printf("Saving probes\n");
#endif
		field_save_probes(file_probes);
#if DEBUG_KERNEL_STEP
printf("Saving advances\n");
#endif
		if (!simulating || !volume_water_surface(field->p, field->n))
		{
#if DEBUG_KERNEL_STEP
printf("kernel_step: end\n");
#endif
			return 0;
		}
#if JBW != JBW_NO
		if (gui && gtk_events_pending()) gtk_main_iteration();
#endif
	}
#if DEBUG_KERNEL_STEP
printf("kernel_step: end\n");
#endif
	return 1;
}

/**
 * \fn int kernel(char *dir, int gui, CalibrateCoefficients *calibrate)
 * \brief Function to perform a simulation.
 * \param dir
 * \brief Directory where the input data files are located.
 * \param gui
 * \brief 1 on graphical GUI, 0 on else.
 * \param calibrate
 * \brief Calibrate coefficientes structure.
 * \return 0 on error, 1 on success.
 */
int kernel(char *dir, int gui, CalibrateCoefficients *calibrate)
{
	int i, j, n;
	char buffer[512];
	FILE *file_probes;

#if DEBUG_KERNEL
JBDOUBLE ud, uds, ef, efs;
printf("kernel: start\n");
printf("gui=%d\n", gui);
printf("Opening kernel\n");
#endif
//	field_destroy(field);
	field->cpu = clock();
	if (!kernel_open(dir, gui, calibrate)) return 0;

	simulating = 1;
#if JBW != JBW_NO
	if (gui) window_run_new();
#endif

#if DEBUG_KERNEL
printf("Openning field parameters\n");
#endif
	field_parameters();
#if DEBUG_KERNEL
printf("Saving parameters\n");
#endif
	field_save_parameters(0, dir);
#if DEBUG_KERNEL
printf("Opening files\n");
#endif
	snprintf(buffer, 512, "%s/probes.out", dir);
	file_probes = fopen(buffer, "w");
#if DEBUG_KERNEL
printf("Calculating\n");
#endif
	for (t=0, i=0; t<field->tf;)
	{
		j = kernel_step(file_probes, gui);
#if DEBUG_KERNEL
printf("Saving parameters\n");
#endif
		field_save_parameters(++i, dir);
		fprintf(stdout,"t="FWL" m="FWL" mc="FWL"\n",
			t,
			volume_water_total(field->p, field->n),
			mass_fertilizer_total(field->p,field->n));
#if JBW != JBW_NO
		if (gui && simulating) window_run_update(field);
#endif
		if (!j) break;
	}
#if JBW != JBW_NO
	if (gui && simulating) window_run_destroy();
#endif
	fclose(file_probes);
	n = field->n;
	if (field->nfurrows > 0)
	{
		n -= field->nb;
		if (field->open) n -= field->nb;
	}
#if DEBUG_KERNEL
ud = field_uniformity_water(field);
uds = field_uniformity_fertilizer(field);
ef = field_efficiency_water(field);
efs = field_efficiency_fertilizer(field);
printf("Water uniformity: "FGL"%%\nFertilizer uniformity: "FGL"%%\n"
"Water efficiency: "FGL"%%\nFertilizer efficiency: "FGL"%%\n",ud,uds,ef,efs);
#endif
	field_save_advances(dir);
	field->cpu = (clock() - field->cpu) / CLOCKS_PER_SEC;
#if DEBUG_KERNEL
printf("kernel: end\n");
#endif
	return 1;
}
