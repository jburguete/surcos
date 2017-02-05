/* src/config.h.  Generated from config.h.in by configure.  */
/*
SURCOS - A software tool to solve irrigation and fertigation in isolated furrows
and furrow networks.

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
 * \file config.h
 * \brief Header file to define basic configuration and compilation options.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2011-2013, Javier Burguete Tolosa.
 */
#ifndef CONFIG__H
#define CONFIG__H 1

#define INLINE static inline
///< Macro definning the inline functions degree.
#define INLINE_VOID INLINE void
///< Inline void type.
#define INLINE_JBDOUBLE INLINE JBDOUBLE
///< Inline JBDOUBLE type.

#define MODEL_DIFFUSION_CONSTANT	0
///< Macro to select the constant diffusion model.
#define MODEL_DIFFUSION_VARIABLE	1
///< Macro to select the variable diffusion model.
#define MODEL_DIFFUSION MODEL_DIFFUSION_VARIABLE
///< Macro to select the diffusion model.

#define FLUX_LIMITER_NULL		0
///< Macro to select the null flux limiter function.
#define FLUX_LIMITER_TOTAL		1
///< Macro to select the total flux limiter function.
#define FLUX_LIMITER_CENTERED	2
///< Macro to select the centered flux limiter function.
#define FLUX_LIMITER_SUPERBEE	3
///< Macro to select the superbee flux limiter function.
#define FLUX_LIMITER_MINMOD		4
///< Macro to select the minmod flux limiter function.
#define FLUX_LIMITER FLUX_LIMITER_SUPERBEE
///< Macro to select the flux limiter function.

#define JUNCTION_CONSERVATIVE	1
///< Macro to select the conservative numerical method to solve the junctions.
#define JUNCTION_SIMPLE			2
///< Macro to select the simple numerical method to solve the junctions.
#define JUNCTION JUNCTION_CONSERVATIVE
///< Macro to select the numerical method to solve the junctions.

#define MODEL_DIFFUSION_K 10.
///< Macro defining the diffusion model coefficient.

#define MODEL_FRICTION_B 0.25
///< Macro defining the friction model coefficient.

#define PARALLELIZING 0
///< Macro to decide if the program have to be parallelized.

#define INITIAL_CONDITIONS_H_MIN 0.
///< Macro defining the minimum depth of the initial conditions.
#define INITIAL_CONDITIONS_H_MAX 100.
///< Macro defining the maximum depth of the initial conditions.
#define INITIAL_CONDITIONS_Q_MIN 0.
///< Macro defining the minimum discharge of the initial conditions.
#define INITIAL_CONDITIONS_Q_MAX 100.
///< Macro defining the maximum discharge of the initial conditions.
#define INITIAL_CONDITIONS_C_MIN 0.
///< Macro defining the minimum fertilizer concentration of the initial 
///< conditions.
#define INITIAL_CONDITIONS_C_MAX 1000.
///< Macro defining the maximum fertilizer concentration of the initial 
///< conditions.

#define FURROW_B_MIN 0.
///< Macro defining the minimum base width of the furrows.
#define FURROW_B_MAX 1e6
///< Macro defining the maximum base width of the furrows.
#define FURROW_Z_MIN 0.
///< Macro defining the minimum lateral slope of the furrows.
#define FURROW_Z_MAX 1e6
///< Macro defining the maximum lateral slope of the furrows.
#define FURROW_H_MIN 0.
///< Macro defining the minimum depth of the furrows.
#define FURROW_H_MAX 100.
///< Macro defining the maximum depth of the furrows.
#define FURROW_W_MIN 0.
///< Macro defining the minimum lateral distance between furrows.
#define FURROW_W_MAX 1e6
///< Macro defining the maximum lateral distance between furrows.
#define FURROW_R_MIN 0.
///< Macro defining the minimum soil retention capacity of the furrows.
#define FURROW_R_MAX 10.
///< Macro defining the maximum soil retention capacity of the furrows.
#define FURROW_N_MIN 0.
///< Macro defining the minimum roughness coeficcient of the furrows.
#define FURROW_N_MAX 10.
///< Macro defining the maximum roughness coefficient of the furrows.
#define FURROW_I1_MIN 0.
///< Macro defining the minimum 1st infiltration coefficient of the furrows.
#define FURROW_I1_MAX 1e6
///< Macro defining the maximum 1st infiltration coefficient of the furrows.
#define FURROW_I2_MIN 0.
///< Macro defining the minimum 2nd infiltration coefficient of the furrows.
#define FURROW_I2_MAX 1.
///< Macro defining the maximum 2nd infiltration coefficient of the furrows.
#define FURROW_I3_MIN 0.
///< Macro defining the minimum 3rd infiltration coefficient of the furrows.
#define FURROW_I3_MAX 1e6
///< Macro defining the maximum 3rd infiltration coefficient of the furrows.
#define FURROW_D_MIN 0.
///< Macro defining the minimum diffusion coefficient of the furrows.
#define FURROW_D_MAX 1e6
///< Macro defining the maximum diffusion coefficient of the furrows.

#define INPUT_X_MIN -1e6
///< Macro defining the minimum x-coordinate of an inlet.
#define INPUT_X_MAX 1e6
///< Macro defining the maximum x-coordinate of an inlet.
#define INPUT_Y_MIN -1e6
///< Macro defining the minimum y-coordinate of an inlet.
#define INPUT_Y_MAX 1e6
///< Macro defining the maximum y-coordinate of an inlet.
#define INPUT_T0_MIN 0.
///< Macro defining the minimum initial time of an inlet.
#define INPUT_T0_MAX 1e6
///< Macro defining the maximum initial time of an inlet.
#define INPUT_TF_MIN 0.
///< Macro defining the minimum final time of an inlet.
#define INPUT_TF_MAX 1e6
///< Macro defining the maximum final time of an inlet.
#define INPUT_Q_MIN 0.
///< Macro defining the minimum discharge of an inlet.
#define INPUT_Q_MAX 1000.
///< Macro defining the maximum discharge of an inlet.

#define FIELD_X_MIN -1e6
///< Macro defining the minimum x-coordinate of a field.
#define FIELD_X_MAX 1e6
///< Macro defining the maximum x-coordinate of a field.
#define FIELD_Y_MIN -1e6
///< Macro defining the minimum x-coordinate of a field.
#define FIELD_Y_MAX 1e6
///< Macro defining the maximum y-coordinate of a field.
#define FIELD_Z_MIN -1000.
///< Macro defining the minimum z-coordinate of a field.
#define FIELD_Z_MAX 10000.
///< Macro defining the maximum z-coordinate of a field.
#define FIELD_SOLUBILITY_MIN 0.
///< Macro defining the minimum solubility of a fertilizer.
#define FIELD_SOLUBILITY_MAX 1000.
///< Macro defining the maximum solubility of a fertilizer.

#endif
