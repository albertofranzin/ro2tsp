#include "tsp_stats.h"

void tsp_stats_init(tsp_stats* stats) {

  stats->num_of_vertices = 0;

  stats->num_of_levels = 0;

  stats->num_of_calls = 0;

  stats->dumb_ub = 0.0;

  stats->rc_ub = 0.0;

  stats->rc2opt_ub = 0.0;

  stats->nn_ub = 0.0;

  stats->nn2opt_ub = 0.0;

  stats->init_ub = 0.0;

  stats->init_lb = 0.0;

  stats->num_fat_edges = 0;

  stats->num_fails_compute_ub = 0;

  stats->num_fails_compute_ot = 0;

  stats->num_fails_integer_rounding = 0;

  stats->num_fails_propagate = 0;

  stats->num_fails_select_node = 0;

  stats->num_3_ways_branching = 0;

  stats->num_2_ways_branching = 0;

}

void tsp_stats_delete(tsp_stats* stats) {

  stats->num_of_vertices = 0;

  stats->num_of_levels = 0;

  stats->num_of_calls = 0;

  stats->dumb_ub = 0.0;

  stats->rc_ub = 0.0;

  stats->rc2opt_ub = 0.0;

  stats->nn_ub = 0.0;

  stats->nn2opt_ub = 0.0;

  stats->init_ub = 0.0;

  stats->init_lb = 0.0;

  stats->num_fat_edges = 0;

  stats->num_fails_compute_ub = 0;

  stats->num_fails_compute_ot = 0;

  stats->num_fails_integer_rounding = 0;

  stats->num_fails_propagate = 0;

  stats->num_fails_select_node = 0;

  stats->num_3_ways_branching = 0;

  stats->num_2_ways_branching = 0;

}

void tsp_stats_print(tsp_stats* stats) {

  printf("----------------------------------------------------------------------\n");
  printf("# maximum depth            = %d\n", stats->num_of_levels);
  printf("# number of nodes          = %d\n", stats->num_of_calls);
  printf("# naive upper bound        = %f\n", stats->dumb_ub);
  printf("# upper bound by RC        = %f\n", stats->rc_ub);
  printf("# upper bound by RC + 2OPT = %f\n", stats->rc2opt_ub);
  printf("# upper bound by NN        = %f\n", stats->nn_ub);
  printf("# upper bound by NN + 2OPT = %f\n", stats->nn2opt_ub);
  printf("# initial upper bound      = %f\n", stats->init_ub);
  printf("# initial lower bound      = %f\n", stats->init_lb);
  printf("# number of fat edges      = %d\n", stats->num_fat_edges);
  printf("# number of fails -> computing upper bound    = %d\n", stats->num_fails_compute_ub);
  printf("# number of fails -> computing 1-tree         = %d\n", stats->num_fails_compute_ot);
  printf("# number of fails -> integer rounding         = %d\n", stats->num_fails_integer_rounding);
  printf("# number of fails -> propagating constraints  = %d\n", stats->num_fails_propagate);
  printf("# number of fails -> selecting branching node = %d\n", stats->num_fails_select_node);
  printf("# number of times -> 3-ways branching         = %d\n", stats->num_3_ways_branching);
  printf("# number of times -> 2-ways branching         = %d\n", stats->num_2_ways_branching);
  printf("----------------------------------------------------------------------\n");

}
