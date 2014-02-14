#ifndef PLOT_H_
#define PLOT_H_


#include <stdlib.h>
#include <stdio.h>
#include "../base/global.h"
#include "../base/utils.h"

#include "../data/coords.h"
#include "../data/graph.h"
#include "../data/tree.h"

/**
* @brief   		Plot a graph.
* @param[in] g  graph
* @param[in] p  points, coordinates of the vertices of the graph
* @return   	= 0 if no error
* 				< 0 if error
* @pre      	g is a graph with n vertices, and p is a list
* 				of n points (coordinates)
* @post			-
* @note			-
*/
int plot_graph(graph* g, coords *crds, char *title);


/**
* @brief   		Plot a tree.
* @param[in] t  tree
* @param[in] p  points, coordinates of the vertices of the tree
* @return   	= 0 if no error
* 				< 0 if error
* @pre      	t is a tree with n vertices, and p is a list
* 				of n points (coordinates)
* @post			-
* @note			-
*/
int plot_tree(tree* t, coords *crds, char* title);


#endif /* PLOT_H_ */
