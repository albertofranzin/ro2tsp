#ifndef __TYPE_GRAPH_H
#define __TYPE_GRAPH_H

// nodo del grafo
struct graph_node {
  int deg; // grado del nodo
  int deg_forced;
  int deg_forbidden;
};

typedef struct graph_node graph_node;

// lato del grafo
struct graph_edge {
  int flag; // flag=1 se il lato è presente, flag=0 altrimenti
  double cost; // costo del lato
  int constr; // vincolo sul lato
  double delta; // See Helsgaun computation of alpha-nearness
};

typedef struct graph_edge graph_edge;

// grafo
typedef struct _graph {
  int n; // numero di nodi del grafo;
  graph_node* V; // array di nodi del grafo
  graph_edge* E; // array di lati del grafo
} graph;

#endif
