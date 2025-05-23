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
 * \file configGeometry.h
 * \brief Header file to define the configuration of the geometry data.
 * \authors Javier Burguete Tolosa, Asier Lacasta Soto.
 * \copyright Copyright 2011-2025, Javier Burguete Tolosa.
 */
#ifndef CONFIG_GEOMETRY__H
#define CONFIG_GEOMETRY__H 1

/**
 * \struct ConfigPoint
 * \brief Structure to configure a point.
 */
typedef struct
{
  double r[3];                  ///< Array of point coordinates.
  GtkButton *button;
  ///< Label GtkButton.
  GtkSpinButton *spin[3];
  ///< GtkSpinButton array to define the point coordinates.
} ConfigPoint;

/**
 * \struct ConfigGeometry
 * \brief Structure to define a field geometry configuration.
 */
typedef struct
{
  ConfigPoint point[4];
  ///< Array of field corner point configuration structures.
  GtkButton *button[4];
  ///< Array of GtkButton labels of field corner points configuration.
  GtkGrid *grid;
  ///< GtkGrid containing the arrays of points and buttons.
  GtkPicture *picture;
  ///< GtkPicture displaying a field geometry illustration.
  GtkGrid *box;
  ///< GtkGrid containing the grid and the picture.
} ConfigGeometry;

void config_point_show (ConfigPoint * w);
void config_point_hide (ConfigPoint * w);

void config_geometry_read (ConfigGeometry * w);
void config_geometry_write (ConfigGeometry * w);
void config_geometry_new (ConfigGeometry * w);

#endif
