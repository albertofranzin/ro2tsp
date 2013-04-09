#ifndef ONETREE_H_
#define ONETREE_H_

#include "tree.h"
#include "egraph.h"

struct onetree_edge {
  int node;
  double cost;
};

typedef struct onetree_edge onetree_edge;

struct onetree {
  int n;
  onetree_edge first_edge;
  onetree_edge second_edge;
  tree tree;
};

typedef struct onetree onetree;

void onetree_init(onetree* OT, int n);
void onetree_delete(onetree* OT);
void onetree_copy(onetree* FROM, onetree* TO);

void onetree_set_root(onetree* OT, int r);
int onetree_get_root(onetree* OT);

void onetree_insert_edge(onetree* OT, int u, int v);
void onetree_remove_edge(onetree* OT, int u, int v);
void onetree_set_edge_cost(onetree* OT, int u, int v, double cost);
double onetree_get_edge_cost(onetree* OT, int u, int v);

int onetree_get_node_deg(onetree* OT, int v);
int onetree_adjacent_nodes(onetree* OT, int u, int v);
double onetree_get_cost(onetree* OT);

void onetree_to_egraph(onetree* OT, egraph* EG);

#endif
