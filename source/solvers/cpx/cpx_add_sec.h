#ifndef CPX_ADD_SEC_H_
#define CPX_ADD_SEC_H_

#include <stdlib.h>
#include <assert.h>
#include <ilcplex/cplex.h>

#include "../../base/utils.h"

/**
 * Add a new subtour elimination constraint to the model
 * @param  env          CPLEX environment
 * @param  lp           problem
 * @param  cbdata
 * @param  wherefrom
 * @param  edge_indices array of edge indices
 * @param  edge_marks   array of edge marks
 * @param  n            number of elements in the arrays
 * @param  my_mark      mark to identify the subtour
 * @param  pars         user parameters
 * @return              status of the operation
 */
int cpx_add_sec(CPXENVptr   env,
                CPXLPptr    lp,
                void       *cbdata,
                int         wherefrom,
                int        *edge_indices,
                int        *edge_marks,
                int         n,
                int         my_mark,
                parameters *pars);

#endif
