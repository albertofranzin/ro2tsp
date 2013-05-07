#include "bb_select_edges.h"

void bb_select_edges(bb_status* status, int* w, int* v, int* u) {
  onetree* OT_curr = &(*status).OT_curr;
  graph* G_curr = &(*status).G_curr;
  double c;
  int n = (*OT_curr).n;
  int i;

  int k = 0;
  // seleziona il primo lato non vietato e non forzato incidente su w
  for (i = 1; i <= n; i++) {
    if (i != *w && onetree_adjacent_nodes(OT_curr, *w, i)) {
      c = graph_get_edge_cost(G_curr, *w, i);
      if (c > SMALL && c < BIG) {
	k = i;
	break;
      }
    }
  }
  int h = 0;
  // seleziona il secondo lato non vietato e non forzato incidente su w
  for (i = 1; i <= n; i++) {
    if (i != *w && i != k && onetree_adjacent_nodes(OT_curr, *w, i)) {
      c = graph_get_edge_cost(G_curr, *w, i);
      if (c > SMALL && c < BIG) {
	h = i;
	break;
      }
    }
  }

  *v = k;
  *u = h;
}
