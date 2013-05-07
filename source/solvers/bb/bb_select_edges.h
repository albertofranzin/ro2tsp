#ifndef BB_SELECT_EDGES_H_
#define BB_SELECT_EDGES_H_

#include "../../base/constants.h"
#include "../../data/graph.h"
#include "../../data/onetree.h"
#include "bb_status.h"

// dato il nodo w, seleziona due lati {w, u} e {w, v} incidenti su w nell'1-albero OT_curr in status, e memorizza gli estremi u e v
void bb_select_edges(bb_status* status, int* w, int* v, int* u);

#endif
