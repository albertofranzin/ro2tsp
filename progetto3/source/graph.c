#include "graph.h"

void graph_init(graph* G, int n) {

  (*G).n = n;
  (*G).V = (graph_node*)calloc(n, sizeof(graph_node));
  (*G).E = (graph_edge*)calloc(n * (n + 1) / 2, sizeof(graph_edge));
}

void graph_delete(graph* G) {
  free((*G).V);
  free((*G).E);
  (*G).n = 0;
  (*G).V = NULL;
  (*G).E = NULL;
}

void graph_copy(graph* FROM, graph* TO) {
  int i;

  int n = (*FROM).n;
  graph_delete(TO);
  graph_init(TO, n);
  for (i = 0; i < n; i++) {
    (*TO).V[i].deg = (*FROM).V[i].deg;
  }
  for (i = 0; i < n * (n + 1) / 2;i++) {
    (*TO).E[i].flag = (*FROM).E[i].flag;
    (*TO).E[i].cost = (*FROM).E[i].cost;
  }
}

void graph_insert_edge(graph* G, int u, int v, double cost) {
  if (graph_adjacent_nodes(G, u, v))
    return;
  (u > v) ? ( (*G).E[ u*(u-1)/2 + v-1 ].flag = 1 ) : ( (*G).E[ v*(v-1)/2 + u-1].flag = 1 );
  (u > v) ? ( (*G).E[ u*(u-1)/2 + v-1 ].cost = cost ) : ( (*G).E[ v*(v-1)/2 + u-1].cost = cost );
  (*G).V[u-1].deg++;
  (*G).V[v-1].deg++;
}

void graph_remove_edge(graph* G, int u, int v) {
  if (!graph_adjacent_nodes(G, u, v))
    return;
  (u > v) ? ( (*G).E[ u*(u-1)/2 + v-1 ].flag = 0 ) : ( (*G).E[ v*(v-1)/2 + u-1].flag = 0 );
  (u > v) ? ( (*G).E[ u*(u-1)/2 + v-1 ].cost = 0.0 ) : ( (*G).E[ v*(v-1)/2 + u-1].cost = 0.0 );
  (*G).V[u-1].deg--;
  (*G).V[v-1].deg--;
}

void graph_set_edge_cost(graph* G, int u, int v, double cost) {
  if (!graph_adjacent_nodes(G, u, v))
    return;
  (u > v) ? ( (*G).E[ u*(u-1)/2 + v-1 ].cost = cost ) : ( (*G).E[ v*(v-1)/2 + u-1 ].cost = cost );
}

double graph_get_edge_cost(graph* G, int u, int v) {
  if (!graph_adjacent_nodes(G, u, v))
    return 0.0;
  return (u > v) ? (*G).E[ u*(u-1)/2 + v-1 ].cost : (*G).E[ v*(v-1)/2 + u-1 ].cost;
}

int graph_get_node_deg(graph* G, int v) {
  return (*G).V[v-1].deg;
}

int graph_adjacent_nodes(graph* G, int u, int v) {
  return (u > v) ? (*G).E[ u*(u-1)/2 + v-1 ].flag : (*G).E[ v*(v-1)/2 + u-1 ].flag;
}

double graph_get_cost(graph* G) {
  int i, j;
  double c;

  int n = (*G).n;
  c = 0.0;
  for (i = 1; i <= n; i++) {
    for (j = 1; j < i; j++) {
      if (graph_adjacent_nodes(G, i, j)) {
        c += graph_get_edge_cost(G, i, j);
      }
    }
  }
  return c;
}
