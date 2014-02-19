#ifndef HEUR_2OPT_H_
#define HEUR_2OPT_H_

#include <stdlib.h>
#include <stdio.h>

#include "../base/global.h"
#include "../base/utils.h"

#include "../data/graph.h"
#include "../data/cycle.h"

int heur_2opt(graph *g, cycle* c, double *ub);

#endif /* HEUR_2OPT_H_ */
