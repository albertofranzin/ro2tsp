#ifndef TSP_ENV_H_
#define TSP_ENV_H_


#include "graph.h"
#include "egraph.h"
#include "cycle.h"
#include "onetree.h"

typedef struct _tsp_env {

  graph G_INPUT;          // Input graph (mandatory).
  graph G_OUTPUT;

  egraph EG_INPUT;

  double input_ub;

  double dumb_ub;

  graph G_CURR;
  // All operations throughout execution will be performed on G_CURR
  // which, at the beginning, will contain a copy of the input graph.
  // Each time, G_CURR will be a copy of G_INPUT, where some edges
  // may be subject to some constraint
  // (one edge can be free, forced or forbidden).

  onetree OT_CURR;
  // Here it will be saved the 1-tree computed at the current node
  // of the recursione tree.

  double z_curr;

  double incumbent;

  cycle TOUR_OPT;

  double z_opt;

  int curr_call;

  int curr_level;

} tsp_env;

void tsp_env_init(tsp_env* env);

void tsp_env_delete(tsp_env* env);

#endif
