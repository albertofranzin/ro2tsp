#include <stdlib.h>
#include <stdio.h>
#include "graph.h"
#include "egraph.h"
#include "tree.h"
#include "onetree.h"
#include "compute_mst.h"
#include "compute_ot.h"
#include "compute_nearest_neighbour.h"
#include "compute_upper_bound.h"
#include "compute_lagrange.h"
#include "solve_tsp.h"

void main() {

  int n = 20;

  graph G;
  onetree OT;
  egraph EG1;
  egraph EG2;

  graph_init(&G, 1);
  onetree_init(&OT, 1);
  egraph_init(&EG1, n);
  egraph_init(&EG2, n);

  egraph_random(&EG1);
  egraph_copy(&EG1, &EG2);

  egraph_to_graph(&EG1, &G);
  compute_ot(&G, &OT);

  onetree_to_egraph(&OT, &EG2);
  egraph_plot(&EG1, &EG2);

  //double L;
  double incumbent;
  incumbent = compute_upper_bound(&G);
  //L = compute_lagrange(&G, &OT, ub, 100, 2.0, 10, 200);
  //printf("L = %f\n", L);

 
  double ub = incumbent;
  double alpha = 2.0;
  int max_iter = 30000;
  int ub_max_iter = 20000;
  int alpha_max_iter = 200;
  double L_best;

  onetree H;
  onetree_init(&H, 1);

  solve_tsp(&G, &H, &incumbent, 0, ub, ub_max_iter, alpha, alpha_max_iter, max_iter);

  onetree_to_egraph(&H, &EG2);
  egraph_plot(&EG1, &EG2);

  graph_delete(&G);
  onetree_delete(&OT);  
  onetree_delete(&H);
  egraph_delete(&EG1);
  egraph_delete(&EG2);
}
