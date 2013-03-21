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
 * - degree of the node
 * - x-coord [0,1]
 * - y-coord [0,1]
 */
typedef struct _node {
	int data;
	int deg;
	double x;
	double y;
} node;

node *newNode(int);
void deleteNode(node *);
void deleteNodeList(node **, size_t);
void appendNode(node ***n, node *, int);

#endif