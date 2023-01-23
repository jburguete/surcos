#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <json-glib/json-glib.h>

typedef struct
{
  double h;
  double q;
  double c;
} InitialConditions;

typedef struct
{
  double b;
  double z;
  double h;
  double D;
  double H;
  double epsilon;
  double n;
  double d;
  double i1;
  double i2;
  double i3;
} Furrow;

typedef struct
{
  double x;
  double y;
  double t0;
  double tf;
  double q;
} Input;

typedef struct
{
  double x;
  double y;
} Probe;

Furrow sb[1];
Furrow sc[1];
Furrow si[1];
InitialConditions cib[1];
InitialConditions cic[1];
InitialConditions cii[1];
Input *input;
Probe *probe;
double x[8];
double y[8];
double z[8];
double tm;
double tf;
double cfl;
double cpu;
unsigned int open;
unsigned int nfurrows;
unsigned int nb;
unsigned int nbi;
unsigned int ni;
unsigned int n;
unsigned int nwinputs;
unsigned int nsinputs;
unsigned int nprobes;
double solubility;
unsigned int type_fertilizer;
unsigned int type_infiltration;
unsigned int type_diffusion_soil;
unsigned int type_beta;

char *message = NULL;

void
uint_write (JsonObject * object, const char *label, unsigned int x)
{
  char buffer[64];
  snprintf (buffer, 64, "%u", x);
  json_object_set_string_member (object, label, buffer);
}

int
dbl_is_zero (double x)
{
  return fabs (x) > DBL_EPSILON ? 0 : 1;
}

void
dbl_write (JsonObject * object, const char *label, double x)
{
  char buffer[64];
  snprintf (buffer, 64, "%.14lg", x);
  json_object_set_string_member (object, label, buffer);
}

int
initial_conditions_read (InitialConditions * ic, FILE * file)
{
  if (fscanf (file, "%lf%lf%lf", &ic->h, &ic->q, &ic->c) != 3)
    {
      message = g_strdup ("Bad initial conditions");
      return 0;
    }
  return 1;
}

void
initial_conditions_write (InitialConditions * ic, JsonObject * object)
{
  JsonNode *node;
  JsonObject *child;
  int h, q, c;
  h = dbl_is_zero (ic->h);
  q = dbl_is_zero (ic->q);
  c = dbl_is_zero (ic->c);
  if (h && q && c)
    return;
  child = json_object_new ();
  if (!h)
    dbl_write (child, "h", ic->h);
  if (!q)
    dbl_write (child, "h", ic->q);
  if (!c)
    dbl_write (child, "h", ic->c);
  node = json_node_new (JSON_NODE_OBJECT);
  json_node_set_object (node, child);
  json_object_set_member (object, "initial-conditions", node);
}

int
furrow_read (Furrow * f, InitialConditions * ic, FILE * file)
{
  if (fscanf (file, "%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf",
              &f->b, &f->z, &f->h, &f->D, &f->H, &f->epsilon, &f->n, &f->i1,
              &f->i2, &f->i3, &f->d) != 11)
    {
      message = g_strdup ("Bad furrow data");
      goto exit;
    }
  if (!initial_conditions_read (ic, file))
    goto exit;
  return 1;
exit:
  return 0;
}

void
furrow_write (Furrow * f, InitialConditions * ic, const char *label,
              JsonObject * object)
{
  JsonNode *node;
  JsonObject *child;
  child = json_object_new ();
  if (!dbl_is_zero (f->b))
    dbl_write (child, "B", f->b);
  if (!dbl_is_zero (f->z))
    dbl_write (child, "Z", f->z);
  if (!dbl_is_zero (f->h))
    dbl_write (child, "H", f->h);
  if (!dbl_is_zero (f->D))
    dbl_write (child, "W", f->D);
  if (!dbl_is_zero (f->H))
    dbl_write (child, "R", f->H);
  if (!dbl_is_zero (f->epsilon))
    dbl_write (child, "epsilon", f->epsilon);
  if (!dbl_is_zero (f->n))
    dbl_write (child, "n", f->n);
  if (!dbl_is_zero (f->i1))
    dbl_write (child, "i1", f->i1);
  if (!dbl_is_zero (f->i2))
    dbl_write (child, "i2", f->i2);
  if (!dbl_is_zero (f->i3))
    dbl_write (child, "i3", f->i3);
  if (!dbl_is_zero (f->d))
    dbl_write (child, "D", f->d);
  initial_conditions_write (ic, child);
  node = json_node_new (JSON_NODE_OBJECT);
  json_node_set_object (node, child);
  json_object_set_member (object, label, node);
}

int
field_read_data (char *directory)
{
  FILE *file;
  char *buffer, *buffer2;
  unsigned int j = 0;
  buffer = g_build_filename (directory, "field.in", NULL);
  file = g_fopen (buffer, "r");
  if (!file)
    {
      message = g_strconcat ("Unable to open the geometry data file\n", buffer,
                             NULL);
      g_free (buffer);
      return 0;
    }
  if (fscanf (file,
              "%u%u%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf",
              &open, &nfurrows, &solubility,
              &x[0], &y[0], &z[0],
              &x[1], &y[1], &z[1],
              &x[2], &y[2], &z[2], &x[3], &y[3], &z[3]) != 15)
    {
      message = g_strdup ("Bad data");
      goto exit0;
    }
  if (!furrow_read (sb, cib, file))
    {
      buffer2 = message;
      message = g_strconcat ("Distribution furrow\n", message, NULL);
      g_free (buffer2);
      goto exit0;
    }
  if (nfurrows > 0)
    {
      if (!furrow_read (si, cii, file))
        {
          buffer2 = message;
          message = g_strconcat ("Irrigation furrows\n", message, NULL);
          g_free (buffer2);
          goto exit0;
        }
      if (open)
        {
          if (!furrow_read (sc, cic, file))
            {
              buffer2 = message;
              message = g_strconcat ("Recirculation furrow\n", message, NULL);
              g_free (buffer2);
              goto exit0;
            }
        }
    }
  j = 1;
exit0:
  fclose (file);
  if (!j)
    {
      buffer2 = message;
      message = g_strconcat ("Bad data set\n", message, "\n", buffer, NULL);
      g_free (buffer2);
    }
  g_free (buffer);
  return j;
}

void
field_write_data (JsonObject * object)
{
  if (open)
    uint_write (object, "open", open);
  if (nfurrows)
    uint_write (object, "furrows-number", nfurrows);
  if (!dbl_is_zero (solubility))
    dbl_write (object, "solubility", solubility);
  if (!dbl_is_zero (x[0]))
    dbl_write (object, "x0", x[0]);
  if (!dbl_is_zero (y[0]))
    dbl_write (object, "y0", y[0]);
  if (!dbl_is_zero (z[0]))
    dbl_write (object, "z0", z[0]);
  if (!dbl_is_zero (x[1]))
    dbl_write (object, "x1", x[1]);
  if (!dbl_is_zero (y[1]))
    dbl_write (object, "y1", y[1]);
  if (!dbl_is_zero (z[1]))
    dbl_write (object, "z1", z[1]);
  if (nfurrows)
    {
      if (!dbl_is_zero (x[2]))
        dbl_write (object, "x2", x[2]);
      if (!dbl_is_zero (y[2]))
        dbl_write (object, "y2", y[2]);
      if (!dbl_is_zero (z[2]))
        dbl_write (object, "z2", z[2]);
      if (!dbl_is_zero (x[3]))
        dbl_write (object, "x3", x[3]);
      if (!dbl_is_zero (y[3]))
        dbl_write (object, "y3", y[3]);
      if (!dbl_is_zero (z[3]))
        dbl_write (object, "z3", z[3]);
      furrow_write (sb, cib, "distribution", object);
      furrow_write (si, cii, "irrigation", object);
      if (open)
        furrow_write (sc, cic, "recirculation", object);
    }
  else
    furrow_write (sb, cib, "irrigation", object);
}

int
field_read_time (char *directory)
{
  FILE *file;
  char *buffer;
  unsigned int i;
  buffer = g_build_filename (directory, "times.in", NULL);
  file = g_fopen (buffer, "r");
  if (!file)
    {
      message = g_strconcat ("Unable to open the times data file\n", buffer,
                             NULL);
      g_free (buffer);
      return 0;
    }
  if (fscanf (file, "%lf%lf%lf", &tf, &cfl, &tm) != 3)
    {
      message = g_strconcat ("Bad times data\n", buffer, NULL);
      i = 0;
    }
  else
    i = 1;
  fclose (file);
  g_free (buffer);
  return i;
}

void
field_write_times (JsonObject * object)
{
  dbl_write (object, "simulation-time", tf);
  dbl_write (object, "cfl", cfl);
  dbl_write (object, "measured-time", tm);
}

int
field_read_mesh (char *directory)
{
  FILE *file;
  char *buffer;
  unsigned int i = 0;
  buffer = g_build_filename (directory, "mesh.in", NULL);
  file = g_fopen (buffer, "r");
  if (!file)
    {
      message = g_strconcat ("Unable to open the mesh data file\n",
                             buffer, NULL);
      g_free (buffer);
      return 0;
    }
  if (fscanf (file, "%u", &nbi) != 1)
    goto exit0;
  if (nfurrows > 0 && fscanf (file, "%u", &ni) != 1)
    goto exit0;
  i = 1;
exit0:
  fclose (file);
  if (!i)
    message = g_strconcat ("Bad mesh data\n", buffer, NULL);
  g_free (buffer);
  return i;
}

void
field_write_mesh (JsonObject * object)
{
  if (nfurrows)
    {
      uint_write (object, "distribution-cells", nbi);
      uint_write (object, "irrigation-cells", ni);
    }
  else
    uint_write (object, "irrigation-cells", nbi);
}

int
field_read_model (char *directory)
{
  FILE *file;
  char *buffer;
  unsigned int i;
  buffer = g_build_filename (directory, "model.in", NULL);
  file = g_fopen (buffer, "r");
  if (!file)
    {
      message = g_strconcat ("Unable to open the model data file\n",
                             buffer, NULL);
      g_free (buffer);
      return 0;
    }
  if (fscanf (file, "%u%u%u%u", &type_fertilizer, &type_infiltration,
              &type_diffusion_soil, &type_beta) != 4)
    {
      message = g_strconcat ("Bad model data\n", buffer, NULL);
      i = 0;
    }
  else
    i = 1;
  fclose (file);
  g_free (buffer);
  return i;
}

void
field_write_model (JsonObject * object)
{
  JsonNode *node;
  JsonObject *child;
  if (!type_fertilizer && !type_infiltration && !type_diffusion_soil
      && !type_beta)
    return;
  child = json_object_new ();
  if (type_fertilizer)
    uint_write (child, "fertilizer", type_fertilizer);
  if (type_infiltration)
    uint_write (child, "infiltration", type_infiltration);
  if (type_diffusion_soil)
    uint_write (child, "diffusion-soil", type_diffusion_soil);
  if (type_beta)
    uint_write (child, "beta", type_beta);
  node = json_node_new (JSON_NODE_OBJECT);
  json_node_set_object (node, child);
  json_object_set_member (object, "models", node);
}

int
input_read (Input * i, FILE * file)
{
  if (fscanf (file, "%lf%lf%lf%lf%lf", &i->x, &i->y, &i->t0, &i->tf, &i->q)
      != 5)
    {
      message = g_strdup ("Bad input data");
      return 0;
    }
  return 1;
}

void
input_write (Input * i, JsonArray * array)
{
  JsonNode *node;
  JsonObject *child;
  child = json_object_new ();
  dbl_write (child, "x", i->x);
  dbl_write (child, "y", i->y);
  dbl_write (child, "t0", i->t0);
  dbl_write (child, "tf", i->tf);
  dbl_write (child, "q", i->q);
  node = json_node_new (JSON_NODE_OBJECT);
  json_node_set_object (node, child);
  json_array_add_element (array, node);
}

int
field_read_inputs (char *directory)
{
  FILE *file;
  char *buffer;
  unsigned int i, n;
  buffer = g_build_filename (directory, "input.in", NULL);
  file = g_fopen (buffer, "r");
  if (!file)
    {
      message = g_strconcat ("Unable to open the input file\n", buffer, NULL);
      g_free (buffer);
      return 0;
    }
  if (fscanf (file, "%u%u", &nwinputs, &nsinputs) != 2)
    {
      message = g_strconcat ("Bad input data\n", buffer, NULL);
      goto error1;
    }
  n = nwinputs + nsinputs;
  input = (Input *) malloc (n * sizeof (Input));
  if (!input)
    {
      message = g_strconcat ("Not enough memory to open the input data\n",
                             buffer, NULL);
      goto error1;
    }
  for (i = 0; i < n; ++i)
    {
      if (!input_read (input + i, file))
        {
          message = g_strconcat ("Bad input data\n", buffer, NULL);
          goto error1;
        }
    }
  fclose (file);
  g_free (buffer);
  return 1;

error1:
  fclose (file);
  g_free (buffer);
  return 0;
}

void
field_write_inputs (JsonObject * object)
{
  JsonNode *node;
  JsonArray *array;
  unsigned int i, j;
  array = json_array_new ();
  for (i = 0; i < nwinputs; ++i)
    input_write (input + i, array);
  node = json_node_new (JSON_NODE_ARRAY);
  json_node_set_array (node, array);
  json_object_set_member (object, "water-inputs", node);
  if (!nsinputs)
    return;
  array = json_array_new ();
  for (j = 0; j < nsinputs; ++j)
    input_write (input + i + j, array);
  node = json_node_new (JSON_NODE_ARRAY);
  json_node_set_array (node, array);
  json_object_set_member (object, "fertilizer-inputs", node);
}

int
field_read_probes (char *directory)
{
  FILE *file;
  char *buffer;
  double x, y;
  unsigned int i, j;
  buffer = g_build_filename (directory, "probe.in", NULL);
  file = g_fopen (buffer, "r");
  if (!file)
    {
      message = g_strconcat ("Unable to open the probes data file\n",
                             buffer, NULL);
      g_free (buffer);
      return 0;
    }
  if (fscanf (file, "%u", &nprobes) != 1)
    {
      j = 0;
      message = g_strconcat ("Bad probes data\n", buffer, NULL);
      goto exit0;
    }
  if (!nprobes)
    {
      j = 1;
      goto exit0;
    }
  probe = (Probe *) malloc (nprobes * sizeof (Probe));
  for (i = 0; i < nprobes; ++i)
    {
      if (fscanf (file, "%lf%lf", &x, &y) != 2)
        {
          j = 0;
          message = g_strconcat ("Bad probes data\n", buffer, NULL);
          goto exit0;
        }
      probe[i].x = x;
      probe[i].y = y;
    }
  j = 1;
exit0:
  fclose (file);
  g_free (buffer);
  return j;
}

void
field_write_probes (JsonObject * object)
{
  JsonNode *node;
  JsonArray *array;
  JsonObject *child;
  unsigned int i;
  if (!nprobes)
    return;
  array = json_array_new ();
  for (i = 0; i < nprobes; ++i)
    {
      child = json_object_new ();
      dbl_write (child, "x", probe[i].x);
      dbl_write (child, "y", probe[i].x);
      node = json_node_new (JSON_NODE_OBJECT);
      json_node_set_object (node, child);
      json_array_add_element (array, node);
    }
  node = json_node_new (JSON_NODE_ARRAY);
  json_node_set_array (node, array);
  json_object_set_member (object, "probes", node);
}

int
field_read (char *dir)
{
  if (!field_read_data (dir))
    goto exit_error;
  if (!field_read_time (dir))
    goto exit_error;
  if (!field_read_mesh (dir))
    goto exit_error;
  if (!field_read_model (dir))
    goto exit_error;
  if (!field_read_inputs (dir))
    goto exit_error;
  if (!field_read_probes (dir))
    goto exit_error;
  return 1;

exit_error:
  return 0;
}

void
field_write (JsonObject * object)
{
  field_write_data (object);
  field_write_times (object);
  field_write_mesh (object);
  field_write_model (object);
  field_write_inputs (object);
  field_write_probes (object);
}

int
main (int argn, char **argc)
{
  JsonGenerator *generator;
  JsonNode *node;
  JsonObject *object;
  char *buffer;
  if (argn != 2)
    {
      puts ("The syntax is:\n./update_format_from_5_to_6 input_directory");
      return 1;
    }
  if (!field_read (argc[1]))
    {
      puts (message);
      return 2;
    }
  object = json_object_new ();
  field_write (object);
  node = json_node_new (JSON_NODE_OBJECT);
  json_node_set_object (node, object);
  generator = json_generator_new ();
  json_generator_set_pretty (generator, TRUE);
  json_generator_set_root (generator, node);
  buffer = g_strconcat (argc[1], ".json", NULL);
  json_generator_to_file (generator, buffer, NULL);
  g_free (buffer);
  g_object_unref (generator);
  return 0;
}
