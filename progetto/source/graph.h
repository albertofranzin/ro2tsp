#ifndef GRAPH_H_
#define GRAPH_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "node.h"
#include "edge.h"

struct graph {
  int n;
  node* V;
  edge* E;
};

typedef struct graph graph;

void initGraph(graph* G, int n);
void deleteGraph(graph* G);
void randomGraph(graph* G);
void plotGraph(graph* G1, graph* G2, char* opt1, char* opt2);
void copyGraph(graph* FROM, graph* TO);

void insert_edge(graph* G, int u, int v);
void remove_edge(graph* G, int u, int v);
void set_node_x(graph* G, int v, double x);
void set_node_y(graph* G, int v, double y);
double get_node_x(graph* G, int v);
double get_node_y(graph* G, int v);
int get_node_deg(graph* G, int v);
void set_edge_cost(graph* G, int u, int v, double cost);
double get_edge_cost(graph* G, int u, int v);
int adjacent(graph* G, int u, int v);
double get_graph_cost(graph* G);

#endif
