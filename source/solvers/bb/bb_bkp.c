#include "bb_bkp.h"



void bb_bkp_init(bb_bkp* bkp, int mode) {

  bkp->mode = mode;

  elist_init(&(bkp->modified_constraints));

  bkp->curr_call = 0;

  bkp->curr_level = 0;

  onetree_init(&(bkp->OT_CURR), 0);

  bkp->z_curr = 0.0;

}



void bb_bkp_delete(bb_bkp* bkp) {

  bkp->mode =  BASIC;

  elist_delete(&(bkp->modified_constraints));

  bkp->curr_call = 0;

  bkp->curr_level = 0;

  onetree_delete(&(bkp->OT_CURR));

  bkp->z_curr = 0.0;

}



void bb_bkp_save_constraint(bb_bkp* bkp, int x, int y, int constr) {

  elist_push_last(&(bkp->modified_constraints), x, y, 0.0, constr);

}



void bb_bkp_save(bb_bkp* bkp, tsp_env* env) {

  bkp->curr_call = env->curr_call;

  bkp->curr_level = env->curr_level;

  if (bkp->mode == SAVE_OT) {

    onetree_copy(&(env->OT_CURR), &(bkp->OT_CURR));

    bkp->z_curr = env->z_curr;

  }

}



void bb_bkp_restore(bb_bkp* bkp, tsp_env* env) {

  int i;
  edge e;
  int m = bkp->modified_constraints.size;
  elist_node* curr_node = bkp->modified_constraints.head.next;

  for (i = 0; i < m; i++) {
    e = curr_node->data;
    graph_set_edge_constr(&(env->G_CURR), e.x, e.y, e.constr);
    curr_node = curr_node->next;
  }

  env->curr_call = bkp->curr_call;

  env->curr_level = bkp->curr_level;

  if (bkp->mode == SAVE_OT) {

    onetree_copy(&(bkp->OT_CURR), &(env->OT_CURR));

    env->z_curr = bkp->z_curr;

  }

}
