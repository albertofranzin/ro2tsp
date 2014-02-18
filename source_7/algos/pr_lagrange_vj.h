#ifndef PR_LAGRANGE_VJ_H_
#define PR_LAGRANGE_VJ_H_


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../base/global.h"
#include "../base/utils.h"

#include "../data/arraylist.h"
#include "../data/environment.h"
#include "../data/graph.h"
#include "../data/set.h"
#include "../data/tree.h"

#include "../algos/pr_onetree.h"

int pr_lagrange_vj(environment *env, int mode, double ub, tree *best_1t, double *best_mults, double *best_lb, int *status);

#endif /* PR_LAGRANGE_VJ_H_ */
