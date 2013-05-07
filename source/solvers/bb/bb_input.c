#include "bb_input.h"

void bb_input_init(bb_input* input, int n) {

  graph_init(&(*input).G, n);
  onetree_init(&(*input).H, n);
  (*input).ub = 0.0;

}

void bb_input_delete(bb_input* input) {

  graph_delete(&(*input).G);
  onetree_delete(&(*input).H);
  (*input).ub = 0.0;

}
