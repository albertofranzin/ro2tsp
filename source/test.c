#include "base/constants.h"
#include "base/base.h"

#include "data/graph.h"
#include "data/egraph.h"

#include "solvers/cpx/cpx_solver.h"
#include "solvers/bb/bb_solver.h"
#include "solvers/bb/bb_env.h"
#include "solvers/bb/bb_stats.h"

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

  /*
  //printf("creating a cycle\n");
  cycle C;
  cycle_init(&C, 1);
  //printf("cycle created\n");

  double heuristic_upper_bound;
  heuristic_upper_bound = compute_upper_bound(&G, &C, pars->heuristic_algo);
  //heuristic_upper_bound = compute_upper_bound(&G, &NN, pars->heuristic_algo);
  printf("@ Nearest Neighbour Heuristic\n# upper bound = %f\n", heuristic_upper_bound);

  double lagr_lower_bound;
  onetree NN;
  onetree_init(&NN, pars->number_of_nodes);
  lagr_lower_bound = compute_lagrange(&G, &NN, heuristic_upper_bound);

  printf("bounds for the solution: [%f, %f]\n", heuristic_upper_bound, lagr_lower_bound);
  

  double incumbent = heuristic_upper_bound;
  */

  switch(pars->solver) {

    case BRANCH_AND_BOUND :
      {

	bb_env env;
	bb_env_init(&env);

	graph_copy(&G, &env.G_INPUT);
	egraph_copy(&EG, &env.EG_INPUT);

	bb_stats stats;
	bb_stats_init(&stats);


	bb_solver(&env, &stats);

	printf("----------------------------------------------------------------------\n");
	printf("# cost of the optimal solution = %f\n", env.z_opt);
	bb_stats_print(&stats);

	cycle_plot(&env.TOUR_OPT, &EG, "OPT TOUR");

#ifdef DEBUG
	/*
          printf("pruned branches: %d\n", stats.pruned_branches);
          printf("took branches these times: %d %d %d %d %d\n",
              stats.took1, stats.took2, stats.took3, stats.took4, stats.took5);
	*/
#endif

	printf("# quality of bounds:\n");
	printf("- best heur. upper bound  = %f\n", stats.init_ub / env.z_opt );
	printf("- lagr. lower bound       = %f\n", stats.init_lb / env.z_opt);

      }
        break;

    case CPLEX :
      {

	graph H;
	graph_init(&H, 1);

	cpx_solver(&G, &H);

	printf("# cost of the optimal solution = %f\n", graph_get_cost(&H));

	graph_plot(&H, &EG, "OPT TOUR");

      }
        break;

  }

/*
  printf("Optimal cost: %f\n", incumbent);
  printf("Quality of bounds:\n");
  printf("- upper bound/opt : %f\n", heuristic_upper_bound / incumbent);
  printf("- lagr. lower bound : %f\n", lagr_lower_bound / incumbent);
*/

  return 0;
}

