#include "bb_select_edges.h"

int bb_select_edges(bb_env* env, int* w, int* v, int* u, int opt) {
  onetree* OT_CURR = &(*env).OT_CURR;
  graph* G_CURR = &(*env).G_CURR;

  int n = (*OT_CURR).n;
  int i, k, h;

  if (opt == DEFAULT) {
    // Select the first edge incident to w which is free.
    k = 0;
    for (i = 1; i <= n; i++) {
      if (i != *w && onetree_adjacent_nodes(OT_CURR, *w, i) && onetree_get_edge_constr(OT_CURR, *w, i) == FREE) {
	k = i;
	break;
      }
    }

    if (k == 0) {
      return FAILURE;
    }

    // Select the second edge incident to w which is free.
    h = 0;
    for (i = 1; i <= n; i++) {
      if (i != *w && i != k && onetree_adjacent_nodes(OT_CURR, *w, i) && onetree_get_edge_constr(OT_CURR, *w, i) == FREE) {
	h = i;
	break;
      }
    }

    if (h == 0) {
      return FAILURE;
    } 

  }

  *v = k;
  *u = h;

  return SUCCESS;
}
