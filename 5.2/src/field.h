/*
SURCOS - A program to simulate furrow fertigation.

Copyright 2011-2013, Javier Burguete Tolosa.

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
 * \file field.h
 * \brief Header file to define the field data.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2011-2013, Javier Burguete Tolosa.
 */
#ifndef FIELD__H
#define FIELD__H 1

#include "scheme.h"

/**
 * \struct _Input
 * \brief Input struct.
 */
struct _Input
{
/**
 * \var x
 * \brief X-coordinate.
 * \var y
 * \brief Y-coordinate.
 * \var t0
 * \brief Initial time.
 * \var tf
 * \brief Final time.
 * \var q
 * \brief Discharge.
 * \var node
 * \brief Number of the nearest node to the input position.
 */
	JBDOUBLE x, y, t0, tf, q;
	unsigned int node;
};

/**
 * \typedef Input
 */
typedef struct _Input Input;

/**
 * \struct _Probe
 * \brief Probe struct.
 */
struct _Probe
{
/**
 * \var x
 * \brief X-coordinate.
 * \var y
 * \brief Y-coordinate.
 * \var node
 * \brief Number of the nearest node to the input position.
 */
	JBDOUBLE x, y;
	unsigned int node;
};

/**
 * \typedef Probe
 */
typedef struct _Probe Probe;

/**
 * \struct _Field
 * \brief Field struct.
 */
struct _Field
{
/**
 * \var sb
 * \brief Distribution furrow structure.
 * \var sc
 * \brief Recirculation furrow structure.
 * \var si
 * \brief Irrigation furrow structure array.
 * \var cib
 * \brief Distribution furrow initial conditions structure.
 * \var cic
 * \brief Recirculation furrow initial conditions structure.
 * \var cii
 * \brief Irrigation furrows initial conditions structure.
 * \var input
 * \brief Array of input structures.
 * \var probe
 * \brief Array of probe structures.
 * \var p
 * \brief Array of mesh node date structures.
 * \var x
 * \brief Array of x-coordinates of the 8 points limiting the field.
 * \var y
 * \brief Array of y-coordinates of the 8 points limiting the field.
 * \var z
 * \brief Array of z-coordinates of the 8 points limiting the field.
 * \var tm
 * \brief Time interval of data dump the results in a file.
 * \var tf
 * \brief Simulation maximum total time.
 * \var cfl
 * \brief Courant-Friedrichs-Levy number controlling the time step size.
 * \var cpu
 * \brief Time of computation.
 * \var open
 * \brief 1 on recirculation furrow presence, 0 else.
 * \var nfurrows
 * \brief Irrigation furrows number.
 * \var nb
 * \brief Node cells number at the distribution and recirculation furrows.
 * \var nbi
 * \brief Node cells number between irrgation furrows at the distribution and
 * 	recirculation furrows.
 * \var ni
 * \brief Node cells number at each irrigation furrow.
 * \var n
 * \brief Total node cells number.
 * \var nwinputs
 * \brief Number of water inputs.
 * \var nsinputs
 * \brief Number of fertilizer inputs.
 * \var nprobes
 * \brief Number of probes.
 */
	Furrow sb[1], sc[1], *si;
	InitialConditions cib[1], cic[1], cii[1];
	Input *input;
	Probe *probe;
	Parameters *p;
	JBDOUBLE x[8], y[8], z[8], tm, tf, cfl, cpu;
	unsigned int open, nfurrows, nb, nbi, ni, n, nwinputs, nsinputs, nprobes;
};

/**
 * \typedef Field
 */
typedef struct _Field Field;

extern Field field[1];

void field_save_parameters(int t, char *directory);
void field_save_probes(FILE *file);
void field_save_advances(char *dir);
void field_dtmax();
void field_step(JBDOUBLE dt);
void field_parameters();
JBDOUBLE field_input_water();
JBDOUBLE field_input_fertilizer();
JBDOUBLE field_uniformity_water();
JBDOUBLE field_uniformity_fertilizer();
JBDOUBLE field_efficiency_water();
JBDOUBLE field_efficiency_fertilizer();
void field_destroy();
int field_open(char *dir, CalibrateCoefficients *calibrate);

void travel_furrows(void (*function)(Parameters*, unsigned int));
void travel_mesh(void (*function)(Parameters*));

#if !PARALLELIZING

	JBDOUBLE volume_water_total(Parameters *p, unsigned int n);
	JBDOUBLE volume_water_surface(Parameters *p, unsigned int n);
	JBDOUBLE volume_water_soil(Parameters *p, unsigned int n);
	JBDOUBLE volume_water_loss(Parameters *p, unsigned int n);
	JBDOUBLE mass_fertilizer_total(Parameters *p, unsigned int n);
	JBDOUBLE mass_fertilizer_surface(Parameters *p, unsigned int n);
	JBDOUBLE mass_fertilizer_soil(Parameters *p, unsigned int n);
	JBDOUBLE mass_fertilizer_loss(Parameters *p, unsigned int n);
	JBDOUBLE mass_fertilizer_solid(Parameters *p, unsigned int n);

#else

/**
 * \struct _Parallel
 * \brief Parallelization structure.
 */
struct _Parallel
{
/**
 * \var GMutex mutex
 * \brief Structure to lock memory writting.
 * \var function_node
 * \brief Pointer to function making parallelized nodal operations.
 * \var function_furrow
 * \brief Pointer to function making parallelized furrow operations.
 * \var function_mass
 * \brief Pointer to function to calculate parallelized mass.
 * \var Parameter parameter
 * \brief Pointer to the mesh node parameter struct to start parallelized
 *   operations.
 * \var JBDOUBLE mass
 * \brief Parallelized mass calculation.
 * \var unsigned int nthreads
 * \brief Parallelized threads number.
 * \var unsigned int nfurrows
 * \brief Furrows number.
 * \var unsigned int *node
 * \brief Node numbers array corresponding to each thread
 *   (calculated in the total field mesh).
 * \var unsigned int *furrow
 * \brief Furrow numbers array corresponding to each thread.
 * \var unsigned int *thread
 * \brief Node numbers array corresponding to each thread
 *   (calculated in a local region of the mesh).
 */
	GMutex *mutex;
	void (*function_node)(Parameters*);
	void (*function_furrow)(Parameters*, unsigned int);
	JBDOUBLE (*function_mass)(Parameters*);
	Parameters *parameter;
	JBDOUBLE mass;
	unsigned int nthreads, nfurrows, *node, *furrow, *thread;
};

/**
 * \typedef Parallel
 */
typedef struct _Parallel Parallel;

extern Parallel parallel[1];

void parallel_open();
JBDOUBLE mass_parallel
	(Parameters *p, unsigned int n, JBDOUBLE (*function)(Parameters*));

#define volume_water_total(p, n) \
	(mass_parallel(p, n, parameters_volume_water_total))
#define volume_water_surface(p, n) \
	(mass_parallel(p, n, parameters_volume_water_surface))
#define volume_water_soil(p, n) \
	(mass_parallel(p, n, parameters_volume_water_soil))
#define volume_water_loss(p, n) \
	(mass_parallel(p, n, parameters_volume_water_loss))
#define mass_fertilizer_total(p, n) \
	(mass_parallel(p, n, parameters_mass_fertilizer_total))
#define mass_fertilizer_surface(p, n) \
	(mass_parallel(p, n, parameters_mass_fertilizer_surface))
#define mass_fertilizer_soil(p, n) \
	(mass_parallel(p, n, parameters_mass_fertilizer_soil))
#define mass_fertilizer_loss(p, n) \
	(mass_parallel(p, n, parameters_mass_fertilizer_loss))
#define mass_fertilizer_solid(p, n) \
	(mass_parallel(p, n, parameters_mass_fertilizer_solid))
#endif

#endif
