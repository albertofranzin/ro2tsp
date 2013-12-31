#ifndef PRIM_1TREE_H_
#define PRIM_1TREE_H_


#include <stdlib.h>
#include <stdio.h>
#include "../base/global.h"
#include "../data/graph.h"
#include "../data/tree.h"
#include "../algos/prim_mst.h"


/**
* @brief				Compute a minimum 1-tree (precisely a 0-tree) spanning
* 						over all vertices of g; first it is computed a mst
* 						with root 0 using prim_mst, then is added to the tree
* 						(thus creating a cycle) the edge with the second minimum
* 						cost among all the edges in g which are incident to 0.
* @param[in] 	g		graph
* @param[out] 	t		tree
* @param[in]	root 	root of the output spanning tree
* @return   			= 0 if no error
* 						< 0 if error
* @pre					t is supposed to be initialized
* @post					-
* @note					-
*/
int prim_1tree(graph *g, tree *t);


#endif /* PRIM_1TREE_H_ */
