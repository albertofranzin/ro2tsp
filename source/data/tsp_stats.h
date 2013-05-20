#ifndef TSP_STATS_H_
#define TSP_STATS_H_

#include <stdio.h>

typedef struct _tsp_stats {

  int num_of_vertices;

  int num_of_levels;

  int num_of_calls;

  double dumb_ub;

  double rc_ub;

  double rc2opt_ub;

  double nn_ub;

  double nn2opt_ub;

  double init_ub;

  double init_lb;

  int num_fat_edges;

  int num_fails_compute_ub;

  int num_fails_compute_ot;

  int num_fails_integer_rounding;

  int num_fails_propagate;

  int num_fails_select_node;

  int num_3_ways_branching;

  int num_2_ways_branching;

} tsp_stats;

void tsp_stats_init(tsp_stats* stats);

void tsp_stats_delete(tsp_stats* stats);

void tsp_stats_print(tsp_stats* stats);

#endif
