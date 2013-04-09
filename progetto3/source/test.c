#include <stdlib.h>
#include <stdio.h>
#include "graph.h"
#include "egraph.h"
#include "tree.h"
#include "onetree.h"
#include "compute_mst.h"
#include "compute_ot.h"

void main() {
  /*
  graph G;
  egraph EG1;
  egraph EG2;
  tree T;
  initTree(&T, 10);

  initGraph(&G, 1);
  initEgraph(&EG1, 10);
  initEgraph(&EG2, 10);

  randomEgraph(&EG1);
  copyEgraph(&EG1, &EG2);

  egraph_to_graph(&EG1, &G);

  compute_mst(&G, &T, 1);
  tree_to_egraph(&T, &EG2);

  //graph_to_egraph(&G, &EG2); 

  plotEgraph(&EG1, &EG2);
  */

  /***********************************/
  /*
  setRootTree(&T, 1);
  insertEdgeTree(&T, 1, 3);
  setEdgeCostTree(&T, 1, 3, 14);
  insertEdgeTree(&T, 3, 8);
  setEdgeCostTree(&T, 1, 3, 14);
  insertEdgeTree(&T, 1, 2);
  setEdgeCostTree(&T, 1, 3, 14);
  insertEdgeTree(&T, 2, 9);
  setEdgeCostTree(&T, 1, 3, 14);
  insertEdgeTree(&T, 9, 4);
  setEdgeCostTree(&T, 1, 3, 14);
  insertEdgeTree(&T, 4, 10);
  setEdgeCostTree(&T, 1, 3, 14);
  insertEdgeTree(&T, 4, 5);
  setEdgeCostTree(&T, 1, 3, 14);
  insertEdgeTree(&T, 5, 7);
  setEdgeCostTree(&T, 1, 3, 14);
  insertEdgeTree(&T, 1, 6);
  setEdgeCostTree(&T, 1, 3, 14);

  tree_to_egraph(&T, &EG2);
  plotEgraph(&EG1, &EG2);
  
  */
  
  /*
  graph G;
  tree T;
  egraph EG1;
  egraph EG2;

  initGraph(&G, 1);
  initTree(&T, 1);
  initEgraph(&EG1, 20);
  initEgraph(&EG2, 20);

  randomEgraph(&EG1);
  int i, j;
  for (i = 1; i <= 20; i++) {
    for (j = i+1; j <= 20; j++) {
      if ((i <= 10 && j > 10) || (i > 10 && j <= 10))
	removeEdgeEgraph(&EG1, i, j);
    }
  }


  copyEgraph(&EG1, &EG2);

  egraph_to_graph(&EG1, &G);
  compute_mst(&G, &T, 1);

  tree_to_egraph(&T, &EG2);
  plotEgraph(&EG1, &EG2);

  deleteGraph(&G);
  deleteTree(&T);  
  deleteEgraph(&EG1);
  deleteEgraph(&EG2);
  */






  graph G;
  onetree OT;
  egraph EG1;
  egraph EG2;

  graph_init(&G, 1);
  onetree_init(&OT, 1);
  egraph_init(&EG1, 20);
  egraph_init(&EG2, 20);

  egraph_random(&EG1);
  egraph_copy(&EG1, &EG2);

  egraph_to_graph(&EG1, &G);
  compute_ot(&G, &OT);

  onetree_to_egraph(&OT, &EG2);
  egraph_plot(&EG1, &EG2);

  graph_delete(&G);
  onetree_delete(&OT);  
  egraph_delete(&EG1);
  egraph_delete(&EG2);
}
