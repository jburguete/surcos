/*
SURCOS - A software tool to solve irrigation and fertigation in isolated furrows
and furrow networks.

Copyright 2011-2018, Javier Burguete Tolosa.

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
 * \file kernel.h
 * \brief Header file to define the simulation.
 * \author Javier Burguete Tolosa
 * \copyright Copyright 2011-2018, Javier Burguete Tolosa.
 */
#ifndef KERNEL__H
#define KERNEL__H 1

#include "field.h"

#if JBW != JBW_NO

#include "mainWindow.h"

/**
 * \struct WindowRun
 * \brief Structure to show the progression of a simulation.
 */
typedef struct
{
  GtkProgressBar *progress;     ///< GtkProgresBar widget.
  GtkDialog *dialog;            ///< GtkDialog dialog.
} WindowRun;

#endif

int kernel_open (char *dir, int gui);
int kernel (char *dir, int gui, int msg);

#endif
