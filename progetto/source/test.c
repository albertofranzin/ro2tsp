#define BIG 1000
#define SMALL -1000

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
  initGraph(&G, 19);
  initGraph(&F, 1);
  initGraph(&H, 1);
  randomGraph(&G, 321);

  double incumbent = BIG;
  copyGraph(&G, &F);
  solve_tsp(&G, &H, &incumbent, 0);
  plotGraph(&G, &H, "default", NULL); 

  deleteGraph(&G);
  deleteGraph(&F);
  deleteGraph(&H);
}
