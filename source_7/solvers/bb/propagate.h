#ifndef PROPAGATE_H_
#define PROPAGATE_H_


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

int propagate(environment *env, int v, int *modedges, int *modcstrs, int *nm);

#endif /* PROPAGATE_H_ */
