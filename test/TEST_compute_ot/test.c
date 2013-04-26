#include "graph.h"
#include "egraph.h"
#include "list.h"
#include "tree.h"
#include "onetree.h"
#include "compute_mst.h"
#include "compute_ot.h"

void main() {
  int n = 30; // numero di nodi
  int s = 800; // seed

  egraph EG;
  egraph_init(&EG, n);
  srand(s);
  egraph_random(&EG);
  //egraph_plot(&EG, NULL);

  graph G;
  graph_init(&G, 1);
  egraph_to_graph(&EG, &G);

  onetree OT;
  onetree_init(&OT, n);

  compute_ot(&G, &OT);

  egraph EGOT;
  egraph_init(&EGOT, 1);
  egraph_copy(&EG, &EGOT);
  onetree_to_egraph(&OT, &EGOT);

  egraph_plot(&EG, &EGOT);

  onetree_delete(&OT);
  graph_delete(&G);
  egraph_delete(&EG);
  egraph_delete(&EGOT);

}

