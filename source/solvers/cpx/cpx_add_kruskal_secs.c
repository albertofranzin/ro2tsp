#include "cpx_add_kruskal_secs.h"

int cpx_add_kruskal_secs(CPXENVptr   env,
                         CPXLPptr    lp,
                         cpx_table  *hash_table,
                         graph      *G,
                         onetree    *OT,
                         parameters *pars)
{

  int i, k, status;

  int n = (*OT).n;

  double cost, cost_min;
  int flag, mark1, mark2, x, y;

  int vrtx_mrks[n];
  for (i = 0; i < n; i++) vrtx_mrks[i] = i;

  int selected_edges[n];
  memset(selected_edges, 0, sizeof(selected_edges));

  for (k = 0; k < n-2; k++) {

    flag = 0;
    for (i = 1; i < n; i++) {
      if (i != (*OT).nn1 && selected_edges[i] == 0) {

        cost = graph_get_edge_cost(G, OT->V[i].pred, i);
             
        if (flag == 0 || cost < cost_min) {
          x = OT->V[i].pred;
          y = i;
          cost_min = cost;
          flag = 1;
        }

      }
    }
    selected_edges[y] = 1;

    mark1 = vrtx_mrks[x];
    mark2 = vrtx_mrks[y];

    for (i = 1; i < n; i++) {

      if (vrtx_mrks[i] == mark2) vrtx_mrks[i] = mark1;

    }

    status = cpx_add_my_sec(env, lp, hash_table,
                            vrtx_mrks, n, mark1, pars);
    assert(status == SUCCESS);

  }

  printf("# added Kruskal SECs\n");
  return status;
}
