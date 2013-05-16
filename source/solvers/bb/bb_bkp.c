#include "bb_bkp.h"

void bb_bkp_init(bb_bkp* bkp, int mode) {

  (*bkp).mode = mode;

  clist_init(&(*bkp).modified_constraints);

  (*bkp).curr_call = 0;

  (*bkp).curr_level = 0;

  onetree_init(&(*bkp).OT_CURR, 0);

  (*bkp).z_curr = 0.0;

}

void bb_bkp_delete(bb_bkp* bkp) {

  (*bkp).mode =  BASIC;

  clist_delete(&(*bkp).modified_constraints);

  (*bkp).curr_call = 0;

  (*bkp).curr_level = 0;

  onetree_delete(&(*bkp).OT_CURR);

  (*bkp).z_curr = 0.0;

}


void bb_bkp_save_constraint(bb_bkp* bkp, int x, int y, int constr) {

  clist_push_last(&(*bkp).modified_constraints, x, y, constr);

}

void bb_bkp_save(bb_bkp* bkp, bb_env* env) {

  (*bkp).curr_call = (*env).curr_call;

  (*bkp).curr_level = (*env).curr_level;

  if ((*bkp).mode == SAVE_OT) {

    onetree_copy(&(*env).OT_CURR, &(*bkp).OT_CURR);

    (*bkp).z_curr = (*env).z_curr;

  }

}

void bb_bkp_restore(bb_bkp* bkp, bb_env* env) {
  int i;
  cnstr c;

  int m = clist_get_size(&(*bkp).modified_constraints);
  clist_node* curr_node = clist_get_first(&(*bkp).modified_constraints);
  for (i = 0; i < m; i++) {
    c = clist_get_element(&(*bkp).modified_constraints, curr_node);
    graph_set_edge_constr(&(*env).G_CURR, c.x, c.y, c.constr);
    curr_node = clist_get_next(&(*bkp).modified_constraints, curr_node);
  }

  (*env).curr_call = (*bkp).curr_call;

  (*env).curr_level = (*bkp).curr_level;

  if ((*bkp).mode == SAVE_OT) {

    onetree_copy(&(*bkp).OT_CURR, &(*env).OT_CURR);

    (*env).z_curr = (*bkp).z_curr;

  }

}
