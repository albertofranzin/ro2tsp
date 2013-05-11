#include "bb_stats.h"

void bb_stats_init(bb_stats* stats, int n) {

  (*stats).number_of_levels = 0;
  (*stats).number_of_calls = 0;

#ifdef DEBUG
  stats->pruned_branches = 0;
  stats->took1 = 0;
  stats->took2 = 0;
  stats->took3 = 0;
  stats->took4 = 0;
  stats->took5 = 0;
#endif

}

void bb_stats_delete(bb_stats* stats) {

  (*stats).number_of_levels = 0;
  (*stats).number_of_calls = 0;

#ifdef DEBUG
  stats->pruned_branches = 0;
  stats->took1 = 0;
  stats->took2 = 0;
  stats->took3 = 0;
  stats->took4 = 0;
  stats->took5 = 0;
#endif

}
