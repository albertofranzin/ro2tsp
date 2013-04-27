#include "tsp_output.h"

void tsp_output_init(tsp_output* output, int n) {
  
  onetree_init(&(*output).H_opt, n);
  (*output).z_opt = 0;

}

void tsp_output_delete(tsp_output* output) {
  
  onetree_delete(&(*output).H_opt);
  (*output).z_opt = 0;

}
