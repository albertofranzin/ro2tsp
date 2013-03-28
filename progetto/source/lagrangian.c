#include "lagrangian.h"

/*
 * initializer
 * -n : number of nodes in the graph
 *
 * default values are initialized here
 * Later may be parameters...
 */
lagrangian* initLagrange(int n) {
	lagrangian l = malloc(sizeof(lagrangian));

	// we need |V| multipliers
	l->mult = malloc(sizeof(double) * n);
	memset(l->mult, 0, sizeof(double) * n);

	// 100 steps is the suggested value
	l->decreased_ago = 0;
	l->wait_for = 100;

	// alpha spans from 0 to 2, according to suggestions
	l->alpha = 2.0;

	return l;
}