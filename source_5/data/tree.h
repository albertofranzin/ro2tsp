#ifndef TREE_H_
#define TREE_H_



#include <stdlib.h>
#include <stdio.h>
#include "../data/graph.h"



typedef struct _tree_node {

  int pred;
  int deg;

} tree_node;



typedef struct _tree {

  int n;
  int root;
  tree_node* V;

} tree;



void tree_init(tree* T, int n, int r);



void tree_delete(tree* T);



void tree_copy(tree* FROM, tree* TO);



void tree_insert_edge(tree* T, int u, int v);



void tree_remove_edge(tree* T, int u, int v);



int tree_adjacent_nodes(tree* T, int u, int v);



double tree_get_cost(tree* T, graph* G);



#endif
