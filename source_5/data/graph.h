#ifndef GRAPH_H_
#define GRAPH_H_


#include <stdlib.h>
#include <stdio.h>
#include "../base/constants.h"


typedef struct _graph_node {

  int deg;
  int deg_frc;
  int deg_frb;

} graph_node;



typedef struct _graph_edge {

  int flag; 
  double cost;
  int constr;
  double delta;

} graph_edge;



typedef struct _graph {

  int n;
  graph_node* V;
  graph_edge* E;

} graph;



void graph_init(graph* G, int n);



void graph_delete(graph* G);



void graph_copy(graph* FROM, graph* TO);



void graph_insert_edge(graph* G, int u, int v, double cost, int constr);



void graph_remove_edge(graph* G, int u, int v);



void graph_set_edge_cost(graph* G, int u, int v, double cost);



double graph_get_edge_cost(graph* G, int u, int v);



void graph_set_edge_constr(graph* G, int u, int v, int constr);



int graph_get_edge_constr(graph* G, int u, int v);



void graph_set_edge_delta(graph* G, int u, int v, double delta);



double graph_get_edge_delta(graph* G, int u, int v);



int graph_adjacent_nodes(graph* G, int u, int v);



double graph_get_cost(graph* G);



#endif
