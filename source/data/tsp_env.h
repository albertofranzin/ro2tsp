#ifndef TSP_ENV_H_
#define TSP_ENV_H_



#include "graph.h"
#include "egraph.h"
#include "cycle.h"
#include "onetree.h"



typedef struct _tsp_env {

  graph G_INPUT;

  egraph EG_INPUT;

  graph G_OUTPUT;


  graph G_CURR;

  onetree OT_CURR;

  double z_curr;


  cycle TOUR_BEST;

  double incumbent;


  cycle TOUR_OPT;

  double z_opt;


  cycle TOUR_INIT;

  double dumb_ub;

  double init_ub;

  double init_lb;


  int curr_call;

  int curr_level;

} tsp_env;



void tsp_env_init(tsp_env* env);



void tsp_env_delete(tsp_env* env);



#endif
