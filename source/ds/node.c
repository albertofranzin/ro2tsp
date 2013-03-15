#include "node.h"

/*
 * newNode
 * data [int] : the number associated to the node
 * 
 * create a new node
 *
 * return : the data
 */
node *newNode(int data) {
	node *nn = malloc(sizeof(int));
	memset(nn, 0, sizeof(nn));
	nn->data = data;
	//nn->parent = NULL;
	//nn->adj = NULL;
	nn->x = 0;
	nn->y = 0;

	return nn;
}

/*
 * delete a node
 * first, delete all the edges connected to that node
 * then, finally, delete the node
 * 
 */
void deleteNode(node *n) {
	free(n);
	return;
}
