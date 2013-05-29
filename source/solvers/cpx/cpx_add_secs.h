#ifndef CPX_ADD_SECS_H_
#define CPX_ADD_SECS_H_

#include <assert.h>
#include <ilcplex/cplex.h>

#include "../../base/constants.h"
#include "../../base/utils.h"
#include "../../data/graph.h"
#include "cpx_table.h"
#include "cpx_add_my_sec.h"

/**
 * add all subtour elimination constraints
 * @param  env          CPLEX environment
 * @param  lp           problem
 * @param  hash_table   hash table to retrieve edge position in the arrays
 *                      given their nodes, and viceversa
 * @param  G            graph
 * @return              number of SECs added (if this number is equal to 1,
                        then no constraint was added
 */
int cpx_add_secs(CPXENVptr   env,
                 CPXLPptr    lp,
                 double     *sol,
                 int         numcol,
                 cpx_table  *hash_table,
                 parameters *pars);


#endif
