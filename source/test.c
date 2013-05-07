#include "base/base.h"
#include "solvers/bb/bb_solver.h"
#include "solvers/cpx/cpx_solver.h"
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
  printf("creating a cycle\n");
  cycle C;
  cycle_init(&C, 1);
  printf("cycle created\n");

  double heuristic_upper_bound;
  heuristic_upper_bound = compute_upper_bound(&G, &C, pars->heuristic_algo);
  printf("@ Nearest Neighbour Heuristic\n# upper bound = %f\n", heuristic_upper_bound);
  */

  switch(pars->solver) {

    case BRANCH_AND_BOUND :
      {
        onetree NN;
	onetree_init(&NN, pars->number_of_nodes);
	double heuristic_upper_bound = compute_upper_bound(&G, &NN, pars->heuristic_algo);

	bb_input input;
	bb_input_init(&input, pars->number_of_nodes);
	graph_copy(&G, &input.G);
	onetree_copy(&NN, &input.H);
	input.ub = heuristic_upper_bound + EPSILON;
	bb_output output;
	bb_output_init(&output, pars->number_of_nodes);
	bb_status status;
	bb_status_init(&status, pars->number_of_nodes);
	bb_stats stats;
	bb_stats_init(&stats, pars->number_of_nodes);

	bb_solver(&input, &output, &status, &stats);

	egraph EG_BB;
	egraph_init(&EG_BB, 1);
	egraph_copy(&EG, &EG_BB);
	onetree_to_egraph(&output.H_opt, &EG_BB);
	egraph_plot(&EG, &EG_BB);
      }
        break;

    case CPLEX :
      {
	graph H;
	graph_init(&H, 1);

	cpx_solver(&G, &H);

	egraph EG_CPX;
	egraph_init(&EG_CPX, 1);
	egraph_copy(&EG, &EG_CPX);
	graph_to_egraph(&H, &EG_CPX);
	egraph_plot(&EG, &EG_CPX);
      }
        break;

  }



  return 0;
}

