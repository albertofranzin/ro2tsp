#include "base/base.h"
#include "solvers/cplex_solver.h"
#include "algos/compute_upper_bound.h"
#include "data/cycle.h"
#include "data/onetree.h"


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
    printf("@ Euclidean TSP\n# tsplib instance\n");
    printf("# number of nodes = %d\n# tsplib file = %s\n\n",
                pars->number_of_nodes, pars->tsp_file);
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
  heuristic_upper_bound = compute_upper_bound(&G, &C, pars->heuristic_algo);
  printf("@ Nearest Neighbour Heuristic\n# upper bound = %f\n", heuristic_upper_bound);

  //
  // solve
  //

  double incumbent;
  double z;

  incumbent = heuristic_upper_bound + 1;
  printf("heuristic_upper_bound = %.30f,\n", heuristic_upper_bound);
  printf("            incumbent = %.30f\n",  incumbent);
  onetree ONE_TREE;
  onetree_init(&ONE_TREE, 1);

  z = compute_lagrange(&G, &ONE_TREE, incumbent);

  printf("gap = %f %%\n", 100 * (heuristic_upper_bound - z) / heuristic_upper_bound);

  switch(pars->solver) {
  onetree H;
  onetree_init(&H, 1);

    case BRANCH_AND_BOUND :

        if (is_cycle(&ONE_TREE) == 1) {
          printf("lagrangiano FTW!\n");
          egraph EG1;
          egraph_init(&EG1, 1);
          egraph_copy(&EG, &EG1);
          onetree_to_egraph(&ONE_TREE, &EG1);
          egraph_plot(&EG, &EG1);
        } else {
          printf("\n@ Branch and Bound\n# initial incumbent = %f, lower bound = %f\n",
            incumbent, z);
          solve_tsp(&G, &H, &incumbent, &ONE_TREE, z, 0);
          egraph EG1;
          egraph_init(&EG1, 1);
          egraph_copy(&EG, &EG1);
          onetree_to_egraph(&H, &EG1);
          egraph_plot(&EG, &EG1);
        }
        break;

    case CPLEX :
        // cplex_solve_problem(&G, &EG, &incumbent);
        solve_kruskal(&G, &EG, &incumbent);
        break;

  }

  printf("final solution has cost %f\n", incumbent);

  return 0;
}

