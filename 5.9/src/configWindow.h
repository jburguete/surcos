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
 * \file configWindow.h
 * \brief Header file to define the configuration of the problem.
 * \authors Javier Burguete Tolosa, Asier Lacasta Soto.
 * \copyright Copyright 2011-2023, Javier Burguete Tolosa.
 */
#ifndef CONFIG_WINDOW__H
#define CONFIG_WINDOW__H 1

/**
 * \struct ConfigFertilizer
 * \brief Structure to configure the fertilizer solubility.
 */
typedef struct
{
  GtkGrid *box;
  ///< GtkGrid containing the fertilizer configuration widgets.
  GtkLabel *label;
  ///< GtkLabel to show the fertilizer solubility label.
  GtkSpinButton *spin;
  ///< GtkSpinButton to configure the fertilizer solubility.
} ConfigFertilizer;

/**
 * \struct WindowConfig
 * \brief Structure to configurate the fertigation problem.
 */
typedef struct
{
  ConfigIrrigation irrigation[1];
  ///< Fertigation inputs configuration structure.
  ConfigGeometry geometry[1];
  ///< Field geometry configuration structure.
  TableConfigFurrows furrows[1];
  ///< Field furrows configuration structure.
  ConfigTimes times[1];
  ///< Advanced options configuration structure.
  ConfigFertilizer fertilizer[1];
  ///< Fertilizer configuration structure.
  ConfigProbes probes[1];
  ///< Probes configuration structure.
  GtkNotebook *notebook;
  ///< GtkNotebook containing the configuration widgets.
  GtkImage *logo;
  ///< GtkImage to show the logo.
  GtkHeaderBar *bar;
  ///< GtkHeaderBar to show the dialog title.
  GtkDialog *dialog;
  ///< GtkDialog to do the configuration.
} WindowConfig;

void window_config_new ();

#endif
