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
 * \file parameters.h
 * \brief Header file to define the parameters of a mesh node.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2011-2014, Javier Burguete Tolosa.
 */
#ifndef PARAMETERS__H
#define PARAMETERS__H 1

#define _GNU_SOURCE
#include <string.h>
#include "furrow.h"

/**
 * \struct _Parameters
 * \brief Struct data of a mesh node.
 */
struct _Parameters
{
/**
 * \var Furrow s
 * \brief Furrow structure.
 * \var A
 * \brief Superficial cross-sectional wetted area.
 * \var Q
 * \brief Discharge.
 * \var Ai
 * \brief Infiltrated cross-sectional area.
 * \var Ap
 * \brief Percolated cross-sectional area.
 * \var Ac
 * \brief Fertilizer mass per unit length of furrow.
 * \var Aci
 * \brief Infiltrated fertilizer mass per unit length of furrow.
 * \var Acp
 * \brief Percolated fertilizer mass per unit length of furrow.
 * \var c
 * \brief Fertilizer concentration.
 * \var m
 * \brief Solid fertilizer mass.
 * \var h
 * \brief Depth.
 * \var B
 * \brief Surface width.
 * \var P
 * \brief Superficial cross-sectional wetted perimeter.
 * \var I1
 * \brief Hydrostatic pressure integral.
 * \var u
 * \brief Superficial water mean velocity.
 * \var uc
 * \brief Velocity of the superficial waves.
 * \var Kx
 * \brief Coefficient of fertilizer diffusion.
 * \var KAx
 * \brief \f$K_x\cdot A\f$.
 * \var Kf
 * \brief Friction factor \f$\frac{g\cdot A\cdot S_f}{Q^2}\f$.
 * \var dx
 * \brief Size of the mesh cell.
 * \var x
 * \brief Cell distance to the furrow init.
 * \var x0
 * \brief Cell x-coordinate.
 * \var y0
 * \brief Cell y-coordinate.
 * \var zs
 * \brief Surface level.
 * \var zf
 * \brief Bottom level.
 * \var Amin
 * \brief Minimum superficial area to enable advance.
 * \var zmin
 * \brief Minimum surface level to enable advance.
 * \var F
 * \brief Momentum flux.
 * \var T
 * \brief Fertilizer flux.
 * \var H
 * \brief Friction source term.
 * \var beta
 * \brief Advection coefficient.
 * \var la
 * \brief 1st eigenvalue of the flux jacobian \f$u+c\f$.
 * \var lb
 * \brief 2nd eigenvalue of the flux jacobian \f$u-c\f$.
 * \var sA
 * \brief \f$\sqrt{A}\f$.
 * \var sAla
 * \brief \f$sA\cdot la\f$.
 * \var sAlb
 * \brief \f$sA\dot lb\f$.
 * \var sAu
 * \brief \f$sA\cdot u\f$.
 * \var sAc
 * \brief \f$sA\cdot c\f$.
 * \var lam
 * \brief Averaged 1st eigenvalue of the flux jacobian.
 * \var lbm
 * \brief Averaged 2nd eigenvalue of the flux jacobian.
 * \var um
 * \brief Roe's averaged velocity.
 * \var sm
 * \brief Roe's averaged fertilizer concentration.
 * \var dQ
 * \brief Difference of discharges.
 * \var dF
 * \brief Difference of momentum fluxes.
 * \var dT
 * \brief Difference of transport fluxes.
 * \var dQp
 * \brief Positive difference of discharges.
 * \var dQm
 * \brief Negative difference of discharges.
 * \var dFp
 * \brief Positive difference of momentum fluxes.
 * \var dFm
 * \brief Negative difference of momentum fluxes.
 * \var dTp
 * \brief Positive difference of transport fluxes.
 * \var dTm
 * \brief Negative difference of transport fluxes.
 * \var dWAp
 * \brief Positive difference of the 1st differential characteristic variables.
 * \var dWAm
 * \brief Negative difference of the 1st differential characteristic variables.
 * \var dWBp
 * \brief Positive difference of the 2nd differential characteristic variables.
 * \var dWBm
 * \brief Negative difference of the 2nd differential characteristic variables.
 * \var dWCp
 * \brief Positive difference of the 3th differential characteristic variables.
 * \var dWCm
 * \brief Negative difference of the 3th differential characteristic variables.
 * \var Qv
 * \brief Old discharge.
 * \var cv
 * \brief Old fertilizer concentration.
 * \var ix
 * \brief Distance between 2 nodes.
 * \var ta
 * \brief Advance time.
 * \var tr
 * \brief Recession time.
*/
	Furrow s[1];
	JBFLOAT A, Q, Ai, Ap, Ac, Aci, Acp, c, m, h, B, P, I1, u, uc, Kx, KAx, Kf,
		dx, x, x0, y0, zs, zf, Amin, zmin, F, T, H, beta, la, lb, sA,
		sAla, sAlb, sAu, sAc, lam, lbm, um, sm, dQ, dF, dT,
		dQp, dQm, dFp, dFm, dTp, dTm, dWAp, dWAm, dWBp, dWBm, dWCp, dWCm,
		Qv, cv, ix, ta, tr;
};

/**
 * \typedef Parameters
 */
typedef struct _Parameters Parameters;

extern unsigned int type_fertilizer, type_infiltration, type_diffusion_soil,
	type_beta;
extern JBDOUBLE solubility, t, dt, dtmax;

/**
 * \def DTMAX
 * \brief Macro to calculate the inverse of the maximum allowed time step.
 * \param u
 * \brief Flow velocity.
 * \param c
 * \brief Velocity of the infinitesimal waves.
 * \param dx
 * \brief Mesh node size.
 */
#define DTMAX(u, c, dx) ((ABS(u) + c) / dx)

/**
 * \def AREA
 * \brief Macro to calculate the area of an isosceles trapezium.
 * \param b
 * \brief Trapezium base.
 * \param z
 * \brief Slope of the trapezium walls.
 * \param h
 * \brief Trapezium height.
 */
#define AREA(h, b, z) (h * (b + z * h))

/**
 * \def parameters_depth_normal
 * \brief Macro to calculate the normal depth at a mesh node.
 * \param p
 * \brief Mesh node parameters structure.
 * \param Q
 * \brief Discharge.
 */
#define parameters_depth_normal(p, Q) \
	(parameters_depth_function(p, Q, parameters_discharge_normal))

/**
 * \def parameters_area_normal
 * \brief Macro to calculate the normal area at a mesh node.
 * \param p
 * \brief Mesh node parameters structure.
 * \param Q
 * \brief Discharge.
 */
#define parameters_area_normal(p, Q) \
	(parameters_area(p, parameters_depth_normal(p, Q)))

/**
 * \def parameters_depth_critical
 * \brief Macro to calculate the critical depth at a mesh node.
 * \param p
 * \brief Mesh node parameters structure.
 * \param Q
 * \brief Discharge.
 */
#define parameters_depth_critical(p, Q) \
	(parameters_depth_function(p, Q, parameters_discharge_critical))

/**
 * \def parameters_area_critical
 * \brief Macro to calculate the critical area at a mesh node.
 * \param p
 * \brief Mesh node parameters structure.
 * \param Q
 * \brief Discharge.
 */
#define parameters_area_critical(p, Q) \
	(parameters_area(p, parameters_depth_critical(p, Q)))

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

/**
 * \fn INLINE_VOID parameters_ix(Parameters *p)
 * \brief Function to calculate mesh node distances.
 * \param p
 * \brief Mesh node parameters structure.
 */
INLINE_VOID parameters_ix(Parameters *p)
{
	p->ix = (p+1)->x - p->x;
}

/**
 * \def PARAMETERS_AREA
 * \brief Macro to calculate the area of a mesh node with the depth.
 * \param p
 * \brief Mesh node parameters structure.
 * \param h
 * \brief Depth.
 */
#define PARAMETERS_AREA(p, h) (AREA(h, p->s->b, p->s->z))

/**
 * \fn INLINE_JBDOUBLE parameters_area(Parameters *p, JBDOUBLE h)
 * \brief Function to calculate the area with the depth for possitive depths.
 * \param p
 * \brief Mesh node parameters structure.
 * \param h
 * \brief Depth.
 */
INLINE_JBDOUBLE parameters_area(Parameters *p, JBDOUBLE h)
{
	if (h <= 0.) return 0.;
	return PARAMETERS_AREA(p, h);
}

/**
 * \def PARAMETERS_WIDTH
 * \brief Macro to calculate the width with the depth.
 * \param p
 * \brief Mesh node parameters structure.
 * \param h
 * \brief Depth.
 */
#define PARAMETERS_WIDTH(p, h) (p->s->b + jbm_fdbl(p->s->z * h))

/**
 * \fn INLINE_JBDOUBLE parameters_width(Parameters *p, JBDOUBLE h)
 * \brief Function to calculate the width with the depth for possitive depths.
 * \param p
 * \brief Mesh node parameters structure.
 * \param h
 * \brief Depth.
 */
INLINE_JBDOUBLE parameters_width(Parameters *p, JBDOUBLE h)
{
	if (h <= 0.) return 0.;
	return PARAMETERS_WIDTH(p, h);
}

/**
 * \def PARAMETERS_PERIMETER
 * \brief Macro to calculate the perimeter with the depth.
 * \param p
 * \brief Mesh node parameters structure.
 * \param h
 * \brief Depth.
 */
#define PARAMETERS_PERIMETER(p, h) (p->s->b + h * p->s->fz)

/**
 * \fn INLINE_JBDOUBLE parameters_perimeter(Parameters *p, JBDOUBLE h)
 * \brief Function to calculate the perimeter with the depth for possitive
 *   depths.
 * \param p
 * \brief Mesh node parameters structure
 * \param h
 * \brief Depth.
 */
INLINE_JBDOUBLE parameters_perimeter(Parameters *p, JBDOUBLE h)
{
	if (h <= 0.) return 0.;
	return PARAMETERS_PERIMETER(p, h);
}

/**
 * \fn INLINE_JBDOUBLE parameters_discharge_critical(Parameters *p, JBDOUBLE h)
 * \brief Function to calculate the critical discharge with the depth.
 * \param p
 * \brief Mesh node parameters structure.
 * \param h
 * \brief Depth.
 */
INLINE_JBDOUBLE parameters_discharge_critical(Parameters *p, JBDOUBLE h)
{
	register JBDOUBLE A, B;
	B = PARAMETERS_WIDTH(p, h);
	A = PARAMETERS_AREA(p, h);
	return A * sqrt(JBM_G * A / B);
}

/**
 * \fn INLINE_JBDOUBLE parameters_discharge_normal(Parameters *p, JBDOUBLE h)
 * \brief Function to calculate the normal discharge with the depth for
 *   possitive depths.
 * \param p
 * \brief Mesh node parameters structure:
 * \param h
 * \brief Depth.
 */
INLINE_JBDOUBLE parameters_discharge_normal(Parameters *p, JBDOUBLE h)
{
	register JBDOUBLE A, P;
	if (h <= 0.) return 0.;
	A = PARAMETERS_AREA(p, h);
	P = PARAMETERS_PERIMETER(p, h);
	return sqrt(p->s->S0) * A * pow(A / P, 2./3.) / p->s->n;
}

/**
 * \fn INLINE_JBDOUBLE parameters_depth_function\
 * 	(Parameters *p, JBDOUBLE Q, JBDOUBLE (*function)(Parameters*, JBDOUBLE))
 * \brief Function to calculate the depth with the discharge for a function Q(h)
 * \param p
 * \brief Mesh node parameters structure.
 * \param Q
 * \brief Discharge.
 * \param function
 * \brief Q(h) function.
 */
INLINE_JBDOUBLE parameters_depth_function
	(Parameters *p, JBDOUBLE Q, JBDOUBLE (*function)(Parameters*, JBDOUBLE))
{
	register int i;
	JBDOUBLE h1, h2, h3, Q1, Q2, Q3;
	if (Q <= 0.) return 0.;
	h1 = 1.;
	while (function(p, h1) < Q) h1 += h1;
	while (function(p, h1) > Q) h1 *= 0.5;
	h2 = h1 + h1;
	Q1 = function(p, h1);
	Q2 = function(p, h2);
	for (i = 64; --i >= 0;)
	{
		h3 = h1 + (h2 - h1) * (Q - Q1) / (Q2 - Q1);
		if (h3 == h1 || h3 == h2) break;
		Q3 = function(p, h3);
		if (Q3 < Q) h1 = h3, Q1 = Q3;
		else if (Q3 > Q) h2 = h3, Q2 = Q3;
		else break;
	}
	return h3;
}

/**
 * \fn INLINE_VOID parameters_depth(Parameters *p)
 * \brief Function to calculate all the parameters of a mesh node depending on
 *   the depth.
 * \param p
 * \brief Mesh node parameters structure.
 */
INLINE_VOID parameters_depth(Parameters *p)
{
	register JBDOUBLE k, k2;
	if (p->A <= 0.)
	{
		p->h = p->I1 = p->Kf = p->Kx = p->uc = p->sA = p->c = p->sAc = 0.;
		p->beta = 1.;
		p->B = p->P = p->s->b;
		p->zs = p->zf;
		if (p->tr == 0.) p->ta = t;
		return;
	}
	p->h = jbm_solve_cuadratic(p->s->z, p->s->b, -p->A, 0., INFINITY);
	p->zs = p->zf + p->h;
	p->B = PARAMETERS_WIDTH(p, p->h);
	p->P = PARAMETERS_PERIMETER(p, p->h);
	p->I1 = (0.5 * p->s->b + 1./3. * p->s->z * p->h) * p->h * p->h;
	if (p->s->friction == TYPE_FRICTION_MANNING)
	{
		p->Kf = p->s->gn2 * pow(p->P / p->A, 4./3.) / p->A;
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
			k = pow(p->s->n, p->s->fb + 1.);
			k2 = sqrt(p->h);
			k = p->s->b * k2 * (pow(p->h, p->s->fb + 1.) - k) +
				p->s->z * ((pow(p->h, p->s->fb + 2.5) -
				pow(p->s->n, p->s->fb + 2.5)) *
				2. / (p->s->fb + 2.5) -
				4./3. * k * (p->h * k2 - p->s->n * sqrt(p->s->n)));
			p->Kf = p->s->gn2 * p->A / (k * k);
			if (type_beta)
			{
				k = pow(p->s->n, 2. * p->s->fb + 1.);
				p->beta = (p->s->fb + 1.) * (p->s->fb + 1.) /
					(2. * p->s->fb + 1.) * p->Kf / p->s->gn2 *
					(p->s->b * p->h * (pow(p->h, 2. * p->s->fb + 1.) - k) +
					p->s->z * ((pow(p->h, 2. * p->s->fb + 3.) -
					pow(p->s->n, 2. * p->s->fb + 3.)) *
					2. / (2. * p->s->fb + 3.) -
					k * (p->h * p->h - p->s->n * p->s->n)));
			}
			else p->beta = 1.;
		}
	}
	#if MODEL_DIFFUSION == MODEL_DIFFUSION_CONSTANT
		p->Kx = MODEL_DIFFUSION_K;
	#else
		p->Kx = MODEL_DIFFUSION_K * p->B * fabs(p->Q) * sqrt(p->Kf / p->P);
	#endif
	p->uc = sqrt(JBM_G * p->A / p->B);
	p->sA = sqrt(p->A);
	p->c = p->Ac / p->A;
	p->sAc = p->sA * p->c;
	p->tr = t;
}

/**
 * \fn INLINE_VOID parameters_flow(Parameters *p)
 * \brief Function to calculate all the parameters of a mesh node depending on
 *   the discharge.
 * \param p
 * \brief Mesh node parameters structure.
 */
INLINE_VOID parameters_flow(Parameters *p)
{
	register JBDOUBLE k,k2;
	if (p->A <= p->Amin)
	{
		p->Q = p->u = p->F = p->H = p->la = p->lb = p->T = p->KAx = p->sAla =
			p->sAlb = p->sAu = 0.;
		return;
	}
	p->u = p->Q / p->A;
	p->H = p->Kf * p->Q * fabs(p->Q);
	#if MODEL_FRICTION == MODEL_FRICTION_MANNING
		k = p->u;
		k2 = p->uc;
	#else
		k = p->beta * p->u;
		k2 = sqrt(p->uc * p->uc + p->beta * (p->beta - 1.) * p->u * p->u);
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
 * \fn INLINE_VOID parameters_save(Parameters *p)
 * \brief Function to save Q and c.
 * \param p
 * \brief Mesh node parameters structure.
 */
INLINE_VOID parameters_save(Parameters *p)
{
	p->Qv = p->Q;
	p->cv = p->c;
}

/**
 * \fn INLINE_VOID parameters_average_roe(Parameters *p)
 * \brief Function to calculate the Ros's averages.
 * \param p
 * \brief Mesh node parameters structure.
 */
INLINE_VOID parameters_average_roe(Parameters *p)
{
	register JBDOUBLE k;
	Parameters *pv=p+1;
	if (p->A <= 0. && pv->A <= 0.) p->lam = p->lbm = p->um = p->sm = 0.; else
	{
		k = p->sA + pv->sA;
		p->lam = (p->sAla + pv->sAla) / k;
		p->lbm = (p->sAlb + pv->sAlb) / k;
		p->um = (p->sAu + pv->sAu) / k;
		p->sm = (p->sAc + pv->sAc) / k;
	}
}

/**
 * \fn INLINE_VOID parameters_flux_scheme(Parameters *p)
 * \brief Function to calculate mumerical flux.
 * \param p
 * \brief Mesh node parameters structure.
 */
INLINE_VOID parameters_flux_scheme(Parameters *p)
{
	register JBDOUBLE k, k2;
	Parameters *pv=p+1;
	p->dQ = pv->Q - p->Q;
	p->dT = pv->T - p->T;
	if (fmax(pv->zs, p->zs) <= p->zmin) p->dF = 0.; else
	{
		p->dF = pv->F - p->F + JBM_G * (pv->I1 - p->I1);
		k = 0.5 * (pv->Q + p->Q) * p->ix / dt - p->dF;
		k2 = 0.5 * (pv->H + p->H) * p->ix;
		if (k < 0.) p->dF += fmax(k, k2); else p->dF += fmin(k, k2);
	}
}

/**
 * \fn INLINE_VOID parameters_flux_scheme_upwind(Parameters *p, JBDOUBLE dt)
 * \brief Function to calculate the 1st order upwind numerical flux.
 * \param p
 * \brief Mesh node parameters structure.
 * \param dt
 * \brief Time step size.
 */
INLINE_VOID parameters_flux_scheme_upwind(Parameters *p, JBDOUBLE dt)
{
	register JBDOUBLE c2, dWA, dWB, dWC, dtx; // dc;
	parameters_average_roe(p);
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
 * \fn INLINE_VOID parameters_flux_scheme_entropy\
 *   (Parameters *p, JBDOUBLE dA, JBDOUBLE dAc, JBDOUBLE nu)
 * \brief Function to calculate the entropy numerical flux.
 * \param p
 * \brief Mesh node parameters structure.
 * \param dA
 * \brief Area variation: \f$\delta A\f$.
 * \param dAc
 * \brief Fertilizer mass variation: \f$\delta(Ac)\f$.
 * \param nu
 * \brief Artificial viscosity coefficient.
 */
INLINE_VOID parameters_flux_scheme_entropy
	(Parameters *p, JBDOUBLE dA, JBDOUBLE dAc, JBDOUBLE nu)
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
 * \fn INLINE_VOID parameters_flux_scheme_correction_entropy(Parameters *p)
 * \brief Function to calculate the entropy artificial viscosit.
 * \param p
 * \brief Mesh node parameters structure.
 */
INLINE_VOID parameters_flux_scheme_correction_entropy(Parameters *p)
{
	Parameters *pv=p+1;
	if (p->la<0. && pv->la>0.)
		parameters_flux_scheme_entropy(p, pv->A - p->A, pv->Ac - p->Ac,
			0.25 * (pv->la - p->la - fabs(p->lam + p->lam)));
	if (p->lb<0. && pv->lb>0.)
		parameters_flux_scheme_entropy(p, pv->A - p->A, pv->Ac - p->Ac,
			0.25 * (pv->lb - p->lb - fabs(p->lbm + p->lbm)));
}

/**
 * \fn INLINE_VOID parameters_scheme(Parameters *p)
 * \brief Function to calculate the numerical scheme flux.
 * \param p
 * \brief Mesh node parameters structure.
 */
INLINE_VOID parameters_scheme(Parameters *p)
{
	parameters_flux_scheme(p);
	if (fmax(p->zs, (p+1)->zs) <= p->zmin)
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
	parameters_flux_scheme_upwind(p, dt);
	parameters_flux_scheme_correction_entropy(p);
}

/**
 * \fn INLINE_VOID parameters_scheme_upwind(Parameters *p)
 * \brief Function to apply the 1st order upwind numerical scheme.
 * \param p
 * \brief Mesh node parameters structure.
 */
INLINE_VOID parameters_scheme_upwind(Parameters *p)
{
	register JBDOUBLE k;
	Parameters *pv = p + 1;
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
 * \fn INLINE_VOID parameters_scheme_tvd(Parameters *p)
 * \brief Function to apply the 2nd order TVD numerical scheme.
 * \param p
 * \brief Mesh node parameters structure.
 */
INLINE_VOID parameters_scheme_tvd(Parameters *p)
{
	JBDOUBLE k, dWA, dWB, dWC, dQ, dF, dT;
	Parameters *pv = p - 1;
	parameters_scheme_upwind(p);
	dWA = p->dWAm * flux_limiter(pv->dWAm, p->dWAm);
	dWB = p->dWBm * flux_limiter(pv->dWBm, p->dWBm);
	dWC = p->dWCm * flux_limiter(pv->dWCm, p->dWCm);
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
	dWA = p->dWAp * flux_limiter(pv->dWAp, p->dWAp);
	dWB = p->dWBp * flux_limiter(pv->dWBp, p->dWBp);
	dWC = p->dWCp * flux_limiter(pv->dWCp, p->dWCp);
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
 * \fn INLINE_VOID parameters_infiltration_base(Parameters *p)
 * \brief Function to calculate the basic infiltration parameters.
 * \param p
 * \brief Mesh node parameters structure.
 */
INLINE_VOID parameters_infiltration_base(Parameters *p)
{
	if (p->A <= 0.)
	{
		p->P = p->s->b;
		p->c = 0.;
	}
	else
	{
		p->P = PARAMETERS_PERIMETER(p,
			jbm_solve_cuadratic(p->s->z, p->s->b, -p->A, 0., INFINITY));
		p->c = p->Ac / p->A;
	}
}

/**
 * \fn INLINE_JBDOUBLE parameters_infiltration_kostiakov_lewis\
 *   (Parameters *p)
 * \brief Function to calculate the Kostiakov-Lewis infiltration model.
 * \param p
 * \brief Mesh node parameters structure.
 * \return Infiltrated area in a time step.
 */
INLINE_JBDOUBLE parameters_infiltration_kostiakov_lewis(Parameters *p)
{
	return pow(pow(p->Ai, p->s->i4) + dt * p->P / p->s->D * pow(p->s->i1 *
		p->s->D, p->s->i4), p->s->i2) + p->s->i3 * p->P * dt - p->Ai;
}

/**
 * \fn INLINE_VOID parameters_infiltration_scheme(Parameters *p)
 * \brief Function to calculate the infiltration numerical scheme.
 * \param p
 * \brief Mesh node parameters structure.
 */
INLINE_VOID parameters_infiltration_scheme(Parameters *p)
{
	JBDOUBLE dA, dM;
	parameters_infiltration_base(p);
	dA = parameters_infiltration_kostiakov_lewis(p);
	dA = fmax(0., fmin(dA, p->A));
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
 * \fn INLINE_VOID parameters_solubility(Parameters *p)
 * \brief Function to calculate the solubility in a mesh node.
 * \param p
 * \brief Mesh node parameters structure.
 */
INLINE_VOID parameters_solubility(Parameters *p)
{
	JBDOUBLE m, mc, vc;
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
	if (vc > 0.) p->c = p->Ac / p->A; else p->c = 0.;
}

/**
 * \fn INLINE_VOID parameters_open(Parameters *p)
 * \brief Function to calculate the numerical parameters in a mesh node.
 * \param p
 * \brief Mesh node parameters structure.
 */
INLINE_VOID parameters_open(Parameters *p)
{
	parameters_depth(p);
	parameters_solubility(p);
	parameters_flow(p);
}

/**
 * \fn INLINE_JBDOUBLE parameters_volume_water_total(Parameters *p)
 * \brief Function to calculate the total water volume in a mesh node.
 * \param p
 * \brief Mesh node parameters structure.
 */
INLINE_JBDOUBLE parameters_volume_water_total(Parameters *p)
{
	return (p->A + p->Ai + p->Ap) * p->dx;
}

/**
 * \fn INLINE_JBDOUBLE parameters_volume_water_surface(Parameters *p)
 * \brief Function to calculate the superficial water volume in a mesh node.
 * \param p
 * \brief Mesh node parameters structure.
 */
INLINE_JBDOUBLE parameters_volume_water_surface(Parameters *p)
{
	return p->A * p->dx;
}

/**
 * \fn INLINE_JBDOUBLE parameters_volume_water_soil(Parameters *p)
 * \brief Function to calculate the infiltrated water volume in a mesh node.
 * \param p
 * \brief Mesh node parameters structure.
 */
INLINE_JBDOUBLE parameters_volume_water_soil(Parameters *p)
{
	return p->Ai * p->dx;
}

/**
 * \fn INLINE_JBDOUBLE parameters_volume_water_loss(Parameters *p)
 * \brief Function to calculate the percolated water volume in a mesh node.
 * \param p
 * \brief Mesh node parameters structure.
 */
INLINE_JBDOUBLE parameters_volume_water_loss(Parameters *p)
{
	return p->Ap * p->dx;
}

/**
 * \fn INLINE_JBDOUBLE parameters_mass_fertilizer_total(Parameters *p)
 * \brief Function to calculate the total fertilizer mass in a mesh node.
 * \param p
 * \brief Mesh node parameters structure.
 */
INLINE_JBDOUBLE parameters_mass_fertilizer_total(Parameters *p)
{
	return (p->Ac + p->Aci + p->Acp) * p->dx + p->m;
}

/**
 * \fn INLINE_JBDOUBLE parameters_mass_fertilizer_surface(Parameters *p)
 * \brief Function to calculate the superficial fertilizer mass in a mesh node.
 * \param p
 * \brief Mesh node parameters structure.
 */
INLINE_JBDOUBLE parameters_mass_fertilizer_surface(Parameters *p)
{
	return p->Ac * p->dx;
}

/**
 * \fn INLINE_JBDOUBLE parameters_mass_fertilizer_soil(Parameters *p)
 * \brief Function to calculate the infiltrated fertilizer mass in a mesh node.
 * \param p
 * \brief Mesh node parameters structure.
 */
INLINE_JBDOUBLE parameters_mass_fertilizer_soil(Parameters *p)
{
	return p->Aci * p->dx;
}

/**
 * \fn INLINE_JBDOUBLE parameters_mass_fertilizer_loss(Parameters *p)
 * \brief Function to calculate the percolated fertilizer mass in a mesh node.
 * \param p
 * \brief Mesh node parameters structure.
 */
INLINE_JBDOUBLE parameters_mass_fertilizer_loss(Parameters *p)
{
	return p->Acp * p->dx;
}

/**
 * \fn INLINE_JBDOUBLE parameters_mass_fertilizer_solid(Parameters *p)
 * \brief Function to calculate the solid fertilizer mass in a mesh node.
 * \param p
 * \brief Mesh node parameters structure.
 */
INLINE_JBDOUBLE parameters_mass_fertilizer_solid(Parameters *p)
{
	return p->m;
}

void parameters_show(Parameters *p, FILE *file);

#endif
