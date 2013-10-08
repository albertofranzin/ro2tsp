#include "cpx_env.h"

void cpx_env_init(cpx_env* ce) {
  graph_init(&ce->G_INPUT, 0);
  egraph_init(&ce->EG_INPUT, 0);
  graph_init(&ce->G_OUTPUT, 0);
  graph_init(&ce->G, 0);
  onetree_init(&ce->OT, 0);
  cpx_table_init(&ce->T, NULL);
  cycle_init(&ce->TOUR_HEUR, 0);
  graph_init(&ce->TOUR_OPT, 0);

  ce->z_opt   = 0.0;
  ce->init_ub = 0.0;
  ce->init_lb = 0.0;
  ce->mylp    = NULL;
}


void cpx_env_delete(cpx_env* ce) {
  graph_delete(&ce->G_INPUT);
  graph_delete(&ce->G_OUTPUT);
  egraph_delete(&ce->EG_INPUT);
  graph_delete(&ce->G);
  onetree_delete(&ce->OT);
  cpx_table_delete(&ce->T);
  cycle_delete(&ce->TOUR_HEUR);
  graph_delete(&ce->TOUR_OPT);

  ce->z_opt   = 0.0;
  ce->init_ub = 0.0;
  ce->init_lb = 0.0;
  ce->mylp    = NULL;
}
