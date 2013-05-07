#ifndef BB_OUTPUT_H_
#define BB_OUTPUT_H_

#include "../../data/onetree.h"

struct bb_output {

  onetree H_opt; // tour ottimo
  double z_opt; // costo del tour ottimo

};

typedef struct bb_output bb_output;

// inizializza output
void bb_output_init(bb_output* output, int n);

// cancella output
void bb_output_delete(bb_output* output);

#endif
