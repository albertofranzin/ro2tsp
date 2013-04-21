#ifndef COMPUTE_NEAREST_NEIGHBOUR_H_
#define COMPUTE_NEAREST_NEIGHBOUR_H_

#include "constants.h"
#include <stdio.h>
#include <string.h>
#include "graph.h"
#include "onetree.h"
#include "tree.h"
#include "cycle.h"


double compute_nearest_neighbour(graph *G, cycle *C, int start_node);

#endif
