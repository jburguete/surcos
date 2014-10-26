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
 * \file scheme.c
 * \brief Source file to define the numerical scheme.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2011-2014, Javier Burguete Tolosa.
 */
#define _GNU_SOURCE
#include <string.h>
#include "scheme.h"

/**
 * \fn void scheme_save(Parameters *p, unsigned int n, char *name)
 * \brief Function to save in a file the longitudinal profile of a furrow.
 * \param p
 * \brief Mesh node parameters struct.
 * \param n
 * \brief Number of mesh cells.
 * \param name
 * \brief File name.
 */
void scheme_save(Parameters *p, unsigned int n, char *name)
{
	unsigned int i;
	FILE *file;
	file=g_fopen(name, "w");
	for (i = 0; i < n; ++p, ++i)
		fprintf(file, FWF" "FWF" "FWF" "FWF" "FWF" "FWF" "FWF" "FWF" "FWF" "FWF
			" "FWF" "FWF"\n",
			p->x, p->h, p->A, p->Q, p->zs, p->zf, -p->Ai, p->c, -p->Aci,
			-p->Ap, -p->Acp, p->beta);
	fclose(file);
}

/**
 * \fn unsigned int scheme_search_node\
 * 	(Parameters *p, unsigned int n, JBDOUBLE x, JBDOUBLE y)
 * \brief Function to search the closest mesh node to a point.
 * \param p
 * \brief Mesh node parameters struct.
 * \param n
 * \brief Number of mesh cells
 * \param x
 * \brief X-coordinate of the point.
 * \param y
 * \brief Y-coordinate of the point.
 * \return Number of the closest mesh node.
 */
unsigned int scheme_search_node
	(Parameters *p, unsigned int n, JBDOUBLE x, JBDOUBLE y)
{
	unsigned int i, j;
	JBDOUBLE d, dmin;
	dmin = INFINITY;
	for (i = j = 0; j < n; ++j)
	{
		d = jbm_fsqr(p[j].x0 - x) + jbm_fsqr(p[j].y0 - y);
		if (d < dmin)
		{
			dmin = d;
			i = j;
		}
	}
	return i;
}

/**
 * \fn void scheme_mesh\
 * 	(Parameters *p, unsigned int n, Furrow *s, InitialConditions *ci)
 * \brief Function to define the mesh and the initial conditions of a furrow
 * \param p
 * \brief Mesh node parameters structure.
 * \param n
 * \brief Number of mesh cells.
 * \param s
 * \brief Furrow data structure.
 * \param ci
 * \brief Furrow initial conditions structure.
 */
void scheme_mesh
	(Parameters *p, unsigned int n, Furrow *s, InitialConditions *ci)
{
	register unsigned int i;
	JBDOUBLE dx, hmin;
	furrow_open(s);
	dx = s->l / (n - 1);
/*	hmin = fmax(0.001, pow(s->gn2 * dx, 0.75));*/
	if (s->friction == TYPE_FRICTION_MANNING) hmin = 0.01;
	else hmin = s->n + 0.002;
	for (i = 0; i < n; ++i)
	{
		furrow_copy(p[i].s, s);
		p[i].dx = dx;
		p[i].x = i * dx;
		p[i].x0 = s->x0 + i * (s->xf - s->x0) / (n - 1);
		p[i].y0 = s->y0 + i * (s->yf - s->y0) / (n - 1);
		p[i].zf = s->z0 + i * (s->zf - s->z0) / (n - 1) - s->h;
		p[i].A = parameters_area(p + i, ci->h);
		p[i].Q = ci->Q;
		p[i].Ac = p[i].A * ci->c;
		p[i].Ai = p[i].Aci = p[i].Ap = p[i].Acp = p[i].m = 0.;
		p[i].Amin = parameters_area(p + i, hmin);
		if (i) p[i-1].zmin = fmax(p[i-1].zf, p[i].zf) + hmin;
		p[i].ta = p[i].tr = 0.;
	}
	p[0].x = 0.;
	p[n-1].x = s->l;
	p[0].dx = p[n-1].dx = 0.5 * dx;
}

/**
 * \fn void scheme_diffusion(Parameters *p, unsigned int n)
 * \brief Function to calculate the superficial diffusion in a furrow.
 * \param p
 * \brief Mesh node parameters structure of the furrow initial point.
 * \param n
 * \brief Number of the furrow mesh cells.
 */
void scheme_diffusion(Parameters *p, unsigned int n)
{
	register int i;
	register JBDOUBLE k;
	JBFLOAT C[n], D[n], E[n], H[n];
	for (i = 0; i < n; ++i)
	{
		D[i] = p[i].A * p[i].dx;
		H[i] = p[i].Ac * p[i].dx;
	}
	for (i = n - 1; --i >= 0;)
	{
		k = dt * fmin(p[i].KAx, p[i+1].KAx) / p[i].ix;
		E[i] = -k;
		C[i] = -k;
		D[i] += k;
		D[i+1] += k;
	}
	jbm_matrix_solve_tridiagonal_zero(C, D, E, H, n - 1);
	for (i = 0; i < n; ++i)
	{
		p[i].c = H[i];
		p[i].Ac = H[i] * p[i].A;
	}
}

/**
 * \fn void scheme_diffusion_soil(Parameters *p, unsigned int n)
 * \brief Function to calculate the soil diffusion in a furrow.
 * \param p
 * \brief Mesh node parameters structure of the furrow initial point.
 * \param n
 * \brief Number of the furrow mesh cells.
 */
void scheme_diffusion_soil(Parameters *p, unsigned int n)
{
	register int i;
	register JBDOUBLE k;
	JBFLOAT C1[n], D1[n], E1[n], H1[n], C2[n], D2[n], E2[n], H2[n];
	for (i=0; i<n; ++i)
	{
		D1[i] = D2[i] = 1.;
		H1[i] = p[i].Ai;
		H2[i] = p[i].Aci;
	}
	k = p->s->d * dt / jbm_fsqr(p[0].dx);
	for (i = n - 1; --i >= 0;)
	{
		E1[i] = E2[i] = -k;
		C1[i] = C2[i] = -k;
		D1[i] = D2[i] += k;
		D1[i+1] = D2[i+1] += k;
	}
	jbm_matrix_solve_tridiagonal_zero(C1, D1, E1, H1, n - 1);
	for (i = 0; i < n; ++i) p[i].Ai = H1[i];
	if (!type_fertilizer) return;
	jbm_matrix_solve_tridiagonal_zero(C2, D2, E2, H2, n - 1);
	for (i = 0; i < n; ++i) p[i].Aci = H2[i];
}

/**
 * \fn void scheme_infiltration(Parameters *p, unsigned int n)
 * \brief Function to calculate the infiltation in a furrow.
 * \param p
 * \brief Mesh node parameters structure of the furrow initial point.
 * \param n
 * \brief Number of the furrow mesh cells.
 */
void scheme_infiltration(Parameters *p, unsigned int n)
{
	register int i;
	for (i = 0; i < n; ++i) parameters_infiltration_scheme(p + i);
}

/**
 * \fn void scheme_step(Parameters *p, unsigned int n)
 * \brief Function to calculate a numerical step in a furrow.
 * \param p
 * \brief Mesh node parameters structure of the furrow initial point.
 * \param n
 * \brief Number of the furrow mesh cells.
 */
void scheme_step(Parameters *p, unsigned int n)
{
	unsigned int i, n2;
	for (i = 0; i < n; ++i) parameters_save(p + i);
	if (type_fertilizer) scheme_diffusion(p, n);
	n2 = n - 1;
	for (i = 0; i < n2; ++i) parameters_scheme(p + i);
	parameters_scheme_upwind(p);
	for (i = 1, --n2; i < n2; ++i) parameters_scheme_tvd(p + i);
	parameters_scheme_upwind(p + i);
	if (type_infiltration) scheme_infiltration(p, n);
	if (type_diffusion_soil) scheme_diffusion_soil(p, n);
}

/**
 * \fn void scheme_ix(Parameters *p, unsigned int n)
 * \brief Function to calculate mesh node distances in a furrow.
 * \param p
 * \brief Mesh node parameters structure of the furrow initial point.
 * \param n
 * \brief Number of the furrow mesh cells.
 */
void scheme_ix(Parameters *p, unsigned int n)
{
	register unsigned int i;
	--n;
	for (i = 0; i < n; ++i) parameters_ix(p + i);
}

/**
 * \fn void scheme_save_advance(Parameters *p, unsigned int n, FILE *file)
 * \brief Function to save advance and recession times of a furrow in a file.
 * \param p
 * \brief Mesh node parameters structure of the furrow initial point.
 * \param n
 * \brief Number of the furrow mesh cells.
 * \param file
 * \brief Output file.
 */
void scheme_save_advance(Parameters *p, unsigned int n, FILE *file)
{
	unsigned int i;
	for (i = 0; i < n; ++i)
		fprintf(file, FWF" "FWF" "FWF"\n", p[i].x, p[i].ta, p[i].tr);
}

/**
 * \fn JBDOUBLE uniformity(JBFLOAT *x, unsigned int n)
 * \brief Function to calculate the uniformity of a variable.
 * \param x
 * \brief Variable array.
 * \param n
 * \brief Number of array elements.
 * \return Calculated uniformity coefficient. 
 */
JBDOUBLE uniformity(JBFLOAT *x, unsigned int n)
{
	unsigned int i,m;
	int ni[n];
	JBDOUBLE xm, xp;
	jbm_index_sort(x, ni, n - 1);
	for (i = 0, xp = 0.; i < n; ++i) xp += x[ni[n - 1 - i]];
	if (xp <= 0.) return 0.;
	xp /= n;
	m = n / 4;
	for (i = 0, xm = 0.; i < m; ++i) xm += x[ni[n - 1 - i]];
	if (xm <= 0.) return 0.;
	xm /= m;
	return 100. * xm / xp;
}

/**
 * \fn JBDOUBLE uniformity_water(Parameters *p, unsigned int n)
 * \brief Function to calculate the uniformity of the infiltrated water.
 * \param p
 * \brief Pointer to the initial mesh node structure array.
 * \param n
 * \brief Number of array elements.
 * \return Infiltrated water uniformity coefficient.
 */
JBDOUBLE uniformity_water(Parameters *p, unsigned int n)
{
	unsigned int i;
	JBFLOAT x[n];
	for (i = 0; i < n; ++i) x[i] = p[i].Ai;
	return uniformity(x, n);
}

/**
 * \fn JBDOUBLE uniformity_fertilizer(Parameters *p, unsigned int n)
 * \brief Function to calculate the uniformity of the infiltrated fertilizer.
 * \param p
 * \brief Pointer to the initial mesh node structure array.
 * \param n
 * \brief Number of array elements.
 * \return Infiltrated fertilizer uniformity coefficient.
 */
JBDOUBLE uniformity_fertilizer(Parameters *p, unsigned int n)
{
	unsigned int i;
	JBFLOAT x[n];
	for (i = 0; i < n; ++i) x[i] = p[i].Aci;
	return uniformity(x, n);
}
