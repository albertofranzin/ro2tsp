#include "graph.h"

void graph_init(graph* G, int n) {

  if (n >= 1) {
    (*G).n = n;
    (*G).V = (graph_node*)calloc(n, sizeof(graph_node));
    (*G).E = (graph_edge*)calloc(n * (n + 1) / 2, sizeof(graph_edge));
  }
  else {
    printf("error: graph_init\n");
    exit(EXIT_FAILURE);
  }
}

void graph_delete(graph* G) {

  free((*G).V);
  free((*G).E);
  (*G).V = NULL;
  (*G).E = NULL;
  (*G).n = 0;
}

void graph_copy(graph* FROM, graph* TO) {

  int i;
  int n = (*FROM).n;
  graph_delete(TO);
  graph_init(TO, n);
  for (i = 0; i < n; i++) {
    (*TO).V[i].deg = (*FROM).V[i].deg;
  }
  for (i = 0; i < n * (n + 1) / 2; i++) {
    (*TO).E[i].flag = (*FROM).E[i].flag;
    (*TO).E[i].cost = (*FROM).E[i].cost;
  }
}

void graph_insert_edge(graph* G, int u, int v, double cost) {
  if (u >= 1 && v >= 1 && u <= (*G).n && v <= (*G).n && u != v && !graph_adjacent_nodes(G, u, v)) {
    (u > v) ? ( (*G).E[ u*(u-1)/2 + v-1 ].flag = 1 ) : ( (*G).E[ v*(v-1)/2 + u-1].flag = 1 );
    (u > v) ? ( (*G).E[ u*(u-1)/2 + v-1 ].cost = cost ) : ( (*G).E[ v*(v-1)/2 + u-1].cost = cost );
    (*G).V[u-1].deg++;
    (*G).V[v-1].deg++;
  }
  else {
    printf("error: graph_insert_edge\n");
    exit(EXIT_FAILURE);
  }
}

void graph_remove_edge(graph* G, int u, int v) {
  if (u >= 1 && v >= 1 && u <= (*G).n && v <= (*G).n && u != v && graph_adjacent_nodes(G, u, v)) {
    (u > v) ? ( (*G).E[ u*(u-1)/2 + v-1 ].flag = 0 ) : ( (*G).E[ v*(v-1)/2 + u-1].flag = 0 );
    (u > v) ? ( (*G).E[ u*(u-1)/2 + v-1 ].cost = 0.0 ) : ( (*G).E[ v*(v-1)/2 + u-1].cost = 0.0 );
    (*G).V[u-1].deg--;
    (*G).V[v-1].deg--;
  }
  else {
    printf("error: graph_remove_edge\n");
    exit(EXIT_FAILURE);
  }
}

void graph_set_edge_cost(graph* G, int u, int v, double cost) {

  if (u >= 1 && v >= 1 && u <= (*G).n && v <= (*G).n && u != v && graph_adjacent_nodes(G, u, v)) {
    (u > v) ? ( (*G).E[ u*(u-1)/2 + v-1 ].cost = cost ) : ( (*G).E[ v*(v-1)/2 + u-1 ].cost = cost );
  }
  else {
    printf("error: graph_set_edge_cost\n");
    exit(EXIT_FAILURE);
  }
}

double graph_get_edge_cost(graph* G, int u, int v) {

  if (u >= 1 && v >= 1 && u <= (*G).n && v <= (*G).n && u != v && graph_adjacent_nodes(G, u, v)) {
    return (u > v) ? (*G).E[ u*(u-1)/2 + v-1 ].cost : (*G).E[ v*(v-1)/2 + u-1 ].cost;
  }
  else {
    printf("error: graph_get_edge_cost\n");
    exit(EXIT_FAILURE);
  }
}

int graph_get_node_deg(graph* G, int v) {
  if (v >= 1 && v <= (*G).n) {
    return (*G).V[v-1].deg;
  }
  else {
    printf("error: graph_get_node_deg\n");
    exit(EXIT_FAILURE);
  }
}

int graph_adjacent_nodes(graph* G, int u, int v) {
  if (u >= 1 && v >= 1 && u <= (*G).n && v <= (*G).n && u != v) {
    return (u > v) ? (*G).E[ u*(u-1)/2 + v-1 ].flag : (*G).E[ v*(v-1)/2 + u-1 ].flag;
  }
  else {
    printf("error: graph_adjacent_nodes\n");
    exit(EXIT_FAILURE);
  }
}

double graph_get_cost(graph* G) {

  int i, j;
  int n = (*G).n;
  double c = 0.0;
  for (i = 1; i <= n; i++) {
    for (j = i+1; j <= n; j++) {
      if (graph_adjacent_nodes(G, i, j)) {
        c += graph_get_edge_cost(G, i, j);
      }
    }
  }
  return c;
}
