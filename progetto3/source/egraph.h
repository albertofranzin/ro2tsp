#ifndef EGRAPH_H_
#define EGRAPH_H_

#include <stdio.h>
#include "graph.h"
#include "tree.h"
#include "onetree.h"

struct egraph_node {
  double x; // coordinata asse x;
  double y; // coordinata asse y;
  int deg; // grado del nodo (numero di lati incidenti nel nodo);
};

typedef struct egraph_node egraph_node;

struct egraph_edge {
  int flag; // flag = 1 se il lato è presente, flag = 0 altrimenti;
  double cost; // costo del lato;
};

typedef struct egraph_edge egraph_edge;

struct egraph {
  int n; // numero nodi del grafo;
  egraph_node* V; // nodi del grafo;
  egraph_edge* E; // lati del grafo;
};

typedef struct egraph egraph;

void egraph_init(egraph* EG, int n);
void egraph_delete(egraph* EG);
void egraph_random(egraph* EG);
void egraph_copy(egraph* FROM, egraph* TO);

void egraph_set_node_x(egraph* EG, int v, double x);
void egraph_set_node_y(egraph* EG, int v, double y);
double egraph_get_node_x(egraph* EG, int v);
double egraph_get_node_y(egraph* EG, int v);

void egraph_insert_edge(egraph* EG, int u, int v, double cost);
void egraph_remove_edge(egraph* EG, int u, int v);
void egraph_set_edge_cost(egraph* EG, int u, int v, double cost);
double egraph_get_edge_cost(egraph* EG, int u, int v);

int egraph_get_node_deg(egraph* EG, int v);
int egraph_adjacent_nodes(egraph* EG, int u, int v);

double egraph_get_cost(egraph* EG);

void egraph_plot(egraph* EG1, egraph* EG2);

void graph_to_egraph(graph* G, egraph* EG);
void egraph_to_graph(egraph* EG, graph* G);
void tree_to_egraph(tree* T, egraph* EG);
void onetree_to_egraph(onetree* OT, egraph* EG);

void egraph_print(egraph *EG);

#endif
