#ifndef __TREE_H
#define __TREE_H

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "node.h"
#include "edge.h"
#include "graph.h"

typedef struct _tree {
	node *nodes;
	node **u;
	node **v;
	edge *e;
	int n;
} tree;

/*
 * createTree
 * size_t size : number of elements in the list
 */
tree *createTree(size_t);

/*
 * setNodes
 * - tt : poiter to tree
 * - G : graph
 *
 * copy list on nodes from G to t
 */
void setNodes(tree **tt, graph *G);

/*
 * getTreeCost
 * - t : the tree
 *
 * compute the cost of a tree
 *
 * return : the cost of the tree
 */
double getTreeCost(tree *t);

/*
 * deleteTree
 * - t : the tree to be deleted
 */
void deleteTree(tree *t);

/*
 * is_cycle
 * - t : the tree to be checked
 */
short is_cycle(tree *t);

/*
 * get_tree_node_deg
 * - t : the tree
 * - u : the node
 */
static inline int get_tree_node_deg(tree *t, int u) {
	return t->nodes[u-1].deg;
}

/*
 * copy tree FROM into tree TO;
 */
void copyTree(tree* FROM, tree* TO);

#endif
