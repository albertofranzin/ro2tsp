#include "base/constants.h"
#include "base/base.h"
#include "base/utils.h"

#include "algos/preprocessing.h"

#include "data/graph.h"
#include "data/egraph.h"
#include "data/tsp_env.h"
#include "data/tsp_stats.h"

#include "solvers/cpx/cpx_solver.h"
#include "solvers/cpx/cpx_local_branching.h"
#include "solvers/cpx/cpx_mip_refine.h"
#include "solvers/bb/bb_solver.h"

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


  tsp_env te;
  tsp_env_init(&te);
  graph_copy(&G, &te.G_INPUT);
  egraph_copy(&EG, &te.EG_INPUT);

  tsp_stats ts;
  tsp_stats_init(&ts);

  start = clock();
  preprocessing(&G, pars, &te, &ts);
  end = clock();
  double preprocessing_time = time_elapsed(start, end);

  switch(pars->solver) {

    case BRANCH_AND_BOUND :
      {
        start = clock();
        bb_solver(&te, &ts);
        end = clock();
      }
        break;

    case CPLEX :
      {
        start = clock();
        cpx_solver(&te, &ts, pars);
        end = clock();
      }
        break;

    case LOCAL_BRANCHING :
      {
        start = clock();
        cpx_local_branching(&te, &ts, pars);
        end = clock();
      }
        break;

    case MIP_AND_REFINE :
      {
        start = clock();
        cpx_mip_refine(&te, &ts, pars);
        end = clock();
      }
        break;

    default :
      fprintf(stderr, "Fatal error in main :: no such solver available\n");
      exit(1);

  }

  tsp_stats_print(&ts, pars);
  graph_plot(&te.G_OUTPUT, &EG, "optimal tour");

  printf("time spent in preprocessing: %f s\n", preprocessing_time);
  printf("time spent in actual solving: %f s\n", time_elapsed(start, end));

  return 0;
}

