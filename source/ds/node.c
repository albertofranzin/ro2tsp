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
	node *nn = malloc(sizeof(node));
	memset(nn, 0, sizeof(nn));
	nn->data = data;
	nn->deg = 0;
	nn->x = 0.;
	nn->y = 0.;

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

void deleteNodeList(node **l, size_t count) {
	int i;
	for (i = 0; i < count; ++i) {
		deleteNode(l[i]);
	}
	free(l);
}

void appendNode(node ***nnl, node *n, int pos) {
	node **nl = (node **)(*nnl);

	node **tmp = realloc(nl, sizeof(nl) + sizeof(node)); 
	if (tmp != NULL) {
		memcpy(&(tmp), &(nl), sizeof(nl));
		// beware : 'pos' correctness is not checked!
		tmp[pos] = n;
		nl = tmp;
	} else {
		fprintf(stderr, "memory allocation failed - smthg very close to a segfault [evil laugh] \n");
		exit(1);
	}
}
