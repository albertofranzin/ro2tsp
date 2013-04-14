#include <stdlib.h>
#include <string.h>
#include "tree.h"

void tree_init(tree* T, int n) {

  (*T).n = n;
  (*T).root = 0;
  (*T).V = (tree_node*)calloc(n, sizeof(tree_node));
}

void tree_delete(tree* T) {
  free((*T).V);
  (*T).n = 0;
  (*T).root = 0;
  (*T).V = NULL;
}

void tree_copy(tree* FROM, tree* TO) {
  int i;

  int n = (*FROM).n;
  tree_delete(TO);
  tree_init(TO, n);
  (*TO).root = (*FROM).root;
  for (i = 0; i < n; i++) {
    (*TO).V[i].pred = (*FROM).V[i].pred;
    (*TO).V[i].succ = (*FROM).V[i].succ; // AGGIUNTA SUCCESSORE
    (*TO).V[i].cost = (*FROM).V[i].cost;
    (*TO).V[i].deg = (*FROM).V[i].deg;
  }
}

void tree_set_root(tree* T, int r) {
  (*T).root = r;
}

int tree_get_root(tree* T) {
  return (*T).root;
}

int tree_get_pred(tree* T, int v) {
  return (*T).V[v-1].pred;
}

int tree_get_succ(tree* T, int v) { // AGGIUNTA SUCCESSORE
  return (*T).V[v-1].succ;
}

void tree_insert_edge(tree* T, int u, int v, double cost) { // inserice il lato {v, p} ovvero associa a v il predecessore u (se si vuole fare il contrario, basta ricordarsi di chiamare la funzione fornendo i parametri u e v invertiti); se esiste già il lato, ovvero se u è predecessore di v oppure v è predecessore di u, allora non fa nulla.
  if (tree_adjacent_nodes(T, u, v)) // due nodi u, v sono adiacenti se u è predecessore di v o viceversa
    return;
  (*T).V[v-1].pred = u;
  (*T).V[u-1].succ = v; // AGGIUNTA SUCCESSORE
  (*T).V[v-1].cost = cost;
  (*T).V[v-1].deg++;
  (*T).V[u-1].deg++;
}

void tree_remove_edge(tree* T, int u, int v) {
  if (!tree_adjacent_nodes(T, u, v))
    return;
  (*T).V[v-1].pred = 0;
  (*T).V[u-1].succ = 0; // AGGIUNTA SUCCESSORE
  (*T).V[v-1].cost = 0.0;
  (*T).V[v-1].deg--;
  (*T).V[u-1].deg--;
}

void tree_set_edge_cost(tree* T, int u, int v, double cost) {
  if (!tree_adjacent_nodes(T, u, v))
    return;

  if ((*T).V[v-1].pred == u) 
    (*T).V[v-1].cost = cost;
  else if ((*T).V[u-1].pred == v) 
    (*T).V[u-1].cost = cost;
}

double tree_get_edge_cost(tree* T, int u, int v) {
  if (!tree_adjacent_nodes(T, u, v))
    return;

  if ((*T).V[v-1].pred == u)
    return (*T).V[v-1].cost;
  else if ((*T).V[u-1].pred == v)
    return (*T).V[u-1].cost;
}

int tree_get_node_deg(tree* T, int v) {
  return (*T).V[v-1].deg;
}

int tree_adjacent_nodes(tree* T, int u, int v) {
  return (((*T).V[v-1].pred == u) || (*T).V[u-1].pred == v);
}

double tree_get_cost(tree* T) {
  int i;
  double c;

  int n = (*T).n;
  c = 0.0;
  for (i = 0; i < n; i++) {
    if ((*T).V[i].pred > 0)
      c += (*T).V[i].cost;
  }
  return c;
}
