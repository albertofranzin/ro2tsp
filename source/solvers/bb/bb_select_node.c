#include "bb_select_node.h"

void bb_select_node(bb_status* status, int* w) {
  onetree* OT_curr = &(*status).OT_curr;
  int n =  (*OT_curr).n;
  int i;

  int k = 0;
  // seleziona il primo nodo dell'1-albero OT_curr con almeno 3 lati incidenti
  for (i = 1; i <= n; i++) {
    if (onetree_get_node_deg(OT_curr, i) >= 3) {
      k = i;
      break;
    }
  }

  *w = k;
}
