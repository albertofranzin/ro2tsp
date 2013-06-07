#include "tree.h"



void tree_init(tree* T, int n, int r) {
  int i;

  if (n == 0) {
    T->n = 0;
    T->root = -1;
    T->V = NULL;
  }
  else if (n >= 1) {
    T->n = n;
    T->root = r;
    T->V = (tree_node*)calloc(n, sizeof(tree_node));
    for (i = 0; i < n; i++) T->V[i].pred = -1;
  }

#ifdef DEBUG
  if (n < 0) {
    printf("error: tree_init\n");
    exit(1);
  }
#endif

}



void tree_delete(tree* T) {

  free(T->V);
  T->V = NULL;
  T->n = 0;
  T->root = -1;

}



void tree_copy(tree* FROM, tree* TO) {

  int i;
  int n = FROM->n;

  tree_delete(TO);
  tree_init(TO, n, FROM->root);
  for (i = 0; i < n; i++) {
    TO->V[i].pred = FROM->V[i].pred;
    TO->V[i].deg = FROM->V[i].deg;
  }

}



void tree_insert_edge(tree* T, int u, int v) {

#ifdef DEBUG
  if (u < 0 || v < 0 || u >= T->n || v >= T->n || u == v || v == T->root || tree_adjacent_nodes(T, u, v) || T->V[v].pred >= 0) {
    printf("error: tree_insert_edge\n");
    exit(1);
  }
#endif

  T->V[v].pred = u;
  T->V[v].deg++;
  T->V[u].deg++;


}



void tree_remove_edge(tree* T, int u, int v) {

#ifdef DEBUG
  if (u < 0 || v < 0 || u >= T->n || v >= T->n || u == v || !tree_adjacent_nodes(T, u, v)) {
    printf("error: tree_remove_edge\n");
    exit(1);
  }
#endif

  if (T->V[v].pred == u) {
    T->V[v].pred = -1;
    T->V[v].deg--;
    T->V[u].deg--;
  }
  else if (T->V[u].pred == v) {
    T->V[u].pred = -1;
    T->V[v].deg--;
    T->V[u].deg--;
  }

}



int tree_adjacent_nodes(tree* T, int u, int v) {

#ifdef DEBUG
  if (u < 0 || v < 0 || u >= T->n || v >= T->n || u == v) {
    printf("error: tree_adjacent_nodes\n");
    exit(1);
  }
#endif	

  return ((T->V[v].pred == u) || T->V[u].pred == v);

}



double tree_get_cost(tree* T, graph* G) {

  int i;
  double c = 0.0;
  int n = T->n;

  for (i = 0; i < n; i++) {
    if (T->V[i].pred >= 0)
      c += graph_get_edge_cost(G, (*T).V[i].pred, i);
  }
  return c;

}

