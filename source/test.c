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

  /**/
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
  /**/

  double incumbent = heuristic_upper_bound;

  switch(pars->solver) {

    case BRANCH_AND_BOUND :
      {
        onetree NN;
        onetree_init(&NN, pars->number_of_nodes);
        //heuristic_upper_bound = compute_upper_bound(&G, &C, pars->heuristic_algo);

        lagr_lower_bound = compute_lagrange(&G, &NN, heuristic_upper_bound);


        if (onetree_is_cycle(&NN)) {
          printf("got the best\n");
          printf("cost = %f\n", lagr_lower_bound);

          egraph EG_BB;
          egraph_init(&EG_BB, 1);
          egraph_copy(&EG, &EG_BB);
          onetree_to_egraph(&NN, &EG_BB);
          egraph_plot(&EG, &EG_BB);
        } else {
          bb_input input;
          bb_input_init(&input, pars->number_of_nodes);
          graph_copy(&G, &input.G);
          onetree_copy(&NN, &input.H);
          input.ub = heuristic_upper_bound;// + EPSILON;
          input.lb = lagr_lower_bound;
          printf("starting ub = %f\n", input.ub);
          bb_output output;
          bb_output_init(&output, pars->number_of_nodes);
          bb_status status;
          bb_status_init(&status, pars->number_of_nodes);
          bb_stats stats;
          bb_stats_init(&stats, pars->number_of_nodes);

          bb_solver(&input, &output, &status, &stats);

          printf("# cost of optimal solution = %f\n", output.z_opt);

#ifdef DEBUG
          printf("pruned branches: %d\n", stats.pruned_branches);
          printf("took branches these times: %d %d %d %d %d\n",
              stats.took1, stats.took2, stats.took3, stats.took4, stats.took5);
#endif
          egraph EG_BB;
          egraph_init(&EG_BB, 1);
          egraph_copy(&EG, &EG_BB);
          onetree_to_egraph(&output.H_opt, &EG_BB);
          egraph_plot(&EG, &EG_BB);
        }
      }
        break;

    case CPLEX :
      {
      graph H;
      graph_init(&H, 1);

      cpx_solver(&G, &H);

      printf("# cost of optimal solution = %f\n", graph_get_cost(&H));

      egraph EG_CPX;
      egraph_init(&EG_CPX, 1);
      egraph_copy(&EG, &EG_CPX);
      graph_to_egraph(&H, &EG_CPX);
      egraph_plot(&EG, &EG_CPX);
      }
        break;

  }

  printf("Optimal cost: %f\n", incumbent);
  printf("Quality of bounds:\n");
  printf("- upper bound/opt : %f\n", heuristic_upper_bound / incumbent);
  printf("- lagr. lower bound : %f\n", lagr_lower_bound / incumbent);

  return 0;
}

