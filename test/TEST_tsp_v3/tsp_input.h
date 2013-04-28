#ifndef TSP_INPUT_H_
#define TSP_INPUT_H_

#include "graph.h"
#include "onetree.h"

struct tsp_input {

  graph G; // grafo completo dell'istanza TSP
  onetree H; // un tour in G
  double ub; // upper bound del costo del tour ottimo in G (e.g. il costo di H) 
};

typedef struct tsp_input tsp_input;

// inizializzat input
void tsp_input_init(tsp_input* input, int n);

// cancella input
void tsp_delete_init(tsp_input* input);

#endif
