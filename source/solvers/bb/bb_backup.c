#include "bb_backup.h"

void bb_backup_init(bb_backup* backup) {

  onetree_init(&(*backup).OT_curr, 3);
  (*backup).z_curr = 0.0;
  vector_init(&(*backup).num_forced_edges, 1);
  vector_init(&(*backup).num_forbidden_edges, 1);
  (*backup).current_call = 0;
  (*backup).current_level = 0;
  lvector_init(&(*backup).forced_edges, 1);
  elist_init(&(*backup).G_modified_edges);

}

void bb_backup_delete(bb_backup* backup) {

  onetree_delete(&(*backup).OT_curr);
  (*backup).z_curr = 0.0;
  vector_delete(&(*backup).num_forced_edges);
  vector_delete(&(*backup).num_forbidden_edges);
  (*backup).current_call = 0;
  (*backup).current_level = 0;
  lvector_delete(&(*backup).forced_edges);
  elist_delete(&(*backup).G_modified_edges);

}

void bb_backup_save_edge(bb_backup* backup, int x, int y, double cost) {

  elist_push_last(&(*backup).G_modified_edges, x, y, cost);

}

void bb_backup_save(bb_backup* backup, bb_status* status, int opt_1, int opt_2, int opt_3, int opt_4, int opt_5, int opt_6, int opt_7) {

  if (opt_1 == Y) {
    onetree_copy(&(*status).OT_curr, &(*backup).OT_curr);
  }
  if (opt_2 == Y) {
    (*backup).z_curr = (*status).z_curr;
  }
  if (opt_3 == Y) {
    vector_copy(&(*status).num_forced_edges, &(*backup).num_forced_edges);
  }
  if (opt_4 == Y) {
    vector_copy(&(*status).num_forbidden_edges, &(*backup).num_forbidden_edges);
  }
  if (opt_5 == Y) {
    (*backup).current_call = (*status).current_call;
  }
  if (opt_6 == Y) {
    (*backup).current_level = (*status).current_level;
  }
  if (opt_7 == Y) {
    lvector_copy(&(*status).forced_edges, &(*backup).forced_edges);
  }
}

void bb_backup_restore(bb_backup* backup, bb_status* status, int opt_1, int opt_2, int opt_3, int opt_4, int opt_5, int opt_6, int opt_7, int opt_8) {

  if (opt_1 == Y) {
    onetree_copy(&(*backup).OT_curr, &(*status).OT_curr);
  }
  if (opt_2 == Y) {
    (*status).z_curr = (*backup).z_curr;
  }
  if (opt_3 == Y) {
    vector_copy(&(*backup).num_forced_edges, &(*status).num_forced_edges);
  }
  if (opt_4 == Y) {
    vector_copy(&(*backup).num_forbidden_edges, &(*status).num_forbidden_edges);
  }
  if (opt_5 == Y) {
    (*status).current_call = (*backup).current_call;
  }
  if (opt_6 == Y) {
    (*status).current_level = (*backup).current_level;
  }
  if (opt_7 == Y) {
    lvector_copy(&(*backup).forced_edges, &(*status).forced_edges);
  }
  if (opt_8 == Y) {
    
    int i;
    edge e;
    int m = elist_get_size(&(*backup).G_modified_edges);
    elist_node* current_node = elist_get_first(&(*backup).G_modified_edges);
    for (i = 0; i < m; i++) {
      e = elist_get_element(&(*backup).G_modified_edges, current_node);
      graph_set_edge_cost(&(*status).G_curr, e.x, e.y, e.cost);
      current_node = elist_get_next(&(*backup).G_modified_edges, current_node);
    }
    
    /*
    edge e;
    while (!elist_is_empty(&(*backup).G_modified_edges)) {
      e = elist_pop_first(&(*backup).G_modified_edges);
      graph_set_edge_cost(&(*status).G_curr, e.x, e.y, e.cost);
    }
    */
    
  }
}
