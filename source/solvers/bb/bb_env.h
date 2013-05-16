#ifndef BB_ENV_H_
#define BB_ENV_H_


#include "../../data/graph.h"
#include "../../data/egraph.h"
#include "../../data/cycle.h"
#include "../../data/onetree.h"

struct bb_env {

  graph G_INPUT;          // Input graph (mandatory).

  egraph EG_INPUT;

  double input_ub;        

  double dumb_ub;        



  graph G_CURR;           // All operations throughout execution will be performed on G_CURR which, at the beginning, will contain a copy of the input graph.
                          // Each time, G_CURR will be a copy of G_INPUT, where some edges may be subject to some constraint (one edge can be free, forced or forbidden).

  onetree OT_CURR;        // Here it will be saved the 1-tree computed at the current node of the recursione tree.

  double z_curr;

  double incumbent;




  cycle TOUR_OPT;

  double z_opt;



  int curr_call;

  int curr_level;

};

typedef struct bb_env bb_env;


void bb_env_init(bb_env* env);

void bb_env_delete(bb_env* env);

#endif