#include "graph.h"
#include "egraph.h"
#include "list.h"
#include "tree.h"
#include "compute_mst.h"

void main() {
  int n = 40; // numero di nodi
  int s = 1321; // seed
  int r = 1; // radice

  egraph EG;
  egraph_init(&EG, n);
  srand(s);
  egraph_random(&EG);
  //egraph_plot(&EG, NULL);

  graph G;
  graph_init(&G, 1);
  egraph_to_graph(&EG, &G);

  tree T;
  tree_init(&T, 1, 1);

  compute_mst(&G, &T, r);

  egraph EGT;
  egraph_init(&EGT, 1);
  egraph_copy(&EG, &EGT);
  tree_to_egraph(&T, &EGT);

  egraph_plot(&EG, &EGT);

  tree_delete(&T);
  graph_delete(&G);
  egraph_delete(&EG);
  egraph_delete(&EGT);

}

