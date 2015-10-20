/*
SURCOS - A software tool to solve irrigation and fertigation in isolated furrows
and furrow networks.

Copyright 2011-2014, Javier Burguete Tolosa.

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
 * \copyright Copyright 2011-2014, Javier Burguete Tolosa.
 */
#ifndef CONFIG_WINDOW__H
#define CONFIG_WINDOW__H 1

/**
 * \struct _ConfigFertilizer
 * \brief Structure to configure the fertilizer solubility.
 */
struct _ConfigFertilizer
{
    /**
     * \var box
     * \brief GtkGrid containing the fertilizer configuration widgets.
     * \var label
     * \brief GtkLabel to show the fertilizer solubility label.
     * \var spin
     * \brief GtkSpinButton to configure the fertilizer solubility.
     */
    GtkGrid *box;
    GtkLabel *label;
    GtkSpinButton *spin;
};

/**
 * \typedef ConfigFertilizer
 */
typedef struct _ConfigFertilizer ConfigFertilizer;

/**
 * \struct _WindowConfig
 * \brief Structure to configurate the fertigation problem.
 */
struct _WindowConfig
{
    /**
     * \var irrigation
     * \brief Fertigation inputs configuration structure.
     * \var geometry
     * \brief Field geometry configuration structure.
     * \var furrows
     * \brief Field furrows configuration structure.
     * \var times
     * \brief Advanced options configuration structure.
     * \var fertilizer
     * \brief Fertilizer configuration structure.
     * \var probes
     * \brief Probes configuration structure.
     * \var notebook
     * \brief GtkNotebook containing the configuration widgets.
     * \var dialog
     * \brief GtkDialog to do the configuration.
     */
    ConfigIrrigation irrigation[1];
    ConfigGeometry geometry[1];
    TableConfigFurrows furrows[1];
    ConfigTimes times[1];
    ConfigFertilizer fertilizer[1];
    ConfigProbes probes[1];
    GtkNotebook *notebook;
    GtkDialog *dialog;
};

/**
 * \typedef WindowConfig
 */
typedef struct _WindowConfig WindowConfig;

void window_config_new();

#endif
