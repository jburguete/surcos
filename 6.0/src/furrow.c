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
 * \file furrow.c
 * \brief Source file to define a furrow.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2011-2023, Javier Burguete Tolosa.
 */
#include "config.h"
#include "jb/src/jb_win.h"
#include "jb/src/jb_json.h"
#include "furrow.h"

#define FURROW_SHOW 0           ///< Macro to compile the furrow_show function.

#define DEBUG_INITIAL_CONDITIONS_READ 0
///< Macro to debug the initial_conditions_read function.
#define DEBUG_FURROW_READ 0     ///< Macro to debug the furrow_read function.
#define DEBUG_FURROW_POSITION 0
///< Macro to debug the furrow_position function.
#define DEBUG_FURROW_OPEN 0     ///< Macro to debug the furrow_open function.

/**
 * Function to read the initial conditions in a file.
 *
 * \return 0 on error, 1 on success.
 */
static inline int
initial_conditions_read (InitialConditions * ic,
                         ///< InitialConditions structure.
                         JsonObject * object)   ///< JSON object.
{
  int e;
#if DEBUG_INITIAL_CONDITIONS_READ
  fprintf (stderr, "initial_conditions_read: start\n");
#endif
  ic->h = jb_json_object_get_float_with_default (object, "h", &e, 0.);
  if (!e
      || JB_PROTECT (ic->h, INITIAL_CONDITIONS_H_MIN, INITIAL_CONDITIONS_H_MAX))
    {
      message = g_strconcat (_("Bad initial conditions"), " h", NULL);
      goto exit;
    }
  ic->q = jb_json_object_get_float_with_default (object, "q", &e, 0.);
  if (!e
      || JB_PROTECT (ic->h, INITIAL_CONDITIONS_Q_MIN, INITIAL_CONDITIONS_Q_MAX))
    {
      message = g_strconcat (_("Bad initial conditions"), " q", NULL);
      goto exit;
    }
  ic->c = jb_json_object_get_float_with_default (object, "c", &e, 0.);
  if (!e
      || JB_PROTECT (ic->h, INITIAL_CONDITIONS_C_MIN, INITIAL_CONDITIONS_C_MAX))
    {
      message = g_strconcat (_("Bad initial conditions"), " c", NULL);
      goto exit;
    }
#if DEBUG_INITIAL_CONDITIONS_READ
  fprintf (stderr, "initial_conditions_read: end\n");
#endif
  return 1;
exit:
#if DEBUG_INITIAL_CONDITIONS_READ
  fprintf (stderr, "initial_conditions_read: end\n");
#endif
  return 0;
}

/**
 * Function to read the furrow data.
 *
 * \return 0 on error or 1 on success.
 */
int
furrow_read (Furrow * f,        ///< Furrow structure.
             InitialConditions * ic,    ///< InitialConditions structure.
             JsonObject * object)       ///< JSON object.
{
  JsonObject *child;
  int e;
#if DEBUG_FURROW_READ
  fprintf (stderr, "furrow_read: start\n");
#endif
  if (!json_object_has_member (object, "initial-conditions"))
    {
      ic->h = ic->q = ic->c = 0.;
      f->initial_conditions = 0;
    }
  else
    {
      child = json_object_get_object_member (object, "initial-conditions");
      if (!initial_conditions_read (ic, child))
        goto exit2;
      f->initial_conditions = 1;
    }
  f->b = jb_json_object_get_float_with_default (object, "B", &e, 0.);
  if (!e || JB_PROTECT (f->b, FURROW_B_MIN, FURROW_B_MAX))
    {
      message = g_strconcat (_("Bad furrow"), " B", NULL);
      goto exit2;
    }
  f->z = jb_json_object_get_float_with_default (object, "Z", &e, 0.);
  if (!e || JB_PROTECT (f->z, FURROW_Z_MIN, FURROW_Z_MAX))
    {
      message = g_strconcat (_("Bad furrow"), " Z", NULL);
      goto exit2;
    }
  f->h = jb_json_object_get_float_with_default (object, "H", &e, 0.);
  if (!e || JB_PROTECT (f->h, FURROW_H_MIN, FURROW_H_MAX))
    {
      message = g_strconcat (_("Bad furrow"), " H", NULL);
      goto exit2;
    }
  f->D = jb_json_object_get_float_with_default (object, "W", &e, 0.);
  if (!e || JB_PROTECT (f->D, FURROW_W_MIN, FURROW_W_MAX))
    {
      message = g_strconcat (_("Bad furrow"), " W", NULL);
      goto exit2;
    }
  f->H = jb_json_object_get_float_with_default (object, "R", &e, 0.);
  if (!e || JB_PROTECT (f->H, FURROW_R_MIN, FURROW_R_MAX))
    {
      message = g_strconcat (_("Bad furrow"), " R", NULL);
      goto exit2;
    }
  f->epsilon
    = jb_json_object_get_float_with_default (object, "epsilon", &e, 0.);
  if (!e)
    {
      message = g_strconcat (_("Bad furrow"), " epsilon", NULL);
      goto exit2;
    }
  f->n = jb_json_object_get_float_with_default (object, "n", &e, 0.);
  if (!e || JB_PROTECT (f->n, FURROW_N_MIN, FURROW_N_MAX))
    {
      message = g_strconcat (_("Bad furrow"), " n", NULL);
      goto exit2;
    }
  f->i1 = jb_json_object_get_float_with_default (object, "i1", &e, 0.);
  if (!e || JB_PROTECT (f->i1, FURROW_I1_MIN, FURROW_I1_MAX))
    {
      message = g_strconcat (_("Bad furrow"), " i1", NULL);
      goto exit2;
    }
  f->i2 = jb_json_object_get_float_with_default (object, "i2", &e, 0.);
  if (!e || JB_PROTECT (f->i2, FURROW_I2_MIN, FURROW_I2_MAX))
    {
      message = g_strconcat (_("Bad furrow"), " i2", NULL);
      goto exit2;
    }
  f->i3 = jb_json_object_get_float_with_default (object, "i3", &e, 0.);
  if (!e || JB_PROTECT (f->i3, FURROW_I3_MIN, FURROW_I3_MAX))
    {
      message = g_strconcat (_("Bad furrow"), " i3", NULL);
      goto exit2;
    }
  f->d = jb_json_object_get_float_with_default (object, "D", &e, 0.);
  if (!e || JB_PROTECT (f->d, FURROW_D_MIN, FURROW_D_MAX))
    {
      message = g_strconcat (_("Bad furrow"), " D", NULL);
      goto exit2;
    }
#if DEBUG_FURROW_READ
  fprintf (stderr, "furrow_read: end\n");
#endif
  return 1;

exit2:
#if DEBUG_FURROW_READ
  fprintf (stderr, "furrow_read: end\n");
#endif
  return 0;
}

/**
 * Debug function to show the characteristics of a furrow.
 */
#if FURROW_SHOW
void
furrow_show (Furrow * f,        ///< Furrow structure.
             FILE * file)       ///< Output file.
{
  fprintf (file, "furrow_show: start\n");
  fprintf (file, "b=" FWF "\n", f->b);
  fprintf (file, "h=" FWF "\n", f->z);
  fprintf (file, "z=" FWF "\n", f->h);
  fprintf (file, "D=" FWF "\n", f->D);
  fprintf (file, "H=" FWF "\n", f->H);
  fprintf (file, "n=" FWF "\n", f->n);
  fprintf (file, "d=" FWF "\n", f->d);
  fprintf (file, "i1=" FWF "\n", f->i1);
  fprintf (file, "i2=" FWF "\n", f->i2);
  fprintf (file, "i3=" FWF "\n", f->i3);
  fprintf (file, "x0=" FWF "\n", f->x0);
  fprintf (file, "y0=" FWF "\n", f->y0);
  fprintf (file, "z0=" FWF "\n", f->z0);
  fprintf (file, "xf=" FWF "\n", f->xf);
  fprintf (file, "yf=" FWF "\n", f->yf);
  fprintf (file, "zf=" FWF "\n", f->zf);
  fprintf (file, "S0=" FWF "\n", f->S0);
  fprintf (file, "fz=" FWF "\n", f->fz);
  fprintf (file, "l=" FWF "\n", f->l);
  fprintf (file, "gn2=" FWF "\n", f->gn2);
  fprintf (file, "i4=" FWF "\n", f->i4);
  fprintf (file, "furrow_show: end\n");
}
#endif

/**
 * Function to fix the initial and final coordinates of a furrow
 */
void
furrow_position (Furrow * f,    ///< Furrow structure.
                 JBDOUBLE x0,   ///< Initial point x-coordinate.
                 JBDOUBLE y0,   ///< Initial point y-coordinate.
                 JBDOUBLE z0,   ///< Initial point z-coordinate.
                 JBDOUBLE xf,   ///< Final point x-coordinate.
                 JBDOUBLE yf,   ///< Final point y-coordinate.
                 JBDOUBLE zf)   ///< Final point z-coordinate.
{
#if DEBUG_FURROW_POSITION
  fprintf (stderr, "furrow_position: start\n");
#endif
  f->x0 = x0;
  f->y0 = y0;
  f->z0 = z0;
  f->xf = xf;
  f->yf = yf;
  f->zf = zf;
#if DEBUG_FURROW_POSITION
  fprintf (stderr, "furrow_position: end\n");
#endif
}

/**
 * Function to calculate geometrical parameters of a furrow.
 */
void
furrow_open (Furrow * f)        ///< Furrow structure.
{
#if DEBUG_FURROW_POSITION
  fprintf (stderr, "furrow_open: start\n");
#endif
  if (f->epsilon > 0.)
    f->friction = TYPE_FRICTION_POTENTIAL;
  else
    f->friction = TYPE_FRICTION_MANNING;
  f->fz = jbm_fdbl (sqrt (1. + jbm_fsqr (f->z)));
  f->l = jbm_v2_length (f->x0, f->y0, f->xf, f->yf);
  f->S0 = (f->z0 - f->zf) / f->l;
  if (f->friction == TYPE_FRICTION_MANNING)
    f->gn2 = JBM_G * jbm_fsqr (f->n);
  else
    {
      f->fb = 0.27;
      f->gn2 =
        f->epsilon * (f->fb + 1.) * (f->fb + 1.) * pow (f->n, 2. * f->fb);
    }
  f->i4 = 1. / f->i2;
  f->Amax = f->D * f->H;
#if DEBUG_FURROW_POSITION
  fprintf (stderr, "furrow_open: end\n");
#endif
}
