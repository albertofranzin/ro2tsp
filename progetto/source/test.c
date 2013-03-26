#include "node.h"
#include "edge.h"
#include "graph.h"
#include "compute_path.h"
#include "compute_mst.h"
#include "compute_ot.h"
#include "solve_tsp.h"
#include "utils.h"

#define BIG 1000
#define SMALL -1000

void main() {

  parameters *pars = getParameters();
  printf("seed used : %d\n", initializeRandom(pars->seed));

  int i, j;
  graph G, F, H;
  clock_t c1, c2;
  initGraph(&G, pars->no_of_nodes);
  initGraph(&F, 1);
  initGraph(&H, 1);
  randomGraph(&G);

  double incumbent = BIG;
  copyGraph(&G, &F);
  solve_tsp(&G, &H, &incumbent, 0);
  plotGraph(&G, &H, "default", NULL); 

  deleteGraph(&G);
  deleteGraph(&F);
  deleteGraph(&H);
}
