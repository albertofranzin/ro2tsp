#ifndef CPX_ADD_MY_SEC_H_
#define CPX_ADD_MY_SEC_H_

#include <stdlib.h>
#include <assert.h>
#include <ilcplex/cplex.h>
#include "cpx_table.h"

/**
 * Add a new subtour elimination constraint to the model
 * @param  env          CPLEX environment
 * @param  lp           problem
 * @param  vrtx_marks   array of vertex marks
 * @param  n            number of elements in the array
 * @param  my_mark      mark to identify the subtour
 * @return              status of the operation
 */
int cpx_add_my_sec(CPXENVptr  env,
                   CPXLPptr   lp,
		   cpx_table *hash_table,
		   int       *vrtx_mrks,
		   int        n,
		   int        my_mark);

#endif
