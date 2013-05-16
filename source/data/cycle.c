#include "cycle.h"

/*
 * cycle_init
 * - C : cycle
 * - n : number of nodes in the cycle
 *
 * initializes a cycle, with n nodes
 */
void cycle_init(cycle *C, int n) {
  if (n == 0) {
    C->n = 0;
    C->nodes = NULL;
    C->costs = NULL;
  }
  else {
    C->n = n;
    C->nodes = (int*)calloc(n, sizeof(int));
    C->costs = (double*)calloc(n, sizeof(double));
  }
}

/*
 * cycle_delete
 * - C : cycle
 *
 * delete a cycle
 */
void cycle_delete(cycle *C) {
  C->n = 0;
  free(C->nodes);
  free(C->costs);
  C->nodes = NULL;
  C->costs = NULL;
}

/*
 * cycle_get_cost
 * - C : cycle
 *
 * compute the cost of a given cycle
 *
 * return : cost of the cycle
 */
double cycle_get_cost(cycle *C) {
  int i;
  int n = C->n;
  double cost = 0.;
  for (i = 0; i < n; ++i) {
    cost += C->costs[i];
  }
  return cost;
}

/*
 * cycle_copy
 * - FROM : source cycle
 * - TO : target cycle
 *
 * copy cycle FROM into cycle TO
 */
void cycle_copy(cycle *FROM, cycle *TO) {
  int i;
  int n = FROM->n;
  cycle_delete(TO);
  cycle_init(TO, n);
  for (i = 0; i < n; i++) {
    TO->nodes[i] = FROM->nodes[i];
    TO->costs[i] = FROM->costs[i];
  }
}

/*
 * cycle_print
 * - C : cycle
 *
 * print the list of nodes composing the cycle
 */
void cycle_print(cycle *C) {
  int i;
  int n = C->n;
  double sum = 0.;
  for (i = 0; i < n; ++i) {
    sum += C->costs[i];
    printf("(%d, %f (%f)) ", C->nodes[i], C->costs[i], sum);
  }
  printf("\n");
}
