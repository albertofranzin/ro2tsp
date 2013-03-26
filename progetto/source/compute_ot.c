#include "compute_ot.h"

void compute_ot(graph* G, graph* T) {
  int i, u, v;
  double min;
  int n = (*G).n;

  deleteGraph(T);
  initGraph(T, n);
  for (i = 1; i <= n; i++) {
    set_node_x(T, i, get_node_x(G, i));
    set_node_y(T, i, get_node_y(G, i));
  }

  u = 2;
  min = get_edge_cost(G, 1, u);
  for (i = 2; i <= n; i++) {
    if (get_edge_cost(G, 1, i) < min) {
      u = i;
      min = get_edge_cost(G, 1, i);
    }
  }

  v = (u == 2) ? 3 : 2;
  min = get_edge_cost(G, 1, v);
  for (i = 2; i <= n; i++) {
    if (i != u && get_edge_cost(G, 1, i) < min) {
      v = i;
      min = get_edge_cost(G, 1, i);
    }
  }

  for (i = 2; i <= n; i++)
    remove_edge(G, 1, i);

  compute_mst(G, T, 2);

  insert_edge(T, 1, u);
  insert_edge(T, 1, v);
  set_edge_cost(T, 1, u, get_edge_cost(G, 1, u));
  set_edge_cost(T, 1, v, get_edge_cost(G, 1, v));

  for (i = 2; i <= n; i++)
    insert_edge(G, 1, i);

}

