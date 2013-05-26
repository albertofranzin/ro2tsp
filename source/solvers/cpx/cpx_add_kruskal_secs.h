#ifndef CPX_ADD_KRUSKAL_SECS_H_
#define CPX_ADD_KRUSKAL_SECS_H_

#include <assert.h>
#include <ilcplex/cplex.h>

#include "../../base/constants.h"
#include "../../base/utils.h"

#include "../../data/graph.h"
#include "../../data/onetree.h"
#include "cpx_table.h"
#include "cpx_add_my_sec.h"

/**
 * add all subtour elimination constraints
 * suggested by Kruskal's algorithm to find a minimum spanning tree
 * @param  env          CPLEX environment
 * @param  lp           problem
 * @param  hash_table   hash table to retrieve edge position in the arrays
 *                        given their nodes, and viceversa
 * @param  OT           1-tree
 * @return              status of the operation performed
 */
int cpx_add_kruskal_secs(CPXENVptr   env,
                         CPXLPptr    lp,
                         cpx_table  *hash_table,
                         onetree    *OT,
                         parameters *pars);

#endif
