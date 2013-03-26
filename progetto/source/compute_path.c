#include "compute_path.h"

int compute_path(graph* G, graph* P, int from, int to, int flag) {
  int i, j, pred;

  int n = (*G).n;

  if (flag == 0) {
    deleteGraph(P);
    initGraph(P, n);
    for (i = 1; i <= n; i++) {
      set_node_x(P, i, get_node_x(G, i));
      set_node_y(P, i, get_node_y(G, i));
    }
    flag = 1;
  }

  if (from == to)
    return 1;

  pred = from;
  for (i = 1; i <= n; i++) {
    if (i != from && adjacent(P, from, i)) {
      pred = i;
      break;
    }
  }

  for (i = 1; i <= n; i++) {
    if (i != from && i != pred && adjacent(G, from, i)) {
      insert_edge(P, from, i);
      if (compute_path(G, P, i, to, flag)) {
	return 1;
      }
      else {
	remove_edge(P, from, i);
      }
    }
  }

  return 0;
}
