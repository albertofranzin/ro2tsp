#ifndef BB_INPUT_H_
#define BB_INPUT_H_

#include "../../data/graph.h"
#include "../../data/onetree.h"

struct bb_input {

  graph G; // grafo completo dell'istanza TSP
  onetree H; // un tour in G
  double ub; // upper bound del costo del tour ottimo in G (e.g. il costo di H) 
};

typedef struct bb_input bb_input;

// inizializzat input
void bb_input_init(bb_input* input, int n);

// cancella input
void bb_delete_init(bb_input* input);

#endif
