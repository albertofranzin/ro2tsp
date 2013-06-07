#ifndef COMPUTE_UPPER_BOUND_H_
#define COMPUTE_UPPER_BOUND_H_

#include "../base/constants.h"
#include "../data/graph.h"
#include "../data/cycle.h"

#include <assert.h>
#include <pthread.h>
#include <stdio.h>

/**
 * parameter struct for rc_thread
 */
typedef struct _rc_params {
  int     th_no; // thread number_of_cycles
  graph  *G;
  cycle  *C;
  double  ub;
  int     number_of_cycles;
  int     return_status;
} rc_params;
/*
 * 
 */

// calcola un upper bound del costo di un tour ottimo su G
// al termine della procedura sarà memorizzato in OT il miglior (di costo minore) tour trovato
// Hp: G, OT inizializzati
int compute_upper_bound(graph* G, cycle* C, int algo, double* ub);

int dumb_upper_bound(graph *G, double* ub);


/**
 * compute NN heuristic starting from each node. Also applies 2-opt
 * @param  G  graph
 * @param  C  best cycle found
 * @param  ub upper bound computed
 * @return    status of the operations
 */
int compute_nearest_neighbour(graph *G, cycle *C, double *ub);

/**
 * compute an ub using RC+2-opt heuristic
 * @param  G  graph
 * @param  C  best cycle found
 * @param  ub uper bound computed
 * @return    status of the operations
 */
int compute_rc(graph *G, cycle *C, double *ub);


/**
 * compute a rc+2opt ub. Thought to be multithreading
 * @param p params
 */
void *rc_thread(void *p);

// calcola un tour di G secondo l'euristica nearest-neighbour
// Hp: G, OT inizializzati
// Hp: 1 <= start_node <= n, dove n è il numero di nodi di G
int heur_nearest_neighbour(graph *G, cycle* C, int start_node, double* ub);

int heur_2_opt(graph *G, cycle *C, double ccost, double* ub);
int generate_random_cycle(graph *G, cycle *C, double* ub);

/**
 * Compute an upper bound for the solution using a genetic algorithm
 * 
 * @param  G the graph
 * @param  C the cycle containing the best solution found
 * @return   cost of the returned solution
 */
double genetic(graph *G, cycle *C);

#endif
