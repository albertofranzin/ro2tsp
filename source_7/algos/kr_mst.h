#ifndef KR_MST_H_
#define KR_MST_H_


#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../base/global.h"
#include "../base/utils.h"

#include "../data/graph.h"
#include "../data/tree.h"
#include "../data/set.h"

/**
* @brief				Compute a minimum spanning tree (rooted in 1)
* 						over vertices 1, 2, ..., n-1, leaving apart
* 					    vertex 0; Kruskal's algorithm is used; forbidden
* 						edges are never selected, while forced edges have
* 						always greater priority than unconstrained edges;
* 						we assume that forced edges never form a cycle.
* @param[in] 	g		graph
* @param[out] 	t		tree
* @return   			= 0 if no error
* 						< 0 if error
* @pre					t is supposed to be initialized
* @pre					n > 1
* @post					-
* @note					-
*/
int kr_mst(int *edges, int num_edges, set *vertexset, tree *t, int* status);

#endif /* KR_MST_H_ */
