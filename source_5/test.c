#include "base/constants.h"
#include "base/base.h"

#include "algos/preprocessing.h"

#include "data/graph.h"
#include "data/egraph.h"

#include "data/tsp_env.h"
#include "data/tsp_stats.h"
#include "solvers/bb/bb_solver.h"

#include "solvers/cpx/cpx_env.h"
#include "solvers/cpx/cpx_stats.h"
#include "solvers/cpx/cpx_solver.h"

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
  } else if (pars->random_instance_option == TRUE) {
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

  //preprocessing(&G, pars, &te, &ts);

  switch(pars->solver) {

    case BRANCH_AND_BOUND :
      {

        start = clock();

        bb_solver(&te, &ts);

        end = clock();

        tsp_stats_print(&ts, pars);

        graph_plot(&te.G_OUTPUT, &EG, "optimal tour");

      }
        break;

    case CPLEX :
      {

        cpx_env ce;
        cpx_stats cs;
        cpx_env_init(&ce, pars);
        cpx_stats_init(&cs);
        egraph_copy(&EG, &ce.EG_INPUT);
        graph_copy(&G, &ce.G_INPUT);

        start = clock();

        cpx_solver(&ce, &cs, pars);

        end = clock();

        cpx_stats_print(&cs);

        graph_plot(&ce.G_OUTPUT, &EG, "optimal tour");
  
      }
        break;
  
  }

  printf("time spent in actual solving: %f s\n",
        ((double) (end - start)) / CLOCKS_PER_SEC);

  return 0;
}

