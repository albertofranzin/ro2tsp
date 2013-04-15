#include "tree.h"

void tree_init(tree* T, int n) {

  (*T).n = n;
  (*T).root = 0;
  /*printf("aa\n");
  printf("sizeof tree : %d nodes\n", n);
  printf("bb\n");*/
  (*T).V = (tree_node *)calloc(n, sizeof(tree_node));
  //printf("cc\n");
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
  /*for (i = 0; i < n; i++) {
    (*TO).V[i].pred = (*FROM).V[i].pred;
    (*TO).V[i].cost = (*FROM).V[i].cost;
    (*TO).V[i].succ = (*FROM).V[i].succ;
    (*TO).V[i].deg = (*FROM).V[i].deg;
  }*/
  memcpy(TO->V, FROM->V, sizeof(FROM->V[0])*n);
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

int tree_get_succ(tree* T, int v) {
  return (*T).V[v-1].succ;
}

/*
 * inserisce il lato {v, p} ovvero associa a v il predecessore u
 * (se si vuole fare il contrario, basta ricordarsi di chiamare la funzione
 * fornendo i parametri u e v invertiti);
 * se esiste già il lato, ovvero se u è predecessore di v
 * oppure v è predecessore di u, allora non fa nulla.
 */
void tree_insert_edge(tree* T, int u, int v, double cost) {
  // due nodi u, v sono adiacenti se u è predecessore di v o viceversa
  if (tree_adjacent_nodes(T, u, v))
    return;

  (*T).V[v-1].pred = u;
  (*T).V[u-1].succ = v;
  (*T).V[v-1].cost = cost;
  (*T).V[v-1].deg++;
  (*T).V[u-1].deg++;
}

void tree_remove_edge(tree* T, int u, int v) {
  if (!tree_adjacent_nodes(T, u, v))
    return;
  (*T).V[v-1].pred = 0;
  (*T).V[u-1].succ = 0;
  (*T).V[v-1].cost = 0.0;
  (*T).V[v-1].deg--;
  (*T).V[u-1].deg--;
}

void tree_set_edge_cost(tree* T, int u, int v, double cost) {
  if (!tree_adjacent_nodes(T, u, v))
    return;

  if ((*T).V[v-1].pred == u)
    (*T).V[v-1].cost = cost;
  else // if ((*T).V[u-1].pred == v)
    (*T).V[u-1].cost = cost;
}

double tree_get_edge_cost(tree* T, int u, int v) {
  if (!tree_adjacent_nodes(T, u, v))
    return BIG;

  if ((*T).V[v-1].pred == u)
    return (*T).V[v-1].cost;
  else //if ((*T).V[u-1].pred == v)
    return (*T).V[u-1].cost;
}

/*
 * tree_swap_edges
 * - G : original graph
 * - T : tree
 * - u, v : nodes
 *
 * swap edges (u, pred(u)), (v, pred(v)) making them (u,v), (pred(u), pred(v))
 */
void tree_swap_edges(graph *G, tree *T, int u, int v) {
  int a = tree_get_pred(T, u), b = tree_get_pred(T, v);

  tree_set_edge_cost(T, u, v, graph_get_edge_cost(G, u, v));
  tree_set_edge_cost(T, a, b, graph_get_edge_cost(G, u, v));
  tree_set_edge_cost(T, u, a, 0.);
  tree_set_edge_cost(T, v, b, 0.);
  (*T).V[v-1].deg--;
  (*T).V[u-1].deg--;
  (*T).V[a-1].deg--;
  (*T).V[b-1].deg--;
}

int tree_get_node_deg(tree* T, int v) {
  return (*T).V[v-1].deg;
}

int tree_adjacent_nodes(tree* T, int u, int v) {
  //printf("adj ?? %d, %d\n", u, v);
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
