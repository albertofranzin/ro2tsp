#include "bb_env.h"

void bb_env_init(bb_env* env) {

  graph_init(&(*env).G_INPUT, 0);

  egraph_init(&(*env).EG_INPUT, 0);

  (*env).input_ub = 0.0;

  (*env).dumb_ub = 0.0;


  graph_init(&(*env).G_CURR, 0);

  onetree_init(&(*env).OT_CURR, 0);

  (*env).z_curr = 0.0;

  (*env).incumbent = 0.0;



  cycle_init(&(*env).TOUR_OPT , 0);

  (*env).z_opt = 0.0;


  (*env).curr_call = 0;

  (*env).curr_level = 0;

}

void bb_env_delete(bb_env* env) {

  graph_delete(&(*env).G_INPUT);

  egraph_delete(&(*env).EG_INPUT);

  (*env).input_ub = 0.0;

  (*env).dumb_ub = 0.0;


  graph_delete(&(*env).G_CURR);

  onetree_delete(&(*env).OT_CURR);

  (*env).z_curr = 0.0;

  (*env).incumbent = 0.0;



  cycle_delete(&(*env).TOUR_OPT);

  (*env).z_opt = 0.0;


  (*env).curr_call = 0;

  (*env).curr_level = 0;


}
