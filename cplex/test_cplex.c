#include "base/base.h"
#include "solvers/cplex_solver.h"
#include "algos/compute_upper_bound.h"
#include "data/cycle.h"


int main (int argc, char *argv[]) {

  parameters* pars = base_problem_setup(argc, argv);

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
    egraph_random(&EG);
    egraph_to_graph(&EG, &G);
    printf("@ Euclidean TSP\n# random instance\n# number of nodes = %d\n# seed = %ld\n\n", pars->number_of_nodes, pars->seed);
  }


  printf("creating a cycle\n");
  cycle C;
  cycle_init(&C, 1);
  printf("cycle created\n");

  double heuristic_upper_bound;
  heuristic_upper_bound = compute_upper_bound(&G, &C, NEAREST_NEIGHBOUR);
  printf("@ Nearest Neighbour Heuristic\n# upper bound = %f\n", heuristic_upper_bound);

  /** /heuristic_upper_bound = heur2opt(&G, &C, heuristic_upper_bound);
  printf("@ 2-opt\nupper bound = %f\n", heuristic_upper_bound);/ **/

  /////////////////////////////////////////////////////////////////
  //
  //   let the cplex part begin
  //
  /////////////////////////////////////////////////////////////////


  double incumbent;

  cplex_solve_problem(&G, &EG, &incumbent);

  printf("final solution has cost %f\n", incumbent);

  return 0;
}

