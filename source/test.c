#include "base/constants.h"
#include "base/base.h"

#include "algos/preprocessing.h"

#include "data/graph.h"
#include "data/egraph.h"
#include "data/tsp_env.h"
#include "data/tsp_stats.h"

#include "solvers/cpx/cpx_solver.h"
#include "solvers/bb/bb_solver.h"
//#include "solvers/bb/bb_env.h"
//#include "solvers/bb/bb_stats.h"

#include <stdio.h>
#include <time.h>

int main (int argc, char *argv[]) {

  clock_t start = 0, end = 0;

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
    printf("@ Euclidean TSP\n# random instance\n# number of nodes = %d\n# seed = %ld\n\n",
                pars->number_of_nodes, pars->seed);
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

  tsp_env te;
  tsp_env_init(&te);
  graph_copy(&G, &te.G_INPUT);
  egraph_copy(&EG, &te.EG_INPUT);

  tsp_stats ts;
  tsp_stats_init(&ts);

  preprocessing(&G, pars, &te, &ts);

  switch(pars->solver) {

    case BRANCH_AND_BOUND :
      {

        /*bb_env env;
        bb_env_init(&env);

        graph_copy(&G, &env.G_INPUT);
        egraph_copy(&EG, &env.EG_INPUT);

        bb_stats stats;
        bb_stats_init(&stats);*/


        // bb_solver(&env, &stats);

        start = clock();

        bb_solver(&te, &ts);

        end = clock();

        printf("--------------------------------------------------------------\n");
        printf("# cost of the optimal solution = %f\n", te.z_opt);
        tsp_stats_print(&ts);

        cycle_plot(&te.TOUR_OPT, &EG, "OPT TOUR");

        printf("# quality of bounds:\n");
        printf("- best heur. upper bound  = %f\n",
                  ts.init_ub / te.z_opt );
        printf("- lagr. lower bound       = %f\n",
                  ts.init_lb / te.z_opt);

      }
        break;

    case CPLEX :
      {

      graph H;
      graph_init(&H, 1);

      printf("upper bound: %f\n", te.input_ub);

      start = clock();

      cpx_solver(&te, &ts, pars);

      end = clock();

      printf("# cost of the optimal solution = %f\n", graph_get_cost(&H));
      tsp_stats_print(&ts);

      graph_plot(&te.G_OUTPUT, &EG, "OPT TOUR");

      printf("# quality of bounds:\n");
      printf("- best heur. upper bound  = %f\n",
                ts.init_ub / te.z_opt );
      printf("- lagr. lower bound       = %f\n",
                ts.init_lb / te.z_opt);

      }
        break;

  }

  printf("time spent in actual solving: %f s\n",
        ((double) (end - start)) / CLOCKS_PER_SEC);

/*
  printf("Optimal cost: %f\n", incumbent);
  printf("Quality of bounds:\n");
  printf("- upper bound/opt : %f\n", heuristic_upper_bound / incumbent);
  printf("- lagr. lower bound : %f\n", lagr_lower_bound / incumbent);
*/

  return 0;
}

