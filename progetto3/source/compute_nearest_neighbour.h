#ifndef COMPUTE_NEAREST_NEIGHBOUR_H_
#define COMPUTE_NEAREST_NEIGHBOUR_H_

#include "constants.h"
#include <stdio.h>
#include <string.h>
#include "graph.h"
#include "onetree.h"
#include "tree.h"


double compute_nearest_neighbour(graph *G, tree *OT, int start_node);

#endif
