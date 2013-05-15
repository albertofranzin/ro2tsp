#ifndef CPX_MARK_SUBTOURS_H_
#define CPX_MARK_SUBTOURS_H_

#include <assert.h>
#include <ilcplex/cplex.h>

#include "../../base/constants.h"

#include "../../data/onetree.h"

#include "cpx_table.h"


/**
 * mark every subtour with a different label
 * @param  hash_table   hash table to retrieve edge position in the arrays
 *                        given their nodes, and viceversa
 * @param  edge_indices array of edge indices
 * @param  edge_marks   array of edge marks
 * @param  n            number of elements in the arrays
 * @return              number of subtours found
 */
int cpx_mark_subtours(cpx_table *hash_table,
                      int       *edge_indices,
                      int       *edge_marks,
                      int        n);

/**
 * mark every subtour with a different label,
 * using an alternative way suggested by a Kruskal-like procedure
 * @param  env          CPLEX environment
 * @param  lp           problem
 * @param  G            graph
 * @param  hash_table   hash table to retrieve edge position in the arrays
 *                        given their nodes, and viceversa
 * @param  n            number of elements in the arrays
 * @return              number of subtours found
 */
int cpx_mark_subtours_the_kruskal_way(CPXENVptr   env,
                                      CPXLPptr    lp,
                                      graph      *G,
                                      cpx_table  *hash_table,
                                      int         n);

#endif
