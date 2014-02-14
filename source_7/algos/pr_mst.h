#ifndef PR_MST_H_
#define PR_MST_H_

#include <stdlib.h>
#include <stdio.h>

#include "../base/global.h"
#include "../base/utils.h"

#include "../data/graph.h"
#include "../data/tree.h"

int pr_mst(graph *g, tree* t, int root, int* status);

#endif /* PR_MST_H_ */
