#include "bb_status.h"

void bb_status_init(bb_status* status, int n) {

  graph_init(&(*status).G_curr, n);
  onetree_init(&(*status).OT_curr, n);
  (*status).z_curr = 0;
  onetree_init(&(*status).H_best, n);
  (*status).z_best = 0;
  vector_init(&(*status).num_forced_edges, n);
  vector_init(&(*status).num_forbidden_edges, n);
  lvector_init(&(*status).forced_edges, n);
  (*status).current_call = 0;
  (*status).current_level = 0;

}

void bb_status_delete(bb_status* status) {

  graph_delete(&(*status).G_curr);
  onetree_delete(&(*status).OT_curr);
  (*status).z_curr = 0;
  onetree_delete(&(*status).H_best);
  (*status).z_best = 0;
  vector_delete(&(*status).num_forced_edges);
  vector_delete(&(*status).num_forbidden_edges);
  lvector_delete(&(*status).forced_edges);
  (*status).current_call = 0;
  (*status).current_level = 0;

}
