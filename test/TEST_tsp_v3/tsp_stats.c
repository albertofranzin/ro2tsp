#include "tsp_stats.h"

void tsp_stats_init(tsp_stats* stats, int n) {

  (*stats).number_of_levels = 0;
  (*stats).number_of_calls = 0;

}

void tsp_stats_delete(tsp_stats* stats) {

  (*stats).number_of_levels = 0;
  (*stats).number_of_calls = 0;

}
