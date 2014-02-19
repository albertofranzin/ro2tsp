#ifndef EDGESEL_H_
#define EDGESEL_H_


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../../base/global.h"
#include "../../base/utils.h"

#include "../../data/graph.h"
#include "../../data/tree.h"
#include "../../data/environment.h"

/* select two free edges incident to vertex v in t */
/* if there is only one free edge incident to v, then e2 < 0 */
/* if there is no free edge incident to v, then e1 < 0 and e2 < 0 */
int edgesel(environment *env, tree *t, int v, int *e1, int *e2);

#endif /* EDGESEL_H_ */
