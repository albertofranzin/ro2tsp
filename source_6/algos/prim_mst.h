#ifndef PRIM_MST_H_
#define PRIM_MST_H_


#include <stdlib.h>
#include <stdio.h>
#include "../base/global.h"
#include "../data/graph.h"
#include "../data/tree.h"


/**
* @brief				Compute a minimum spanning tree (rooted in root)
* 						for the graph g; Prim's algorithm is used; forbidden
* 						edges are never selected, while forced edges have
* 						always greater priority than unconstrained edges;
* 						in case forced edges form a cycle, the output
* 						tree is not guaranteed to have the minimum
* 						cost among all the spanning tree computed giving
* 						priority to forced edges.
* @param[in] 	g		graph
* @param[in]	root 	root of the output spanning tree
* @param[out] 	t		tree
* @return   			= 0 if no error
* 						< 0 if error
* @pre					t is supposed to be initialized
* @post					-
* @note					-
*/
int prim_mst(graph *g, int root, tree *t);


#endif /* PRIM_MST_H_ */
