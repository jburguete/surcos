/*
SURCOS - A program to simulate furrow fertigation.

Copyright 2011-2013, Javier Burguete Tolosa.

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
 * \copyright Copyright 2011-2013, Javier Burguete Tolosa.
 */
#ifndef CONFIG_TIMES__H
#define CONFIG_TIMES__H 1

/**
 * \struct _ConfigTimes
 * \brief Structure to configure the advanced options.
 */
struct _ConfigTimes
{
/**
 * \var label
 * \brief Array of GtkLabels showing the advanced options.
 * \var spin
 * \brief Array of GtkSpinButtons to configure the advanced options.
 * \var table
 * \brief GtkGrid containing all the advanced options widgets.
 */
	GtkLabel *label[5];
	GtkSpinButton *spin[5];
	GtkGrid *table;
};

/**
 * \typedef ConfigTimes
 */
typedef struct _ConfigTimes ConfigTimes;

void config_times_write(ConfigTimes *w);
void config_times_new(ConfigTimes *w);

#endif
