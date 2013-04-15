#ifndef GRAPH_H_
#define GRAPH_H_

#include <stdlib.h>
#include <string.h>
#include <math.h>

struct graph_node {
  int deg; // grado del nodo (numero di lati incidenti nel nodo);
};

typedef struct graph_node graph_node;

struct graph_edge {
  int flag; // flag = 1 se il lato è presente, flag = 0 altrimenti;
  double cost; // costo del lato;
};

typedef struct graph_edge graph_edge;

struct graph {
  int n; // numero nodi del grafo;
  graph_node* V; // nodi del grafo;
  graph_edge* E; // lati del grafo;
};

typedef struct graph graph;

void graph_init(graph* G, int n);
void graph_delete(graph* G);
void graph_copy(graph* FROM, graph* TO);

void graph_insert_edge(graph* G, int u, int v, double cost);
void graph_remove_edge(graph* G, int u, int v);
void graph_set_edge_cost(graph* G, int u, int v, double cost);
double graph_get_edge_cost(graph* G, int u, int v);

int graph_get_node_deg(graph* G, int v);
int graph_adjacent_nodes(graph* G, int u, int v);

double graph_get_cost(graph* G);

#endif
