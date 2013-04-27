#include "graph.h"
#include "egraph.h"
#include "list.h"
#include "tree.h"
#include "onetree.h"
#include "compute_mst.h"
#include "compute_ot.h"
#include "compute_lagrange.h"

void main() {
  int n = 40; // numero di nodi
  int s = 800; // seed

  egraph EG;
  egraph_init(&EG, n);
  srand(s);
  egraph_random(&EG);
  //egraph_plot(&EG, NULL);

  graph G;
  graph_init(&G, 1);
  egraph_to_graph(&EG, &G);

  onetree OT_LGR;
  onetree_init(&OT_LGR, n);

  double lb = compute_lagrange(&G, &OT_LGR, 10);

  printf("lower bound = %f\n", lb);

  egraph EG_LGR;
  egraph_init(&EG_LGR, 1);
  egraph_copy(&EG, &EG_LGR);
  onetree_to_egraph(&OT_LGR, &EG_LGR);

  egraph_plot(&EG, &EG_LGR);

  onetree_delete(&OT_LGR);
  graph_delete(&G);
  egraph_delete(&EG);
  egraph_delete(&EG_LGR);

}

