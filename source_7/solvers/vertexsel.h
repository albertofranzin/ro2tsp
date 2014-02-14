#ifndef VERTEXSEL_H_
#define VERTEXSEL_H_


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../base/global.h"
#include "../base/utils.h"

#include "../data/graph.h"
#include "../data/tree.h"
#include "../data/environment.h"


/* select a vertex such that at least 3 edges are incident to it in t */;
int vertexsel(environment *env, tree *t, int *v);

#endif /* VERTEXSEL_H_ */
