#ifndef KR_ONETREE_H_
#define KR_ONETREE_H_


#include <stdlib.h>
#include <stdio.h>
#include "../base/global.h"
#include "../base/utils.h"

#include "../data/graph.h"
#include "../data/tree.h"

#include "../algos/kr_mst.h"


/**
* @brief				Compute a minimum 1-tree (precisely a 0-tree) spanning
* 						over all vertices of g; first it is computed a mst
* 						over vertices 1, 2, ..., n-1 using kruskal_mst, then the
* 						two least-cost edges incident to 0 are added to the
* 						tree (thus creating a cycle).
* @param[in] 	g		graph
* @param[out] 	t		tree
* @return   			= 0 if no error
* 						< 0 if error
* @pre					t is supposed to be initialized
* @post					-
* @note					-
*/
int kr_onetree(int *mstedges, int num_mstedges, int *oneedges, int num_oneedges, int *weights, set *vertexset, tree* min1t, int *status);

int kr_onetree2(graph *g, arraylist *edgelist, set *part_vs, tree *part_1t, tree* min1t, int *status);

#endif /* KR_ONETREE_H_ */
