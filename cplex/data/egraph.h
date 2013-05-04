#ifndef EGRAPH_H_
#define EGRAPH_H_

#include "../base/constants.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
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
  // coords:
  double max_x;
  double min_x;
  double max_y;
  double min_y;
};

typedef struct egraph egraph;

void egraph_init(egraph* EG, int n);
void egraph_delete(egraph* EG);
void egraph_random(egraph* EG);
void egraph_copy(egraph* FROM, egraph* TO);

inline void egraph_set_node_x(egraph* EG, int v, double x);
inline void egraph_set_node_y(egraph* EG, int v, double y);
inline double egraph_get_node_x(egraph* EG, int v);
inline double egraph_get_node_y(egraph* EG, int v);

inline void egraph_insert_edge(egraph* EG, int u, int v, double cost);
inline void egraph_remove_edge(egraph* EG, int u, int v);
inline void egraph_set_edge_cost(egraph* EG, int u, int v, double cost);
inline double egraph_get_edge_cost(egraph* EG, int u, int v);

inline int egraph_get_node_deg(egraph* EG, int v);
inline int egraph_adjacent_nodes(egraph* EG, int u, int v);

double egraph_get_cost(egraph* EG);

void egraph_plot(egraph* EG1, egraph* EG2);

/*
 * print egraph as (diagonal) matrix of costs
 */
void egraph_print(egraph *EG);

void graph_to_egraph(graph* G, egraph* EG);
void egraph_to_graph(egraph* EG, graph* G);
void tree_to_egraph(tree* T, egraph* EG);
void onetree_to_egraph(onetree* OT, egraph* EG);

#endif
