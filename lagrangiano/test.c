#include "node.h"
#include "edge.h"
#include "graph.h"
#include "compute_path.h"
#include "compute_mst.h"
#include "compute_ot.h"
#include "heuristics.h"
#include "solve_tsp.h"
#include "lagrange.h"
#include "utils.h"

#define BIG 1000
#define SMALL -1000

void main() {

  parameters *pars = getParameters();
  printf("seed used : %lu\n", initializeRandom(pars->seed));

  int i, j;
  graph G, F, H, NN;
  initGraph(&G, pars->no_of_nodes);
  initGraph(&F, 1);
  initGraph(&H, 1);
  initGraph(&NN, 1);
  randomGraph(&G);

  double incumbent = nearestNeighbour(&G, &NN);
  deleteGraph(&NN);

  copyGraph(&G, &F);
  //double ub = incumbent;
  double ub = 13;
  double alpha = 0.9;
  double K = 1000;
  double L;

  //L = lagrange(&F, &H, K, ub, alpha); 
  L = plot_lagrange(&F, &H, K, ub, alpha); 

  printf("best tsp lower bound (lagrange): %f\n", L);
  //plotGraph(&G, &H, "onetree", NULL);

  //printf("should be %f\n", get_graph_cost(&NN));
  //print_graph(&G);
  //print_graph(&NN);

  //solve_tsp(&G, &H, &incumbent, 0);
  //plotGraph(&G, &H, "default", NULL); 

  free(pars);
  deleteGraph(&G);
  deleteGraph(&F);
  deleteGraph(&H);
}
