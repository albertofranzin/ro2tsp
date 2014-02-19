#ifndef TREE_H_
#define TREE_H_

#include <stdlib.h>
#include <stdio.h>
#include "../base/global.h"
#include "../base/utils.h"

#include "../data/graph.h"


/**
* @brief    A tree or 1-tree with vrtx_num vertices. A list
* 			of vrtx_num edges is saved (vrxt_num+1 edges if
* 			a 1-tree is represented).
* @note		NO CONTROL while adding new edges: it is up to you
* 			to avoid the insertion of duplicate edges or to
* 			avoid the creation of cycles!
*/
typedef struct _tree {

	int		vrtx_num;		/* number of vertices */
	int		edge_num;		/* number of edges added up to now */
	int		*vrtx_deg;		/* vertex-degree number of incident
	 	 	 	 	 	 	 * edges */
	int		*vrtx_deg_frc;	/* vertex-degree number of incident
							 * forced edges */
	int		*vrtx_deg_frb;	/* vertex-degree number of incident
							 * forbidden edge */
	int		*vrtx_pred;		/* vertex predecessors */
	int		*edges;			/* edges indexes */

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


int tree_empty(tree *t);



/**
* @brief		Setup a tree, allocate memory
*           	space for it.
* @param[in] t	tree
* @param[in] n  number of vertices
* @return   	= 0 if no error
* 				< 0 if error
* @pre      	t is supposed to be initialized
* @pre      	n >= 1
* @post			-
* @note			-
*/
int tree_setup(tree *t, int n);


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
* @brief		Add an edge to the tree.
* @param[in] t 	tree
* @param[in] ie edge index
* @return   	= 0 if no error
* 				< 0 if error
* @pre		 	t is supposed to be initialized
* @pre      	ie is a valid edge index
* @pre			no more than n+1 edges could be added
* @post			-
* @note			NO CONTROL while adding new edges: it is up to you
* 				to avoid the insertion of duplicate edges or to
* 				avoid the creation of cycles!
*/
int tree_insert_edge(tree *t, int ie);


/**
* @brief    	Remove an edge.
* @param[in] t  tree
* @param[in] ie edge index
* @return   	= 0 if no error
* 				< 0 if error
* @pre		 	t is supposed to be initialized
* @pre      	ie is a valid edge index
* @post			-
* @note			THIS FUNCTION HASN'T BEEN IMPLEMENTED YET
*/
//int tree_remove_edge(tree *t, int ie);


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


/**
* @brief    		Test whether a CONNECTED 1-TREE is
* 					a cycle
* @param[in] t  	tree
* @return   		TRUE if t is a cycle
* 					FALSE otherwise
* @pre      		-
* @post				-
* @note				-
*/
int onetree_is_cycle(tree *t);

int tree_rooting(tree *t, int root);

#endif /* TREE_H_ */
