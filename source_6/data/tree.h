#ifndef TREE_H_
#define TREE_H_

#include <stdlib.h>
#include <stdio.h>
#include "../base/global.h"
#include "../data/graph.h"


/**
* @brief    A rooted tree with vrtx_num vertices. Each vertex
*           has at most one predecessor, except for the
*           root which should have no predecessor. The structure can
*           also be used to represent a 1-tree, setting the predecessor
*           of the root in the right way.
*/
typedef struct _tree {

	int	vrtx_num;	/* number of vertices */
	int	vrtx_root;	/* root vertex */
	int	*vrtx_pred;	/* list of predecessors, one for each vertex */
	int *vrtx_deg;	/* list containing the number of edges */
					/* incident to each vertex */

} tree;


/**
* @brief		Initialize a tree structure.
* @param[in] t	tree
* @return   	= 0 if no error
* 				< 0 if error
* @pre			-
* @post			-
* @note			-
*/
int tree_init(tree *t);


/**
* @brief		Delete a tree.
* @param[in] t	tree
* @return   	= 0 if no error
* 				< 0 if error
* @pre			t is supposed to be initialized
* @post			-
* @note			-
*/
int tree_delete(tree *t);


/**
* @brief		Setup a rooted tree, allocate memory
*           	space for it.
* @param[in] t	tree
* @param[in] n  number of vertices
* @param[in] r  root
* @return   	= 0 if no error
* 				< 0 if error
* @pre      	t is supposed to be initialized
* @pre      	n >= 1
* @pre      	0 <= r <= n
* @post			-
* @note			-
*/
int tree_setup(tree *t, int n, int r);


/**
* @brief        	Copy a tree.
* @param[in]  from	tree to be copied
* @param[out] to	copied tree
* @return   		= 0 if no error
* 					< 0 if error
* @pre		  		from is supposed to be initialized
* @pre		  		to is supposed to be initialized
* @post				-
* @note				-
*/
int tree_copy(tree *from, tree *to);


/**
* @brief		Check if there is an edge between two vertices.
* @param[in] t  tree
* @param[in] u  first vertex
* @param[in] v  second vertex
* @return   	= 0 if no error and if there is no edge between u and v
* 				= 1 if no error and if there is an edge between u and v
* 				< 0 if an error occurred
* @pre       	t is supposed to be initialized
* @pre      	u is a valid vertex index
* @pre      	v is a valid vertex index
* @pre      	u != v i.e. no self-loops
* @post			-
* @note			-
*/
int tree_is_edge(tree* t, int u, int v);


/**
* @brief		Add an edge to the tree.
* @param[in] t 	tree
* @param[in] p  predecessor vertex
* @param[in] s  successor vertex
* @return   	= 0 if no error
* 				< 0 if error
* @pre		 	t is supposed to be initialized
* @pre      	p is a valid vertex index
* @pre      	s is a valid vertex index
* @pre      	p != s i.e. no self-loops
* @pre      	vertex s has no predecessor
* @post			-
* @note			-
*/
int tree_insert_edge(tree *t, int p, int s);


/**
* @brief    	Remove an edge.
* @param[in] t  tree
* @param[in] u  first vertex
* @param[in] v  second vertex
* @return   	= 0 if no error
* 				< 0 if error
* @pre		 	t is supposed to be initialized
* @pre      	u is a valid vertex index
* @pre      	v is a valid vertex index
* @pre      	u != v i.e. no self-loops
* @pre      	there is an edge between
*           	vertices u and v i.e.
*           	u is the predecessor
*           	of vertex v or v is the
*           	predecessor of vertex u
* @post			-
* @note			-
*/
int tree_remove_edge(tree *t, int u, int v);


/**
* @brief    		Compute the cost of a tree, using as weights
* 					the edge-weights of g.
* @param[in] t  	tree
* @param[in] g  	graph
* @param[in] cost  	cost of the tree
* @return   		= 0 if no error
* 					< 0 if error
* @pre      		-
* @post				-
* @note				-
*/
int tree_get_cost(tree *t, graph* g, double *cost);


#endif /* TREE_H_ */
