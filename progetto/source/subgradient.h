#ifndef __SUBGRADIENT_H
#define __SUBGRADIENT_H

#include "compute_ot.h"
#include "graph.h"
#include "heuristics.h"
#include "lagrangian.h"

double *subgradient(graph *G, lagrangian *l);

#endif