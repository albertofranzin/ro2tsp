#include <stdlib.h>
#include "graph.h"
#include "compute_nearest_neighbour.h"
#include "compute_upper_bound.h"

double compute_upper_bound(graph* G) { // da migliorare magari usando altri euristici
  int i;
  double min, cost_of_cycle;
  int n = (*G).n;

  min = compute_nearest_neighbour(G, 1);
  for (i = 2; i <= n; i++) {
    cost_of_cycle = compute_nearest_neighbour(G, i);
    if (cost_of_cycle < min)
      min = cost_of_cycle;
  }
  return min;
}

