#ifndef __CPLEX_SOLVER_H
#define __CPLEX_SOLVER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <ilcplex/cplex.h>
#include "utils.h"

/*************************************************
 *
 * some constants
 *
 *************************************************/

#define MAXNAME 64
#define INFINITY 1e30
#define EPSILON 1e-6
#define MAXITN 10
#define TRUE 1
#define FALSE 0

/*************************************************
 *
 * hash table (commodity)
 *
 *************************************************/

/*
 * table
 *
 * hash table for computing position of a constraint starting from vertices
 * and viceversa. It's a size x 3 matrix
 *
 * its structure is:
 * |  v1  |  v2  |  position  |
 */
struct cplex_table {

  int size;
  int n; // numero di nodi del grafo a cui è associata la tabella: servirà per calcolare pos a partire dagli indici dei vertici estremi di un lato
  int** entries;

};

typedef struct cplex_table cplex_table;

/*
 * table_init
 *
 * create a table starting from a graph
 */
void cplex_table_init(cplex_table* CPX_TAB, int size);

/*
 * table_delete
 *
 * delete a table
 */
void cplex_table_delete(cplex_table* CPX_TAB);

/*
 * pos_from_vertices
 *
 * hash (v1, v2)->(pos)
 *
 * table * : hash table
 * int * : pointer to index of vertex 1 (not modified)
 * int * : pointer to index of vertex 2 (not modified)
 * int * : pointer to index of position (to be modified)
 */
/* 
 * le posizioni dei lati (i, j) sono assegnate come segue (sia n=5)
 * lato: (1,2) (1,3) (1,4) (1,5) (2,3) (2,4) (2,5) (3,4) (3,5) ...
 * pos :   1     2     3     4     5     6     7     8     9   ...
 */
void cplex_table_populate(cplex_table* CPX_TAB, graph* G);

/*
 * vertices_from_pos
 *
 * hash (pos)->(v1, v2)
 *
 * table * : hash table
 * int * : pointer to index of vertex 1 (to be modified)
 * int * : pointer to index of vertex 2 (to be modified)
 * int * : pointer to index of position (not modified)
 */
void vertices_from_pos(cplex_table* CPX_TAB, int* x, int* y, int* pos);

/*
 * pos_from_vertices
 *
 * hash (pos)<-(v1, v2)
 *
 * table * : hash table
 * int * : pointer to index of vertex 1 (not modified)
 * int * : pointer to index of vertex 2 (not modified)
 * int * : pointer to index of position (to be modified)
 */
void pos_from_vertices(cplex_table* CPX_TAB, int* x, int* y, int* pos);




/*************************************************
 *
 * cplex-specific methods
 *
 *************************************************/


/*
 * cplex_create_problem
 *
 * - CPXENVptr : pointer to the CPLEX environment
 * - CPXLPptr : pointer to the CPLEX LP problem
 *
 * create a new environment and problem
 *
 * return : operation status
 */
int cplex_create_problem(CPXENVptr, CPXLPptr);

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
 * cplex_create_obj_function
 *
 * - CPXENVptr : pointer to the CPLEX environment
 * - CPXLPptr : pointer to the CPLEX LP problem
 * - int : number of coefficients
 * - double * : coefficients of the objective function
 *
 * return : operation status
 */
int cplex_create_obj_function(CPXENVptr, CPXLPptr, int, double *);

/*
 * cplex_solve_problem
 *
 * - CPXENVptr : pointer to the CPLEX environment
 * - CPXLPptr : pointer to the CPLEX LP problem
 *
 * solve a LP problem using CPLEX
 *
 * return : operation status
 */
int cplex_solve_problem(CPXENVptr, CPXLPptr);

/*
 * cplex_add_SEC
 *
 * - CPXENVptr : pointer to the CPLEX environment
 * - CPXLPptr : pointer to the CPLEX LP problem
 * - int : number of components
 * - double * : vector of components
 * - double
 *
 * add a new SEC constraint to the lp
 *
 * return : operation status
 */
int cplex_add_SEC(CPXENVptr, CPXLPptr, int, double *, double);

#endif
