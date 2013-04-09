#include "node.h"
#include "edge.h"
#include "graph.h"
#include "compute_lagrange.h"
#include "compute_path.h"
#include "compute_mst.h"
#include "compute_ot.h"
#include "heuristics.h"
#include "solve_tsp.h"
#include "utils.h"
#include "subgradient.h"

#define BIG 100000
#define SMALL -100000

void main() {

  parameters *pars = getParameters();
  printf("## %d\n", pars->no_of_nodes);
  printf("## %s\n", pars->tsp_file);
  printf("seed used : %lu\n", initializeRandom(pars->seed));

  int i, j;
  graph G, F, H, NN;
  clock_t c1, c2;
  initGraph(&G, pars->no_of_nodes);
  initGraph(&F, 1);
  initGraph(&H, 1);
  initGraph(&NN, 1);
  //randomGraph(&G);
  if (pars->tsp_file != NULL) {
    printf("using tsplib file %s\n", pars->tsp_file);
    read_tsp_from_file(&G, pars);
  } else {
    randomGraph(&G);
  }
  // print_graph(&G);

  copyGraph(&G, &F);
  double incumbent = heuristicBound(&G, &NN, pars->heuristic_trials);
  printf("starting incumbent is %f\n", incumbent);
  //printf("should be %f\n", get_graph_cost(&NN));
  //print_graph(&G);
  //print_graph(&NN);
  deleteGraph(&NN);

  //incumbent = 4.31;
  double ub = incumbent;
  double alpha = 2.0;
  double max_iter = 30000;
  double ub_max_iter = 20000;
  double alpha_max_iter = 200;
  double L_best;

  //incumbent = 4.31;

  //L_best = compute_and_plot_lagrange(&G, ub, ub_max_iter, alpha, alpha_max_iter, max_iter);
  L_best = compute_and_plot_lagrange(&F, ub, ub_max_iter, alpha, alpha_max_iter, max_iter);

  incumbent = L_best * 1.02;
  ub = incumbent;

  printf("lower bound : %.20lf, upper bound : %.20lf\n", L_best, incumbent);

  if (!is_cycle(&F)) {
    //compute_ot(&G, &F);
    solve_tsp(&G, &F, &H, &incumbent, 0);
    plotGraph(&G, &H, "default", NULL);
    printf("ratio : %f\n", L_best / incumbent);
  } else {
    printf("COOL! Lagrangean relaxation solved the problem alone!\n");
    plotGraph(&G, &F, "default", NULL);
  }

  //double *vals = subgradient(&G, lag);
  //printf("exited\n");

  free(pars);
  deleteGraph(&G);
  deleteGraph(&F);
  deleteGraph(&H);
}
