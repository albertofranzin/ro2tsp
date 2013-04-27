#include "tree.h"

void tree_init(tree* T, int n, int r) {

  if (n >= 1) {
    (*T).n = n;
    (*T).root = r;
    (*T).V = (tree_node*)calloc(n, sizeof(tree_node));
  }
  else {
    printf("error: tree_init\n");
    exit(EXIT_FAILURE);
  }
}

void tree_delete(tree* T) {

  free((*T).V);
  (*T).V = NULL;
  (*T).n = 0;
  (*T).root = 0;
}

void tree_copy(tree* FROM, tree* TO) {
  int i;

  int n = (*FROM).n;
  tree_delete(TO);
  tree_init(TO, n, (*FROM).root);
  for (i = 0; i < n; i++) {
    (*TO).V[i].pred = (*FROM).V[i].pred;
    (*TO).V[i].cost = (*FROM).V[i].cost;
    (*TO).V[i].deg = (*FROM).V[i].deg;
  }
}

int tree_get_root(tree* T) {

  return (*T).root;
}

int tree_get_pred(tree* T, int v) {

  if ( v >= 1 && v <= (*T).n && v != (*T).root) {
    return (*T).V[v-1].pred;
  }
  else {
    printf("error: tree_get_pred\n");
    exit(EXIT_FAILURE);
  }
}

void tree_insert_edge(tree* T, int u, int v, double cost) {

  if (u >= 1 && v >= 1 && u <= (*T).n && v <= (*T).n && u != v && v != (*T).root && !tree_adjacent_nodes(T, u, v) && (*T).V[v-1].pred == 0) {
    (*T).V[v-1].pred = u;
    (*T).V[v-1].cost = cost;
    (*T).V[v-1].deg++;
    (*T).V[u-1].deg++;
  }
  else {
    printf("error: tree_insert_edge\n");
    exit(EXIT_FAILURE);
  }
}

void tree_remove_edge(tree* T, int u, int v) {

  if (u >= 1 && v >= 1 && u <= (*T).n && v <= (*T).n && u != v && tree_adjacent_nodes(T, u, v)) {
    if ((*T).V[v-1].pred == u) {
      (*T).V[v-1].pred = 0;
      (*T).V[v-1].cost = 0.0;
      (*T).V[v-1].deg--;
      (*T).V[u-1].deg--;
    }
    else if ((*T).V[u-1].pred == v) {
      (*T).V[u-1].pred = 0;
      (*T).V[u-1].cost = 0.0; 
      (*T).V[v-1].deg--;
      (*T).V[u-1].deg--;
    }
  }
  else {
    printf("error: tree_remove_edge\n");
    exit(EXIT_FAILURE);
  }
}

void tree_set_edge_cost(tree* T, int u, int v, double cost) {

  if (u >= 1 && v >= 1 && u <= (*T).n && v <= (*T).n && u != v && tree_adjacent_nodes(T, u, v)) {
    if ((*T).V[v-1].pred == u) {
      (*T).V[v-1].cost = cost;
    }
    else if ((*T).V[u-1].pred == v) {
      (*T).V[u-1].cost = cost;
    }
  }
  else  {
    printf("error: tree_set_edge_cost\n");
    exit(EXIT_FAILURE);
  }
}

double tree_get_edge_cost(tree* T, int u, int v) {

  if (u >= 1 && v >= 1 && u <= (*T).n && v <= (*T).n && u != v && tree_adjacent_nodes(T, u, v)) {
    if ((*T).V[v-1].pred == u) {
      return (*T).V[v-1].cost;
    }
    else if ((*T).V[u-1].pred == v) {
      return (*T).V[u-1].cost;
    }
  }
  else {
    printf("error: tree_get_edge_cost\n");
    return;
  }
}

int tree_get_node_deg(tree* T, int v) {

  if (v >= 1 && v <= (*T).n) {
    return (*T).V[v-1].deg;
  }
  else {
    printf("error: tree_get_node_deg\n");
    exit(EXIT_FAILURE);
  }  
}

int tree_adjacent_nodes(tree* T, int u, int v) {

  if (u >= 1 && v >= 1 && u <= (*T).n && v <= (*T).n && u != v) {
    return (((*T).V[v-1].pred == u) || (*T).V[u-1].pred == v);
  }
  else {
    printf("error: tree_adjacent_nodes\n");
    exit(EXIT_FAILURE);
  }
}

double tree_get_cost(tree* T) {
  int i;

  double c = 0.0;
  int n = (*T).n;
  for (i = 0; i < n; i++) {
    if ((*T).V[i].pred > 0) // se si assume che T sia effettivamente un albero, si può togliere l'if ed eseguire il for da i=1 anzichè i=0
      c += (*T).V[i].cost;
  }
  return c;
}
