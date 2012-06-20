/*
SURCOS - A program to simulate furrow fertigation.

Copyright 2011-2012, Javier Burguete Tolosa.

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
 * \file scheme.h
 * \brief Header file to define the numerical scheme.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2011-2012, Javier Burguete Tolosa.
 */
#ifndef SCHEME__H
#define SCHEME__H 1

#include "parameters.h"

void scheme_save(Parameters *p, unsigned int n, char *name);
unsigned int scheme_search_node
	(Parameters *p,  unsigned int n,  JBDOUBLE x,  JBDOUBLE y);
void scheme_mesh
	(Parameters *p, unsigned int n, Furrow *s, InitialConditions *ci);
void scheme_diffusion(Parameters *p, unsigned int n);
void scheme_diffusion_soil(Parameters *p, unsigned int n);
void scheme_infiltration(Parameters *p, unsigned int n);
void scheme_step(Parameters *p, unsigned int n);
void scheme_ix(Parameters *p, unsigned int n);
void scheme_save_advance(Parameters *p, unsigned int n, FILE *file);
JBDOUBLE uniformity_water(Parameters *p, unsigned int n);
JBDOUBLE uniformity_fertilizer(Parameters *p, unsigned int n);

#endif
