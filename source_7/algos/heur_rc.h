#ifndef HEUR_RC_H_
#define HEUR_RC_H_

#include <stdlib.h>
#include <stdio.h>

#include "../base/global.h"
#include "../base/utils.h"

#include "../data/graph.h"
#include "../data/cycle.h"


/**
 * parameter struct for rc_thread
 */
typedef struct _rc_params {

	graph  *g;
	cycle  *c;
	double  ub;
	int     th_no; // thread number_of_cycles
	int     num_cycles;
	int    *ones;
	int     n_ones;
	int    *zeros;
	int     n_zeros;
	//int     return_status;

} rc_params;


int heur_rc(graph *g, cycle* c, double *ub);


#endif /* HEUR_RC_H_ */
