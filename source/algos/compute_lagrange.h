#ifndef COMPUTE_LAGRANGE_H_
#define COMPUTE_LAGRANGE_H_



#include <stdlib.h>
#include <math.h>
#include "../base/constants.h"
#include "../data/graph.h"
#include "../data/edge.h"
#include "../data/elist.h"
#include "../data/onetree.h"
#include "compute_min_ot.h"



int compute_lagrange(graph* G, onetree* OT, double ub, double* lb);



#endif

