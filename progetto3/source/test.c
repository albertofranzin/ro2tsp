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

#include "list.h"
#include "edge.h"
#include "edgelist.h"

#include "utils.h"

int main(int argc, char** argv) {
  int i;
  char* opt;

  parameters* pars = getParameters();

  /* ======================== */
  /* parse command line input */
  /* ======================== */


  for (i = 1; i < argc; i++) {
    opt = argv[i];
    if (strcmp(opt, "-n") == 0)
      pars->number_of_nodes = atoi(argv[++i]);
    if (strcmp(opt, "-s") == 0)
      pars->seed = atoi(argv[++i]);
  }







  /* ================= */
  /* inizializza grafo */
  /* ================= */

  graph G;
  graph_init(&G, 1);

  egraph EG;
  egraph_init(&EG, pars->number_of_nodes);

  if (pars->tsp_file_option == TRUE) {
    read_tsp_from_file(&EG, pars);
    egraph_to_graph(&EG, &G);
    printf("@ Euclidean TSP\n# tsplib instance\n# number of nodes = %d\n# tsplib file = %s\n\n", pars->number_of_nodes, pars->tsp_file);
  }
  else if (pars->random_instance_option == TRUE) {
    if (pars->random_seed_option == FALSE) {
      srand(pars->seed);
      egraph_random(&EG);
      egraph_to_graph(&EG, &G);
      printf("@ Euclidean TSP\n# random instance\n# number of nodes = %d\n# seed = %d\n\n", pars->number_of_nodes, pars->seed);
    }
    else {
      ; // aggiungi caso generazione pseudocasuale del seed
    }
  }


  /* ========================================= */
  /* Algoritmo Euristico - ricerca upper bound */
  /* ========================================= */

  tree T;
  tree_init(&T, 1);

  double heuristic_upper_bound;
  heuristic_upper_bound = compute_upper_bound(&G, &T);
  printf("@ Nearest Neighbour Heuristic\n# upper bound = %f\n", heuristic_upper_bound); 

  heuristic_upper_bound = heur2opt(&G, &T, heuristic_upper_bound);
  printf("@ 2-opt\nupper bound = %f\n", heuristic_upper_bound);




  /* ==================================== */
  /* Algoritmo Subgradiente - Lagrangiano */
  /* ==================================== */

  /*
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
  */






  /* ======================== */
  /* Algoritmo Branch & Bound */
  /* ======================== */
 
  double incumbent = heuristic_upper_bound + EPSILON;
  onetree H;
  onetree_init(&H, 1);
  printf("\n@ Branch and Bound\n# initial incumbent = %f\n", incumbent);

  onetree ONE_TREE;
  onetree_init(&ONE_TREE, 1);

  double z;


  //compute_ot(&G, &ONE_TREE);
  //z = onetree_get_cost(&ONE_TREE);

  z = compute_lagrange(&G, &ONE_TREE, incumbent);

  solve_tsp(&G, &H, &incumbent, &ONE_TREE, z, 0); 

  egraph EG1;
  egraph_init(&EG1, 1);
  egraph_copy(&EG, &EG1);
  onetree_to_egraph(&H, &EG1);
  egraph_plot(&EG, &EG1);
  

}
