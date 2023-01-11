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
 * \file configFurrows.h
 * \brief Header file to define the configuration of the furrows data.
 * \authors Javier Burguete Tolosa, Asier Lacasta Soto.
 * \copyright Copyright 2011-2023, Javier Burguete Tolosa.
 */
#ifndef CONFIG_FURROW__H
#define CONFIG_FURROW__H 1

/**
 * \struct ConfigFurrow
 * \brief Structure to define the parameters of a furrow.
 */
typedef struct
{
  GtkButton *button;
  ///< GtkButton to show the furrow label.
  GtkSpinButton *spin[10];
  ///< Array of GtkSpinButtons to set the furrow parameters.
} ConfigFurrow;

/**
 * \struct TableConfigFurrows
 * \brief Structure to configurate the field furrows.
 */
typedef struct
{
  ConfigFurrow furrow[3];
  ///< Array of furrow configurations.
  GtkLabel *label_furrow;
  ///< GtkLabel to show the furrows number.
  GtkLabel *label_models;
  ///< GtkLabel to show the main of the model coefficients symbols.
  GtkSpinButton *spin;
  ///< GtkSpinButton to configure the furrows number.
  GtkCheckButton *button_recirculation;
  ///< GtkCheckButton to configure the recirculation furrow.
  GtkGrid *box_picture;
  ///< GtkGrid containing the picture and the label.
  GtkGrid *box_furrows;
  ///< GtkGrid containing the furrows number configuration.
  GtkButton *button_furrow;
  ///< GtkButton label to show the furrow type.
  GtkButton *button_geometry;
  ///< GtkButton label to show the furrow geometry.
  GtkButton *button_soil;
  ///< GtkButton label to show the furrow soil properties.
  GtkButton *button_friction;
  ///< GtkButton label to show the furrow friction.
  GtkButton *button_infiltration;
  ///< GtkButton label to show the furrow infiltration porperties.
  GtkButton *button_coefficient[10];
  ///< GtkButton label to show the furrow coefficients.
  GtkPicture *picture;
  ///< GtkPicture displaying an illustration of the furrow characteristics.
  GtkGrid *grid;
  ///< GtkGrid containing all configuration widgets.
} TableConfigFurrows;

void config_furrow_read (ConfigFurrow * w, FILE * file);
void config_furrow_write (ConfigFurrow * w, FILE * file);

void table_config_furrows_new (TableConfigFurrows * w);

#endif
