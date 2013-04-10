#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
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

void main(int argc, char** argv) {
  int i;
  char* opt;
  int n = DEFAULT_NUMBER_OF_NODES;
  int s = DEFAULT_SEED;



  /* ======================== */
  /* parse command line input */
  /* ======================== */

  for (i = 1; i < argc; i++) {
    opt = argv[i];
    if (strcmp(opt, "-n") == 0)
      n = atoi(argv[++i]);
    if (strcmp(opt, "-s") == 0)
      s = atoi(argv[++i]);
  }



  egraph EG;
  egraph_init(&EG, n);

  srand(s);
  egraph_random(&EG);

  graph G;
  graph_init(&G, 1);
  egraph_to_graph(&EG, &G);

  printf("@ Euclidean TSP\n# number of nodes = %d\n# seed = %d\n\n", n, s);

  double heuristic_upper_bound;
  heuristic_upper_bound = compute_upper_bound(&G);
  printf("@ Nearest Neighbour Heuristic\n# upper bound = %f\n\n", heuristic_upper_bound); 

  double incumbent = heuristic_upper_bound;
  onetree H;
  onetree_init(&H, 1);
  printf("@ Branch and Bound\n# initial incumbent = %f\n", incumbent);
  solve_tsp(&G, &H, &incumbent, 0); 

  egraph EG1;
  egraph_init(&EG1, 1);
  egraph_copy(&EG, &EG1);
  onetree_to_egraph(&H, &EG1);
  egraph_plot(&EG, &EG1);

}
