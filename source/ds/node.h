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
 * - pointer to parent (if needed)
 * - pointer to list of adjacent nodes (or children - if any)
 * - x-coord [0,1]
 * - y-coord [0,1]
 */
typedef struct _node {
	int data;
	//struct _node *parent;
	//struct _node **adj;
	double x;
	double y;
} node;

node *newNode(int data);
void deleteNode(node *n);

#endif