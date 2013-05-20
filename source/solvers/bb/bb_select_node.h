#ifndef BB_SELECT_NODE_H_
#define BB_SELECT_NODE_H_

#include "../../base/constants.h"
#include "../../data/onetree.h"
#include "../../data/tsp_env.h"
//#include "bb_env.h"

// seleziona un nodo w dell'1-albero OT_curr in status
int bb_select_node(tsp_env* env, int* w, int opt);

#endif
