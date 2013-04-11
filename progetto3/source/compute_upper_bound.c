#include "compute_upper_bound.h"

double compute_upper_bound(graph* G, tree *OT) { // da migliorare magari usando altri euristici
  int i, best_index;
  double min, cost_of_cycle;
  int n = (*G).n;

  tree BEST_OT;
  tree_init(&BEST_OT, n);

  tree_delete(OT);
  tree_init(OT, n);

  min = compute_nearest_neighbour(G, &BEST_OT, 1);
  best_index = 1;
  for (i = 2; i <= n; i++) {
    cost_of_cycle = compute_nearest_neighbour(G, OT, i);
    if (cost_of_cycle < min) {
        min = cost_of_cycle;
        best_index = i;
        tree_copy(OT, &BEST_OT);
      }
  }
  tree_copy(&BEST_OT, OT);
  tree_delete(&BEST_OT);
  printf("best_index is %d\n", best_index);
  return min;
}

/*
 * heur2opt
 * G: graph
 * OT : tree computed by another heuristic
 *     (will be modified if it's not 2-opt)
 * cost : cost of the solution computed by the heuristic
 *
 * compute a 2-opt admissible solution, in order to swap crossing edges
 *
 * return : cost of the 2-opt solution
 */
double heur2opt(onetree *G, tree *OT, double cost) {
  
}
