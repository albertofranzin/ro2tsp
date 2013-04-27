#include "compute_upper_bound.h"

double compute_upper_bound(graph* G, onetree* OT) {
  int i;
  double min, tour_cost;
  int n = (*G).n;

  onetree OT_tmp;
  onetree_init(&OT_tmp, n);

  min = compute_nearest_neighbour(G, &OT_tmp, 1);
  for (i = 2; i <= n; i++) {
    tour_cost = compute_nearest_neighbour(G, &OT_tmp, i);
    if (tour_cost < min) {
      min = tour_cost;
      onetree_copy(&OT_tmp, OT);
    }
  }

  return min;
}

