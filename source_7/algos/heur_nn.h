#ifndef HEUR_NN_H_
#define HEUR_NN_H_

#include <stdlib.h>
#include <stdio.h>

#include "../base/global.h"
#include "../base/utils.h"

#include "../data/graph.h"
#include "../data/cycle.h"

int heur_nn(graph *g, cycle* c, int start, double *ub);

#endif /* HEUR_NN_H_ */
