#ifndef EGRAPH_H_
#define EGRAPH_H_



#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../base/constants.h"
#include "graph.h"
#include "cycle.h"
#include "tree.h"
#include "onetree.h"
#include "egraph.h"



typedef struct _egraph_node {

  double x;
  double y;
  int deg;

} egraph_node;



typedef struct _egraph_edge {

  int flag; 
  double cost;

} egraph_edge;



typedef struct _egraph {

  int n; 
  egraph_node* V;
  egraph_edge* E;

  double max_x;
  double min_x;
  double max_y;
  double min_y;

} egraph;



void egraph_init(egraph* EG, int n);



void egraph_delete(egraph* EG);



void egraph_copy(egraph* FROM, egraph* TO);



void egraph_random(egraph* EG);



void egraph_insert_edge(egraph* EG, int u, int v, double cost);



void egraph_remove_edge(egraph* EG, int u, int v);



void egraph_set_edge_cost(egraph* EG, int u, int v, double cost);



double egraph_get_edge_cost(egraph* EG, int u, int v);



int egraph_adjacent_nodes(egraph* EG, int u, int v);



double egraph_get_cost(egraph* EG);








void egraph_plot(egraph* EG1, egraph* EG2, char* title);



void egraph_to_graph(egraph* EG, graph* G);



void graph_to_egraph(graph* G, egraph* EG);



void tree_to_egraph(tree* T, egraph* EG);



void cycle_to_egraph(cycle* C, egraph* EG);



void onetree_to_egraph(onetree* OT, egraph* EG);



void egraph_print(egraph *EG);



void graph_plot(graph* G, egraph* EG, char* title);



void tree_plot(tree* T, egraph* EG, char* title);



void onetree_plot(onetree* OT, egraph* EG, char* title);



void cycle_plot(cycle* C, egraph* EG, char* title);


#endif
