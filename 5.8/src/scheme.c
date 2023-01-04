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
 * \file scheme.c
 * \brief Source file to define the numerical scheme.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2011-2023, Javier Burguete Tolosa.
 */
#include "config.h"
#include "jb/jb_win.h"
#include "furrow.h"
#include "parameters.h"
#include "scheme.h"

#define SCHEME_SHOW 0
///< Macro to compile the scheme_show function.

#define DEBUG_SCHEME_SAVE 0     ///< Macro to debug the scheme_save function.
#define DEBUG_SCHEME_SEARCH_NODE 0
///< Macro to debug the scheme_seach_node function.
#define DEBUG_SCHEME_MESH 0     ///< Macro to debug the scheme_mesh function.
#define DEBUG_SCHEME_DIFFUSION 0
///< Macro to debug the scheme_diffusion function.
#define DEBUG_SCHEME_DIFFUSION_SOIL 0
///< Macro to debug the scheme_diffusion_soil function.
#define DEBUG_SCHEME_INFILTRATION 0
///< Macro to debug the scheme_infiltration function.
#define DEBUG_SCHEME_STEP 0     ///< Macro to debug the scheme_step function.
#define DEBUG_SCHEME_IX 0       ///< Macro to debug the scheme_ix function.
#define DEBUG_SCHEME_SAVE_ADVANCE 0
///< Macro to debug the scheme_save_advance function.
#define DEBUG_SCHEME_TIME 0.
///< Macro to define the time to begin the debug of scheme functions.

#if SCHEME_SHOW
void
scheme_show (Parameters * p,
///< Mesh node parameters structure of the furrow initial point.
             unsigned int n,    ///< Number of the furrow mesh cells.
	     FILE * file)       ///< Output file.
{
  unsigned int i;
  if (t >= DEBUG_SCHEME_TIME)
    for (i = 0; i < n; ++i)
      parameters_show (p + i, file);
}
#endif

/**
 * Function to save in a file the longitudinal profile of a furrow.
 */
void
scheme_save (Parameters * p,    ///< Mesh node parameters struct.
             unsigned int n,    ///< Number of mesh cells.
             char *name)        ///< File name.
{
  FILE *file;
  unsigned int i;
#if DEBUG_SCHEME_SAVE
  fprintf (stderr, "scheme_save: start\n");
#endif
  file = g_fopen (name, "w");
  for (i = 0; i < n; ++p, ++i)
    fprintf (file,
             FWF " " FWF " " FWF " " FWF " " FWF " " FWF " " FWF " " FWF " "
             FWF " " FWF " " FWF " " FWF "\n", p->x, p->h, p->A, p->Q, p->zs,
             p->zf, -p->Ai, p->c, -p->Aci, -p->Ap, -p->Acp, p->beta);
  fclose (file);
#if DEBUG_SCHEME_SAVE
  fprintf (stderr, "scheme_save: end\n");
#endif
}

/**
 * Function to search the closest mesh node to a point.
 *
 * \return Number of the closest mesh node.
 */
unsigned int
scheme_search_node (Parameters * p,     ///< Mesh node parameters struct.
                    unsigned int n,     ///< Number of mesh cells
                    JBDOUBLE x, ///< X-coordinate of the point.
                    JBDOUBLE y) ///< Y-coordinate of the point.
{
  JBDOUBLE d, dmin;
  register unsigned int i, j;
#if DEBUG_SCHEME_SEARCH_NODE
  fprintf (stderr, "scheme_search_node: start\n");
#endif
  dmin = INFINITY;
  for (i = j = 0; j < n; ++j)
    {
      d = jbm_fsqr (p[j].x0 - x) + jbm_fsqr (p[j].y0 - y);
      if (d < dmin)
        {
          dmin = d;
          i = j;
        }
    }
#if DEBUG_SCHEME_SEARCH_NODE
  fprintf (stderr, "scheme_search_node: i=%u\n", i);
  fprintf (stderr, "scheme_search_node: end\n");
#endif
  return i;
}

/**
 * Function to define the mesh and the initial conditions of a furrow
 */
void
scheme_mesh (Parameters * p,    ///< Mesh node parameters structure.
             unsigned int n,    ///< Number of mesh cells.
             Furrow * s,        ///< Furrow data structure.
             InitialConditions * ci)
             ///< Furrow initial conditions structure.
{
  JBDOUBLE dx, hmin;
  register unsigned int i;
#if DEBUG_SCHEME_MESH
  fprintf (stderr, "scheme_mesh: start\n");
#endif
  furrow_open (s);
  dx = s->l / (n - 1);
  /*  hmin = fmax(0.001, pow(s->gn2 * dx, 0.75)); */
  if (s->friction == TYPE_FRICTION_MANNING)
    hmin = 0.01;
  else
    hmin = s->n + 0.002;
  for (i = 0; i < n; ++i)
    {
      furrow_copy (p[i].s, s);
      p[i].dx = dx;
      p[i].x = i * dx;
      p[i].x0 = s->x0 + i * (s->xf - s->x0) / (n - 1);
      p[i].y0 = s->y0 + i * (s->yf - s->y0) / (n - 1);
      p[i].zf = s->z0 + i * (s->zf - s->z0) / (n - 1) - s->h;
      p[i].A = parameters_area (p + i, ci->h);
      p[i].Q = ci->Q;
      p[i].Ac = p[i].A * ci->c;
      p[i].Ai = p[i].Aci = p[i].Ap = p[i].Acp = p[i].m = 0.;
      p[i].Amin = parameters_area (p + i, hmin);
      if (i)
        p[i - 1].zmin = fmax (p[i - 1].zf, p[i].zf) + hmin;
      p[i].ta = p[i].tr = 0.;
    }
  p[0].x = 0.;
  p[n - 1].x = s->l;
  p[0].dx = p[n - 1].dx = 0.5 * dx;
#if DEBUG_SCHEME_MESH
  scheme_show (p, n, stderr);
  fprintf (stderr, "scheme_mesh: end\n");
#endif
}

/**
 * Function to calculate the superficial diffusion in a furrow.
 */
static void
scheme_diffusion (Parameters * p,
///< Mesh node parameters structure of the furrow initial point.
                  unsigned int n)       ///< Number of furrow mesh cells.
{
  JBFLOAT C[n], D[n], E[n], H[n];
  register JBDOUBLE k;
  register int i;
#if DEBUG_SCHEME_DIFFUSION
  fprintf (stderr, "scheme_diffusion: start\n");
#endif
  for (i = 0; i < (int) n; ++i)
    {
      D[i] = p[i].A * p[i].dx;
      H[i] = p[i].Ac * p[i].dx;
    }
  for (i = n - 1; --i >= 0;)
    {
      k = dt * fmin (p[i].KAx, p[i + 1].KAx) / p[i].ix;
      E[i] = -k;
      C[i] = -k;
      D[i] += k;
      D[i + 1] += k;
    }
  jbm_matrix_solve_tridiagonal_zero (C, D, E, H, n - 1);
  for (i = 0; i < (int) n; ++i)
    {
      p[i].c = H[i];
      p[i].Ac = H[i] * p[i].A;
    }
#if DEBUG_SCHEME_DIFFUSION
  scheme_show (p, n, stderr);
  fprintf (stderr, "scheme_diffusion: end\n");
#endif
}

/**
 * Function to calculate the soil diffusion in a furrow.
 */
static void
scheme_diffusion_soil (Parameters * p,
///< Mesh node parameters structure of the furrow initial point.
                       unsigned int n)  ///< Number of furrow mesh cells.
{
  JBFLOAT C1[n], D1[n], E1[n], H1[n], C2[n], D2[n], E2[n], H2[n];
  register JBDOUBLE k;
  register int i;
#if DEBUG_SCHEME_DIFFUSION_SOIL
  fprintf (stderr, "scheme_diffusion_soil: start\n");
#endif
  for (i = 0; i < (int) n; ++i)
    {
      D1[i] = D2[i] = 1.;
      H1[i] = p[i].Ai;
      H2[i] = p[i].Aci;
    }
  k = p->s->d * dt / jbm_fsqr (p[0].dx);
  for (i = n - 1; --i >= 0;)
    {
      E1[i] = E2[i] = -k;
      C1[i] = C2[i] = -k;
      D1[i] = D2[i] += k;
      D1[i + 1] = D2[i + 1] += k;
    }
  jbm_matrix_solve_tridiagonal_zero (C1, D1, E1, H1, n - 1);
  for (i = 0; i < (int) n; ++i)
    p[i].Ai = H1[i];
  if (!type_fertilizer)
    return;
  jbm_matrix_solve_tridiagonal_zero (C2, D2, E2, H2, n - 1);
  for (i = 0; i < (int) n; ++i)
    p[i].Aci = H2[i];
#if DEBUG_SCHEME_DIFFUSION_SOIL
  scheme_show (p, n, stderr);
  fprintf (stderr, "scheme_diffusion_soil: end\n");
#endif
}

/**
 * Function to calculate the infiltation in a furrow.
 */
static void
scheme_infiltration (Parameters * p,
///< Mesh node parameters structure of the furrow initial point.
                     unsigned int n)
///< Number of the furrow mesh cells.
{
  register unsigned int i;
#if DEBUG_SCHEME_INFILTRATION
  fprintf (stderr, "scheme_infiltration: start\n");
#endif
  for (i = 0; i < n; ++i)
    parameters_infiltration_scheme (p + i);
#if DEBUG_SCHEME_INFILTRATION
  scheme_show (p, n, stderr);
  fprintf (stderr, "scheme_infiltration: end\n");
#endif
}

/**
 * Function to calculate a numerical step in a furrow.
 */
void
scheme_step (Parameters * p,
///< Mesh node parameters structure of the furrow initial point.
             unsigned int n)    ///< Number of the furrow mesh cells.
{
  unsigned int i, n2;
#if DEBUG_SCHEME_STEP
  fprintf (stderr, "scheme_step: start\n");
#endif
  for (i = 0; i < n; ++i)
    parameters_save (p + i);
  if (type_fertilizer)
    scheme_diffusion (p, n);
#if DEBUG_SCHEME_STEP
  fprintf (stderr, "scheme_step: flow start\n");
#endif
  n2 = n - 1;
  for (i = 0; i < n2; ++i)
    parameters_scheme (p + i);
  parameters_scheme_upwind (p);
  for (i = 1, --n2; i < n2; ++i)
    parameters_scheme_tvd (p + i);
  parameters_scheme_upwind (p + i);
#if DEBUG_SCHEME_STEP
  scheme_show (p, n, stderr);
  fprintf (stderr, "scheme_step: flow end\n");
#endif
  if (type_infiltration)
    scheme_infiltration (p, n);
  if (type_diffusion_soil)
    scheme_diffusion_soil (p, n);
#if DEBUG_SCHEME_STEP
  scheme_show (p, n, stderr);
  fprintf (stderr, "scheme_step: end\n");
#endif
}

/**
 * Function to calculate mesh node distances in a furrow.
 */
void
scheme_ix (Parameters * p,
///< Mesh node parameters structure of the furrow initial point.
           unsigned int n)      ///< Number of the furrow mesh cells.
{
  register unsigned int i;
#if DEBUG_SCHEME_IX
  fprintf (stderr, "scheme_ix: start\n");
#endif
  --n;
  for (i = 0; i < n; ++i)
    parameters_ix (p + i);
#if DEBUG_SCHEME_IX
  scheme_show (p, n, stderr);
  fprintf (stderr, "scheme_ix: end\n");
#endif
}

/**
 * Function to save advance and recession times of a furrow in a file.
 */
void
scheme_save_advance (Parameters * p,
///< Mesh node parameters structure of the furrow initial point.
                     unsigned int n,    ///< Number of the furrow mesh cells.
                     FILE * file)       ///< Output file.
{
  unsigned int i;
#if DEBUG_SCHEME_SAVE_ADVANCE
  fprintf (stderr, "scheme_save_advance: start\n");
#endif
  for (i = 0; i < n; ++i)
    fprintf (file, FWF " " FWF " " FWF "\n", p[i].x, p[i].ta, p[i].tr);
#if DEBUG_SCHEME_SAVE_ADVANCE
  fprintf (stderr, "scheme_save_advance: end\n");
#endif
}

/**
 * Function to calculate the uniformity of a variable.
 *
 * \return Calculated uniformity coefficient.
 */
JBDOUBLE
uniformity (JBFLOAT * x,        ///< Variable array.
            unsigned int n)     ///< Number of array elements.
{
  unsigned int ni[n];
  JBDOUBLE xm, xp;
  register unsigned int i, m;
  jbm_index_sort (x, ni, (int) n - 1);
  for (i = 0, xp = 0.; i < n; ++i)
    xp += x[ni[n - 1 - i]];
  if (xp <= 0.)
    return 0.;
  xp /= n;
  m = n / 4;
  for (i = 0, xm = 0.; i < m; ++i)
    xm += x[ni[n - 1 - i]];
  if (xm <= 0.)
    return 0.;
  xm /= m;
  return 100. * xm / xp;
}

/**
 * Function to calculate the uniformity of the infiltrated water.
 * \return Infiltrated water uniformity coefficient.
 */
JBDOUBLE
uniformity_water (Parameters * p,
///< Pointer to the initial mesh node structure array.
                  unsigned int n)       ///< Number of array elements.
{
  JBFLOAT x[n];
  register unsigned int i;
  for (i = 0; i < n; ++i)
    x[i] = p[i].Ai;
  return uniformity (x, n);
}

/**
 * Function to calculate the uniformity of the infiltrated fertilizer.
 * \return Infiltrated fertilizer uniformity coefficient.
 */
JBDOUBLE
uniformity_fertilizer (Parameters * p,
///< Pointer to the initial mesh node structure array.
                       unsigned int n)  ///< Number of array elements.
{
  JBFLOAT x[n];
  register unsigned int i;
  for (i = 0; i < n; ++i)
    x[i] = p[i].Aci;
  return uniformity (x, n);
}
