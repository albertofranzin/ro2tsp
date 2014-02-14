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


int kr_lagrange_vj(environment *env, arraylist *edgelist, tree *part1t, set *partvs, int mode, double ub, tree *best1t, double *lb, double *gen_step, int *status);

#endif /* KR_LAGRANGE_VJ_H_ */
