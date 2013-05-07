#ifndef CPX_SETUP_PROBLEM_H
#define CPX_SETUP_PROBLEM_H

#include <stdlib.h>
#include <assert.h>
#include <ilcplex/cplex.h>

#include "../../data/graph.h"
#include "cpx_table.h"

int cpx_setup_problem(CPXENVptr env, CPXLPptr lp, graph* G, cpx_table* hash_table);

#endif
