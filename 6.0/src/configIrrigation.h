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
 * \file configIrrigation.h
 * \brief Header file to define the configuration of the irrigation.
 * \authors Javier Burguete Tolosa, Asier Lacasta Soto.
 * \copyright Copyright 2011-2023, Javier Burguete Tolosa.
 */
#ifndef CONFIG_IRRIGATION__H
#define CONFIG_IRRIGATION__H 1

/**
 * \struct ConfigIrrigation
 * \brief Struct to configure the irrigation.
 */
typedef struct
{
  Input *input[2];
  ///< Array of Input structures to define the water and fertilizer inputs
  ///< parameters.
  GtkButton *button[6][2];
  ///< Matrix of GtkSpinButtons to define the water and fertilizer inputs
  ///< parameters.
  GtkLabel *label[2];
  ///< Array of GtkLabels to show the water and fertilizer inputs numbers.
  GtkSpinButton *spin[2];
  ///< Array of GtkSpinButtons to define the water and fertilizer inputs
  ///< numbers.
  GtkGrid *grid[2];
  ///< Array of GtkGrids containing the water and fertilizer inputs.
  GtkFrame *frame[2];
  ///< Array of GtkFrames containing the water and fertilizer inputs grids.
  GList *list[2];
  ///< Array of water and fertilizer inputs lists.
  unsigned int n[2];
  ///< Array of water and fertilizer inputs numbers.
  GtkLabel *label_picture;
  ///< label of the picture displaying the inputs variables.
  GtkPicture *picture;
  ///< GtkPicture displaying the inputs variables.
  GtkGrid *box_picture;
  ///< GtkGrid containing the inputs picture and label.
  GtkGrid *box_input;
  ///< GtkGrid containing the water and fertilizer inputs labels and grids.
  GtkGrid *box_number;
  ///< GtkGrid containing the water and fertilizer inputs numbers.
  GtkGrid *box;
  ///< GtkGrid containing the picture box and the input box.
} ConfigIrrigation;

void config_irrigation_read (ConfigIrrigation * w);
void config_irrigation_write (ConfigIrrigation * w);
void config_irrigation_new (ConfigIrrigation * w);

#endif
