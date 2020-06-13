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

/*
SURCOS - A software tool to solve irrigation and fertigation in isolated furrows
and furrow networks.

Copyright 2011-2020, Javier Burguete Tolosa.

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
 * \copyright Copyright 2011-2020, Javier Burguete Tolosa.
 */
#ifndef FIELD__H
#define FIELD__H 1

/**
 * \struct Input
 * \brief Input struct.
 */
typedef struct
{
  JBDOUBLE x; ///< X-coordinate.
  JBDOUBLE y; ///< Y-coordinate.
  JBDOUBLE t0; ///< Initial time.
  JBDOUBLE tf; ///< Final time.
  JBDOUBLE q; ///< Discharge.
  unsigned int node; ///< Number of the nearest node to the input position.
} Input;

/**
 * \struct Probe
 * \brief Probe struct.
 */
typedef struct
{
  JBDOUBLE x; ///< X-coordinate.
  JBDOUBLE y; ///< Y-coordinate.
  unsigned int node; ///< Number of the nearest node to the input position.
} Probe;

/**
 * \struct Field
 * \brief Field struct.
 */
typedef struct
{
  Furrow sb[1]; ///< Distribution furrow structure.
  Furrow sc[1]; ///< Recirculation furrow structure.
  Furrow *si;  ///< Irrigation furrow structure array.
  InitialConditions cib[1];
  ///< Distribution furrow initial conditions structure.
  InitialConditions cic[1];
  ///< Recirculation furrow initial conditions structure.
  InitialConditions cii[1];
  ///< Irrigation furrows initial conditions structure.
  Input *input; ///< Array of input structures.
  Probe *probe; ///< Array of probe structures.
  Parameters *p; ///< Array of mesh node date structures.
  JBDOUBLE x[8]; ///< Array of x-coordinates of the 8 points limiting the field.
  JBDOUBLE y[8]; ///< Array of y-coordinates of the 8 points limiting the field.
  JBDOUBLE z[8]; ///< Array of z-coordinates of the 8 points limiting the field.
  JBDOUBLE tm; ///< Time interval of data dump the results in a file.
  JBDOUBLE tf; ///< Simulation maximum total time.
  JBDOUBLE cfl;
  ///< Courant-Friedrichs-Levy number controlling the time step size.
  JBDOUBLE cpu; ///< Time of computation.
  unsigned int open; ///< 1 on recirculation furrow presence, 0 else.
  unsigned int nfurrows; ///< Irrigation furrows number.
  unsigned int nb;
  ///< Node cells number at the distribution and recirculation furrows.
  unsigned int nbi;
  ///< Node cells number between irrgation furrows at the distribution and
  ///< recirculation furrows.
  unsigned int ni; ///< Node cells number at each irrigation furrow.
  unsigned int n; ///< Total node cells number.
  unsigned int nwinputs; ///< Number of water inputs.
  unsigned int nsinputs; ///< Number of fertilizer inputs.
  unsigned int nprobes; ///< Number of probes.
} Field;

extern Field field[1];

void field_save_parameters (unsigned long int t, char *directory);
void field_save_probes (FILE * file);
void field_save_advances (char *dir);
void field_dtmax ();
void field_step (JBDOUBLE dt);
void field_parameters ();
JBDOUBLE field_input_water ();
JBDOUBLE field_input_fertilizer ();
JBDOUBLE field_uniformity_water ();
JBDOUBLE field_uniformity_fertilizer ();
JBDOUBLE field_efficiency_water ();
JBDOUBLE field_efficiency_fertilizer ();
void field_destroy ();
int field_open (char *dir);

#if !PARALLELIZING

JBDOUBLE volume_water_total (Parameters * p, unsigned int n);
JBDOUBLE volume_water_surface (Parameters * p, unsigned int n);
JBDOUBLE volume_water_soil (Parameters * p, unsigned int n);
JBDOUBLE volume_water_loss (Parameters * p, unsigned int n);
JBDOUBLE mass_fertilizer_total (Parameters * p, unsigned int n);
JBDOUBLE mass_fertilizer_surface (Parameters * p, unsigned int n);
JBDOUBLE mass_fertilizer_soil (Parameters * p, unsigned int n);
JBDOUBLE mass_fertilizer_loss (Parameters * p, unsigned int n);
JBDOUBLE mass_fertilizer_solid (Parameters * p, unsigned int n);

#else

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

/**
 * \struct Parallel
 * \brief Parallelization structure.
 */
typedef struct
{
  GMutex *mutex;
  ///< Structure to lock memory writting.
  void (*function_node) (Parameters *);
  ///< Pointer to function making parallelized nodal operations.
  void (*function_furrow) (Parameters *, unsigned int);
  ///< Pointer to function making parallelized furrow operations.
  JBDOUBLE (*function_mass) (Parameters *);
  ///< Pointer to function to calculate parallelized mass.
  Parameters *parameter;
  ///< Pointer to the mesh node parameter struct to start parallelized
  ///< operations.
  JBDOUBLE mass;
  ///< Parallelized mass calculation.
  unsigned int nthreads;
  ///< Parallelized threads number.
  unsigned int nfurrows;
  ///< Furrows number.
  unsigned int *node;
  ///< Node numbers array corresponding to each thread
  ///< (calculated in the total field mesh).
  unsigned int *furrow;
  ///< Furrow numbers array corresponding to each thread.
  unsigned int *thread;
  ///< Node numbers array corresponding to each thread
  ///< (calculated in a local region of the mesh).
} Parallel;

extern Parallel parallel[1];

void parallel_open ();
JBDOUBLE mass_parallel (Parameters * p, unsigned int n,
                        JBDOUBLE (*function) (Parameters *));

#endif

#endif
