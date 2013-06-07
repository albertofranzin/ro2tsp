#include "tsp_env.h"

void tsp_env_init(tsp_env* env) {

  graph_init(&env->G_INPUT, 0);

  egraph_init(&env->EG_INPUT, 0);

  graph_init(&env->G_OUTPUT, 0);



  graph_init(&env->G_CURR, 0);

  onetree_init(&env->OT_CURR, 0);

  env->z_curr = 0.0;


  cycle_init(&env->TOUR_BEST, 0);

  env->incumbent = 0.0;


  cycle_init(&env->TOUR_OPT, 0);

  env->z_opt = 0.0;


  cycle_init(&env->TOUR_INIT, 0);

  env->dumb_ub = 0.0;

  env->init_ub = 0.0;

  env->init_lb = 0.0;


  env->curr_call = 0;

  env->curr_level = 0;

}

void tsp_env_delete(tsp_env* env) {

  graph_delete(&env->G_INPUT);

  graph_delete(&env->G_OUTPUT);

  egraph_delete(&env->EG_INPUT);



  graph_delete(&env->G_CURR);

  onetree_delete(&env->OT_CURR);

  env->z_curr = 0.0;


  cycle_delete(&env->TOUR_BEST);

  env->incumbent = 0.0;


  cycle_delete(&env->TOUR_OPT);

  env->z_opt = 0.0;


  cycle_delete(&env->TOUR_INIT);

  env->dumb_ub = 0.0;

  env->init_ub = 0.0;

  env->init_lb = 0.0;


  env->curr_call = 0;

  env->curr_level = 0;

}
