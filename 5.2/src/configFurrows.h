/*
SURCOS - A program to simulate furrow fertigation.

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
 * \file configFurrows.h
 * \brief Header file to define the configuration of the furrows data.
 * \authors Javier Burguete Tolosa, Asier Lacasta Soto.
 * \copyright Copyright 2011-2013, Javier Burguete Tolosa.
 */
#ifndef CONFIG_FURROW__H
#define CONFIG_FURROW__H 1

/**
 * \struct _ConfigFurrow
 * \brief Structure to define the parameters of a furrow.
 */
struct _ConfigFurrow
{
/**
 * \var label
 * \brief GtkLabel to show the furrow label.
 * \var spin
 * \brief Array of GtkSpinButtons to set the furrow parameters.
 */
	GtkLabel *label;
	GtkSpinButton *spin[10];
};

/**
 * \typedef ConfigFurrow
 */
typedef struct _ConfigFurrow ConfigFurrow;

/**
 * \struct _TableConfigFurrows
 * \brief Structure to configurate the field furrows.
 */
struct _TableConfigFurrows
{
/**
 * \var furrow
 * \brief Array of furrow configurations.
 * \var label_furrow
 * \brief GtkLabel to show the furrows number.
 * \var label_models
 * \brief GtkLabel to show the main of the model coefficients symbols.
 * \var spin
 * \brief GtkSpinButton to configure the furrows number.
 * \var button_recirculation
 * \brief GtkCheckButton to configure the recirculation furrow.
 * \var box_image
 * \brief GtkHBox containing the image and the label.
 * \var box_furrows
 * \brief GtkHBox containing the furrows number configuration. 
 * \var button_furrow
 * \brief GtkButton label to show the furrow type.
 * \var button_geometry
 * \brief GtkButton label to show the furrow geometry.
 * \var button_soil
 * \brief GtkButton label to show the furrow soil properties.
 * \var button_friction
 * \brief GtkButton label to show the furrow friction.
 * \var button_infiltration
 * \brief GtkButton label to show the furrow infiltration porperties.
 * \var button_coefficient
 * \brief GtkButton label to show the furrow coefficients.
 * \var image
 * \brief GtkImage displaying an illustration of the furrow characteristics.
 * \var table
 * \brief GtkGrid containing all configuration widgets.
 */
	ConfigFurrow furrow[3];
	GtkLabel *label_furrow, *label_models;
	GtkSpinButton *spin;
	GtkCheckButton *button_recirculation;
	GtkHBox *box_image, *box_furrows;
	GtkButton *button_furrow, *button_geometry, *button_soil, *button_friction,
		*button_infiltration, *button_coefficient[10];
	GtkImage *image;
	GtkGrid *table;
};

/**
 * \typedef TableConfigFurrows
 */
typedef struct _TableConfigFurrows TableConfigFurrows;

void config_furrow_read(ConfigFurrow *w, FILE *file);
void config_furrow_write(ConfigFurrow *w, FILE *file);

void table_config_furrows_new(TableConfigFurrows *w);

#endif
