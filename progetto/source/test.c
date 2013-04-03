#include "node.h"
#include "edge.h"
#include "graph.h"
#include "compute_path.h"
#include "compute_mst.h"
#include "compute_ot.h"
#include "heuristics.h"
#include "lagrangian.h"
#include "solve_tsp.h"
#include "utils.h"
#include "subgradient.h"

#define BIG 1000
#define SMALL -1000

void main() {

  parameters *pars = getParameters();
  printf("seed used : %lu\n", initializeRandom(pars->seed));

  int i, j;
  graph G, F, H, NN;
  clock_t c1, c2;
  initGraph(&G, pars->no_of_nodes);
  initGraph(&F, 1);
  initGraph(&H, 1);
  initGraph(&NN, 1);
  randomGraph(&G);

  copyGraph(&G, &F);
  double incumbent = heuristicBound(&G, &NN, pars->heuristic_trials);
  printf("starting incumbent is %f\n", incumbent);
  //printf("should be %f\n", get_graph_cost(&NN));
  //print_graph(&G);
  //print_graph(&NN);
  deleteGraph(&NN);

  lagrangian *lag = initLagrange(pars->no_of_nodes);

  //solve_tsp(&G, &H, &lag, &incumbent, 0);
  //solve_tsp(&G, &H, &incumbent, 0);
  //plotGraph(&G, &H, "default", NULL);

  double *vals = subgradient(&G, lag);
  printf("exited\n");

  /*free(pars);
  deleteGraph(&G);
  deleteGraph(&F);
  deleteGraph(&H);*/
}
