#ifndef CPX_SOLVER_H_
#define CPX_SOLVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ilcplex/cplex.h>

#include "../../data/graph.h"
#include "../../data/egraph.h"

#include "cpx_table.h"
#include "cpx_create_problem.h"
#include "cpx_setup_problem.h"
#include "cpx_add_sec.h"
#include "cpx_mark_subtours.h"

void cpx_solver(graph* G, graph* H);

#endif
