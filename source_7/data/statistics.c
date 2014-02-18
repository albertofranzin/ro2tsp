#include "../data/statistics.h"


int statistics_init(statistics *stats) {

	stats->num_nodes	= -1;
	stats->num_levels	= -1;
	stats->curr_node 	= -1;
	stats->curr_level 	= -1;
	return 0;

}

int statistics_delete(statistics *stats) {

	stats->num_nodes	= -1;
	stats->num_levels	= -1;
	stats->curr_node 	= -1;
	stats->curr_level 	= -1;
	return 0;

}
