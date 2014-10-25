/*
SURCOS - A program to simulate furrow fertigation.

Copyright 2011-2014, Javier Burguete Tolosa.

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
 * \file furrow.h
 * \brief Header file to define the furrow data.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2011-2014, Javier Burguete Tolosa.
 */
#ifndef FURROW__H
#define FURROW__H 1

#include "jb/jb_math.h"
#include "config.h"

/**
 * \enum TypeFriction
 * \brief Enumeration to define the friction model type.
 * \var TYPE_FRICTION_MANNING
 * \brief Manning friction type.
 * \var TYPE_FRICTION_POTENTIAL
 * \brief Burguete friction type.
 */
enum TypeFriction
{
	TYPE_FRICTION_MANNING=0,
	TYPE_FRICTION_POTENTIAL=1
};

/**
 * \struct _CalibrateCoefficients
 * \brief Struct to define the empirical coefficients to calibrate.
 */
struct _CalibrateCoefficients
{
/**
 * \var epsilon
 * \brief If 0 Gauckler-Manning's friction model,
 *   if >0 aerodynamical coefficient of Burguete's friction model.
 * \var n
 * \brief Gauckler-Manning's number (if #epsilon = 0),
 *   Burguete's characteristic roughness length (if #epsilon > 0).
 * \var i1
 * \brief Constant of the Kostiakov's infiltration model.
 * \var i2
 * \brief Exponent of the Kostiakov's infiltration model.
 * \var i3
 * \brief Infiltration velocity in saturated soil.
 * \var d
 * \brief Diffusion coefficient.
 * \var input
 * \brief Number of the calibration test.
 */
	JBDOUBLE epsilon, n, i1, i2, i3, d;	
	unsigned int input;
};

/**
 * \struct _InitialConditions
 * \brief Struct to define the furrow initial conditions.
 */
struct _InitialConditions
{
/**
 * \var h
 * \brief Depth.
 * \var Q
 * \brief Discharge.
 * \var c
 * \brief Fertilizer concentration.
 */
	JBFLOAT h, Q, c;
};

/**
 * \struct _Furrow
 * \brief Structure to define the data of a furrow.
 */
struct _Furrow
{
/**
 * \var b
 * \brief Base width.
 * \var z
 * \brief Angle slope of the lateral wall with the vertical.
 * \var h
 * \brief Furrow depth.
 * \var D
 * \brief Distance between furrows.
 * \var H
 * \brief Soil water retention capacity.
 * \var epsilon
 * \brief If 0, Gauckler-Manning's friction model;
 *   if >0, aerodynamical coefficient of Burguete's friction model.
 * \var n
 * \brief Gauckler-Manning's number (if epsilon = 0),
 *   characteristic roughness length of Burguete (if epsilon > 0).
 * \var d
 * \brief Diffusion coefficient.
 * \var i1
 * \brief Constant of the Kostiakov's infiltration model.
 * \var i2
 * \brief Exponent of the Kostiakov's infiltration model.
 * \var i3
 * \brief Infiltration velocity in saturated soil.
 * \var x0
 * \brief Initial spatial x-coordinate.
 * \var y0
 * \brief Initial spatial y-coordinate.
 * \var z0
 * \brief Initial spatial z-coordinate.
 * \var xf
 * \brief Final spatial x-coordinate.
 * \var yf
 * \brief Final spatial y-coordinate.
 * \var zf
 * \brief Final spatial z-coordinate.
 * \var S0
 * \brief Longitudinal slope.
 * \var Amax
 * \brief Retention area: \f$D\cdot H\f$.
 * \var fz
 * \brief Geometrical factor: \f$\sqrt{1+z^2}\f$.
 * \var l
 * \brief Length.
 * \var gn2
 * \brief Gauckler-Manning friction factor: \f$g\cdot n^2\f$.
 * \var i4
 * \brief Infiltration factor: \f$\frac{1}{i2}\f$.
 * \var fb
 * \brief Exponent of the Burguete's friction model.
 * \var friction
 * \brief Type of model friction (0: Gauckler-Manning, 1: Burguete).
 */
	JBFLOAT b, z, h, D, H, epsilon, n, d, i1, i2, i3, x0, y0, z0, xf, yf, zf,
		S0, Amax, fz, l, gn2, i4, fb;
	unsigned int friction;
};

/**
 * \typedef CalibrateCoefficients
 */
typedef struct _CalibrateCoefficients CalibrateCoefficients;

/**
 * \typedef InitialConditions
 */
typedef struct _InitialConditions InitialConditions;

/**
 * \typedef Furrow
 */
typedef struct _Furrow Furrow;

extern char *message;

/**
 * \def furrow_copy
 * \brief Macro to copy two furrows.
 * \param s
 * \brief Furrow structure copy.
 * \param sc
 * \brief Furrow structure model.
 */
#define furrow_copy(s, sc) (memcpy(s, sc, sizeof(Furrow)))

int calibrate_coefficients_read(CalibrateCoefficients *c, char *dir);

int initial_conditions_read(InitialConditions *ci, FILE *file);
void initial_conditions_dry(InitialConditions *ci);

void furrow_show(Furrow *s, FILE *file);
int furrow_read(Furrow *s, FILE *file);
void furrow_set(Furrow *s, CalibrateCoefficients *calibrate);
void furrow_position(Furrow *s, JBDOUBLE x0, JBDOUBLE y0, JBDOUBLE z0,
	JBDOUBLE xf, JBDOUBLE yf, JBDOUBLE zf);
void furrow_open(Furrow *s);

#endif
