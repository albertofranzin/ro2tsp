#ifndef LAGRANGE_H_
#define LAGRANGE_H_


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../base/global.h"
#include "../data/graph.h"
#include "../data/tree.h"
#include "../algos/prim_1tree.h"


int lagrange(graph *g, double ub, int max_iter, int max_noimp_iter, int alpha_ht, tree* t, double *lb);


#endif /* LAGRANGE_H_ */
