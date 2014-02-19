#ifndef HEUR_3OPT_H_
#define HEUR_3OPT_H_

#include <stdlib.h>
#include <stdio.h>

#include "../base/global.h"
#include "../base/utils.h"

#include "../data/graph.h"
#include "../data/cycle.h"

int heur_3opt(graph *g, cycle* c, double *ub);

#endif /* HEUR_3OPT_H_ */
