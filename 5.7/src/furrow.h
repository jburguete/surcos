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
 * \file furrow.h
 * \brief Header file to define the furrow data.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2011-2020, Javier Burguete Tolosa.
 */
#ifndef FURROW__H
#define FURROW__H 1

#define furrow_copy(s, sc) (memcpy(s, sc, sizeof(Furrow)))
///< Macro to copy two furrows.

///> Enumeration to define the friction model type.
enum TypeFriction
{
  TYPE_FRICTION_MANNING = 0,    ///< Manning friction type.
  TYPE_FRICTION_POTENTIAL = 1   ///< Burguete friction type.
};

/**
 * \struct InitialConditions
 * \brief Struct to define the furrow initial conditions.
 */
typedef struct
{
  JBFLOAT h;                    ///< Depth.
  JBFLOAT Q;                    ///< Discharge.
  JBFLOAT c;                    ///< Fertilizer concentration.
} InitialConditions;

/**
 * \struct Furrow
 * \brief Structure to define the data of a furrow.
 */
typedef struct
{
  JBFLOAT b;
  ///< Base width.
  JBFLOAT z;
  ///< Angle slope of the lateral wall with the vertical.
  JBFLOAT h;
  ///< Furrow depth.
  JBFLOAT D;
  ///< Distance between furrows.
  JBFLOAT H;
  ///< Soil water retention capacity.
  JBFLOAT epsilon;
  ///< If 0, Gauckler-Manning's friction model;
  ///< if >0, aerodynamical coefficient of Burguete's friction model.
  JBFLOAT n;
  ///< Gauckler-Manning's number (if epsilon = 0),
  ///< characteristic roughness length of Burguete (if epsilon > 0).
  JBFLOAT d;
  ///< Diffusion coefficient.
  JBFLOAT i1;
  ///< Constant of the Kostiakov's infiltration model.
  JBFLOAT i2;
  ///< Exponent of the Kostiakov's infiltration model.
  JBFLOAT i3;
  ///< Infiltration velocity in saturated soil.
  JBFLOAT x0;
  ///< Initial spatial x-coordinate.
  JBFLOAT y0;
  ///< Initial spatial y-coordinate.
  JBFLOAT z0;
  ///< Initial spatial z-coordinate.
  JBFLOAT xf;
  ///< Final spatial x-coordinate.
  JBFLOAT yf;
  ///< Final spatial y-coordinate.
  JBFLOAT zf;
  ///< Final spatial z-coordinate.
  JBFLOAT S0;
  ///< Longitudinal slope.
  JBFLOAT Amax;
  ///< Retention area: \f$D\cdot H\f$.
  JBFLOAT fz;
  ///< Geometrical factor: \f$\sqrt{1+z^2}\f$.
  JBFLOAT l;
  ///< Length.
  JBFLOAT gn2;
  ///< Gauckler-Manning friction factor: \f$g\cdot n^2\f$.
  JBFLOAT i4;
  ///< Infiltration factor: \f$\frac{1}{i2}\f$.
  JBFLOAT fb;
  ///< Exponent of the Burguete's friction model.
  unsigned int friction;
  ///< Type of model friction (0: Gauckler-Manning, 1: Burguete).
} Furrow;

extern char *message;

int initial_conditions_read (InitialConditions * ci, FILE * file);
void initial_conditions_dry (InitialConditions * ci);

void furrow_show (Furrow * s, FILE * file);
int furrow_read (Furrow * s, FILE * file);
void furrow_position (Furrow * s, JBDOUBLE x0, JBDOUBLE y0, JBDOUBLE z0,
                      JBDOUBLE xf, JBDOUBLE yf, JBDOUBLE zf);
void furrow_open (Furrow * s);

#endif
