#ifndef SETNCHECK_H_
#define SETNCHECK_H_


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../../base/global.h"
#include "../../base/utils.h"

#include "../../data/arraylist.h"
#include "../../data/graph.h"
#include "../../data/set.h"
#include "../../data/tree.h"
#include "../../data/environment.h"

int setncheck(environment *env, arraylist *edgelist, tree *part_1t, set *part_vs, int *modedges, int *modcstrs, int *tmppreds, int *nm, int *status);

#endif /* SETNCHECK_H_ */
