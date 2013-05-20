#include "bb_select_node.h"

int bb_select_node(tsp_env* env, int* w, int opt) {
  onetree* OT_CURR = &(*env).OT_CURR;
  int n =  (*OT_CURR).n;
  int i, k, num_free_edges, min_num_free_edges, min_i;


  if (opt == DEFAULT) {

    // Select the first vertex of the 1-tree such that 
    // it has at least 3 edges incident to it, 
    // and at least 1 of them is not forced nor forbidden.
    k = 0;
    for (i = 1; i <= n; i++) {
      num_free_edges = onetree_get_node_deg(OT_CURR, i) -
                       onetree_get_node_deg_forced(OT_CURR, i) -
                       onetree_get_node_deg_forbidden(OT_CURR, i);
      if (onetree_get_node_deg(OT_CURR, i) >= 3 && num_free_edges > 0) {
        k = i;
        break;
      }
    }

  }

  if (k > 0) {
    *w = k;
    return SUCCESS;
  }
  else {
    return FAILURE;
  }
}
