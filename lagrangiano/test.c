#include "node.h"
#include "edge.h"
#include "graph.h"
#include "compute_path.h"
#include "compute_mst.h"
#include "compute_ot.h"
#include "heuristics.h"
#include "solve_tsp.h"
#include "compute_lagrange.h"
#include "utils.h"

#define BIG 1000
#define SMALL -1000

void main() {
  int i, j;
  graph G;

  /* ================================== */
  /* Costruisci G grafo euclideo random */
  /* ================================== */

  graph NN;
  parameters *pars = getParameters();
  printf("seed used : %lu\n", initializeRandom(pars->seed));

  initGraph(&G, pars->no_of_nodes);
  initGraph(&NN, 1);
  randomGraph(&G);
  free(pars);

  /* ============================================================================== */
  /* Calcola con algoritmo euristico upper bound per ciclo Hamiltoniano ottimo di G */
  /* ============================================================================== */

  initGraph(&NN, 1);
  double incumbent = nearestNeighbour(&G, &NN);
  deleteGraph(&NN);

  /* ========================================================== */
  /* Calcola con branch&bound un ciclo Hamiltoniano ottimo di G */
  /* ========================================================== */

  graph F, H;
  initGraph(&F, 1);
  initGraph(&H, 1);

  //solve_tsp(&G, &H, &incumbent, 0);
  //plotGraph(&G, &H, "default", NULL);

  deleteGraph(&F);
  deleteGraph(&H);

  /* ==================================================================================================================== */
  /* Calcola con algoritmo subgradiente e rilassamento lagrangiano un buon lower bound per ciclo Hamiltoniano ottimo di G */
  /* ==================================================================================================================== */

  double ub = incumbent;
  double alpha = 1;
  double max_iter = 2200;
  double ub_max_iter = 2200;
  double alpha_max_iter = 100;
  double L_best;

  //L_best = compute_lagrange(&G, ub, ub_max_iter, alpha, alpha_max_iter, max_iter);
  L_best = compute_and_plot_lagrange(&G, ub, ub_max_iter, alpha, alpha_max_iter, max_iter);
 
//	solve_tsp(&G, &H, &incumbent, 0);

  deleteGraph(&G);;
}
