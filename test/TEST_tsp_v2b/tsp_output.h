#ifndef TSP_OUTPUT_H_
#define TSP_OUTPUT_H_

#include "onetree.h"

struct tsp_output {

  onetree H_opt; // tour ottimo
  double z_opt; // costo del tour ottimo

};

typedef struct tsp_output tsp_output;

// inizializza output
void tsp_output_init(tsp_output* output, int n);

// cancella output
void tsp_output_delete(tsp_output* output);

#endif
