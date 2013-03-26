#define INF 1000

#include <time.h>

#include "node.h"
#include "edge.h"
#include "graph.h"
#include "compute_path.h"
#include "compute_mst.h"
#include "compute_ot.h"
#include "solve_tsp.h"

#include "graph.c"
#include "compute_path.c"
#include "compute_mst.c"
#include "compute_ot.c"
#include "solve_tsp.c"

void main() {
  int i, j;
  graph G, F, H;
  clock_t c1, c2;
  initGraph(&G, 19);
  initGraph(&F, 1);
  initGraph(&H, 1);
  randomGraph(&G, 321);

  double incumbent = INF;
  copyGraph(&G, &F);

  c1 = clock();

  solve_tsp(&G, &H, &incumbent, 0);

  c2 = clock();

  printf("exec time = %f\n", ((double)c2 / CLOCKS_PER_SEC) - ((double)c1 / CLOCKS_PER_SEC));

  plotGraph(&G, &H, "default", NULL); 

  deleteGraph(&G);
  deleteGraph(&F);
  deleteGraph(&H);
}
