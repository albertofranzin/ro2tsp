#include "graph.h"
#include "compute_nearest_neighbour.h"

double compute_nearest_neighbour(graph *G, int start_node) {
  double min, cost_of_cycle;
  int k, i, current, next;

  int n = (*G).n;
  int visited[n+1];

  for (i = 0; i < n+1; i++)
    visited[i] = 0;

  cost_of_cycle = 0;

  current = start_node;
  visited[current] = 1;

  for (k = 1; k < n; k++) {
    for (i = 1; i <= n; i++) {
      if (visited[i] == 0) {
	min = graph_get_edge_cost(G, current, i);
	next = i;
	break;
      }
    }
    for (i = 1; i <= n; i++) {
      if (visited[i] == 0 && graph_get_edge_cost(G, current, i) < min) {
	min = graph_get_edge_cost(G, current, i);
	next = i;
      }
    }
    cost_of_cycle += min;
    current = next;
    visited[current] = 1;
  }

  cost_of_cycle += graph_get_edge_cost(G, current, start_node);

  return cost_of_cycle;
}
