#ifndef __CPLEX_SOLVER_H
#define __CPLEX_SOLVER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <ilcplex/cplex.h>

#include "../base/utils.h"

#include "../data/cplex_table.h"
#include "../data/graph.h"

#define MAXNAME  64
//#define INFINITY 1e30
#define MAXITN   10
#define TRUE     1
#define FALSE    0

void cplex_solve_problem(graph* G, egraph* EG, double *incumbent);
int cplex_create_problem(CPXENVptr *env, CPXLPptr *lp, char *probname);
int cplex_setup_problem(CPXENVptr env, CPXLPptr lp, graph* G, cplex_table* hash_table);
int cplex_add_SEC(CPXENVptr env, CPXLPptr lp, cplex_table *hash_table, int* vertices, int* successors, int n);
void solve_kruskal(graph* G, egraph* EG, double *incumbent);

#endif
