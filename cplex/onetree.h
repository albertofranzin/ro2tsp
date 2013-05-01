#ifndef ONETREE_H_
#define ONETREE_H_

#include "tree.h"
#include <stdlib.h>
#include <string.h>

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
int onetree_get_pred(onetree* OT, int v);
int onetree_get_first_node(onetree* OT);
int onetree_get_second_node(onetree* OT);

void onetree_insert_edge(onetree* OT, int u, int v, double cost);
void onetree_remove_edge(onetree* OT, int u, int v);
void onetree_set_edge_cost(onetree* OT, int u, int v, double cost);
double onetree_get_edge_cost(onetree* OT, int u, int v);

int onetree_get_node_deg(onetree* OT, int v);
int onetree_adjacent_nodes(onetree* OT, int u, int v);

double onetree_get_cost(onetree* OT);

/*
 * onetree_reroot
 * OT : 1-tree
 * root : new root node
 *
 * We suppose OT contains a cycle (or subcycle), and root belongs to it.
 * Root gives the direction.
 *
 * performs O(n)-time re-rooting and re-orienting of a 1-tree
 */
void onetree_reroot(onetree *OT, int root);

#endif
