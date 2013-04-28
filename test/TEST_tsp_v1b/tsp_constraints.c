#include "tsp_constraints.h"

void tsp_constraints_a(int w, int v, int u, double cost_wv, double cost_wu, tsp_status* status, tsp_backup* backup, tsp_backup* update) {

  graph* G_curr = &(*status).G_curr;
  int n = (*status).G_curr.n;
  double c;
  int i;

  tsp_backup_save_edge(backup, w, v, graph_get_edge_cost(G_curr, w, v));

  graph_set_edge_cost(G_curr, w, v, cost_wv);

  if (update != NULL) {
    tsp_backup_save_edge(update, w, v, cost_wv);
  }

  if (cost_wv <= SMALL) {
    (*status).num_forced_edges.array[w-1]++;
    (*status).num_forced_edges.array[v-1]++;
  }
  else if (cost_wv >= BIG) {
    (*status).num_forbidden_edges.array[w-1]++;
    (*status).num_forbidden_edges.array[v-1]++;
  }

  if (u > 0) {

    tsp_backup_save_edge(backup, w, u, graph_get_edge_cost(G_curr, w, u));

    graph_set_edge_cost(G_curr, w, u, cost_wu);

    if (update != NULL) {
      tsp_backup_save_edge(update, w, u, cost_wu);
    }

    if (cost_wu <= SMALL) {
      (*status).num_forced_edges.array[w-1]++;
      (*status).num_forced_edges.array[u-1]++;
    }
    else if (cost_wu >= BIG) {
      (*status).num_forbidden_edges.array[w-1]++;
      (*status).num_forbidden_edges.array[u-1]++;
    }

  }

}

void tsp_constraints_ap_simple(int w, int v, int u, double cost_wv, double cost_wu, tsp_status* status, tsp_backup* backup, tsp_backup* update) {
  graph* G_curr = &(*status).G_curr;
  int n = (*status).G_curr.n;
  double c;
  int i;

  tsp_constraints_a(w, v, u, cost_wv, cost_wu, status, backup, update);

  // propagazione su nodo w 
  if ((*status).num_forced_edges.array[w-1] >= 2 && (*status).num_forbidden_edges.array[w-1] < n-3) {
    for (i = 1; i <= n; i++) {
      if (i != w) {
	c = graph_get_edge_cost(G_curr, w, i);
	if (c > SMALL && c < BIG) {

	  tsp_constraints_a(w, i, 0, BIG, 0.0, status, backup, update);

	}
      }
    }
  }
  else if ((*status).num_forbidden_edges.array[w-1] >= n-3 && (*status).num_forced_edges.array[w-1] < 2) {
    for (i = 1; i <= n; i++) {
      if (i != w) {
	c = graph_get_edge_cost(G_curr, w, i);
	if (c > SMALL && c < BIG) {

	  tsp_constraints_a(w, i, 0, SMALL, 0.0, status, backup, update);

	}
      }
    }
  }

  // propagazione su nodo v
  if ((*status).num_forced_edges.array[v-1] >= 2 && (*status).num_forbidden_edges.array[v-1] < n-3) {
    for (i = 1; i <= n; i++) {
      if (i != v) {
	c = graph_get_edge_cost(G_curr, v, i);
	if (c > SMALL && c < BIG) {

	  tsp_constraints_a(v, i, 0, BIG, 0.0, status, backup, update);

	}
      }
    }
  }
  else if ((*status).num_forbidden_edges.array[v-1] >= n-3 && (*status).num_forced_edges.array[v-1] < 2) {
    for (i = 1; i <= n; i++) {
      if (i != v) {
	c = graph_get_edge_cost(G_curr, v, i);
	if (c > SMALL && c < BIG) {

	  tsp_constraints_a(v, i, 0, SMALL, 0.0, status, backup, update);

	}
      }
    }
  }


  if (u > 0) {

    // propagazione su nodo u
    if ((*status).num_forced_edges.array[u-1] >= 2 && (*status).num_forbidden_edges.array[u-1] < n-3) {
      for (i = 1; i <= n; i++)  {
	if (i != u) {
	  c = graph_get_edge_cost(G_curr, u, i);
	  if (c > SMALL && c < BIG) {

	    tsp_constraints_a(u, i, 0, BIG, 0.0, status, backup, update);

	  }
	}
      }
    }
    else if ((*status).num_forbidden_edges.array[u-1] >= n-3 && (*status).num_forced_edges.array[u-1] < 2) {
      for (i = 1; i <= n; i++) {
	if (i != u) {
	  c = graph_get_edge_cost(G_curr, u, i);
	  if (c > SMALL && c < BIG) {

	    tsp_constraints_a(u, i, 0, SMALL, 0.0, status, backup, update);

	  }
	}
      }
    }
  }

  
}


void tsp_constraints_ap_recursive(int w, int v, int u, double cost_wv, double cost_wu, tsp_status* status, tsp_backup* backup, tsp_backup* update) {
  graph* G_curr = &(*status).G_curr;
  int n = (*status).G_curr.n;
  double c;
  int i;

  tsp_constraints_a(w, v, u, cost_wv, cost_wu, status, backup, update);
  
  // propagazione su nodo w 
  if ((*status).num_forced_edges.array[w-1] == 2 && (*status).num_forbidden_edges.array[w-1] < n-3) {
    for (i = 1; i <= n; i++) {
      if (i != w) {
	c = graph_get_edge_cost(G_curr, w, i);
	if (c > SMALL && c < BIG) {

	  tsp_constraints_ap_recursive(w, i, 0, BIG, 0.0, status, backup, update);

	}
      }
    }
  }
  else if ((*status).num_forbidden_edges.array[w-1] == n-3 && (*status).num_forced_edges.array[w-1] < 2) {
    for (i = 1; i <= n; i++) {
      if (i != w) {
	c = graph_get_edge_cost(G_curr, w, i);
	if (c > SMALL && c < BIG) {

	  tsp_constraints_ap_recursive(w, i, 0, SMALL, 0.0, status, backup, update);

	}
      }
    }
  }

  // propagazione su nodo v
  if ((*status).num_forced_edges.array[v-1] == 2 && (*status).num_forbidden_edges.array[v-1] < n-3) {
    for (i = 1; i <= n; i++) {
      if (i != v) {
	c = graph_get_edge_cost(G_curr, v, i);
	if (c > SMALL && c < BIG) {

	  tsp_constraints_ap_recursive(v, i, 0, BIG, 0.0, status, backup, update);

	}
      }
    }
  }
  else if ((*status).num_forbidden_edges.array[v-1] == n-3 && (*status).num_forced_edges.array[v-1] < 2) {
    for (i = 1; i <= n; i++) {
      if (i != v) {
	c = graph_get_edge_cost(G_curr, v, i);
	if (c > SMALL && c < BIG) {

	  tsp_constraints_ap_recursive(v, i, 0, SMALL, 0.0, status, backup, update);

	}
      }
    }
  }


  if (u > 0) {

    // propagazione su nodo u
    if ((*status).num_forced_edges.array[u-1] == 2 && (*status).num_forbidden_edges.array[u-1] < n-3) {
      for (i = 1; i <= n; i++)  {
	if (i != u) {
	  c = graph_get_edge_cost(G_curr, u, i);
	  if (c > SMALL && c < BIG) {

	    tsp_constraints_ap_recursive(u, i, 0, BIG, 0.0, status, backup, update);

	  }
	}
      }
    }
    else if ((*status).num_forbidden_edges.array[u-1] == n-3 && (*status).num_forced_edges.array[u-1] < 2) {
      for (i = 1; i <= n; i++) {
	if (i != u) {
	  c = graph_get_edge_cost(G_curr, u, i);
	  if (c > SMALL && c < BIG) {

	    tsp_constraints_ap_recursive(u, i, 0, SMALL, 0.0, status, backup, update);

	  }
	}
      }
    }
  }

  
}


void tsp_constraints_ap_iterative(int w, int v, int u, double cost_wv, double cost_wu, tsp_status* status, tsp_backup* backup, tsp_backup* update) {
  graph* G_curr = &(*status).G_curr;
  int n = (*status).G_curr.n;
  double c;
  int i;
  edge e;

  elist modified;
  elist_init(&modified);

  tsp_constraints_a(w, v, u, cost_wv, cost_wu, status, backup, update);
  elist_push_last(&modified, w, v, cost_wv);
  elist_push_last(&modified, w, u, cost_wu);

  elist tmplist;
  elist_init(&tmplist);

  while (!elist_is_empty(&modified)) {

    elist_copy(&modified, &tmplist);
    elist_empty(&modified);

    while (!elist_is_empty(&tmplist)) {

      e = elist_pop_first(&tmplist);

      // propagazione su nodo e.x
      if ((*status).num_forced_edges.array[e.x-1] == 2 && (*status).num_forbidden_edges.array[e.x-1] < n-3) {
	for (i = 1; i <= n; i++) {
	  if (i != e.x) {
	    c = graph_get_edge_cost(G_curr, e.x, i);
	    if (c > SMALL && c < BIG) {

	      tsp_constraints_a(e.x, i, 0, BIG, 0.0, status, backup, update);
	      elist_push_last(&modified, e.x, i, BIG);

	    }
	  }
	}
      }
      else if ((*status).num_forbidden_edges.array[e.x-1] == n-3 && (*status).num_forced_edges.array[e.x-1] < 2) {
	for (i = 1; i <= n; i++) {
	  if (i != e.x) {
	    c = graph_get_edge_cost(G_curr, e.x, i);
	    if (c > SMALL && c < BIG) {

	      tsp_constraints_a(e.x, i, 0, SMALL, 0.0, status, backup, update);
	      elist_push_last(&modified, e.x, i, SMALL);

	    }
	  }
	}
      }

      // propagazione su nodo e.y
      if ((*status).num_forced_edges.array[e.y-1] == 2 && (*status).num_forbidden_edges.array[e.y-1] < n-3) {
	for (i = 1; i <= n; i++) {
	  if (i != e.y) {
	    c = graph_get_edge_cost(G_curr, e.y, i);
	    if (c > SMALL && c < BIG) {

	      tsp_constraints_a(e.y, i, 0, BIG, 0.0, status, backup, update);
	      elist_push_last(&modified, e.y, i, BIG);

	    }
	  }
	}
      }
      else if ((*status).num_forbidden_edges.array[e.y-1] == n-3 && (*status).num_forced_edges.array[e.y-1] < 2) {
	for (i = 1; i <= n; i++) {
	  if (i != e.y) {
	    c = graph_get_edge_cost(G_curr, e.y, i);
	    if (c > SMALL && c < BIG) {

	      tsp_constraints_a(e.y, i, 0, BIG, 0.0, status, backup, update);
	      elist_push_last(&modified, e.y, i, SMALL);

	    }
	  }
	}
      }

    }
  }

  elist_delete(&modified);
  elist_delete(&tmplist);
}


int tsp_constraints_ac(int w, int v, int u, double cost_wv, double cost_wu, tsp_status* status, tsp_backup* backup, tsp_backup* update) {
  graph* G_curr = &(*status).G_curr;
  int n = (*status).G_curr.n;
  double c;
  int i;

  tsp_constraints_a(w, v, u, cost_wv, cost_wu, status, backup, update);

  /*
  if ((*status).num_forced_edges.array[w-1] > 2 || (*status).num_forbidden_edges.array[w-1] > n-3) {
    return FAILURE;
  }
  if ((*status).num_forced_edges.array[v-1] > 2 || (*status).num_forbidden_edges.array[v-1] > n-3) {
    return FAILURE;
  }
  if (u > 0) {
    if ((*status).num_forced_edges.array[w-1] > 2 || (*status).num_forbidden_edges.array[w-1] > n-3) {
      return FAILURE;
    }
  }
  */
 
  
  if (u > 0) {
    if (cost_wv <= SMALL && cost_wu <= SMALL) {
      if ((*status).num_forced_edges.array[w-1] > 2 || (*status).num_forced_edges.array[v-1] > 2 || (*status).num_forced_edges.array[u-1] > 2)
	return FAILURE;
    }
    else if (cost_wv <= SMALL && cost_wu >= BIG) { // basterebbe un else anzichè un else if tanto i casi successivi non si verificheranno
      if ((*status).num_forced_edges.array[w-1] > 2 || (*status).num_forced_edges.array[v-1] > 2 || (*status).num_forbidden_edges.array[w-1] > n-3 || (*status).num_forbidden_edges.array[u-1] > n-3)
	return FAILURE;
    }
    else if (cost_wv >= BIG && cost_wu <= SMALL) { // non si verificherà mai per come è fatto il branch and bound
      if ((*status).num_forbidden_edges.array[w-1] > n-3 || (*status).num_forbidden_edges.array[v-1] > n-3 || (*status).num_forced_edges.array[w-1] > 2 || (*status).num_forced_edges.array[u-1] > 2)
	return FAILURE;
    }
    else if (cost_wv >= BIG && cost_wu >= BIG) { // non si verificherà mai per come è fatto il branch and bound
      if ((*status).num_forbidden_edges.array[w-1] > n-3 || (*status).num_forbidden_edges.array[v-1] > n-3 || (*status).num_forbidden_edges.array[u-1] > n-3)
	return FAILURE;
    }
  }
  else {
    if (cost_wv <= SMALL) {
      if ((*status).num_forced_edges.array[w-1] > 2 || (*status).num_forced_edges.array[v-1] > 2)
	return FAILURE;
    }
    else {
      if ((*status).num_forbidden_edges.array[w-1] > n-3 || (*status).num_forbidden_edges.array[v-1] > n-3)
	return FAILURE;
    }
  }
  
  
  return SUCCESS;
}


int tsp_constraints_ace(int w, int v, int u, double cost_wv, double cost_wu, tsp_status* status, tsp_backup* backup, tsp_backup* update) {
  graph* G_curr = &(*status).G_curr;
  lvector* forced_edges = &(*status).forced_edges;
  int n = (*status).G_curr.n;
  double c;
  int i;

  if (cost_wv <= SMALL) {

    int wp, ws, vp, vs, p, s, t, k;
    wp = lvector_get_pred(forced_edges, w);
    ws = lvector_get_succ(forced_edges, w);
    vp = lvector_get_pred(forced_edges, v);
    vs = lvector_get_succ(forced_edges, v);

    if (wp == 0 && ws == 0 && vp == 0 && vs == 0) {
      lvector_insert_link(forced_edges, w, v);
    }
    else if ((wp != 0 && ws != 0) || (vp != 0 && vs != 0)) {
      return FAILURE;
    }
    else if (wp != 0 && ws == 0 && vp == 0 && vs == 0) {
      lvector_insert_link(forced_edges, w, v);
    }
    else if (wp == 0 && ws != 0 && vp == 0 && vs == 0) {
      lvector_insert_link(forced_edges, v, w); 
    }
    else if (wp == 0 && ws == 0 && vp != 0 && vs == 0) {
      lvector_insert_link(forced_edges, v, w);
    }
    else if (wp == 0 && ws == 0 && vp == 0 && vs != 0) {
      lvector_insert_link(forced_edges, w, v);
    }
    else if (wp != 0 && ws == 0 && vp == 0 && vs != 0) {

      p = v;
      s = lvector_get_succ(forced_edges, v);
      k = 1;
      while (s != 0 && s != w) {
	p = s;
	s = lvector_get_succ(forced_edges, s);
	k++;
      }
      if (s == w && k < n-1) {
	return FAILURE;
      }
      else {
	lvector_insert_link(forced_edges, w, v);
      }

    }
    else if (wp == 0 && ws != 0 && vp != 0 && vs == 0) {

      p = w;
      s = lvector_get_succ(forced_edges, w);
      k = 1;
      while (s != 0 && s != v) {
	p = s;
	s = lvector_get_succ(forced_edges, s);
	k++;
      }
      if (s == v && k < n-1) {
	return FAILURE;
      }
      else {
	lvector_insert_link(forced_edges, v, w);
      }

    }
    else if (wp != 0 && ws == 0 && vp != 0 && vs == 0) {

      p = v;
      s = lvector_get_pred(forced_edges, p);
      while (s != 0) {
	t = lvector_get_pred(forced_edges, s);
	if (p == v) {
	  (*forced_edges).array[p-1].pred = 0;
	}
	(*forced_edges).array[p-1].succ = s;
	(*forced_edges).array[s-1].pred = p;
	if (t == 0) {
	  (*forced_edges).array[s-1].succ = 0;
	}
	p = s;
	s = t;
      }
      lvector_insert_link(forced_edges, w, v);

    }
    else if (wp == 0 && ws != 0 && vp == 0 && vs != 0) {

      s = w;
      p = lvector_get_succ(forced_edges, w);
      while (p != 0) {
	t = lvector_get_succ(forced_edges, p);
	if (s == w) {
	  (*forced_edges).array[s-1].succ = 0;
	}
	(*forced_edges).array[s-1].pred = p;
	(*forced_edges).array[p-1].succ = s;
	if (t == 0) {
	  (*forced_edges).array[p-1].pred = 0;
	}
	s = p;
	p = t;
      }
      lvector_insert_link(forced_edges, w, v);
    }
 
  } 
  if (u > 0) {
    if (cost_wu <= SMALL) {

      int wp, ws, up, us, p, s, t, k;
      wp = lvector_get_pred(forced_edges, w);
      ws = lvector_get_succ(forced_edges, w);
      up = lvector_get_pred(forced_edges, u);
      us = lvector_get_succ(forced_edges, u);

      if (wp == 0 && ws == 0 && up == 0 && us == 0) {
	lvector_insert_link(forced_edges, w, u);
      }
      else if ((wp != 0 && ws != 0) || (up != 0 && us != 0)) {
	return FAILURE;
      }
      else if (wp != 0 && ws == 0 && up == 0 && us == 0) {
	lvector_insert_link(forced_edges, w, u);
      }
      else if (wp == 0 && ws != 0 && up == 0 && us == 0) {
	lvector_insert_link(forced_edges, u, w); 
      }
      else if (wp == 0 && ws == 0 && up != 0 && us == 0) {
	lvector_insert_link(forced_edges, u, w);
      }
      else if (wp == 0 && ws == 0 && up == 0 && us != 0) {
	lvector_insert_link(forced_edges, w, u);
      }
      else if (wp != 0 && ws == 0 && up == 0 && us != 0) {

	p = u;
	s = lvector_get_succ(forced_edges, u);
	k = 1;
	while (s != 0 && s != w) {
	  p = s;
	  s = lvector_get_succ(forced_edges, s);
	  k++;
	}
	if (s == w && k < n-1) {
	  return FAILURE;
	}
	else {
	  lvector_insert_link(forced_edges, w, u);
	}

      }
      else if (wp == 0 && ws != 0 && up != 0 && us == 0) {

	p = w;
	s = lvector_get_succ(forced_edges, w);
	k = 1;
	while (s != 0 && s != u) {
	  p = s;
	  s = lvector_get_succ(forced_edges, s);
	  k++;
	}
	if (s == v && k < n-1) {
	  return FAILURE;
	}
	else {
	  lvector_insert_link(forced_edges, u, w);
	}

      }
      else if (wp != 0 && ws == 0 && up != 0 && us == 0) {

	p = u;
	s = lvector_get_pred(forced_edges, p);
	while (s != 0) {
	  t = lvector_get_pred(forced_edges, s);
	  if (p == u) {
	    (*forced_edges).array[p-1].pred = 0;
	  }
	  (*forced_edges).array[p-1].succ = s;
	  (*forced_edges).array[s-1].pred = p;
	  if (t == 0) {
	    (*forced_edges).array[s-1].succ = 0;
	  }
	  p = s;
	  s = t;
	}
	lvector_insert_link(forced_edges, w, u);

      }
      else if (wp == 0 && ws != 0 && up == 0 && us != 0) {

	s = w;
	p = lvector_get_succ(forced_edges, w);
	while (p != 0) {
	  t = lvector_get_succ(forced_edges, p);
	  if (s == w) {
	    (*forced_edges).array[s-1].succ = 0;
	  }
	  (*forced_edges).array[s-1].pred = p;
	  (*forced_edges).array[p-1].succ = s;
	  if (t == 0) {
	    (*forced_edges).array[p-1].pred = 0;
	  }
	  s = p;
	  p = t;
	}
	lvector_insert_link(forced_edges, w, u);
      }
    
    }
  }

  tsp_constraints_a(w, v, u, cost_wv, cost_wu, status, backup, update);

  /*
  if ((*status).num_forced_edges.array[w-1] > 2 || (*status).num_forbidden_edges.array[w-1] > n-3) {
    return FAILURE;
  }
  if ((*status).num_forced_edges.array[v-1] > 2 || (*status).num_forbidden_edges.array[v-1] > n-3) {
    return FAILURE;
  }
  if (u > 0) {
    if ((*status).num_forced_edges.array[w-1] > 2 || (*status).num_forbidden_edges.array[w-1] > n-3) {
      return FAILURE;
    }
  }
  */  
  
  if (u > 0) {
    if (cost_wv <= SMALL && cost_wu <= SMALL) {
      if ((*status).num_forced_edges.array[w-1] > 2 || (*status).num_forced_edges.array[v-1] > 2 || (*status).num_forced_edges.array[u-1] > 2)
	return FAILURE;
    }
    else if (cost_wv <= SMALL && cost_wu >= BIG) { // basterebbe un else anzichè un else if tanto i casi successivi non si verificheranno
      if ((*status).num_forced_edges.array[w-1] > 2 || (*status).num_forced_edges.array[v-1] > 2 || (*status).num_forbidden_edges.array[w-1] > n-3 || (*status).num_forbidden_edges.array[u-1] > n-3)
	return FAILURE;
    }
    else if (cost_wv >= BIG && cost_wu <= SMALL) { // non si verificherà mai per come è fatto il branch and bound
      if ((*status).num_forbidden_edges.array[w-1] > n-3 || (*status).num_forbidden_edges.array[v-1] > n-3 || (*status).num_forced_edges.array[w-1] > 2 || (*status).num_forced_edges.array[u-1] > 2)
	return FAILURE;
    }
    else if (cost_wv >= BIG && cost_wu >= BIG) { // non si verificherà mai per come è fatto il branch and bound
      if ((*status).num_forbidden_edges.array[w-1] > n-3 || (*status).num_forbidden_edges.array[v-1] > n-3 || (*status).num_forbidden_edges.array[u-1] > n-3)
	return FAILURE;
    }
  }
  else {
    if (cost_wv <= SMALL) {
      if ((*status).num_forced_edges.array[w-1] > 2 || (*status).num_forced_edges.array[v-1] > 2)
	return FAILURE;
    }
    else {
      if ((*status).num_forbidden_edges.array[w-1] > n-3 || (*status).num_forbidden_edges.array[v-1] > n-3)
	return FAILURE;
    }
  }
  
  
  return SUCCESS;
}


int tsp_constraints_acp_simple(int w, int v, int u, double cost_wv, double cost_wu, tsp_status* status, tsp_backup* backup, tsp_backup* update) {
  graph* G_curr = &(*status).G_curr;
  int n = (*status).G_curr.n;
  double c;
  int i;
  int state;

  state = tsp_constraints_ac(w, v, u, cost_wv, cost_wu, status, backup, update);
  if (state == FAILURE)
    return FAILURE;

  // propagazione su nodo w 
  if ((*status).num_forced_edges.array[w-1] == 2 && (*status).num_forbidden_edges.array[w-1] < n-3) {
    for (i = 1; i <= n; i++) {
      if (i != w) {
	c = graph_get_edge_cost(G_curr, w, i);
	if (c > SMALL && c < BIG) {

	  state = tsp_constraints_ac(w, i, 0, BIG, 0.0, status, backup, update);
	  if (state == FAILURE)
	    return FAILURE;

	}
      }
    }
  }
  else if ((*status).num_forbidden_edges.array[w-1] == n-3 && (*status).num_forced_edges.array[w-1] < 2) {
    for (i = 1; i <= n; i++) {
      if (i != w) {
	c = graph_get_edge_cost(G_curr, w, i);
	if (c > SMALL && c < BIG) {

	  state = tsp_constraints_ac(w, i, 0, SMALL, 0.0, status, backup, update);
	  if (state == FAILURE)
	    return FAILURE;

	}
      }
    }
  }

  // propagazione su nodo v
  if ((*status).num_forced_edges.array[v-1] == 2 && (*status).num_forbidden_edges.array[v-1] < n-3) {
    for (i = 1; i <= n; i++) {
      if (i != v) {
	c = graph_get_edge_cost(G_curr, v, i);
	if (c > SMALL && c < BIG) {

	  state = tsp_constraints_ac(v, i, 0, BIG, 0.0, status, backup, update);
	  if (state == FAILURE)
	    return FAILURE;

	}
      }
    }
  }
  else if ((*status).num_forbidden_edges.array[v-1] == n-3 && (*status).num_forced_edges.array[v-1] < 2) {
    for (i = 1; i <= n; i++) {
      if (i != v) {
	c = graph_get_edge_cost(G_curr, v, i);
	if (c > SMALL && c < BIG) {

	  state = tsp_constraints_ac(v, i, 0, SMALL, 0.0, status, backup, update);
	  if (state == FAILURE)
	    return FAILURE;

	}
      }
    }
  }


  if (u > 0) {

    // propagazione su nodo u
    if ((*status).num_forced_edges.array[u-1] == 2 && (*status).num_forbidden_edges.array[u-1] < n-3) {
      for (i = 1; i <= n; i++)  {
	if (i != u) {
	  c = graph_get_edge_cost(G_curr, u, i);
	  if (c > SMALL && c < BIG) {

	    state = tsp_constraints_ac(u, i, 0, BIG, 0.0, status, backup, update);
	    if (state == FAILURE)
	      return FAILURE;

	  }
	}
      }
    }
    else if ((*status).num_forbidden_edges.array[u-1] == n-3 && (*status).num_forced_edges.array[u-1] < 2) {
      for (i = 1; i <= n; i++) {
	if (i != u) {
	  c = graph_get_edge_cost(G_curr, u, i);
	  if (c > SMALL && c < BIG) {

	    state = tsp_constraints_ac(u, i, 0, SMALL, 0.0, status, backup, update);
	    if (state == FAILURE)
	      return FAILURE;

	  }
	}
      }
    }
  }

  return SUCCESS;
}

int tsp_constraints_acp_recursive(int w, int v, int u, double cost_wv, double cost_wu, tsp_status* status, tsp_backup* backup, tsp_backup* update) {
  graph* G_curr = &(*status).G_curr;
  int n = (*status).G_curr.n;
  double c;
  int i, state;

  state = tsp_constraints_ac(w, v, u, cost_wv, cost_wu, status, backup, update);
  if (state == FAILURE)
    return FAILURE;
  
  // propagazione su nodo w 
  if ((*status).num_forced_edges.array[w-1] == 2 && (*status).num_forbidden_edges.array[w-1] < n-3) {
    for (i = 1; i <= n; i++) {
      if (i != w) {
	c = graph_get_edge_cost(G_curr, w, i);
	if (c > SMALL && c < BIG) {

	  state = tsp_constraints_acp_recursive(w, i, 0, BIG, 0.0, status, backup, update);
	  if (state == FAILURE)
	    return FAILURE;

	}
      }
    }
  }
  else if ((*status).num_forbidden_edges.array[w-1] == n-3 && (*status).num_forced_edges.array[w-1] < 2) {
    for (i = 1; i <= n; i++) {
      if (i != w) {
	c = graph_get_edge_cost(G_curr, w, i);
	if (c > SMALL && c < BIG) {

	  state = tsp_constraints_acp_recursive(w, i, 0, SMALL, 0.0, status, backup, update);
	  if (state == FAILURE)
	    return FAILURE;

	}
      }
    }
  }

  // propagazione su nodo v
  if ((*status).num_forced_edges.array[v-1] == 2 && (*status).num_forbidden_edges.array[v-1] < n-3) {
    for (i = 1; i <= n; i++) {
      if (i != v) {
	c = graph_get_edge_cost(G_curr, v, i);
	if (c > SMALL && c < BIG) {

	  state = tsp_constraints_acp_recursive(v, i, 0, BIG, 0.0, status, backup, update);
	  if (state == FAILURE)
	    return FAILURE;

	}
      }
    }
  }
  else if ((*status).num_forbidden_edges.array[v-1] == n-3 && (*status).num_forced_edges.array[v-1] < 2) {
    for (i = 1; i <= n; i++) {
      if (i != v) {
	c = graph_get_edge_cost(G_curr, v, i);
	if (c > SMALL && c < BIG) {

	  state = tsp_constraints_acp_recursive(v, i, 0, SMALL, 0.0, status, backup, update);
	  if (state == FAILURE)
	    return FAILURE;

	}
      }
    }
  }


  if (u > 0) {

    // propagazione su nodo u
    if ((*status).num_forced_edges.array[u-1] == 2 && (*status).num_forbidden_edges.array[u-1] < n-3) {
      for (i = 1; i <= n; i++)  {
	if (i != u) {
	  c = graph_get_edge_cost(G_curr, u, i);
	  if (c > SMALL && c < BIG) {

	    state = tsp_constraints_acp_recursive(u, i, 0, BIG, 0.0, status, backup, update);
	    if (state == FAILURE)
	      return FAILURE;

	  }
	}
      }
    }
    else if ((*status).num_forbidden_edges.array[u-1] == n-3 && (*status).num_forced_edges.array[u-1] < 2) {
      for (i = 1; i <= n; i++) {
	if (i != u) {
	  c = graph_get_edge_cost(G_curr, u, i);
	  if (c > SMALL && c < BIG) {

	    state = tsp_constraints_acp_recursive(u, i, 0, SMALL, 0.0, status, backup, update);
	    if (state == FAILURE)
	      return FAILURE;

	  }
	}
      }
    }
  }

  return SUCCESS;
}

int tsp_constraints_acp_iterative(int w, int v, int u, double cost_wv, double cost_wu, tsp_status* status, tsp_backup* backup, tsp_backup* update) {
  graph* G_curr = &(*status).G_curr;
  int n = (*status).G_curr.n;
  double c;
  int i, state;
  edge e;

  elist modified;
  elist_init(&modified);

  state = tsp_constraints_ac(w, v, u, cost_wv, cost_wu, status, backup, update);
  if (state == FAILURE)
    return FAILURE;

  elist_push_last(&modified, w, v, cost_wv);
  elist_push_last(&modified, w, u, cost_wu);

  elist tmplist;
  elist_init(&tmplist);

  while (!elist_is_empty(&modified)) {

    elist_copy(&modified, &tmplist);
    elist_empty(&modified);

    while (!elist_is_empty(&tmplist)) {

      e = elist_pop_first(&tmplist);

      // propagazione su nodo e.x
      if ((*status).num_forced_edges.array[e.x-1] == 2 && (*status).num_forbidden_edges.array[e.x-1] < n-3) {
	for (i = 1; i <= n; i++) {
	  if (i != e.x) {
	    c = graph_get_edge_cost(G_curr, e.x, i);
	    if (c > SMALL && c < BIG) {

	      state = tsp_constraints_ac(e.x, i, 0, BIG, 0.0, status, backup, update);
	      if (state == FAILURE)
		return FAILURE;
	      elist_push_last(&modified, e.x, i, BIG);

	    }
	  }
	}
      }
      else if ((*status).num_forbidden_edges.array[e.x-1] == n-3 && (*status).num_forced_edges.array[e.x-1] < 2) {
	for (i = 1; i <= n; i++) {
	  if (i != e.x) {
	    c = graph_get_edge_cost(G_curr, e.x, i);
	    if (c > SMALL && c < BIG) {

	      state = tsp_constraints_ac(e.x, i, 0, SMALL, 0.0, status, backup, update);
	      if (state == FAILURE)
		return FAILURE;
	      elist_push_last(&modified, e.x, i, SMALL);

	    }
	  }
	}
      }

      // propagazione su nodo e.y
      if ((*status).num_forced_edges.array[e.y-1] == 2 && (*status).num_forbidden_edges.array[e.y-1] < n-3) {
	for (i = 1; i <= n; i++) {
	  if (i != e.y) {
	    c = graph_get_edge_cost(G_curr, e.y, i);
	    if (c > SMALL && c < BIG) {

	      state = tsp_constraints_ac(e.y, i, 0, BIG, 0.0, status, backup, update);
	      if (state == FAILURE)
		return FAILURE;
	      elist_push_last(&modified, e.y, i, BIG);

	    }
	  }
	}
      }
      else if ((*status).num_forbidden_edges.array[e.y-1] == n-3 && (*status).num_forced_edges.array[e.y-1] < 2) {
	for (i = 1; i <= n; i++) {
	  if (i != e.y) {
	    c = graph_get_edge_cost(G_curr, e.y, i);
	    if (c > SMALL && c < BIG) {

	      state = tsp_constraints_ac(e.y, i, 0, SMALL, 0.0, status, backup, update);
	      if (state == FAILURE)
		return FAILURE;
	      elist_push_last(&modified, e.y, i, SMALL);

	    }
	  }
	}
      }

    }
  }

  elist_delete(&modified);
  elist_delete(&tmplist);
  return SUCCESS;
}





int tsp_constraints_acep_simple(int w, int v, int u, double cost_wv, double cost_wu, tsp_status* status, tsp_backup* backup, tsp_backup* update) {
  graph* G_curr = &(*status).G_curr;
  int n = (*status).G_curr.n;
  double c;
  int i;
  int state;

  state = tsp_constraints_ace(w, v, u, cost_wv, cost_wu, status, backup, update);
  if (state == FAILURE)
    return FAILURE;

  // propagazione su nodo w 
  if ((*status).num_forced_edges.array[w-1] == 2 && (*status).num_forbidden_edges.array[w-1] < n-3) {
    for (i = 1; i <= n; i++) {
      if (i != w) {
	c = graph_get_edge_cost(G_curr, w, i);
	if (c > SMALL && c < BIG) {

	  state = tsp_constraints_ace(w, i, 0, BIG, 0.0, status, backup, update);
	  if (state == FAILURE)
	    return FAILURE;

	}
      }
    }
  }
  else if ((*status).num_forbidden_edges.array[w-1] == n-3 && (*status).num_forced_edges.array[w-1] < 2) {
    for (i = 1; i <= n; i++) {
      if (i != w) {
	c = graph_get_edge_cost(G_curr, w, i);
	if (c > SMALL && c < BIG) {

	  state = tsp_constraints_ace(w, i, 0, SMALL, 0.0, status, backup, update);
	  if (state == FAILURE)
	    return FAILURE;

	}
      }
    }
  }

  // propagazione su nodo v
  if ((*status).num_forced_edges.array[v-1] == 2 && (*status).num_forbidden_edges.array[v-1] < n-3) {
    for (i = 1; i <= n; i++) {
      if (i != v) {
	c = graph_get_edge_cost(G_curr, v, i);
	if (c > SMALL && c < BIG) {

	  state = tsp_constraints_ace(v, i, 0, BIG, 0.0, status, backup, update);
	  if (state == FAILURE)
	    return FAILURE;

	}
      }
    }
  }
  else if ((*status).num_forbidden_edges.array[v-1] == n-3 && (*status).num_forced_edges.array[v-1] < 2) {
    for (i = 1; i <= n; i++) {
      if (i != v) {
	c = graph_get_edge_cost(G_curr, v, i);
	if (c > SMALL && c < BIG) {

	  state = tsp_constraints_ace(v, i, 0, SMALL, 0.0, status, backup, update);
	  if (state == FAILURE)
	    return FAILURE;

	}
      }
    }
  }


  if (u > 0) {

    // propagazione su nodo u
    if ((*status).num_forced_edges.array[u-1] == 2 && (*status).num_forbidden_edges.array[u-1] < n-3) {
      for (i = 1; i <= n; i++)  {
	if (i != u) {
	  c = graph_get_edge_cost(G_curr, u, i);
	  if (c > SMALL && c < BIG) {

	    state = tsp_constraints_ace(u, i, 0, BIG, 0.0, status, backup, update);
	    if (state == FAILURE)
	      return FAILURE;

	  }
	}
      }
    }
    else if ((*status).num_forbidden_edges.array[u-1] == n-3 && (*status).num_forced_edges.array[u-1] < 2) {
      for (i = 1; i <= n; i++) {
	if (i != u) {
	  c = graph_get_edge_cost(G_curr, u, i);
	  if (c > SMALL && c < BIG) {

	    state = tsp_constraints_ace(u, i, 0, SMALL, 0.0, status, backup, update);
	    if (state == FAILURE)
	      return FAILURE;

	  }
	}
      }
    }
  }

  return SUCCESS;
}


int tsp_constraints_acep_recursive(int w, int v, int u, double cost_wv, double cost_wu, tsp_status* status, tsp_backup* backup, tsp_backup* update) {
  graph* G_curr = &(*status).G_curr;
  int n = (*status).G_curr.n;
  double c;
  int i, state;

  state = tsp_constraints_ace(w, v, u, cost_wv, cost_wu, status, backup, update);
  if (state == FAILURE)
    return FAILURE;
  
  // propagazione su nodo w 
  if ((*status).num_forced_edges.array[w-1] == 2 && (*status).num_forbidden_edges.array[w-1] < n-3) {
    for (i = 1; i <= n; i++) {
      if (i != w) {
	c = graph_get_edge_cost(G_curr, w, i);
	if (c > SMALL && c < BIG) {

	  state = tsp_constraints_acep_recursive(w, i, 0, BIG, 0.0, status, backup, update);
	  if (state == FAILURE)
	    return FAILURE;

	}
      }
    }
  }
  else if ((*status).num_forbidden_edges.array[w-1] == n-3 && (*status).num_forced_edges.array[w-1] < 2) {
    for (i = 1; i <= n; i++) {
      if (i != w) {
	c = graph_get_edge_cost(G_curr, w, i);
	if (c > SMALL && c < BIG) {

	  state = tsp_constraints_acep_recursive(w, i, 0, SMALL, 0.0, status, backup, update);
	  if (state == FAILURE)
	    return FAILURE;

	}
      }
    }
  }

  // propagazione su nodo v
  if ((*status).num_forced_edges.array[v-1] == 2 && (*status).num_forbidden_edges.array[v-1] < n-3) {
    for (i = 1; i <= n; i++) {
      if (i != v) {
	c = graph_get_edge_cost(G_curr, v, i);
	if (c > SMALL && c < BIG) {

	  state = tsp_constraints_acep_recursive(v, i, 0, BIG, 0.0, status, backup, update);
	  if (state == FAILURE)
	    return FAILURE;

	}
      }
    }
  }
  else if ((*status).num_forbidden_edges.array[v-1] == n-3 && (*status).num_forced_edges.array[v-1] < 2) {
    for (i = 1; i <= n; i++) {
      if (i != v) {
	c = graph_get_edge_cost(G_curr, v, i);
	if (c > SMALL && c < BIG) {

	  state = tsp_constraints_acep_recursive(v, i, 0, SMALL, 0.0, status, backup, update);
	  if (state == FAILURE)
	    return FAILURE;

	}
      }
    }
  }


  if (u > 0) {

    // propagazione su nodo u
    if ((*status).num_forced_edges.array[u-1] == 2 && (*status).num_forbidden_edges.array[u-1] < n-3) {
      for (i = 1; i <= n; i++)  {
	if (i != u) {
	  c = graph_get_edge_cost(G_curr, u, i);
	  if (c > SMALL && c < BIG) {

	    state = tsp_constraints_acep_recursive(u, i, 0, BIG, 0.0, status, backup, update);
	    if (state == FAILURE)
	      return FAILURE;

	  }
	}
      }
    }
    else if ((*status).num_forbidden_edges.array[u-1] == n-3 && (*status).num_forced_edges.array[u-1] < 2) {
      for (i = 1; i <= n; i++) {
	if (i != u) {
	  c = graph_get_edge_cost(G_curr, u, i);
	  if (c > SMALL && c < BIG) {

	    state = tsp_constraints_acep_recursive(u, i, 0, SMALL, 0.0, status, backup, update);
	    if (state == FAILURE)
	      return FAILURE;

	  }
	}
      }
    }
  }

  return SUCCESS;
}

int tsp_constraints_acep_iterative(int w, int v, int u, double cost_wv, double cost_wu, tsp_status* status, tsp_backup* backup, tsp_backup* update) {
  graph* G_curr = &(*status).G_curr;
  int n = (*status).G_curr.n;
  double c;
  int i, state;
  edge e;

  elist modified;
  elist_init(&modified);

  state = tsp_constraints_ace(w, v, u, cost_wv, cost_wu, status, backup, update);
  if (state == FAILURE)
    return FAILURE;

  elist_push_last(&modified, w, v, cost_wv);
  elist_push_last(&modified, w, u, cost_wu);

  elist tmplist;
  elist_init(&tmplist);

  while (!elist_is_empty(&modified)) {

    elist_copy(&modified, &tmplist);
    elist_empty(&modified);

    while (!elist_is_empty(&tmplist)) {

      e = elist_pop_first(&tmplist);

      // propagazione su nodo e.x
      if ((*status).num_forced_edges.array[e.x-1] == 2 && (*status).num_forbidden_edges.array[e.x-1] < n-3) {
	for (i = 1; i <= n; i++) {
	  if (i != e.x) {
	    c = graph_get_edge_cost(G_curr, e.x, i);
	    if (c > SMALL && c < BIG) {

	      state = tsp_constraints_ace(e.x, i, 0, BIG, 0.0, status, backup, update);
	      if (state == FAILURE)
		return FAILURE;
	      elist_push_last(&modified, e.x, i, BIG);

	    }
	  }
	}
      }
      else if ((*status).num_forbidden_edges.array[e.x-1] == n-3 && (*status).num_forced_edges.array[e.x-1] < 2) {
	for (i = 1; i <= n; i++) {
	  if (i != e.x) {
	    c = graph_get_edge_cost(G_curr, e.x, i);
	    if (c > SMALL && c < BIG) {

	      state = tsp_constraints_ace(e.x, i, 0, SMALL, 0.0, status, backup, update);
	      if (state == FAILURE)
		return FAILURE;
	      elist_push_last(&modified, e.x, i, SMALL);

	    }
	  }
	}
      }

      // propagazione su nodo e.y
      if ((*status).num_forced_edges.array[e.y-1] == 2 && (*status).num_forbidden_edges.array[e.y-1] < n-3) {
	for (i = 1; i <= n; i++) {
	  if (i != e.y) {
	    c = graph_get_edge_cost(G_curr, e.y, i);
	    if (c > SMALL && c < BIG) {

	      state = tsp_constraints_ace(e.y, i, 0, BIG, 0.0, status, backup, update);
	      if (state == FAILURE)
		return FAILURE;
	      elist_push_last(&modified, e.y, i, BIG);

	    }
	  }
	}
      }
      else if ((*status).num_forbidden_edges.array[e.y-1] == n-3 && (*status).num_forced_edges.array[e.y-1] < 2) {
	for (i = 1; i <= n; i++) {
	  if (i != e.y) {
	    c = graph_get_edge_cost(G_curr, e.y, i);
	    if (c > SMALL && c < BIG) {

	      state = tsp_constraints_ace(e.y, i, 0, SMALL, 0.0, status, backup, update);
	      if (state == FAILURE)
		return FAILURE;
	      elist_push_last(&modified, e.y, i, SMALL);

	    }
	  }
	}
      }

    }
  }

  elist_delete(&modified);
  elist_delete(&tmplist);
  return SUCCESS;
}
