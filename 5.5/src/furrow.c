/*
SURCOS - A software tool to solve irrigation and fertigation in isolated furrows
and furrow networks.

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
 * \file furrow.c
 * \brief Source file to define a furrow.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2011-2014, Javier Burguete Tolosa.
 */
#define _GNU_SOURCE
#include <string.h>
#include "furrow.h"

/**
 * \fn int initial_conditions_read(InitialConditions *ci, FILE *file)
 * \brief Function to read the initial conditions in a file.
 * \param ci
 * \brief InitialConditions structure.
 * \param file
 * \brief Input file.
 * \return 0 on error, 1 on success.
 */
int initial_conditions_read(InitialConditions *ci, FILE *file)
{
	if (fscanf(file, FRF FRF FRF, &ci->h, &ci->Q, &ci->c) != 3)
	{
		message = g_strdup(gettext("Bad initial conditions"));
		return 0;
	}
	if (JB_PROTECT(ci->h, INITIAL_CONDITIONS_H_MIN, INITIAL_CONDITIONS_H_MAX))
	{
		message = g_strconcat(gettext("Bad initial conditions"), " h", NULL);
		return 0;
	}
	if (JB_PROTECT(ci->Q, INITIAL_CONDITIONS_Q_MIN, INITIAL_CONDITIONS_Q_MAX))
	{
		message = g_strconcat(gettext("Bad initial conditions"), " Q", NULL);
		return 0;
	}
	if (JB_PROTECT(ci->c, INITIAL_CONDITIONS_C_MIN, INITIAL_CONDITIONS_C_MAX))
	{
		message = g_strconcat(gettext("Bad initial conditions"), " c", NULL);
		return 0;
	}
	return 1;
}

/**
 * \fn void initial_conditions_dry(InitialConditions *ci)
 * \brief Function to read dry initial conditions.
 * \param ci
 * \brief InitialConditions structure.
 */
void initial_conditions_dry(InitialConditions *ci)
{
	ci->h = ci->Q = ci->c = 0.;
}

/**
 * \fn int furrow_read(Furrow *s, FILE *file)
 * \brief Function to read the furrow data.
 * \param s
 * \brief Furrow structure.
 * \param file
 * \brief Input file.
 * \return 0 on error or 1 on success.
 */
int furrow_read(Furrow *s, FILE *file)
{
	if (fscanf(file, FRF FRF FRF FRF FRF FRF FRF FRF FRF FRF FRF,
		&s->b, &s->z, &s->h, &s->D, &s->H, &s->epsilon, &s->n, &s->i1, &s->i2,
		&s->i3, &s->d) != 11)
	{
		message = g_strdup(gettext("Bad furrow data"));
		return 0;
	}
	if (s->epsilon > 0.) s->friction = TYPE_FRICTION_POTENTIAL;
	else s->friction = TYPE_FRICTION_MANNING;
	if (JB_PROTECT(s->b, FURROW_B_MIN, FURROW_B_MAX))
	{
		message = g_strconcat(gettext("Bad furrow"), " B", NULL);
		return 0;
	}
	if (JB_PROTECT(s->z, FURROW_Z_MIN, FURROW_Z_MAX))
	{
		message = g_strconcat(gettext("Bad furrow"), " Z", NULL);
		return 0;
	}
	if (JB_PROTECT(s->h, FURROW_H_MIN, FURROW_H_MAX))
	{
		message = g_strconcat(gettext("Bad furrow"), " H", NULL);
		return 0;
	}
	if (JB_PROTECT(s->D, FURROW_W_MIN, FURROW_W_MAX))
	{
		message = g_strconcat(gettext("Bad furrow"), " W", NULL);
		return 0;
	}
	if (JB_PROTECT(s->H, FURROW_R_MIN, FURROW_R_MAX))
	{
		message = g_strconcat(gettext("Bad furrow"), " R", NULL);
		return 0;
	}
	if (JB_PROTECT(s->n, FURROW_N_MIN, FURROW_N_MAX))
	{
		message = g_strconcat(gettext("Bad furrow"), " n", NULL);
		return 0;
	}
	if (JB_PROTECT(s->i1, FURROW_I1_MIN, FURROW_I1_MAX))
	{
		message = g_strconcat(gettext("Bad furrow"), " Z", NULL);
		return 0;
	}
	if (JB_PROTECT(s->i2, FURROW_I2_MIN, FURROW_I2_MAX))
	{
		message = g_strconcat(gettext("Bad furrow"), " a", NULL);
		return 0;
	}
	if (JB_PROTECT(s->i3, FURROW_I3_MIN, FURROW_I3_MAX))
	{
		message = g_strconcat(gettext("Bad furrow"), " f0", NULL);
		return 0;
	}
	if (JB_PROTECT(s->d, FURROW_D_MIN, FURROW_D_MAX))
	{
		message = g_strconcat(gettext("Bad furrow"), " D", NULL);
		return 0;
	}
	return 1;
}

/**
 * \fn void furrow_show(Furrow *s, FILE *file)
 * \brief Debug function to show the characteristics of a furrow.
 * \param s
 * \brief Furrow structure.
 * \param file
 * \brief Output file.
 */
void furrow_show(Furrow *s, FILE *file)
{
	fprintf(file, "furrow_show: start\n");
	fprintf(file, "b="FWF"\n", s->b);
	fprintf(file, "h="FWF"\n", s->z);
	fprintf(file, "z="FWF"\n", s->h);
	fprintf(file, "D="FWF"\n", s->D);
	fprintf(file, "H="FWF"\n", s->H);
	fprintf(file, "n="FWF"\n", s->n);
	fprintf(file, "d="FWF"\n", s->d);
	fprintf(file, "i1="FWF"\n", s->i1);
	fprintf(file, "i2="FWF"\n", s->i2);
	fprintf(file, "i3="FWF"\n", s->i3);
	fprintf(file, "x0="FWF"\n", s->x0);
	fprintf(file, "y0="FWF"\n", s->y0);
	fprintf(file, "z0="FWF"\n", s->z0);
	fprintf(file, "xf="FWF"\n", s->xf);
	fprintf(file, "yf="FWF"\n", s->yf);
	fprintf(file, "zf="FWF"\n", s->zf);
	fprintf(file, "S0="FWF"\n", s->S0);
	fprintf(file, "fz="FWF"\n", s->fz);
	fprintf(file, "l="FWF"\n", s->l);
	fprintf(file, "gn2="FWF"\n", s->gn2);
	fprintf(file, "i4="FWF"\n", s->i4);
	fprintf(file, "furrow_show: end\n");
}

/**
 * \fn void furrow_position(Furrow *s, JBDOUBLE x0, JBDOUBLE y0, JBDOUBLE z0,
 *	JBDOUBLE xf, JBDOUBLE yf, JBDOUBLE zf)
 * \brief Function to fix the initial and final coordinates of a furrow
 * \param s
 * \brief Furrow structure.
 * \param x0
 * \brief Initial point x-coordinate.
 * \param y0
 * \brief Initial point y-coordinate.
 * \param z0
 * \brief Initial point z-coordinate.
 * \param xf
 * \brief Final point x-coordinate.
 * \param yf
 * \brief Final point y-coordinate.
 * \param zf
 * \brief Final point z-coordinate.
 */
void furrow_position(Furrow *s, JBDOUBLE x0, JBDOUBLE y0, JBDOUBLE z0,
	JBDOUBLE xf, JBDOUBLE yf, JBDOUBLE zf)
{
	s->x0 = x0;
	s->y0 = y0;
	s->z0 = z0;
	s->xf = xf;
	s->yf = yf;
	s->zf = zf;
}

/**
 * \fn void furrow_open(Furrow *s)
 * \brief Function to calculate geometrical parameters of a furrow.
 * \param s
 * \brief Furrow structure.
 */
void furrow_open(Furrow *s)
{
	s->fz = jbm_fdbl(sqrt(1.+jbm_fsqr(s->z)));
	s->l = jbm_v2_length(s->x0, s->y0, s->xf, s->yf);
	s->S0 = (s->z0 - s->zf) / s->l;
	if (s->friction == TYPE_FRICTION_MANNING)
		s->gn2 = JBM_G * jbm_fsqr(s->n);
	else
	{
		s->fb = 0.27;
		s->gn2 =
			s->epsilon * (s->fb + 1.) * (s->fb + 1.) * pow(s->n, 2. * s->fb);
	}
	s->i4 = 1. / s->i2;
	s->Amax = s->D * s->H;
}
