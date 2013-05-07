#include "bb_output.h"

void bb_output_init(bb_output* output, int n) {
  
  onetree_init(&(*output).H_opt, n);
  (*output).z_opt = 0;

}

void bb_output_delete(bb_output* output) {
  
  onetree_delete(&(*output).H_opt);
  (*output).z_opt = 0;

}
