#ifndef CPX_ADD_SEC_H_
#define CPX_ADD_SEC_H_

#include <stdlib.h>
#include <assert.h>
#include <ilcplex/cplex.h>

int cpx_add_sec(CPXENVptr env, CPXLPptr lp, int* edge_indexes, int* edge_marks, int n, int my_mark);

#endif
