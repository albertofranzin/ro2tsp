#ifndef TREE_H_
#define TREE_H_

#include <stdio.h>

struct tree_node {
  int pred; // indice nodo predecessore
  int deg; // grado del nodo (numero di lati incidenti nel nodo);
  double cost; // costo del lato dal nodo al suo predecessore
};

typedef struct tree_node tree_node;

struct tree {
  int n; // numero nodi dell'albero;
  int root; // radice dell'albero;
  tree_node* V; // nodi dell'albero;
};

typedef struct tree tree;

void tree_init(tree* T, int n);
void tree_delete(tree* T);
void tree_copy(tree* FROM, tree* TO);

void tree_set_root(tree* T, int r);
int tree_get_root(tree* T);
int tree_get_pred(tree* T, int v);

void tree_insert_edge(tree* T, int u, int v, double cost);
void tree_remove_edge(tree* T, int u, int v);
void tree_set_edge_cost(tree* T, int u, int v, double cost);
double tree_get_edge_cost(tree* T, int u, int v);

int tree_get_node_deg(tree* T, int v);
int tree_adjacent_nodes(tree* T, int u, int v);

double tree_get_cost(tree* T);

#endif
