#ifndef REDUCE_H_
#define REDUCE_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../base/global.h"
#include "../base/utils.h"

#include "../data/graph.h"
#include "../data/tree.h"
#include "../data/environment.h"
#include "../data/coords.h"
#include "../base/plot.h"

int reduce(graph *g, tree *min1t, double *mults, double lb, double ub, int *rmvedges, int *num_rmvedges);

#endif /* REDUCE_H_ */
