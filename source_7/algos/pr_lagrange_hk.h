#ifndef PR_LAGRANGE_HK_H_
#define PR_LAGRANGE_HK_H_


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

int pr_lagrange_hk(environment *env, double ub, int max_num_step, int max_num_noimprov, int alpha_ht, tree* t, double *best_mults, double *best_lb, int *status);
#endif /* PR_LAGRANGE_HK_H_ */
