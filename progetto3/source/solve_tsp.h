#ifndef SOLVE_TSP_H_
#define SOLVE_TSP_H_

#include "constants.h"
#include "onetree.h"
#include "compute_lagrange.h"
#include "compute_ot.h"
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

void solve_tsp(graph* G, onetree* H, double* incumbent, int call_flag);
int is_cycle(onetree* OT);

#endif
