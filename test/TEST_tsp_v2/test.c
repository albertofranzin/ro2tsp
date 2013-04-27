#include <stdio.h>
#include "constants.h"
#include "graph.h"
#include "egraph.h"
#include "onetree.h"
#include "compute_upper_bound.h"
#include "tsp_input.h"
#include "tsp_output.h"
#include "tsp_stats.h"
#include "tsp_status.h"
#include "tsp_backup.h"
#include "tsp_solve.h"

void main() {
  int n = 30; // numero di nodi
  int s = 123; // seed

  egraph EG;
  egraph_init(&EG, n);
  srand(s);
  egraph_random(&EG);

  graph G;
  graph_init(&G, 1);
  egraph_to_graph(&EG, &G);

  onetree NN;
  onetree_init(&NN, n);
  double ub = compute_upper_bound(&G, &NN);

  tsp_input input;
  tsp_input_init(&input, n);
  graph_copy(&G, &input.G);
  onetree_copy(&NN, &input.H);
  input.ub = ub;
  tsp_output output;
  tsp_output_init(&output, n);
  tsp_status status;
  tsp_status_init(&status, n);
  tsp_stats stats;
  tsp_stats_init(&stats, n);

  tsp_solve(&input, &output, &status, &stats);

  egraph EG_H;
  egraph_init(&EG_H, 1);
  egraph_copy(&EG, &EG_H);
  onetree_to_egraph(&output.H_opt, &EG_H);

  egraph_plot(&EG, &EG_H);

  graph_delete(&G);
  egraph_delete(&EG);
  egraph_delete(&EG_H);
  tsp_input_delete(&input);
  tsp_output_delete(&output);
  tsp_status_delete(&status);
  tsp_stats_delete(&stats);
}

