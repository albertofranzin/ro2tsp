#include "onetree.h"

void onetree_init(onetree* OT, int n) {

  if (n == 0) {
    (*OT).n = 0;
    (*OT).V = NULL;
    (*OT).v1 = 0;
    (*OT).v2 = 0;
  }
  else if (n >= 2) {
    (*OT).n = n;
    (*OT).V = (onetree_node*)calloc(n, sizeof(onetree_node));
    (*OT).v1 = 0;
    (*OT).v2 = 0;
  }
  else {
    printf("error: onetree_init\n");
    exit(EXIT_FAILURE);
  }
}

void onetree_delete(onetree* OT) {

  free((*OT).V);
  (*OT).V = NULL;
  (*OT).n = 0;
  (*OT).v1 = 0;
  (*OT).v2 = 0;
}

void onetree_copy(onetree* FROM, onetree* TO) {
  int i;

  int n = (*FROM).n;
  onetree_delete(TO);
  onetree_init(TO, n);
  (*TO).v1 = (*FROM).v1;
  (*TO).v2 = (*FROM).v2;
  for (i = 0; i < n; i++) {
    (*TO).V[i].pred = (*FROM).V[i].pred;
    (*TO).V[i].cost = (*FROM).V[i].cost;
    (*TO).V[i].constr = (*FROM).V[i].constr;
    (*TO).V[i].deg = (*FROM).V[i].deg;
    (*TO).V[i].deg_forced = (*FROM).V[i].deg_forced;
    (*TO).V[i].deg_forbidden = (*FROM).V[i].deg_forbidden;
  }
}

int onetree_get_pred(onetree* OT, int v) {

  if ( v >= 1 && v <= (*OT).n) {
    return (*OT).V[v-1].pred;
  }
  else {
    printf("error: onetree_get_pred: %d\n", v);
    exit(EXIT_FAILURE);
  }
}

void onetree_insert_edge(onetree* OT, int u, int v, double cost, int constr) {

  if (u >= 1 && v >= 1 && u <= (*OT).n && v <= (*OT).n && u != v && !onetree_adjacent_nodes(OT, u, v) && (*OT).V[v-1].pred == 0) {
    (*OT).V[v-1].pred = u;
    (*OT).V[v-1].cost = cost;
    (*OT).V[v-1].constr = constr;
    (*OT).V[v-1].deg++;
    (*OT).V[u-1].deg++;
    if (constr == FORCED) {
      (*OT).V[v-1].deg_forced++;
      (*OT).V[u-1].deg_forced++;
    }
    else if (constr == FORBIDDEN) {
      (*OT).V[v-1].deg_forbidden++;
      (*OT).V[u-1].deg_forbidden++;
    }
  }
  else {
    printf("error: onetree_insert_edge: %d %d\n", u, v);
    exit(EXIT_FAILURE);
  }
}

void onetree_remove_edge(onetree* OT, int u, int v) {

  if (u >= 1 && v >= 1 && u <= (*OT).n && v <= (*OT).n && u != v && onetree_adjacent_nodes(OT, u, v)) {
    int constr;

    if ((*OT).V[v-1].pred == u) {
      constr = (*OT).V[v-1].constr;

      (*OT).V[v-1].pred = 0;
      (*OT).V[v-1].cost = 0.0;
      (*OT).V[v-1].constr = FREE;
      (*OT).V[v-1].deg--;
      (*OT).V[u-1].deg--;

      if (constr == FORCED) {
	(*OT).V[v-1].deg_forced--;
	(*OT).V[u-1].deg_forced--;
      }
      else if (constr == FORBIDDEN) {
	(*OT).V[v-1].deg_forbidden--;
	(*OT).V[u-1].deg_forbidden--;
      }

    }
    else if ((*OT).V[u-1].pred == v) {
      constr = (*OT).V[u-1].constr;

      (*OT).V[u-1].pred = 0;
      (*OT).V[u-1].cost = 0.0;
      (*OT).V[u-1].constr = FREE; 
      (*OT).V[v-1].deg--;
      (*OT).V[u-1].deg--;

      if (constr == FORCED) {
	(*OT).V[v-1].deg_forced--;
	(*OT).V[u-1].deg_forced--;
      }
      else if (constr == FORBIDDEN) {
	(*OT).V[v-1].deg_forbidden--;
	(*OT).V[u-1].deg_forbidden--;
      }

    }
  }
  else {
    printf("error: onetree_remove_edge: %d %d\n", u, v);
    exit(EXIT_FAILURE);
  }
}

void onetree_set_edge_cost(onetree* OT, int u, int v, double cost) {

  if (u >= 1 && v >= 1 && u <= (*OT).n && v <= (*OT).n && u != v && onetree_adjacent_nodes(OT, u, v)) {
    if ((*OT).V[v-1].pred == u) {
      (*OT).V[v-1].cost = cost;
    }
    else if ((*OT).V[u-1].pred == v) {
      (*OT).V[u-1].cost = cost;
    }
  }
  else  {
    printf("error: onetree_set_edge_cost: %d %d\n", u, v);
    exit(EXIT_FAILURE);
  }
}

void onetree_set_edge_constr(onetree* OT, int u, int v, int constr) {

  if (u >= 1 && v >= 1 && u <= (*OT).n && v <= (*OT).n && u != v && onetree_adjacent_nodes(OT, u, v)) {
    int old_constr;

    if ((*OT).V[v-1].pred == u) {
      old_constr = (*OT).V[v-1].constr;

      (*OT).V[v-1].cost = constr;

      if (old_constr == FORCED) {
	(*OT).V[v-1].deg_forced--;
	(*OT).V[u-1].deg_forced--;
      }
      else if (old_constr == FORBIDDEN) {
	(*OT).V[v-1].deg_forbidden--;
	(*OT).V[u-1].deg_forbidden--;
      }

      if (constr == FORCED) {
	(*OT).V[v-1].deg_forced++;
	(*OT).V[u-1].deg_forced++;
      }
      else if (constr == FORBIDDEN) {
	(*OT).V[v-1].deg_forbidden++;
	(*OT).V[u-1].deg_forbidden++;
      }

    }
    else if ((*OT).V[u-1].pred == v) {
      old_constr = (*OT).V[u-1].constr;

      (*OT).V[u-1].constr = constr;

      if (old_constr == FORCED) {
	(*OT).V[v-1].deg_forced--;
	(*OT).V[u-1].deg_forced--;
      }
      else if (old_constr == FORBIDDEN) {
	(*OT).V[v-1].deg_forbidden--;
	(*OT).V[u-1].deg_forbidden--;
      }

      if (constr == FORCED) {
	(*OT).V[v-1].deg_forced++;
	(*OT).V[u-1].deg_forced++;
      }
      else if (constr == FORBIDDEN) {
	(*OT).V[v-1].deg_forbidden++;
	(*OT).V[u-1].deg_forbidden++;
      }

    }
  }
  else  {
    printf("error: onetree_set_edge_constr\n");
    exit(EXIT_FAILURE);
  }
}

double onetree_get_edge_cost(onetree* OT, int u, int v) {

  if (u >= 1 && v >= 1 && u <= (*OT).n && v <= (*OT).n && u != v && onetree_adjacent_nodes(OT, u, v)) {
    if ((*OT).V[v-1].pred == u) {
      return (*OT).V[v-1].cost;
    }
    else if ((*OT).V[u-1].pred == v) {
      return (*OT).V[u-1].cost;
    }
  }
  else {
    printf("error: onetree_get_edge_cost: %d %d\n", u, v);
    return;
  }
}

int onetree_get_edge_constr(onetree* OT, int u, int v) {

  if (u >= 1 && v >= 1 && u <= (*OT).n && v <= (*OT).n && u != v && onetree_adjacent_nodes(OT, u, v)) {
    if ((*OT).V[v-1].pred == u) {
      return (*OT).V[v-1].constr;
    }
    else if ((*OT).V[u-1].pred == v) {
      return (*OT).V[u-1].constr;
    }
  }
  else {
    printf("error: onetree_get_edge_constr\n");
    return;
  }
}

int onetree_get_node_deg(onetree* OT, int v) {

  if (v >= 1 && v <= (*OT).n) {
    return (*OT).V[v-1].deg;
  }
  else {
    printf("error: onetree_get_node_deg: %d\n", v);
    exit(EXIT_FAILURE);
  }  
}

int onetree_get_node_deg_forced(onetree* OT, int v) {

  if (v >= 1 && v <= (*OT).n) {
    return (*OT).V[v-1].deg_forced;
  }
  else {
    printf("error: onetree_get_node_deg_forced\n");
    exit(EXIT_FAILURE);
  }  
}

int onetree_get_node_deg_forbidden(onetree* OT, int v) {

  if (v >= 1 && v <= (*OT).n) {
    return (*OT).V[v-1].deg_forbidden;
  }
  else {
    printf("error: onetree_get_node_deg_forbidden\n");
    exit(EXIT_FAILURE);
  }  
}

int onetree_adjacent_nodes(onetree* OT, int u, int v) {

  if (u >= 1 && v >= 1 && u <= (*OT).n && v <= (*OT).n && u != v) {
    return (((*OT).V[v-1].pred == u) || (*OT).V[u-1].pred == v);
  }
  else {
    printf("error: onetree_adjacent_nodes: %d %d\n", u, v);
    exit(EXIT_FAILURE);
  }
}

double onetree_get_cost(onetree* OT) {
  int i;

  double c = 0.0;
  int n = (*OT).n;
  for (i = 0; i < n; i++) {
    if ((*OT).V[i].pred > 0) // se si assume che OT sia effettivamente un 1-albero, si può togliere l'if
      c += (*OT).V[i].cost;
  }
  return c;
}

int onetree_is_cycle(onetree* OT) {
  int i;

  int n = (*OT).n; 
  for (i = 1; i <= n; i++) {
    if (onetree_get_node_deg(OT, i) != 2)
      return 0;
  }
  return 1;
}

void onetree_to_graph(onetree* OT, graph* G) {
  int i;

  int n = (*OT).n;

  graph_delete(G);
  graph_init(G, n);

  for (i = 0; i < n; i++) {
    if ((*OT).V[i].pred > 0)
      graph_insert_edge(G, (*OT).V[i].pred, i+1, (*OT).V[i].cost, (*OT).V[i].constr);
  }
}

void onetree_to_cycle(onetree* OT, cycle* C) {
  int i;
  int v;

  int n = (*OT).n;

  cycle_delete(C);
  cycle_init(C, n);

  v = 1;
  for (i = 0; i < n; i++) {

    (*C).nodes[i] = v;
    (*C).costs[i] = (*OT).V[v-1].cost;
    v = onetree_get_pred(OT, v);
  
  }
}
