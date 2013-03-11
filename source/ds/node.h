#ifndef __NODE_H
#define __NODE_H

//#include "edge.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>

/*
 * define a generic node of a graph
 * contains:
 * - pointer to data
 */
typedef struct _node {
	int data;
} node;

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

	return nn;
}

/*
 * delete a node
 * first, delete all the edges connected to that node
 * then, finally, delete the node
 * 
 */
void deleteNode(node *n) {
	// edges *es = getEdgesConnectedToNode()
	return;
}

#endif