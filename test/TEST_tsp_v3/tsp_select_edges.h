#ifndef TSP_SELECT_EDGES_H_
#define TSP_SELECT_EDGES_H_

#include "constants.h"
#include "graph.h"
#include "onetree.h"
#include "tsp_status.h"

// dato il nodo w, seleziona due lati {w, u} e {w, v} incidenti su w nell'1-albero OT_curr in status, e memorizza gli estremi u e v
void tsp_select_edges(tsp_status* status, int* w, int* v, int* u);

#endif
