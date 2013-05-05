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


/*
 * cplex_solve_problem
 *
 * - graph *  : pointer to the graph
 * - egraph * : pointer to the egraph object
 * - double * : pointer to incumbent
 *
 * solve a LP problem using CPLEX
 */
void cplex_solve_problem(graph *, egraph *, double *);

/*
 * cplex_create_problem
 *
 * - CPXENVptr : pointer to the CPLEX environment
 * - CPXLPptr  : pointer to the CPLEX LP problem
 * - char *    : problem name
 *
 * create a new environment and problem
 *
 * return : operation status
 */
int cplex_create_problem(CPXENVptr *, CPXLPptr *, char *);

/*
 * cplex_setup_problem
 *
 * general parameters:
 * - graph *   : pointer to the graph
 * - table *   : pointer to hash table
 * - CPXENVptr : pointer to the CPLEX environment
 * - CPXLPptr  : pointer to the CPLEX LP problem
 * - char *    : problem name                  [max 16 chars]
 * - int *     : pointer to number of rows
 * - int *     : pointer number of columns
 * - int *     : pointer to problem type       [CPX_MIN/CPX_MAX]
 *
 * constraints parameters:
 * - double ** : object function coefficients  [# : numcols]
 * - double ** : right-hand-side coefficients  [# : numrows]
 * - char **   : sense of the inequalities     [# : numrows]
 * - int **    : indices of the i-th variable constraints
 *                                             [# : numcols]
 * - int **    : number of occurrences of the i-th variable
 *                                             [# : numcols]
 * - int **    : index of the i-th variable    [# : non-zero vars]
 * - double ** : coefficients                  [# : non-zero vars]
 * - double ** : lower bound for the variables [# : numcols]
 * - double ** : upper bound for the variables [# : numcols]
 * - char **   : type of the variables         [# : numcols]
 *
 * set up the cplex data structures
 *
 * return : operation status
 */
int cplex_setup_problem(graph *, cplex_table *, CPXENVptr, CPXLPptr,
                        char *, int *, int *, int *,
                        double **, double **, char **,
                        int **, int **, int **, double **,
                        double **, double **, char **);

/*
 * cplex_add_SEC
 *
 * - CPXENVptr : pointer to the CPLEX environment
 * - CPXLPptr  : pointer to the CPLEX LP problem
 * - int       : number of components
 * - int *     : vector of indices
 * - double *  : vector of components
 * - double *  : vector containing the right-hand-side of the constraint
 *
 * add a new SEC constraint to the lp
 *
 * return : operation status
 */
int cplex_add_SEC(cplex_table *, CPXENVptr, CPXLPptr,
                  int, int *, double *, double *);

#endif
