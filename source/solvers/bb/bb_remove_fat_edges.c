#include "bb_remove_fat_edges.h"

void bb_remove_fat_edges(bb_env* env, bb_stats* stats, double ub) {

  
  int i, j;

  graph* G_CURR = &(*env).G_CURR;
  int n = (*G_CURR).n;

  onetree MIN_OT;
  onetree_init(&MIN_OT, 0);

  compute_ot(G_CURR, &MIN_OT);
  compute_deltas(G_CURR, &MIN_OT);

  double lb = onetree_get_cost(&MIN_OT);

  for (i = 1; i <= n; i++) {
    for (j = i+1; j <= n; j++) {
      if (lb + graph_get_edge_delta(G_CURR, i, j) > ub) {
	graph_set_edge_constr(G_CURR, i, j, FORBIDDEN);
	(*stats).num_fat_edges++;
      }
    }
  }
  printf("# node %d : number of fat edges           = %d\n", (*env).curr_call, (*stats).num_fat_edges);

}
