#include "bb_select_node.h"

int bb_select_node(tsp_env* env, int* w, int opt) {

  graph* G_CURR = &(*env).G_CURR;
  onetree* OT_CURR = &(*env).OT_CURR;
  int n =  G_CURR->n;
  int i, j, k, num_free_edges;

  if (opt == DEFAULT) {

    // Select the first vertex of the 1-tree such that 
    // it has at least 3 edges incident to it, 
    // and at least 1 of them is not forced nor forbidden.
    k = -1;
    for (i = 0; i < n; i++) {

      if (OT_CURR->V[i].deg >= 3) {

        num_free_edges = 0;

        for (j = 0; j < n; j++) {
          if (num_free_edges < 1 && j != i && onetree_adjacent_nodes(OT_CURR, i, j) &&
              graph_get_edge_constr(G_CURR, i, j) == FREE) {

            num_free_edges++;

          }
        }

        if (num_free_edges > 0) {
          k = i;
          break;
        }
      }
    }

    if (k >= 0) {
      *w = k;
      return SUCCESS;
    }
    else {
      return FAILURE;
    }
  
  }

  return SUCCESS;
}
