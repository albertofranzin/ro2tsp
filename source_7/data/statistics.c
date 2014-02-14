#include "../data/statistics.h"


int statistics_init(statistics *stats) {

	stats->bb_nod_num	=  0;
	stats->bb_lev_num	=  0;
	stats->reduce_nr 	= -1;
	return 0;

}

int statistics_delete(statistics *stats) {

	stats->bb_nod_num	=  0;
	stats->bb_lev_num	=  0;
	stats->reduce_nr 	= -1;
	return 0;

}
