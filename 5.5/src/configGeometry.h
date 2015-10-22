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
 * \file configGeometry.h
 * \brief Header file to define the configuration of the geometry data.
 * \authors Javier Burguete Tolosa, Asier Lacasta Soto.
 * \copyright Copyright 2011-2014, Javier Burguete Tolosa.
 */
#ifndef CONFIG_GEOMETRY__H
#define CONFIG_GEOMETRY__H 1

#define _GNU_SOURCE
#include <gtk/gtk.h>

/**
 * \struct _ConfigPoint
 * \brief Structure to configure a point.
 */
struct _ConfigPoint
{
    /**
     * \var button
     * \brief label GtkButton.
     * \var spin
     * \brief GtkSpinButton array to define the point coordinates.
     */
  GtkButton *button;
  GtkSpinButton *spin[3];
};

/**
 * \typedef ConfigPoint
 */
typedef struct _ConfigPoint ConfigPoint;

/**
 * \struct _ConfigGeometry
 * \brief Structure to define a field geometry configuration.
 */
struct _ConfigGeometry
{
    /**
     * \var point
     * \brief Array of field corner point configuration structures.
     * \var button
     * \brief Array of GtkButton labels of field corner points configuration.
     * \var table
     * \brief GtkGrid containing the arrays of points and buttons.
     * \var image
     * \brief GtkImage displaying a field geometry illustration.
     * \var box
     * \brief GtkGrid containing the table and the image.
     */
  ConfigPoint point[4];
  GtkButton *button[4];
  GtkGrid *table;
  GtkImage *image;
  GtkGrid *box;
};

/**
 * \typedef ConfigGeometry
 */
typedef struct _ConfigGeometry ConfigGeometry;

void config_point_read (ConfigPoint * w, FILE * file);
void config_point_write (ConfigPoint * w, FILE * file);

void config_geometry_new (ConfigGeometry * w);

#endif
