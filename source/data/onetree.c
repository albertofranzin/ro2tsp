#include "onetree.h"

void onetree_init(onetree* OT, int n) {

  if (n >= 2) {
    (*OT).n = n;
    (*OT).V = (tree_node*)calloc(n, sizeof(tree_node));
  }
  else {
    printf("error: onetree_init: %d\n", n);
    exit(EXIT_FAILURE);
  }
}

void onetree_delete(onetree* OT) {

  free((*OT).V);
  (*OT).V = NULL;
  (*OT).n = 0;
}

void onetree_copy(onetree* FROM, onetree* TO) {
  int i;

  int n = (*FROM).n;
  onetree_delete(TO);
  onetree_init(TO, n);
  for (i = 0; i < n; i++) {
    (*TO).V[i].pred = (*FROM).V[i].pred;
    (*TO).V[i].cost = (*FROM).V[i].cost;
    (*TO).V[i].deg = (*FROM).V[i].deg;
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

void onetree_insert_edge(onetree* OT, int u, int v, double cost) {

  if (u >= 1 && v >= 1 && u <= (*OT).n && v <= (*OT).n && u != v && !onetree_adjacent_nodes(OT, u, v) && (*OT).V[v-1].pred == 0) {
    (*OT).V[v-1].pred = u;
    (*OT).V[v-1].cost = cost;
    (*OT).V[v-1].deg++;
    (*OT).V[u-1].deg++;
  }
  else {
    printf("error: onetree_insert_edge: %d %d\n", u, v);
    exit(EXIT_FAILURE);
  }
}

void onetree_remove_edge(onetree* OT, int u, int v) {

  if (u >= 1 && v >= 1 && u <= (*OT).n && v <= (*OT).n && u != v && onetree_adjacent_nodes(OT, u, v)) {
    if ((*OT).V[v-1].pred == u) {
      (*OT).V[v-1].pred = 0;
      (*OT).V[v-1].cost = 0.0;
      (*OT).V[v-1].deg--;
      (*OT).V[u-1].deg--;
    }
    else if ((*OT).V[u-1].pred == v) {
      (*OT).V[u-1].pred = 0;
      (*OT).V[u-1].cost = 0.0; 
      (*OT).V[v-1].deg--;
      (*OT).V[u-1].deg--;
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

int onetree_get_node_deg(onetree* OT, int v) {

  if (v >= 1 && v <= (*OT).n) {
    return (*OT).V[v-1].deg;
  }
  else {
    printf("error: onetree_get_node_deg: %d\n", v);
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
/*
void onetree_get_cost_safe(onetree* OT) {
  
  int n = (*OT).n;
  int i, v1, v2, pred, succ, current;
  int* predecessors;

  v2 = onetree_get_pred(OT, 1);
  p = onetree_get_pred(OT, v2);
  while (p != 1) {
    v1 = p;
    p = onetree_get_pred(OT, p);
  }

  if (v1 <= v2) { // ok calcola i costi normalmente
    ;
  }
  else { // voglio che il successore di 1 sia il più piccolo dei due vertici adiacenti a 1, devo fare una sorta di rerooting

    predecessors = (int*)calloc(n, sizeof(int));
    for (i = 1; i <= n; i++) {
      predecessors[i-1] = onetree_get_pred(OT, i);
    }

    s = v2;
    current = pred[v2-1];
    p = pred[current-1];


    s = pred[v2-1];
    p_tmp = pred[s-1];
    pred[s-1] = v2;
*/
