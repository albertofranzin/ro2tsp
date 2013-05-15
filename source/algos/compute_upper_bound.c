#include "compute_upper_bound.h"

double compute_upper_bound(graph* G, onetree* OT, int algo) {
  int i;
  double min, tour_cost;
  int n = (*G).n;

  onetree OT_tmp;
  onetree_init(&OT_tmp, n);

  min = compute_nearest_neighbour(G, OT, 1);
  for (i = 2; i <= n; i++) {
    tour_cost = compute_nearest_neighbour(G, &OT_tmp, i);
    if (tour_cost < min) {
      min = tour_cost;
      onetree_copy(&OT_tmp, OT);
    }
  }

  return min;
}

double compute_nearest_neighbour(graph *G, onetree* OT, int start_node) {
  double min, tour_cost;
  int k, i, current, next;

  int n = (*G).n;
  int visited[n];

  onetree_delete(OT);
  onetree_init(OT, n);

  for (i = 0; i < n; i++)
    visited[i] = 0;

  // MOD: ORDINAMENTO SOMMA LATI CONFORME A QUELLO STABILITO PER 1-ALBERI (convenzione)
  //tour_cost = 0.0;
  int* predecessors = (int*)malloc(sizeof(int) * n);
  int* successors = (int*)malloc(sizeof(int) * n);

  current = start_node;
  visited[current-1] = 1;

  for (k = 1; k < n; k++) {
    for (i = 1; i <= n; i++) {
      if (visited[i-1] == 0) {
	min = graph_get_edge_cost(G, current, i);
	next = i;
	break;
      }
    }
    for (i = 1; i <= n; i++) {
      if (visited[i-1] == 0 && graph_get_edge_cost(G, current, i) < min) {
	min = graph_get_edge_cost(G, current, i);
	next = i;
      }
    }
    onetree_insert_edge(OT, current, next, min);

    // MOD: ORDINAMENTO SOMMA LATI
    //tour_cost += min;
    predecessors[next-1] = current;
    successors[current-1] = next;

    current = next;
    visited[current-1] = 1;
  }

  // MOD: ORDINAMENTO SOMMA LATI
  predecessors[start_node-1] = current;
  successors[current-1] = start_node;

  // MOD: ORDINAMENTO SOMMA LATI
   //tour_cost += graph_get_edge_cost(G, current, start_node);

  onetree_insert_edge(OT, current, start_node, graph_get_edge_cost(G, current, start_node));

  // MOD: ORDINAMENTO SOMMA LATI
  int pred_1;
  int succ_1;
  pred_1 = predecessors[0];
  succ_1 = successors[0];

  tour_cost = 0.0;
  if (succ_1 >= pred_1) {
    for (i = 1; i <= n; i++) {
      tour_cost += graph_get_edge_cost(G, predecessors[i-1], i);
    }
  }
  else {
    for (i = 1; i <= n; i++) {
      tour_cost += graph_get_edge_cost(G, i, successors[i-1]);
    }
  }

  free(predecessors);
  free(successors);
  return tour_cost;
}


/**
 * Compute an upper bound for the solution using a genetic algorithm
 * 
 * @param  G the graph
 * @param  C the cycle containing the best solution found
 * @return   cost of the returned solution
 */
double genetic(graph *G, cycle *C) {
  double cost = 0.;

  int i, j;
  int n = G->n;
  int iterations,
      iterations_wo_improvement = 0;

  cycle c,
        best_cycle;
  cycle_init(&c, n);
  cycle_init(&best_cycle, n);

  // reset
  cycle_delete(C);
  cycle_init(C, n);

  int population = 30;
  int tour[population][n];

  double fitness[population],
         norm_fitness[population], // normalized f., probability to be chosen
         costs[population];

  double best_fitness,
         best_cost;
  int best_index;

  double crossover_prob,
         mutation_prob;

  int target, remaining, tmp;

  // initialize
  for (i = 0; i < n; ++i) {
    for (j = 0; j < population; ++j) {
      tour[j][i] = i+1;
    }
  }

  for (i = 0; i < population; ++i) {

    // generate permutation of the node
    remaining = n;
    for (j = 0; j < n; ++j) {

      target = rand() % remaining;

      tmp                 = tour[i][j];
      tour[i][j]          = tour[i][target + j];
      tour[i][target + j] = tmp;

      remaining--;

    }

  }

  best_index = population + 1;
  best_fitness = 0.;

  for (i = 0; i < population; ++i) {
    costs[i] = 0.;
    for (j = 0; j < n-1; ++j) {
      costs[i] += graph_get_edge_cost(G, tour[i][j], tour[i][j+1]);
    }
    costs[i]  += graph_get_edge_cost(G, tour[i][n], tour[i][0]);
    fitness[i] = 1. / costs[i];

    if (fitness[i] > best_fitness) {
      best_fitness = fitness[i];
      best_cost    = costs[i];
      best_index   = i;
    }

  }

  // normalize probabilities
  for (i = 0; i < population; ++i) {
    norm_fitness[i] = fitness[i] / best_fitness;
  }

  int cp1, cp2; // crossover points




  return cost;
}
