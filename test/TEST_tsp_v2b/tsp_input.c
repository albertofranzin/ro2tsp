#include "tsp_input.h"

void tsp_input_init(tsp_input* input, int n) {

  graph_init(&(*input).G, n);
  onetree_init(&(*input).H, n);
  (*input).ub = 0.0;

}

void tsp_input_delete(tsp_input* input) {

  graph_delete(&(*input).G);
  onetree_delete(&(*input).H);
  (*input).ub = 0.0;

}
