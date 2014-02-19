#ifndef KR_LAGRANGE_VJ_H_
#define KR_LAGRANGE_VJ_H_


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

#include "../algos/compare_sort.h"
#include "../algos/counting_sort.h"
#include "../algos/radix_sort.h"
#include "../algos/kr_onetree.h"

int kr_lagrange_vj(environment *env, int mode, double ub, arraylist *edgelist, tree *part_1t, set *part_vs, tree *best_1t, double *best_mults, double *best_lb, int *status);

#endif /* KR_LAGRANGE_VJ_H_ */
