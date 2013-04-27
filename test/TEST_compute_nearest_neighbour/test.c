#include <stdio.h>
#include "graph.h"
#include "egraph.h"
#include "tree.h"
#include "onetree.h"
#include "compute_nearest_neighbour.h"

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

  onetree OT_NN;
  onetree_init(&OT_NN, n);

  double nn = compute_nearest_neighbour(&G, &OT_NN, 20);
  printf("nn cost = %f\n", nn);

  egraph EG_NN;
  egraph_init(&EG_NN, 1);
  egraph_copy(&EG, &EG_NN);
  onetree_to_egraph(&OT_NN, &EG_NN);

  egraph_plot(&EG, &EG_NN);

  onetree_delete(&OT_NN);
  graph_delete(&G);
  egraph_delete(&EG);
  egraph_delete(&EG_NN);

}

