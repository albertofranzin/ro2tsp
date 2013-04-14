#include "compute_lagrange.h"

double compute_lagrange(graph* G, onetree* H, double ub) {
  int i, j, v, num_of_iterations, time_since_improvement;
  double alpha, square_norm, step_size, L, L_OPT;

  // if ub has a forbidden edge, then the branch is useless
  // this won't afflict the initial computation of lb
  if (ub >= BIG) {
    return BIG;
  }

  graph WORK_GRAPH;
  onetree ONE_TREE;

  int n = (*G).n;

  double* mult = (double*)calloc(n+1, sizeof(double));
  double* subgrad = (double*)calloc(n+1, sizeof(double));

  onetree_delete(H);
  onetree_init(&ONE_TREE, 1);
  graph_init(&WORK_GRAPH, 1);
  graph_copy(G, &WORK_GRAPH);

  L_OPT = VERY_SMALL;
  alpha = ALPHA;
  num_of_iterations = 0;
  time_since_improvement = 0;

  while (num_of_iterations < MAX_NUM_OF_ITERATIONS) {
    num_of_iterations++;

    /* calcolo 1-albero usando i costi originali modificati dai moltiplicatori lagrangiani correnti.
     */
    compute_ot(&WORK_GRAPH, &ONE_TREE);

    /* calcolo costo 1-albero usando i costi originali;
     * - sommo 2 * (sommatoria tutti moltiplicatori) al costo dell'1-albero con i costi modificati per ottenere il costo originale dell'1-albero.
     */
    L = onetree_get_cost(&ONE_TREE);
    for (i = 2; i <= n; i++) 
      L += 2 * mult[i];
    
    /* aggiorno contatore iterazioni in cui il valore L non è migliorato.
     */
    if (L > L_OPT) {
      L_OPT = L;
      time_since_improvement = 0;
      onetree_copy(&ONE_TREE, H);
      //printf("# updated lagrangean lower bound = %f : current iteration = %d\n", L_OPT, num_of_iterations);
    }
    else {
      time_since_improvement++;
    }

    /* STOP: interrompi ciclo se L non è migliorato nelle ultime k iterazioni.
     */
    if (time_since_improvement > SLACK_TIME)
      break;

    /* riduci (dimezza) alpha se L non è migliorato nelle ultime h iterazioni.
     */
    if (time_since_improvement > ALPHA_HALVING_TIME)
      alpha /= 2.0;

    /* calcolo le componenti del vettore subgradiente,
     * calcolo norma al quadrato del vettore subgradiente
     */
    square_norm = 0.0;
    for (i = 2; i <= n; i++) {
      subgrad[i] = 2 - onetree_get_node_deg(&ONE_TREE, i);
      square_norm += subgrad[i] * subgrad[i];
    }

    /* STOP: interrompi ciclo se il subgradiente è il vettore nullo.
     */
    if (square_norm == 0)
      break;

    /* calcolo l'ampiezza del passo di avanzamento attuale.
     */
    step_size = alpha * (ub - L) / square_norm;
  
    /* calcolo i nuovi moltiplicatori lagrangiani.
     */
    for (i = 2; i <= n; i++) {
      mult[i] += step_size * (subgrad[i] / sqrt(square_norm));
    }

    /* aggiorno i costi dei lati tenendo conto dei nuovi moltiplicatori.
     */
    for (i = 2; i <= n; i ++) {
      for (j = i+1; j <= n; j++) {
        graph_set_edge_cost(&WORK_GRAPH, i, j, graph_get_edge_cost(G, i, j) - mult[i] - mult[j]);
      }
    }

    for (j = 2; j <= n; j++) {
      graph_set_edge_cost(&WORK_GRAPH, 1, j, graph_get_edge_cost(G, 1, j) - mult[j]);
    }
  } 


  onetree_delete(&ONE_TREE);
  graph_delete(&WORK_GRAPH);
  free(subgrad);
  free(mult);

  v = onetree_get_first_node(H);
  if (v > 0) 
    onetree_set_edge_cost(H, 1, v, graph_get_edge_cost(G, 1, v));
  v = onetree_get_second_node(H);
  if (v > 0) 
    onetree_set_edge_cost(H, 1, v, graph_get_edge_cost(G, 1, v));
  for (i = 2; i <= n; i++)
    onetree_set_edge_cost(H, onetree_get_pred(H, i), i, graph_get_edge_cost(G, onetree_get_pred(H, i), i));

  return L_OPT;
}
