#include "tsp_constraints.h"

void tsp_constraints_a(int w, int v, int u, double cost_wv, double cost_wu, tsp_status* status, tsp_backup* backup, tsp_backup* update) {

  graph* G_curr = &(*status).G_curr;
  int n = (*status).G_curr.n;
  double c;
  int i;

  tsp_backup_save_edge(backup, w, v, graph_get_edge_cost(G_curr, w, v));

  graph_set_edge_cost(G_curr, w, v, cost_wv);

  if (update != NULL) {
    tsp_backup_save_edge(update, w, v, cost_wv);
  }

  if (cost_wv <= SMALL) {
    (*status).num_forced_edges.array[w-1]++;
    (*status).num_forced_edges.array[v-1]++;
  }
  else if (cost_wv >= BIG) {
    (*status).num_forbidden_edges.array[w-1]++;
    (*status).num_forbidden_edges.array[v-1]++;
  }

  if (u > 0) {

    tsp_backup_save_edge(backup, w, u, graph_get_edge_cost(G_curr, w, u));

    graph_set_edge_cost(G_curr, w, u, cost_wu);

    if (update != NULL) {
      tsp_backup_save_edge(update, w, u, cost_wu);
    }

    if (cost_wu <= SMALL) {
      (*status).num_forced_edges.array[w-1]++;
      (*status).num_forced_edges.array[u-1]++;
    }
    else if (cost_wu >= BIG) {
      (*status).num_forbidden_edges.array[w-1]++;
      (*status).num_forbidden_edges.array[u-1]++;
    }

  }

}
