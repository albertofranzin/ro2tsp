#ifndef ONETREE_H_
#define ONETREE_H_



#include <stdlib.h>
#include <stdio.h>

#include "../base/constants.h"
#include "graph.h"
#include "tree.h"
#include "cycle.h"



typedef struct _onetree_node {

  int pred;
  int deg;

} onetree_node;



typedef struct _onetree {

  int n;
  onetree_node* V;
  int nn1;
  int nn2;

} onetree;



void onetree_init(onetree* OT, int n);



void onetree_delete(onetree* OT);



void onetree_copy(onetree* FROM, onetree* TO);



void onetree_insert_edge(onetree* OT, int u, int v);



void onetree_remove_edge(onetree* OT, int u, int v);



int onetree_adjacent_nodes(onetree* OT, int u, int v);



int onetree_is_cycle(onetree* OT);



void cycle_to_onetree(cycle* C, onetree* OT);



double onetree_get_cost(onetree* OT, graph* G);



#endif
