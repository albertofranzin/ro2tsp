#include "bb_stats.h"

void bb_stats_init(bb_stats* stats, int n) {

  (*stats).number_of_levels = 0;
  (*stats).number_of_calls = 0;

}

void bb_stats_delete(bb_stats* stats) {

  (*stats).number_of_levels = 0;
  (*stats).number_of_calls = 0;

}
