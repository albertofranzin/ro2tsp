#ifndef COMPUTE_NEAREST_NEIGHBOUR_H_
#define COMPUTE_NEAREST_NEIGHBOUR_H_

#include "../base/constants.h"
#include <stdio.h>
#include <string.h>
#include "../data/graph.h"
#include "../data/onetree.h"
#include "../data/tree.h"
#include "../data/cycle.h"


double compute_nearest_neighbour(graph *G, cycle *C, int start_node);

#endif
