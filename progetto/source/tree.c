#include "tree.h"

/*
 * createTreeNode
 * size : number of elements in the list
 *
 * works this way:
 * - e is the list of edges in the tree
 * - nodes is the list of nodes in the tree (copied from the list in the graph)
 * - u, v are list of pointers to nodes, s.t. u[i], v[i] are the two nodes of edge e[i]
 */
tree *createTree(size_t size) {
	tree *t = malloc(sizeof(tree));

	t->n = size;

	// list of pointers to first node of the edge
	node *u[size];
	t->u = u;

	// list of pointers to second node of the edge
	node *v[size];
	t->v = v;

	// list of pointers to edges
	edge e[size];
	t->e = e;

	// clean mem
	memset(t->nodes, 0, sizeof(node) * size);
	memset(t->u, 0, sizeof(node* ) * size);
	memset(t->v, 0, sizeof(node* ) * size);
	memset(t->e, 0, sizeof(edge) * size);

	return t;
}

/*
 * setNodes
 * - tt : poiter to tree
 * - G : graph
 *
 * copy list on nodes from G to t
 */
void setNodes(tree **tt, graph *G) {
	tree *t = (tree *)(tt);
	memcpy(&(t->nodes), &(G->V), sizeof(node) * G->n);
}

/*
 * getTreeCost
 * -t : the tree
 *
 * compute the cost of a tree
 *
 * return : the cost of the tree
 */
double getTreeCost(tree *t) {
	int i;
	double cost = 0.;
	for (i = 0; i < t->n ; ++i) {
		cost += t->e[i].cost;
	}
	return cost;
}

/*
 * deleteTree
 * - t : the tree to be deleted
 */
void deleteTree(tree *t) {
	free(t->u);
	free(t->v);
	free(t->e);
	free(t);
}

/*
 * is_cycle
 * - t : the tree to be checked
 */
short is_cycle(tree *t) {
	int i;
	for (i = 0; i < t->n; ++i) {
		if (t->nodes[i].deg != 2) {
			return 0;
		}
	}
	return 1;
}

/*
 * copy tree FROM into tree TO;
 */
void copyTree(tree* FROM, tree* TO) {
  int i;

  int n = (*FROM).n;
  //deleteTree(TO);
  //initGraph(TO, n);
  memcpy(&((*TO).nodes), &((*FROM).nodes), sizeof(node) * n);
  /*for (i = 0; i < n; i++) {
    (*TO).V[i].x = (*FROM).V[i].x;
    (*TO).V[i].y = (*FROM).V[i].y;
    (*TO).V[i].deg = (*FROM).V[i].deg;
  }*/
  memcpy(&((*TO).e), &((*FROM).e), sizeof(edge) * n);
  /*for (i = 0; i < n * (n + 1) / 2;i++) {
    (*TO).E[i].flag = (*FROM).E[i].flag;
    (*TO).E[i].cost = (*FROM).E[i].cost;
  }*/
  for (i = 0; i < n; ++i) {
  	// hope this is correct (can't test yet):
  	// since u, v contain pointers to nodes, I compute the offset in the
  	// original list, and then apply it to the destination list
  	TO->u[i] = (node *)(&(TO->nodes) + (&(FROM->u[i]) - &(FROM->nodes)));
  	TO->v[i] = (node *)(&(TO->nodes) + (&(FROM->v[i]) - &(FROM->nodes)));
  }
}
