#ifndef BB_SELECT_EDGES_H_
#define BB_SELECT_EDGES_H_

#include "../../base/constants.h"
#include "../../data/graph.h"
#include "../../data/onetree.h"
#include "../../data/tsp_env.h"

// dato il nodo w, seleziona due lati {w, u} e {w, v} incidenti su w nell'1-albero OT_curr in status, e memorizza gli estremi u e v
int bb_select_edges(tsp_env* env, int* w, int* v, int* u, int opt);

#endif
