/*
SURCOS - A software tool to solve irrigation and fertigation in isolated furrows
and furrow networks.

Copyright 2011-2017, Javier Burguete Tolosa.

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
 * \file configProbes.h
 * \brief Header file to define the configuration of the probes data.
 * \authors Javier Burguete Tolosa, Asier Lacasta Soto.
 * \copyright Copyright 2011-2017, Javier Burguete Tolosa.
 */
#ifndef CONFIG_PROBES__H
#define CONFIG_PROBES__H 1

/**
 * \struct ConfigProbes
 * \brief Structure to configure the probes.
 */
struct _ConfigProbes
{
  GList *list;
  ///< Probes list.
  GtkLabel *label;
  ///< GtkLabel showing the probes number configuration.
  GtkSpinButton *spin;
  ///< GtkSpinButton to configure the probes number.
  GtkButton *button[3];
  ///< Array of GtkButton labels showing the probes characteristics.
  GtkGrid *table;
  ///< GtkGrid containing the probes configurations.
  GtkGrid *hbox;
  ///< GtkGrid containing the probes number configuration.
  GtkGrid *vbox;
  ///< GtkGrid containing all probes configuration widgets.
};
typedef struct _ConfigProbes ConfigProbes;

void config_probes_write (ConfigProbes * w);
void config_probes_new (ConfigProbes * w);

#endif
