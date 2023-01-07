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
 * \file parameters.c
 * \brief Source file to define the parameters of a mesh node.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2011-2023, Javier Burguete Tolosa.
 */
#include "config.h"
#include "jb/src/jb_win.h"
#include "furrow.h"
#include "parameters.h"

#define PARAMETERS_SHOW 0
///< Macro to compile the parameters_show function.

/**
 * Debug function to show the parameters of a mesh node.
 */
#if PARAMETERS_SHOW
void
parameters_show (Parameters * p,        ///< Mesh node parameters structure.
                 FILE * file)   ///< Output file.
{
  fprintf (file, "parameters_show: start\n");
  fprintf (file, "h=" FWF " A=" FWF " Q=" FWF " Ai=" FWF "\n",
           p->h, p->A, p->Q, p->Ai);
  fprintf (file, "c=" FWF " Ac=" FWF " Aci=" FWF " Ap=" FWF " dx=" FWF "\n",
           p->c, p->Ac, p->Aci, p->Ap, p->dx);
  fprintf (file, "B=" FWF " P=" FWF " u=" FWF " uc=" FWF "\n",
           p->B, p->P, p->u, p->uc);
  fprintf (file, "F=" FWF " H=" FWF " Kx=" FWF " Kf=" FWF "\n",
           p->F, p->H, p->Kx, p->Kf);
  fprintf (file, "la=" FWF " lb=" FWF " zs=" FWF " zf=" FWF "\n",
           p->la, p->lb, p->zs, p->zf);
  fprintf (file, "x=" FWF " Amin=" FWF " zmin=" FWF " sA=" FWF "\n",
           p->x, p->Amin, p->zmin, p->sA);
  fprintf (file, "dQp=" FWF " dQm=" FWF " dFp=" FWF " dFm=" FWF "\n",
           p->dQp, p->dQm, p->dFp, p->dFm);
  fprintf (file, "dTp=" FWF " dTm=" FWF "\n", p->dTp, p->dTm);
  fprintf (file, "parameters_show: end\n");
}
#endif
