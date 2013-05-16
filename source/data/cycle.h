#ifndef __CYCLE_H
#define __CYCLE_H

#include "../base/constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

typedef struct _cycle {
  // number of distinct nodes in the cycle,
  // i.e. cycle closes on first node, without need to store it twice
  int n;
  // sequence of nodes
  int *nodes;
  // list of edge costs
  // costs[i] = cost of edge (node[i], node[i%n])
  double *costs; 

  // list of edge constraints
  // constr[i] = constraint of edge (node[i], node[i%n])
} cycle;

/*
 * cycle_init
 * - C : cycle
 * - n : number of nodes in the cycle
 *
 * initializes a cycle, with n nodes
 */
void cycle_init(cycle *C, const int n);

/*
 * cycle_delete
 * - C : cycle
 *
 * delete a cycle
 */
void cycle_delete(cycle *C);

/*
 * cycle_get_cost
 * - C : cycle
 *
 * compute the cost of a given cycle
 *
 * return : cost of the cycle
 */
double cycle_get_cost(cycle *C);

/*
 * cycle_copy
 * - FROM : source cycle
 * - TO : target cycle
 *
 * copy cycle FROM into cycle TO
 */
void cycle_copy(cycle *FROM, cycle *TO);

/*
 * cycle_print
 * - C : cycle
 *
 * print the list of nodes composing the cycle
 */
void cycle_print(cycle *C);

#endif

