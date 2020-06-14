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
 * \file parameters.h
 * \brief Header file to define the parameters of a mesh node.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2011-2020, Javier Burguete Tolosa.
 */
#ifndef PARAMETERS__H
#define PARAMETERS__H 1

#define DTMAX(u, c, dx) ((ABS(u) + c) / dx)
///< Macro to calculate the inverse of the maximum allowed time step.

#define AREA(h, b, z) (h * (b + z * h))
///< Macro to calculate the area of an isosceles trapezium.

#define parameters_depth_normal(p, Q) \
	(parameters_depth_function(p, Q, parameters_discharge_normal))
///< Macro to calculate the normal depth at a mesh node.

#define parameters_area_normal(p, Q) \
	(parameters_area(p, parameters_depth_normal(p, Q)))
///< Macro to calculate the normal area at a mesh node.

#define parameters_depth_critical(p, Q) \
	(parameters_depth_function(p, Q, parameters_discharge_critical))
///< Macro to calculate the critical depth at a mesh node.

#define parameters_area_critical(p, Q) \
	(parameters_area(p, parameters_depth_critical(p, Q)))
///< Macro to calculate the critical area at a mesh node.

/**
 * \def flux_limiter
 * \brief Macro to define the flux limiter
 */
#if FLUX_LIMITER == FLUX_LIMITER_NULL
#define flux_limiter jbm_flux_limiter_null
#elif FLUX_LIMITER == FLUX_LIMITER_TOTAL
#define flux_limiter jbm_flux_limiter_total
#elif FLUX_LIMITER == FLUX_LIMITER_CENTERED
#define flux_limiter jbm_flux_limiter_centered
#elif FLUX_LIMITER == FLUX_LIMITER_SUPERBEE
#define flux_limiter jbm_flux_limiter_superbee
#elif FLUX_LIMITER == FLUX_LIMITER_MINMOD
#define flux_limiter jbm_flux_limiter_minmod
#else
#error "Bad flux limiter"
#endif

#define PARAMETERS_AREA(p, h) (AREA(h, p->s->b, p->s->z))
///< Macro to calculate the area of a mesh node with the depth.

#define PARAMETERS_WIDTH(p, h) (p->s->b + jbm_fdbl(p->s->z * h))
///< Macro to calculate the width with the depth.

#define PARAMETERS_PERIMETER(p, h) (p->s->b + h * p->s->fz)
///< Macro to calculate the perimeter with the depth.

/**
 * \struct Parameters
 * \brief Struct data of a mesh node.
 */
typedef struct
{
  Furrow s[1];
  ///< Furrow structure.
  JBFLOAT A;
  ///< Superficial cross-sectional wetted area.
  JBFLOAT Q;
  ///< Discharge.
  JBFLOAT Ai;
  ///< Infiltrated cross-sectional area.
  JBFLOAT Ap;
  ///< Percolated cross-sectional area.
  JBFLOAT Ac;
  ///< Fertilizer mass per unit length of furrow.
  JBFLOAT Aci;
  ///< Infiltrated fertilizer mass per unit length of furrow.
  JBFLOAT Acp;
  ///< Percolated fertilizer mass per unit length of furrow.
  JBFLOAT c;
  ///< Fertilizer concentration.
  JBFLOAT m;
  ///< Solid fertilizer mass.
  JBFLOAT h;
  ///< Depth.
  JBFLOAT B;
  ///< Surface width.
  JBFLOAT P;
  ///< Superficial cross-sectional wetted perimeter.
  JBFLOAT I1;
  ///< Hydrostatic pressure integral.
  JBFLOAT u;
  ///< Superficial water mean velocity.
  JBFLOAT uc;
  ///< Velocity of the superficial waves.
  JBFLOAT Kx;
  ///< Coefficient of fertilizer diffusion.
  JBFLOAT KAx;
  ///< Fertilizer diffusion factor \f$K_x\cdot A\f$.
  JBFLOAT Kf;
  ///< Friction factor \f$\frac{g\cdot A\cdot S_f}{Q^2}\f$.
  JBFLOAT dx;
  ///< Size of the mesh cell.
  JBFLOAT x;
  ///< Cell distance to the furrow init.
  JBFLOAT x0;
  ///< Cell x-coordinate.
  JBFLOAT y0;
  ///< Cell y-coordinate.
  JBFLOAT zs;
  ///< Surface level.
  JBFLOAT zf;
  ///< Bottom level.
  JBFLOAT Amin;
  ///< Minimum superficial area to enable advance.
  JBFLOAT zmin;
  ///< Minimum surface level to enable advance.
  JBFLOAT F;
  ///< Momentum flux.
  JBFLOAT T;
  ///< Fertilizer flux.
  JBFLOAT H;
  ///< Friction source term.
  JBFLOAT beta;
  ///< Advection coefficient.
  JBFLOAT la;
  ///< 1st eigenvalue of the flux jacobian \f$u+c\f$.
  JBFLOAT lb;
  ///< 2nd eigenvalue of the flux jacobian \f$u-c\f$.
  JBFLOAT sA;
  ///< \f$\sqrt{A}\f$.
  JBFLOAT sAla;
  ///< \f$sA\cdot la\f$.
  JBFLOAT sAlb;
  ///< \f$sA\dot lb\f$.
  JBFLOAT sAu;
  ///< \f$sA\cdot u\f$.
  JBFLOAT sAc;
  ///< \f$sA\cdot c\f$.
  JBFLOAT lam;
  ///< Averaged 1st eigenvalue of the flux jacobian.
  JBFLOAT lbm;
  ///< Averaged 2nd eigenvalue of the flux jacobian.
  JBFLOAT um;
  ///< Roe's averaged velocity.
  JBFLOAT sm;
  ///< Roe's averaged fertilizer concentration.
  JBFLOAT dQ;
  ///< Difference of discharges.
  JBFLOAT dF;
  ///< Difference of momentum fluxes.
  JBFLOAT dT;
  ///< Difference of transport fluxes.
  JBFLOAT dQp;
  ///< Positive difference of discharges.
  JBFLOAT dQm;
  ///< Negative difference of discharges.
  JBFLOAT dFp;
  ///< Positive difference of momentum fluxes.
  JBFLOAT dFm;
  ///< Negative difference of momentum fluxes.
  JBFLOAT dTp;
  ///< Positive difference of transport fluxes.
  JBFLOAT dTm;
  ///< Negative difference of transport fluxes.
  JBFLOAT dWAp;
  ///< Positive difference of the 1st differential characteristic variables.
  JBFLOAT dWAm;
  ///< Negative difference of the 1st differential characteristic variables.
  JBFLOAT dWBp;
  ///< Positive difference of the 2nd differential characteristic variables.
  JBFLOAT dWBm;
  ///< Negative difference of the 2nd differential characteristic variables.
  JBFLOAT dWCp;
  ///< Positive difference of the 3th differential characteristic variables.
  JBFLOAT dWCm;
  ///< Negative difference of the 3th differential characteristic variables.
  JBFLOAT Qv;
  ///< Old discharge.
  JBFLOAT cv;
  ///< Old fertilizer concentration.
  JBFLOAT ix;
  ///< Distance between 2 nodes.
  JBFLOAT ta;
  ///< Advance time.
  JBFLOAT tr;
  ///< Recession time.
} Parameters;

extern unsigned int type_fertilizer, type_infiltration, type_diffusion_soil,
  type_beta;
extern JBDOUBLE solubility, t, dt, dtmax;

/**
 * Function to calculate mesh node distances.
 */
INLINE_VOID
parameters_ix (Parameters * p)  ///< Mesh node parameters structure.
{
  p->ix = (p + 1)->x - p->x;
}

/**
 * Function to calculate the area with the depth for possitive depths.
 *
 * \return Area.
 */
INLINE_JBDOUBLE
parameters_area (Parameters * p,        ///< Mesh node parameters structure.
                 JBDOUBLE h)    ///< Depth.
{
  if (h <= 0.)
    return 0.;
  return PARAMETERS_AREA (p, h);
}

/**
 * Function to calculate the width with the depth for possitive depths.
 *
 * \return Width.
 */
INLINE_JBDOUBLE
parameters_width (Parameters * p,       ///< Mesh node parameters structure.
                  JBDOUBLE h)   ///< Depth.
{
  if (h <= 0.)
    return 0.;
  return PARAMETERS_WIDTH (p, h);
}

/**
 * Function to calculate the perimeter with the depth for possitive depths.
 *
 * \return Perimeter.
 */
INLINE_JBDOUBLE
parameters_perimeter (Parameters * p,   ///< Mesh node parameters structure.
                      JBDOUBLE h)       ///< Depth.
{
  if (h <= 0.)
    return 0.;
  return PARAMETERS_PERIMETER (p, h);
}

/**
 * Function to calculate the critical discharge with the depth.
 *
 * \return Critical discharge.
 */
INLINE_JBDOUBLE
parameters_discharge_critical (Parameters * p,
///< Mesh node parameters structure.
                               JBDOUBLE h)      ///< Depth.
{
  register JBDOUBLE A, B;
  B = PARAMETERS_WIDTH (p, h);
  A = PARAMETERS_AREA (p, h);
  return A * sqrt (JBM_G * A / B);
}

/**
 * Function to calculate the normal discharge with the depth for possitive
 * depths.
 *
 * \return Normal discharge.
 */
INLINE_JBDOUBLE
parameters_discharge_normal (Parameters * p,
///< Mesh node parameters structure.
                             JBDOUBLE h)        ///< Depth.
{
  register JBDOUBLE A, P;
  if (h <= 0.)
    return 0.;
  A = PARAMETERS_AREA (p, h);
  P = PARAMETERS_PERIMETER (p, h);
  return sqrt (p->s->S0) * A * pow (A / P, 2. / 3.) / p->s->n;
}

/**
 * Function to calculate the depth with the discharge for a function Q(h)
 *
 * \return Depth.
 */
INLINE_JBDOUBLE
parameters_depth_function (Parameters * p,      ///< Mesh node parameters structure.
                           JBDOUBLE Q,  ///< Discharge.
                           JBDOUBLE (*function) (Parameters *, JBDOUBLE))
///< Q(h) function.
{
  JBDOUBLE h1, h2, h3, Q1, Q2, Q3;
  register int i;
  if (Q <= 0.)
    return 0.;
  h1 = 1.;
  while (function (p, h1) < Q)
    h1 += h1;
  while (function (p, h1) > Q)
    h1 *= 0.5;
  h2 = h1 + h1;
  Q1 = function (p, h1);
  Q2 = function (p, h2);
  for (i = 64; --i >= 0;)
    {
      h3 = h1 + (h2 - h1) * (Q - Q1) / (Q2 - Q1);
      if (h3 == h1 || h3 == h2)
        break;
      Q3 = function (p, h3);
      if (Q3 < Q)
        h1 = h3, Q1 = Q3;
      else if (Q3 > Q)
        h2 = h3, Q2 = Q3;
      else
        break;
    }
  return h3;
}

/**
 * Function to calculate all the parameters of a mesh node depending on the
 * depth.
 */
INLINE_VOID
parameters_depth (Parameters * p)       ///< Mesh node parameters structure.
{
  register JBDOUBLE k, k2;
  if (p->A <= 0.)
    {
      p->h = p->I1 = p->Kf = p->Kx = p->uc = p->sA = p->c = p->sAc = 0.;
      p->beta = 1.;
      p->B = p->P = p->s->b;
      p->zs = p->zf;
      if (p->tr == 0.)
        p->ta = t;
      return;
    }
  p->h = jbm_solve_quadratic (p->s->z, p->s->b, -p->A, 0., INFINITY);
  p->zs = p->zf + p->h;
  p->B = PARAMETERS_WIDTH (p, p->h);
  p->P = PARAMETERS_PERIMETER (p, p->h);
  p->I1 = (0.5 * p->s->b + 1. / 3. * p->s->z * p->h) * p->h * p->h;
  if (p->s->friction == TYPE_FRICTION_MANNING)
    {
      p->Kf = p->s->gn2 * pow (p->P / p->A, 4. / 3.) / p->A;
      p->beta = 1.;
    }
  else
    {
      if (p->h <= p->s->n)
        {
          p->Kf = 0.;
          p->beta = 1.;
        }
      else
        {
          k = pow (p->s->n, p->s->fb + 1.);
          k2 = sqrt (p->h);
          k = p->s->b * k2 * (pow (p->h, p->s->fb + 1.) - k) +
            p->s->z * ((pow (p->h, p->s->fb + 2.5) -
                        pow (p->s->n, p->s->fb + 2.5)) *
                       2. / (p->s->fb + 2.5) -
                       4. / 3. * k * (p->h * k2 - p->s->n * sqrt (p->s->n)));
          p->Kf = p->s->gn2 * p->A / (k * k);
          if (type_beta)
            {
              k = pow (p->s->n, 2. * p->s->fb + 1.);
              p->beta = (p->s->fb + 1.) * (p->s->fb + 1.) /
                (2. * p->s->fb + 1.) * p->Kf / p->s->gn2 *
                (p->s->b * p->h * (pow (p->h, 2. * p->s->fb + 1.) - k) +
                 p->s->z * ((pow (p->h, 2. * p->s->fb + 3.) -
                             pow (p->s->n, 2. * p->s->fb + 3.)) *
                            2. / (2. * p->s->fb + 3.) -
                            k * (p->h * p->h - p->s->n * p->s->n)));
            }
          else
            p->beta = 1.;
        }
    }
#if MODEL_DIFFUSION == MODEL_DIFFUSION_CONSTANT
  p->Kx = MODEL_DIFFUSION_K;
#else
  p->Kx = MODEL_DIFFUSION_K * p->B * fabs (p->Q) * sqrt (p->Kf / p->P);
#endif
  p->uc = sqrt (JBM_G * p->A / p->B);
  p->sA = sqrt (p->A);
  p->c = p->Ac / p->A;
  p->sAc = p->sA * p->c;
  p->tr = t;
}

/**
 * Function to calculate all the parameters of a mesh node depending on the
 * discharge.
 */
INLINE_VOID
parameters_flow (Parameters * p)        ///< Mesh node parameters structure.
{
  register JBDOUBLE k, k2;
  if (p->A <= p->Amin)
    {
      p->Q = p->u = p->F = p->H = p->la = p->lb = p->T = p->KAx = p->sAla =
        p->sAlb = p->sAu = 0.;
      return;
    }
  p->u = p->Q / p->A;
  p->H = p->Kf * p->Q * fabs (p->Q);
#if MODEL_FRICTION == MODEL_FRICTION_MANNING
  k = p->u;
  k2 = p->uc;
#else
  k = p->beta * p->u;
  k2 = sqrt (p->uc * p->uc + p->beta * (p->beta - 1.) * p->u * p->u);
#endif
  p->F = k * p->Q;
  p->la = k + k2;
  p->lb = k - k2;
  p->sAla = p->sA * p->la;
  p->sAlb = p->sA * p->lb;
  p->sAu = p->sA * p->u;
  p->T = p->u * p->Ac;
  p->KAx = p->Kx * p->A;
}

/**
 * Function to save Q and c.
 */
INLINE_VOID
parameters_save (Parameters * p)        ///< Mesh node parameters structure.
{
  p->Qv = p->Q;
  p->cv = p->c;
}

/**
 * Function to calculate the Ros's averages.
 */
INLINE_VOID
parameters_average_roe (Parameters * p) ///< Mesh node parameters structure.
{
  Parameters *pv;
  register JBDOUBLE k;
  pv = p + 1;
  if (p->A <= 0. && pv->A <= 0.)
    p->lam = p->lbm = p->um = p->sm = 0.;
  else
    {
      k = p->sA + pv->sA;
      p->lam = (p->sAla + pv->sAla) / k;
      p->lbm = (p->sAlb + pv->sAlb) / k;
      p->um = (p->sAu + pv->sAu) / k;
      p->sm = (p->sAc + pv->sAc) / k;
    }
}

/**
 * Function to calculate mumerical flux.
 */
INLINE_VOID
parameters_flux_scheme (Parameters * p)
///< Mesh node parameters structure.
{
  Parameters *pv;
  register JBDOUBLE k, k2;
  pv = p + 1;
  p->dQ = pv->Q - p->Q;
  p->dT = pv->T - p->T;
  if (fmax (pv->zs, p->zs) <= p->zmin)
    p->dF = 0.;
  else
    {
      p->dF = pv->F - p->F + JBM_G * (pv->I1 - p->I1);
      k = 0.5 * (pv->Q + p->Q) * p->ix / dt - p->dF;
      k2 = 0.5 * (pv->H + p->H) * p->ix;
      if (k < 0.)
        p->dF += fmax (k, k2);
      else
        p->dF += fmin (k, k2);
    }
}

/**
 * Function to calculate the 1st order upwind numerical flux.
 */
INLINE_VOID
parameters_flux_scheme_upwind (Parameters * p,
///< Mesh node parameters structure.
                               JBDOUBLE dt)
///< Time step size.
{
  register JBDOUBLE c2, dWA, dWB, dWC, dtx;     // dc;
  parameters_average_roe (p);
  dtx = dt / p->ix;
  c2 = p->lam - p->lbm;
  dWA = (p->dF - p->lbm * p->dQ) / c2;
  dWB = (p->lam * p->dQ - p->dF) / c2;
  dWC = p->dT - p->sm * p->dQ;
  if (p->lam <= 0.)
    {
      p->dQm = p->dQ;
      p->dFm = p->dF;
      p->dTm = p->dT;
      p->dWAm = 0.5 * (1 + p->lam * dtx) * dWA;
      p->dWBm = 0.5 * (1 + p->lbm * dtx) * dWB;
      p->dWCm = 0.5 * (1 + p->um * dtx) * dWC;
      p->dQp = p->dFp = p->dTp = p->dWAp = p->dWBp = p->dWCp = 0.;
    }
  else if (p->lbm >= 0.)
    {
      p->dQp = p->dQ;
      p->dFp = p->dF;
      p->dTp = p->dT;
      p->dWAp = 0.5 * (1 - p->lam * dtx) * dWA;
      p->dWBp = 0.5 * (1 - p->lbm * dtx) * dWB;
      p->dWCp = 0.5 * (1 - p->um * dtx) * dWC;
      p->dQm = p->dFm = p->dTm = p->dWAm = p->dWBm = p->dWCm = 0.;
    }
  else
    {
      p->dQp = dWA;
      p->dFp = p->lam * dWA;
      p->dWAp = 0.5 * (1. - p->lam * dtx) * dWA;
      p->dWBp = p->dWAm = 0.;
      p->dQm = dWB;
      p->dFm = p->lbm * dWB;
      p->dWBm = 0.5 * (1. + p->lbm * dtx) * dWB;
      if (p->um >= 0.)
        {
          p->dTm = p->dQm * p->sm;
          p->dTp = p->dT - p->dTm;
          p->dWCp = 0.5 * (1 - p->um * dtx) * dWC;
          p->dWCm = 0.;
        }
      else
        {
          p->dTp = p->dQp * p->sm;
          p->dTm = p->dT - p->dTp;
          p->dWCm = 0.5 * (1 + p->um * dtx) * dWC;
          p->dWCp = 0.;
        }
      /*
         dc = (p+1)->c - p->c;
         if (dc != 0.)
         {
         dc *= fmax(0., fmax(p->dQm * (p->sm - p->c) / dc,
         (p->um * (p->sm * ((p+1)->A - p->A) - (p+1)->Ac + p->Ac)
         + p->dQp * ((p+1)->c - p->sm)) / dc)) * ((p+1)->Ac - p->Ac);
         p->dTp += dc;
         p->dTm -= dc;
         }
       */
    }
}

/**
 * Function to calculate the entropy numerical flux.
 */
INLINE_VOID
parameters_flux_scheme_entropy (Parameters * p,
///< Mesh node parameters structure.
                                JBDOUBLE dA,
///< Area variation: \f$\delta A\f$.
                                JBDOUBLE dAc,
///< Fertilizer mass variation: \f$\delta(Ac)\f$.
                                JBDOUBLE nu)
///< Artificial viscosity coefficient.
{
  register JBDOUBLE k;
  k = nu * dA;
  p->dQm -= k;
  p->dQp += k;
  k = nu * p->dQ;
  p->dFm -= k;
  p->dFp += k;
  k = nu * dAc;
  p->dTm -= k;
  p->dTp += k;
}

/**
 * Function to calculate the entropy artificial viscosit.
 */
INLINE_VOID
parameters_flux_scheme_correction_entropy (Parameters * p)
///< Mesh node parameters structure.
{
  Parameters *pv = p + 1;
  if (p->la < 0. && pv->la > 0.)
    parameters_flux_scheme_entropy (p, pv->A - p->A, pv->Ac - p->Ac,
                                    0.25 * (pv->la - p->la -
                                            fabs (p->lam + p->lam)));
  if (p->lb < 0. && pv->lb > 0.)
    parameters_flux_scheme_entropy (p, pv->A - p->A, pv->Ac - p->Ac,
                                    0.25 * (pv->lb - p->lb -
                                            fabs (p->lbm + p->lbm)));
}

/**
 * Function to calculate the numerical scheme flux.
 */
INLINE_VOID
parameters_scheme (Parameters * p)
///< Mesh node parameters structure.
{
  parameters_flux_scheme (p);
  if (fmax (p->zs, (p + 1)->zs) <= p->zmin)
    {
      p->lam = p->lbm = p->um = p->sm = p->dFp = p->dFm = 0.;
      if (p->A > p->Amin)
        {
          p->dQm = p->dQ;
          p->dTm = p->dT;
          p->dQp = p->dTp =
            p->dWAp = p->dWBp = p->dWCp = p->dWAm = p->dWBm = p->dWCm = 0.;
        }
      else
        {
          p->dQp = p->dQ;
          p->dTp = p->dT;
          p->dQm = p->dTm =
            p->dWAp = p->dWBp = p->dWCp = p->dWAm = p->dWBm = p->dWCm = 0.;
        }
      return;
    }
  parameters_flux_scheme_upwind (p, dt);
  parameters_flux_scheme_correction_entropy (p);
}

/**
 * Function to apply the 1st order upwind numerical scheme.
 */
INLINE_VOID
parameters_scheme_upwind (Parameters * p)
///< Mesh node parameters structure.
{
  Parameters *pv;
  register JBDOUBLE k;
  pv = p + 1;
  k = dt / p->dx;
  p->A -= k * p->dQm;
  p->Q -= k * p->dFm;
  p->Ac -= k * p->dTm;
  k = dt / pv->dx;
  pv->A -= k * p->dQp;
  pv->Q -= k * p->dFp;
  pv->Ac -= k * p->dTp;
}

/**
 * Function to apply the 2nd order TVD numerical scheme.
 */
INLINE_VOID
parameters_scheme_tvd (Parameters * p)
///< Mesh node parameters structure.
{
  Parameters *pv;
  JBDOUBLE k, dWA, dWB, dWC, dQ, dF, dT;
  pv = p - 1;
  parameters_scheme_upwind (p);
  dWA = p->dWAm * flux_limiter (pv->dWAm, p->dWAm);
  dWB = p->dWBm * flux_limiter (pv->dWBm, p->dWBm);
  dWC = p->dWCm * flux_limiter (pv->dWCm, p->dWCm);
  dQ = dWA + dWB;
  dF = p->lam * dWA + p->lbm * dWB;
  dT = p->sm * dQ + dWC;
  k = dt / p->dx;
  p->A -= k * dQ;
  p->Q -= k * dF;
  p->Ac -= k * dT;
  k = dt / pv->dx;
  pv->A += k * dQ;
  pv->Q += k * dF;
  pv->Ac += k * dT;
  pv = p + 1;
  dWA = p->dWAp * flux_limiter (pv->dWAp, p->dWAp);
  dWB = p->dWBp * flux_limiter (pv->dWBp, p->dWBp);
  dWC = p->dWCp * flux_limiter (pv->dWCp, p->dWCp);
  dQ = dWA + dWB;
  dF = p->lam * dWA + p->lbm * dWB;
  dT = p->sm * dQ + dWC;
  k = dt / pv->dx;
  pv->A -= k * dQ;
  pv->Q -= k * dF;
  pv->Ac -= k * dT;
  ++pv;
  k = dt / pv->dx;
  pv->A += k * dQ;
  pv->Q += k * dF;
  pv->Ac += k * dT;
}

/**
 * Function to calculate the basic infiltration parameters.
 */
INLINE_VOID
parameters_infiltration_base (Parameters * p)
///< Mesh node parameters structure.
{
  if (p->A <= 0.)
    {
      p->P = p->s->b;
      p->c = 0.;
    }
  else
    {
      p->P = PARAMETERS_PERIMETER (p,
                                   jbm_solve_quadratic (p->s->z, p->s->b,
                                                        -p->A, 0., INFINITY));
      p->c = p->Ac / p->A;
    }
}

/**
 * Function to calculate the Kostiakov-Lewis infiltration model.
 *
 * \return Infiltrated area in a time step.
 */
INLINE_JBDOUBLE
parameters_infiltration_kostiakov_lewis (Parameters * p)
///< Mesh node parameters structure.
{
  return pow (pow (p->Ai, p->s->i4)
              + dt * p->P / p->s->D * pow (p->s->i1 * p->s->D, p->s->i4),
              p->s->i2) + p->s->i3 * p->P * dt - p->Ai;
}

/**
 * Function to calculate the infiltration numerical scheme.
 */
INLINE_VOID
parameters_infiltration_scheme (Parameters * p)
///< Mesh node parameters structure.
{
  register JBDOUBLE dA, dM;
  parameters_infiltration_base (p);
  dA = parameters_infiltration_kostiakov_lewis (p);
  dA = fmax (0., fmin (dA, p->A));
  p->A -= dA;
  p->Ai += dA;
  dA *= p->c;
  p->Ac -= dA;
  p->Aci += dA;
  if (p->Ai > p->s->Amax)
    {
      dA = p->Ai - p->s->Amax;
      dM = dA * p->Aci / p->Ai;
      p->Ai -= dA;
      p->Ap += dA;
      p->Aci -= dM;
      p->Acp += dM;
    }
}

/**
 * Function to calculate the solubility in a mesh node.
 */
INLINE_VOID
parameters_solubility (Parameters * p)
///< Mesh node parameters structure.
{
  register JBDOUBLE m, mc, vc;
  mc = p->Ac * p->dx + p->m;
  vc = p->A * p->dx;
  m = solubility * vc;
  if (mc > m)
    {
      p->m = mc - m;
      p->Ac = m / p->dx;
    }
  else
    {
      p->Ac = mc / p->dx;
      p->m = 0.;
    }
  if (vc > 0.)
    p->c = p->Ac / p->A;
  else
    p->c = 0.;
}

/**
 * Function to calculate the numerical parameters in a mesh node.
 */
INLINE_VOID
parameters_open (Parameters * p)
///< Mesh node parameters structure.
{
  parameters_depth (p);
  parameters_solubility (p);
  parameters_flow (p);
}

/**
 * Function to calculate the total water volume in a mesh node.
 *
 * \return Total water volume.
 */
INLINE_JBDOUBLE
parameters_volume_water_total (Parameters * p)
///< Mesh node parameters structure.
{
  return (p->A + p->Ai + p->Ap) * p->dx;
}

/**
 * Function to calculate the superficial water volume in a mesh node.
 *
 * \return Superficial water volume.
 */
INLINE_JBDOUBLE
parameters_volume_water_surface (Parameters * p)
///< Mesh node parameters structure.
{
  return p->A * p->dx;
}

/**
 * Function to calculate the infiltrated water volume in a mesh node.
 *
 * \return Infiltrated water volume.
 */
INLINE_JBDOUBLE
parameters_volume_water_soil (Parameters * p)
///< Mesh node parameters structure.
{
  return p->Ai * p->dx;
}

/**
 * Function to calculate the percolated water volume in a mesh node.
 *
 * \return Percolated water volume.
 */
INLINE_JBDOUBLE
parameters_volume_water_loss (Parameters * p)
///< Mesh node parameters structure.
{
  return p->Ap * p->dx;
}

/**
 * Function to calculate the total fertilizer mass in a mesh node.
 *
 * \return Total fertilizer mass.
 */
INLINE_JBDOUBLE
parameters_mass_fertilizer_total (Parameters * p)
///< Mesh node parameters structure.
{
  return (p->Ac + p->Aci + p->Acp) * p->dx + p->m;
}

/**
 * Function to calculate the superficial fertilizer mass in a mesh node.
 *
 * \return Superficial fertilizer mass.
 */
INLINE_JBDOUBLE
parameters_mass_fertilizer_surface (Parameters * p)
///< Mesh node parameters structure.
{
  return p->Ac * p->dx;
}

/**
 * Function to calculate the infiltrated fertilizer mass in a mesh node.
 *
 * \return Infiltrated fertilizer mass.
 */
INLINE_JBDOUBLE
parameters_mass_fertilizer_soil (Parameters * p)
///< Mesh node parameters structure.
{
  return p->Aci * p->dx;
}

/**
 * Function to calculate the percolated fertilizer mass in a mesh node.
 *
 * \return Percolated fertilizer mass.
 */
INLINE_JBDOUBLE
parameters_mass_fertilizer_loss (Parameters * p)
///< Mesh node parameters structure.
{
  return p->Acp * p->dx;
}

/**
 * Function to calculate the solid fertilizer mass in a mesh node.
 *
 * \return Solid fertizer mass.
 */
INLINE_JBDOUBLE
parameters_mass_fertilizer_solid (Parameters * p)
///< Mesh node parameters structure.
{
  return p->m;
}

void parameters_show (Parameters * p, FILE * file);

#endif
