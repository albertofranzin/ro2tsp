#include "bb_propagate.h"


int bb_apply_and_check(int       w,
                       int       v,
                       int       u,
                       double    constr_wv,
                       double    constr_wu,
                       tsp_env  *env,
                       bb_bkp   *bkp,
                       bb_bkp   *update,
                       int       mode) {

  graph* G_CURR = &env->G_CURR;
  int n = G_CURR->n;


  bb_bkp_save_constraint(bkp, w, v, graph_get_edge_constr(G_CURR, w, v));
  // In fact when we apply this procedure we know for sure that (w, v) is
  // a free edge so it is not necessary to call graph_get_constr.

  graph_set_edge_constr(G_CURR, w, v, constr_wv);

  if (mode == FC_MODE) {
    bb_bkp_save_constraint(update, w, v, constr_wv);
  }


  if (u >= 0) {

    bb_bkp_save_constraint(bkp, w, u, graph_get_edge_constr(G_CURR, w, u));
    // In fact when we apply this procedure we know for sure that (w, v) is
    // a free edge so it is not necessary to call graph_get_constr.

    graph_set_edge_constr(G_CURR, w, u, constr_wu);

    if (mode == FC_MODE) {
      bb_bkp_save_constraint(update, w, u, constr_wu);
    }

  }

  // Check if the constraints just imposed prevent b&b from finding a feasible solution.
  if (G_CURR->V[w].deg_frc > 2 || G_CURR->V[w].deg_frb > n-3 ||
      G_CURR->V[v].deg_frc > 2 || G_CURR->V[v].deg_frc > n-3 ||
      (u >= 0 && (G_CURR->V[u].deg_frc > 2 || G_CURR->V[u].deg_frc > n-3))) {

    return FAILURE;
  
  }
  
  return SUCCESS;
}



/* This is another implementation of bb_propagate_and_check */

int bb_propagate_and_check(int      w,
                           int      v,
                           int      u,
                           int      constr_wv,
                           int      constr_wu,
                           tsp_env *env,
                           bb_bkp  *bkp,
                           bb_bkp  *update,
                           int      mode) {

  graph* G_CURR = &env->G_CURR;
  int n = env->G_CURR.n;

  int i, status;

  edge e;
  elist mod_constraints;

  elist_init(&mod_constraints);

  status = bb_apply_and_check(w, v, u, constr_wv, constr_wu, env, bkp, update, mode);
  if (status == FAILURE) return FAILURE;

  elist_push_last(&mod_constraints, w, v, 0.0, constr_wv);
  if (u >= 0) {
    elist_push_last(&mod_constraints, w, u, 0.0, constr_wu);
  }

  elist tmp_list;
  elist_init(&tmp_list);


  while (mod_constraints.size > 0) {

    elist_copy(&mod_constraints, &tmp_list);
    elist_delete(&mod_constraints);

    while (tmp_list.size > 0) {

      e = elist_pop_first(&tmp_list);

      // If possibile, constrain the edges incident to vertex constr.x which are not already forbidden nor forced.
      if (G_CURR->V[e.x].deg_frc >= 2 && G_CURR->V[e.x].deg_frb < n-3) {

	for (i = 0; i < n; i++) {
	  if (i != e.x) {

	    if (graph_get_edge_constr(G_CURR, e.x, i) == FREE) {

	      status = bb_apply_and_check(e.x, i, -1, FORBIDDEN, -1, env, bkp, update, mode);
	      if (status == FAILURE) return FAILURE;
	      elist_push_last(&mod_constraints, e.x, i, 0.0, FORBIDDEN);

	    }
	  }
	}
      }
      else if (G_CURR->V[e.x].deg_frc < 2 && G_CURR->V[e.x].deg_frb >= n-3) {

	for (i = 0; i < n; i++) {
	  if (i != e.x) {

	    if (graph_get_edge_constr(G_CURR, e.x, i) == FREE) {

	      status = bb_propagate_and_check(e.x, i, -1, FORCED, -1, env, bkp, update, mode);
	      if (status == FAILURE) return FAILURE;
	      elist_push_last(&mod_constraints, e.x, i, 0.0, FORCED);

	    }
	  }
	}
      }

      // If possibile, constrain the edges incident to vertex constr.y which are not already forbidden nor forced.
      if (G_CURR->V[e.y].deg_frc >= 2 && G_CURR->V[e.y].deg_frb < n-3) {

	for (i = 0; i < n; i++) {
	  if (i != e.y) {

	    if (graph_get_edge_constr(G_CURR, e.y, i) == FREE) {

	      status = bb_apply_and_check(e.y, i, -1, FORBIDDEN, -1, env, bkp, update, mode);
	      if (status == FAILURE) return FAILURE;
	      elist_push_last(&mod_constraints, e.y, i, 0.0, FORBIDDEN);

	    }
	  }
	}
      }
      else if (G_CURR->V[e.y].deg_frc < 2 && G_CURR->V[e.y].deg_frb >= n-3) {

	for (i = 0; i < n; i++) {
	  if (i != e.y) {

	    if (graph_get_edge_constr(G_CURR, e.y, i) == FREE) {

	      status = bb_propagate_and_check(e.y, i, -1, FORCED, -1, env, bkp, update, mode);
	      if (status == FAILURE) return FAILURE;
	      elist_push_last(&mod_constraints, e.y, i, 0.0, FORCED);

	    }
	  }
	}
      }

    }

  }

  elist_delete(&mod_constraints);
  elist_delete(&tmp_list);

  return SUCCESS;
}

