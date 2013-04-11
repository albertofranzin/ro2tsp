#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
#include "utils.h"

#include "list.h"

int main(int argc, char** argv) {
  int i;
  char* opt;
  int n = DEFAULT_NUMBER_OF_NODES;
  int s = DEFAULT_SEED;

  parameters *pars = getParameters();
  n = pars->no_of_nodes;
  s = pars->seed;

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

  srand(s);

  egraph EG;
  egraph_init(&EG, n);

  if (pars->tsp_file != NULL) {
    printf("using tsplib file %s\n", pars->tsp_file);
    read_tsp_from_file(&EG, pars);
  } else {
    egraph_random(&EG);
  }



  graph G;
  graph_init(&G, 1);
  egraph_to_graph(&EG, &G);

  tree T;
  tree_init(&T, n);

  printf("@ Euclidean TSP\n# number of nodes = %d\n# seed = %d\n\n", n, s);
  double heuristic_upper_bound;
  heuristic_upper_bound = compute_upper_bound(&G, &T);
  printf("@ Nearest Neighbour Heuristic\n# upper bound = %f\n", heuristic_upper_bound); 

  /*egraph EG1;
  egraph_init(&EG1, 1);
  egraph_copy(&EG, &EG1);
  tree_to_egraph(&T, &EG1);
  egraph_plot(&EG, &EG1);*/

  /**/
  double lagrangean_lower_bound;
  double ub = heuristic_upper_bound;
  int max_num_of_iterations = 30000;
  int slack_time = 20000;
  int alpha_halving_time = 200;
  onetree HL;
  onetree_init(&HL, 1);
  printf("@ Lagrangean Dual Problem - Subgradient Algorithm\n");
  printf("# upper bound = %f\n# max number of iterations = %d\n# max number of iterations without improvements = %d\n# alpha = %f\n# alpha halving time = %d\n", ub, max_num_of_iterations, slack_time, ALPHA, alpha_halving_time);
  lagrangean_lower_bound = compute_lagrange(&G, &HL, ub);
  egraph EG2;
  egraph_init(&EG2, 1);
  egraph_copy(&EG, &EG2);
  onetree_to_egraph(&HL, &EG2);
  egraph_plot(&EG, &EG2);
  /**/

  double incumbent = heuristic_upper_bound + EPSILON;
  onetree H;
  onetree_init(&H, 1);
  printf("\n@ Branch and Bound\n# initial incumbent = %f\n", incumbent);
  printf("Lagrangean lower bound = %f\n", lagrangean_lower_bound);
  solve_tsp(&G, &H, &incumbent, 0);

  printf("@main\nsolve_tsp terminated\n");

  egraph EG1;
  egraph_init(&EG1, 1);
  egraph_copy(&EG, &EG1);
  onetree_to_egraph(&H, &EG1);
  egraph_plot(&EG, &EG1);

  return 0;
}
