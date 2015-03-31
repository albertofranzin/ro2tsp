#ifndef PR_BB_H_
#define PR_BB_H_


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../../base/global.h"
#include "../../base/utils.h"
#include "../../base/plot.h"

#include "../../data/graph.h"
#include "../../data/tree.h"
#include "../../data/environment.h"
#include "../../data/statistics.h"
#include "../../data/set.h"
#include "../../data/arraylist.h"

#include "../../algos/pr_onetree.h"
#include "../../algos/pr_lagrange_vj.h"

#include "../../solvers/bb/vertexsel.h"
#include "../../solvers/bb/edgesel.h"


int pr_bb(environment *env, statistics *stats);

#endif /* PR_BB_H_ */
