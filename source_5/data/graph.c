#include "graph.h"



void graph_init(graph* G, int n) {

  if (n == 0) {
    G->n = 0;
    G->V = NULL;
    G->E = NULL;
  }
  else if (n >= 1) {
    G->n = n;
    G->V = (graph_node*)calloc(n, sizeof(graph_node));
    G->E = (graph_edge*)calloc(n * (n + 1) / 2, sizeof(graph_edge));
  }
  
#ifdef DEBUG
  if (n < 0) {
    printf("error: graph_init\n");
    exit(1);
  }
#endif

}



void graph_delete(graph* G) {

  free(G->V);
  free(G->E);
  G->V = NULL;
  G->E = NULL;
  G->n = 0;

}



void graph_copy(graph* FROM, graph* TO) {

  int i;
  int n = FROM->n;

  graph_delete(TO);
  graph_init(TO, n);

  for (i = 0; i < n; i++) {
    TO->V[i].deg = FROM->V[i].deg;
    TO->V[i].deg_frc = FROM->V[i].deg_frc;
    TO->V[i].deg_frb = FROM->V[i].deg_frb;
  }
  for (i = 0; i < n * (n + 1) / 2; i++) {
    TO->E[i].flag = FROM->E[i].flag;
    TO->E[i].cost = FROM->E[i].cost;
    TO->E[i].constr = FROM->E[i].constr;
    TO->E[i].delta = FROM->E[i].delta;
  }

}



void graph_insert_edge(graph* G, int u, int v, double cost, int constr) {

#ifdef DEBUG
  if (u < 0 || v < 0 || u >= G->n || v >= G->n || u == v || graph_adjacent_nodes(G, u, v)) {
    printf("error: graph_insert_edge: %d %d\n", u, v);
    exit(1);
  }
#endif

  (u > v) ? ( G->E[ u*(u+1)/2 + v ].flag = 1 ) : ( G->E[ v*(v+1)/2 + u ].flag = 1 );
  (u > v) ? ( G->E[ u*(u+1)/2 + v ].cost = cost ) : ( G->E[ v*(v+1)/2 + u ].cost = cost );
  (u > v) ? ( G->E[ u*(u+1)/2 + v ].constr = constr ) : ( G->E[ v*(v+1)/2 + u ].constr = constr );
  (u > v) ? ( G->E[ u*(u+1)/2 + v ].delta = 0.0 ) : ( G->E[ v*(v+1)/2 + u ].delta = 0.0 );

  G->V[u].deg++;
  G->V[v].deg++;

  if (constr == FORCED) {
    G->V[u].deg_frc++;
    G->V[v].deg_frc++;
  }
  else if (constr == FORBIDDEN) {
    G->V[u].deg_frb++;
    G->V[v].deg_frb++;
  }

}



void graph_remove_edge(graph* G, int u, int v) {

#ifdef DEBUG
  if (u < 0 || v < 0 || u >= G->n || v >= G->n || u == v || !graph_adjacent_nodes(G, u, v)) {
    printf("error: graph_remove_edge: %d %d\n", u, v);
    exit(1);
  }
#endif

  int constr = (u > v) ? G->E[ u*(u+1)/2 + v ].constr : G->E[ v*(v+1)/2 + u].constr;

  (u > v) ? ( G->E[ u*(u+1)/2 + v ].flag = 0 ) : ( G->E[ v*(v+1)/2 + u].flag = 0 );
  (u > v) ? ( G->E[ u*(u+1)/2 + v ].cost = 0.0 ) : ( G->E[ v*(v+1)/2 + u].cost = 0.0 );
  (u > v) ? ( G->E[ u*(u+1)/2 + v ].constr = FREE ) : ( G->E[ v*(v+1)/2 + u].constr = FREE );
  (u > v) ? ( G->E[ u*(u+1)/2 + v ].delta = 0.0 ) : ( G->E[ v*(v+1)/2 + u].delta = 0.0 );

  G->V[u].deg--;
  G->V[v].deg--;

  if (constr == FORCED) {
    G->V[u].deg_frc--;
    G->V[v].deg_frc--;
  }
  else if (constr == FORBIDDEN) {
    G->V[u].deg_frb--;
    G->V[v].deg_frb--;
  }

}



void graph_set_edge_cost(graph* G, int u, int v, double cost) {

#ifdef DEBUG
  if (u < 0 || v < 0 || u >= G->n || v >= G->n || u == v || !graph_adjacent_nodes(G, u, v)) {
    printf("error: graph_set_edge_cost: %d %d\n", u, v);
    exit(1);
  }
#endif

  (u > v) ? ( G->E[ u*(u+1)/2 + v ].cost = cost ) : ( G->E[ v*(v+1)/2 + u ].cost = cost );

}

double graph_get_edge_cost(graph* G, int u, int v) {

#ifdef DEBUG
  if (u < 0 || v < 0 || u >= G->n || v >= G->n || u == v || !graph_adjacent_nodes(G, u, v)) {
    printf("error: graph_get_edge_cost: %d %d\n", u, v);
    exit(1);
  }
#endif

  return (u > v) ? G->E[ u*(u+1)/2 + v ].cost : G->E[ v*(v+1)/2 + u ].cost;

}



void graph_set_edge_constr(graph* G, int u, int v, int constr) {

#ifdef DEBUG
  if (u < 0 || v < 0 || u >= G->n || v >= G->n || u == v || !graph_adjacent_nodes(G, u, v)) {
    printf("error: graph_set_edge_constr\n");
    exit(1);
  }
#endif

  int old_constr = (u > v) ? G->E[ u*(u+1)/2 + v ].constr : G->E[ v*(v+1)/2 + u ].constr;

  (u > v) ? ( G->E[ u*(u+1)/2 + v ].constr = constr ) : ( G->E[ v*(v+1)/2 + u ].constr = constr );

  if (old_constr == FORCED) {
    G->V[u].deg_frc--;
    G->V[v].deg_frc--;
  }
  else if (old_constr == FORBIDDEN) {
    G->V[u].deg_frb--;
    G->V[v].deg_frb--;
  }

  if (constr == FORCED) {
    G->V[u].deg_frc++;
    G->V[v].deg_frc++;
  }
  else if (constr == FORBIDDEN) {
    G->V[u].deg_frb++;
    G->V[v].deg_frb++;
  }

}



int graph_get_edge_constr(graph* G, int u, int v) {

#ifdef DEBUG
  if (u < 0 || v < 0 || u >= G->n || v >= G->n || u == v || !graph_adjacent_nodes(G, u, v)) {
    printf("error: graph_get_edge_constr: %d %d\n", u, v);
    exit(1);
  }
#endif

  return (u > v) ? G->E[ u*(u+1)/2 + v ].constr : G->E[ v*(v+1)/2 + u ].constr;

}


void graph_set_edge_delta(graph* G, int u, int v, double delta) {

#ifdef DEBUG
  if (u < 0 || v < 0 || u >= G->n || v >= G->n || u == v || !graph_adjacent_nodes(G, u, v)) {
    printf("error: graph_set_edge_delta: %d %d\n", u, v);
    exit(1);
  }
#endif

  (u > v) ? ( G->E[ u*(u+1)/2 + v ].delta = delta ) : ( G->E[ v*(v+1)/2 + u ].delta = delta );

}

double graph_get_edge_delta(graph* G, int u, int v) {

#ifdef DEBUG
  if (u < 0 || v < 0 || u >= G->n || v >= G->n || u == v || !graph_adjacent_nodes(G, u, v)) {
    printf("error: graph_get_edge_delta: %d %d\n", u, v);
    exit(1);
  }
#endif

  return (u > v) ? G->E[ u*(u+1)/2 + v ].delta : G->E[ v*(v+1)/2 + u ].delta;

}



int graph_adjacent_nodes(graph* G, int u, int v) {

#ifdef DEBUG
  if (u < 0 || v < 0 || u >= G->n || v >= G->n || u == v) {
    printf("error: graph_adjacent_nodes: %d %d\n", u, v);
    exit(1);
  }
#endif

  return (u > v) ? G->E[ u*(u+1)/2 + v ].flag : G->E[ v*(v+1)/2 + u ].flag;

}



double graph_get_cost(graph* G) {

  int i, j;
  int n = G->n;
  double c = 0.0;
  for (i = 0; i < n; i++) {
    for (j = i+1; j < n; j++) {
      if (graph_adjacent_nodes(G, i, j)) {
        c += graph_get_edge_cost(G, i, j);
      }
    }
  }
  return c;

}

