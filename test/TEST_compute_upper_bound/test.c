#include <stdio.h>
#include "graph.h"
#include "egraph.h"
#include "tree.h"
#include "onetree.h"
#include "compute_nearest_neighbour.h"
#include "compute_upper_bound.h"

void main() {
  int n = 100; // numero di nodi
  int s = 1321; // seed
  int r = 1; // radice

  egraph EG;
  egraph_init(&EG, n);
  srand(s);
  egraph_random(&EG);
  //egraph_plot(&EG, NULL);

  graph G;
  graph_init(&G, 1);
  egraph_to_graph(&EG, &G);

  onetree OT_UB;
  onetree_init(&OT_UB, n);

  double ub = compute_upper_bound(&G, &OT_UB);
  printf("upper bound = %f\n", ub);

  egraph EG_UB;
  egraph_init(&EG_UB, 1);
  egraph_copy(&EG, &EG_UB);
  onetree_to_egraph(&OT_UB, &EG_UB);

  egraph_plot(&EG, &EG_UB);

  onetree_delete(&OT_UB);
  graph_delete(&G);
  egraph_delete(&EG);
  egraph_delete(&EG_UB);

}

