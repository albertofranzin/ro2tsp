#ifndef CPX_MARK_SUBTOURS_H_
#define CPX_MARK_SUBTOURS_H_

#include <assert.h>
#include <ilcplex/cplex.h>
#include "../../base/constants.h"
#include "cpx_table.h"

// per ogni subtour, marca tutti i lati di quel subtour con uno stesso mark
// ritorna il numero di subtour
int cpx_mark_subtours(cpx_table* hash_table, int* edge_indexes, int* edge_marks, int n);

#endif
