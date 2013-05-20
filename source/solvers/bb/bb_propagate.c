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
  int n = (*G_CURR).n;


  bb_bkp_save_constraint(bkp, w, v, graph_get_edge_constr(G_CURR, w, v));
  // In fact when we apply this procedure we know for sure that (w, v) is
  // a free edge so it is not necessary to call graph_get_constr.

  graph_set_edge_constr(G_CURR, w, v, constr_wv);

  if (mode == FC_MODE) {
    bb_bkp_save_constraint(update, w, v, constr_wv);
  }


  if (u > 0) {

    bb_bkp_save_constraint(bkp, w, u, graph_get_edge_constr(G_CURR, w, u));
    // In fact when we apply this procedure we know for sure that (w, v) is
    // a free edge so it is not necessary to call graph_get_constr.

    graph_set_edge_constr(G_CURR, w, u, constr_wu);

    if (mode == FC_MODE) {
      bb_bkp_save_constraint(update, w, u, constr_wu);
    }

  }

  // Check if the constraints just imposed prevent b&b from finding a feasible solution.
  if (graph_get_node_deg_forced(G_CURR, w) > 2 || graph_get_node_deg_forbidden(G_CURR, w) > n-3 ||
      graph_get_node_deg_forced(G_CURR, v) > 2 || graph_get_node_deg_forbidden(G_CURR, v) > n-3 ||
      (u > 0 && (graph_get_node_deg_forced(G_CURR, u) > 2 || graph_get_node_deg_forbidden(G_CURR, u) > n-3))) {

    return FAILURE;
  
  }
  
  return SUCCESS;
}

/* An implementation of bb_propagate_and_check */
/*
int bb_propagate_and_check(int w, int v, int u, int constr_wv, int constr_wu, bb_env* env, bb_bkp* bkp, bb_bkp* update, int mode) {
  graph* G_CURR = env->G_CURR;
  int n = env->G_CURR.n;
  int i, status;

  status = bb_apply_and_check(w, v, u, constr_wv, constr_wu, env, bkp, update, mode);


  // If possibile, constrain the edges incident to vertex w which are not already forbidden nor forced.
  if (graph_get_node_deg_forced(G_CURR, w) >= 2 && graph_get_node_deg_forbidden(G_CURR, w) < n-3) {

    for (i = 1; i <= n; i++) {
      if (i != w) {

	if (graph_get_edge_constr(G_CURR, w, i) == FREE) {

	  status = bb_propagate_and_check(w, i, 0, FORBIDDEN, 0, env, bkp, update, mode);

	}
      }
    }
  }
  else if (graph_get_node_deg_forced(G_CURR, w) < 2 && graph_get_node_deg_forbidden(G_CURR, w) >= n-3) {

    for (i = 1; i <= n; i++) {
      if (i != w) {

	if (graph_get_edge_constr(G_CURR, w, i) == FREE) {

	  status = bb_propagate_and_check(w, i, 0, FORCED, 0, env, bkp, update, mode);


	}
      }
    }
  }

  // If possibile, constrain the edges incident to vertex v which are not already forbidden nor forced.
  if (graph_get_node_deg_forced(G_CURR, v) >= 2 && graph_get_node_deg_forbidden(G_CURR, v) < n-3) {

    for (i = 1; i <= n; i++) {
      if (i != v) {

	if (graph_get_edge_constr(G_CURR, v, i) == FREE) {

	  status = bb_propagate_and_check(v, i, 0, FORBIDDEN, 0, env, bkp, update, mode);


	}
      }
    }
  }
  else if (graph_get_node_deg_forced(G_CURR, v) < 2 && graph_get_node_deg_forbidden(G_CURR, v) >= n-3) {

    for (i = 1; i <= n; i++) {
      if (i != v) {

	if (graph_get_edge_constr(G_CURR, v, i) == FREE) {

	  status = bb_propagate_and_check(v, i, 0, FORCED, 0, env, bkp, update, mode);


	}
      }
    }
  }

  if (u > 0) {

    // If possibile, constrain the edges incident to vertex u which are not already forbidden nor forced.
    if (graph_get_node_deg_forced(G_CURR, u) >= 2 && graph_get_node_deg_forbidden(G_CURR, u) < n-3) {

      for (i = 1; i <= n; i++) { 
	if (i != u) {

	  if (graph_get_edge_constr(G_CURR, u, i) == FREE) {

	    status = bb_propagate_and_check(u, i, 0, FORBIDDEN, 0, env, bkp, update, mode);


	  }
	}
      }
    }
    else if (graph_get_node_deg_forced(G_CURR, u) < 2 && graph_get_node_deg_forbidden(G_CURR, u) >= n-3) {

      for (i = 1; i <= n; i++) {
	if (i != u) {

	  if (graph_get_edge_constr(G_CURR, u, i) == FREE) {

	    status = bb_propagate_and_check(u, i, 0, FORCED, 0, env, bkp, update, mode);


	  }
	}
      }
    }
  
  }

  return SUCCESS;
}
*/

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

  cnstr constr;
  clist mod_constraints;

  clist_init(&mod_constraints);

  status = bb_apply_and_check(w, v, u, constr_wv, constr_wu, env, bkp, update, mode);
  if (status == FAILURE) return FAILURE;

  clist_push_last(&mod_constraints, w, v, constr_wv);
  if (u > 0) {
    clist_push_last(&mod_constraints, w, u, constr_wu);
  }

  clist tmp_list;
  clist_init(&tmp_list);


  while (!clist_is_empty(&mod_constraints)) {

    clist_copy(&mod_constraints, &tmp_list);
    clist_empty(&mod_constraints);

    while (!clist_is_empty(&tmp_list)) {

      constr = clist_pop_first(&tmp_list);

      // If possibile, constrain the edges incident to vertex constr.x which are not already forbidden nor forced.
      if (graph_get_node_deg_forced(G_CURR, constr.x) >= 2 && graph_get_node_deg_forbidden(G_CURR, constr.x) < n-3) {

	for (i = 1; i <= n; i++) {
	  if (i != constr.x) {

	    if (graph_get_edge_constr(G_CURR, constr.x, i) == FREE) {

	      status = bb_apply_and_check(constr.x, i, 0, FORBIDDEN, 0, env, bkp, update, mode);
	      if (status == FAILURE) return FAILURE;
	      clist_push_last(&mod_constraints, constr.x, i, FORBIDDEN);

	    }
	  }
	}
      }
      else if (graph_get_node_deg_forced(G_CURR, constr.x) < 2 && graph_get_node_deg_forbidden(G_CURR, constr.x) >= n-3) {

	for (i = 1; i <= n; i++) {
	  if (i != constr.x) {

	    if (graph_get_edge_constr(G_CURR, constr.x, i) == FREE) {

	      status = bb_propagate_and_check(constr.x, i, 0, FORCED, 0, env, bkp, update, mode);
	      if (status == FAILURE) return FAILURE;
	      clist_push_last(&mod_constraints, constr.x, i, FORCED);

	    }
	  }
	}
      }

      // If possibile, constrain the edges incident to vertex constr.y which are not already forbidden nor forced.
      if (graph_get_node_deg_forced(G_CURR, constr.y) >= 2 && graph_get_node_deg_forbidden(G_CURR, constr.y) < n-3) {

	for (i = 1; i <= n; i++) {
	  if (i != constr.y) {

	    if (graph_get_edge_constr(G_CURR, constr.y, i) == FREE) {

	      status = bb_apply_and_check(constr.y, i, 0, FORBIDDEN, 0, env, bkp, update, mode);
	      if (status == FAILURE) return FAILURE;
	      clist_push_last(&mod_constraints, constr.y, i, FORBIDDEN);

	    }
	  }
	}
      }
      else if (graph_get_node_deg_forced(G_CURR, constr.y) < 2 && graph_get_node_deg_forbidden(G_CURR, constr.y) >= n-3) {

	for (i = 1; i <= n; i++) {
	  if (i != constr.y) {

	    if (graph_get_edge_constr(G_CURR, constr.y, i) == FREE) {

	      status = bb_propagate_and_check(constr.y, i, 0, FORCED, 0, env, bkp, update, mode);
	      if (status == FAILURE) return FAILURE;
	      clist_push_last(&mod_constraints, constr.y, i, FORCED);

	    }
	  }
	}
      }

    }

  }

  clist_delete(&mod_constraints);
  clist_delete(&tmp_list);

  return SUCCESS;
}

