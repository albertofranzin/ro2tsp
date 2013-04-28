#include "compute_nearest_neighbour.h"

double compute_nearest_neighbour(graph *G, onetree* OT, int start_node) {
  double min, tour_cost;
  int k, i, current, next;

  int n = (*G).n;
  int visited[n];

  onetree_delete(OT);
  onetree_init(OT, n);

  for (i = 0; i < n; i++)
    visited[i] = 0;

  tour_cost = 0.0;

  current = start_node;
  visited[current-1] = 1;

  for (k = 1; k < n; k++) {
    for (i = 1; i <= n; i++) {
      if (visited[i-1] == 0) {
	min = graph_get_edge_cost(G, current, i);
	next = i;
	break;
      }
    }
    for (i = 1; i <= n; i++) {
      if (visited[i-1] == 0 && graph_get_edge_cost(G, current, i) < min) {
	min = graph_get_edge_cost(G, current, i);
	next = i;
      }
    }
    onetree_insert_edge(OT, current, next, min);
    tour_cost += min;
    current = next;
    visited[current-1] = 1;
  }

  tour_cost += graph_get_edge_cost(G, current, start_node);
  onetree_insert_edge(OT, current, start_node, graph_get_edge_cost(G, current, start_node));

  return tour_cost;
}
