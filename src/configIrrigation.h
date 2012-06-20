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
 * \file configIrrigation.h
 * \brief Header file to define the configuration of the irrigation.
 * \authors Javier Burguete Tolosa, Asier Lacasta Soto.
 * \copyright Copyright 2011-2012, Javier Burguete Tolosa.
 */
#ifndef CONFIG_IRRIGATION__H
#define CONFIG_IRRIGATION__H 1

#define _GNU_SOURCE
#include <gtk/gtk.h>

/**
 * \struct _ConfigIrrigation
 * \brief Struct to configure the irrigation.
 */
struct _ConfigIrrigation
{
/**
 * \var label
 * \brief Array of GtkLabels to show the water and fertilizer inputs numbers.
 * \var label_image
 * \brief label of the image displaying the inputs variables.
 * \var spin
 * \brief Array of GtkSpinButtons to define the water and fertilizer inputs
 * 	numbers.
 * \var button
 * \brief Matrix of GtkSpinButtons to define the water and fertilizer inputs
 * 	parameters.
 * \var table
 * \brief Array of GtkTables containing the water and fertilizer inputs.
 * \var frame
 * \brief Array of GtkFrames containing the water and fertilizer inputs tables.
 * \var image
 * \brief GtkImage displaying the inputs variables.
 * \var box_image
 * \brief GtkVBox containing the inputs image and label.
 * \var box_input
 * \brief GtkVBox containing the water and fertilizer inputs labels and tables.
 * \var box_number
 * \brief GtkHBox containing the water and fertilizer inputs numbers.
 * \var box
 * \brief GtkHBox containing the image box and the input box.
 * \var list
 * \brief Array of water and fertilizer inputs lists.
 * \var n
 * \brief Array of water and fertilizer inputs numbers.
 */
	GtkLabel *label[2], *label_image;
	GtkSpinButton *spin[2];
	GtkButton *button[6][2];
	GtkTable *table[2];
	GtkFrame *frame[2];
	GtkImage *image;
	GtkVBox *box_image, *box_input;
	GtkHBox *box_number, *box;
	GList *list[2];
	int n[2];
};

/**
 * \typedef ConfigIrrigation
 */
typedef struct _ConfigIrrigation ConfigIrrigation;

void config_irrigation_write(ConfigIrrigation *w);
void config_irrigation_new(ConfigIrrigation *w);

#endif
