#include "bb_forbid_fat_edges.h"

void bb_forbid_fat_edges(tsp_env* te, tsp_stats* ts, double ub) {

  
  int i, j;

  graph* G_CURR = &(te->G_CURR);
  int n = G_CURR->n;

  onetree MIN_OT;
  onetree_init(&MIN_OT, 0);

  compute_ot(G_CURR, &MIN_OT);
  compute_deltas(G_CURR, &MIN_OT);

  double lb = onetree_get_cost(&MIN_OT, G_CURR);

  for (i = 0; i < n; i++) {
    for (j = i+1; j < n; j++) {
      if (lb + graph_get_edge_delta(G_CURR, i, j) > ub) {
        graph_set_edge_constr(G_CURR, i, j, FORBIDDEN);
        ts->num_fat_edges++;
      }
    }
  }
  printf("# node %d : number of fat edges           = %d\n", te->curr_call, ts->num_fat_edges);

}
