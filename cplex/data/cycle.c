#include "cycle.h"

/*
 * cycle_init
 * - C : cycle
 * - n : number of nodes in the cycle
 *
 * initializes a cycle, with n nodes
 */
void cycle_init(cycle *C, const int n) {
  C->n = n;
  C->node = calloc(n, sizeof(n));
  C->costs = calloc(n, sizeof(double));
}

/*
 * cycle_delete
 * - C : cycle
 *
 * delete a cycle
 */
void cycle_delete(cycle *C) {
  //printf("%d\n", C->n);
  C->n = 0;
  //printf("erased counter\n");
  free(C->node);
  //printf("nodes deleted\n");
  free(C->costs);
  //printf("costs deleted\n");
  //free(C); // <<-- error here!
  //printf("ev'rything done\n");
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
  double cost = 0.;
  for (i = 0; i < C->n; ++i) {
    cost += C->costs[i];
    //printf("cost = %f\n", cost);
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
  cycle_delete(TO);
  cycle_init(TO, FROM->n);
  memcpy(TO->node, FROM->node, sizeof(FROM->node[0]) * FROM->n);
  memcpy(TO->costs, FROM->costs, sizeof(FROM->costs[0]) * FROM->n);
}

/*
 * cycle_print
 * - C : cycle
 *
 * print the list of nodes composing the cycle
 */
void cycle_print(cycle *C) {
  int i;
  double sum = 0.;
  for (i = 0; i < C->n; ++i) {
    sum += C->costs[i];
    printf("(%d, %f (%f)) ", C->node[i], C->costs[i], sum);
  }
  printf("\n");
}

