/*
SURCOS - A software tool to solve irrigation and fertigation in isolated furrows
and furrow networks.

Copyright 2011-2025, Javier Burguete Tolosa.

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
 * \file configTimes.h
 * \brief Header file to define the configuration of the advanced options.
 * \authors Javier Burguete Tolosa, Asier Lacasta Soto.
 * \copyright Copyright 2011-2025, Javier Burguete Tolosa.
 */
#ifndef CONFIG_TIMES__H
#define CONFIG_TIMES__H 1

/**
 * \struct ConfigTimes
 * \brief Structure to configure the advanced options.
 */
typedef struct
{
  GtkLabel *label[5];
  ///< Array of GtkLabels showing the advanced options.
  GtkSpinButton *spin[5];
  ///< Array of GtkSpinButtons to configure the advanced options.
  GtkGrid *grid;
  ///< GtkGrid containing all the advanced options widgets.
  double ts;                    ///< Maximum simulation time.
  double cfl;                   ///< CFL number.
  double tm;                    ///< Measured time.
  unsigned int c1;              ///< Number of cells in distribution furrow.
  unsigned int c2;              ///< Number of cells in irrigation furrow.
} ConfigTimes;

void config_times_read (ConfigTimes * w);
void config_times_write (ConfigTimes * w);
void config_times_new (ConfigTimes * w);

#endif
