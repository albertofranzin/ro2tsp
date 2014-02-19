#ifndef KR_BB_H_
#define KR_BB_H_


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

#include "../../algos/kr_onetree.h"
#include "../../algos/kr_lagrange_vj.h"

#include "../../algos/reduce.h"


#include "../../solvers/bb/propagate.h"
#include "../../solvers/bb/setncheck.h"
#include "../../solvers/bb/vertexsel.h"
#include "../../solvers/bb/edgesel.h"


int kr_bb(environment *env, statistics *stats, arraylist *edgelist, tree *part1t, set *partvs, double *gap);

#endif /* KR_BB_H_ */
