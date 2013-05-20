#include "graph.h"

void graph_init(graph* G, int n) {

  if (n == 0) {
    (*G).n = 0;
    (*G).V = NULL;
    (*G).E = NULL;
  }
  else if (n >= 1) {
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
    (*TO).V[i].deg_forced = (*FROM).V[i].deg_forced;
    (*TO).V[i].deg_forbidden = (*FROM).V[i].deg_forbidden;
  }
  for (i = 0; i < n * (n + 1) / 2; i++) {
    (*TO).E[i].flag = (*FROM).E[i].flag;
    (*TO).E[i].cost = (*FROM).E[i].cost;
    (*TO).E[i].constr = (*FROM).E[i].constr;
    (*TO).E[i].delta = (*FROM).E[i].delta;
  }

}

void graph_insert_edge(graph* G, int u, int v, double cost, int constr) {
  if (u >= 1 && v >= 1 && u <= (*G).n && v <= (*G).n && u != v && !graph_adjacent_nodes(G, u, v)) {
    (u > v) ? ( (*G).E[ u*(u-1)/2 + v-1 ].flag = 1 ) : ( (*G).E[ v*(v-1)/2 + u-1].flag = 1 );
    (u > v) ? ( (*G).E[ u*(u-1)/2 + v-1 ].cost = cost ) : ( (*G).E[ v*(v-1)/2 + u-1].cost = cost );
    (u > v) ? ( (*G).E[ u*(u-1)/2 + v-1 ].constr = constr ) : ( (*G).E[ v*(v-1)/2 + u-1].constr = constr );
    (u > v) ? ( (*G).E[ u*(u-1)/2 + v-1 ].delta = 0.0 ) : ( (*G).E[ v*(v-1)/2 + u-1].delta = 0.0 );

    (*G).V[u-1].deg++;
    (*G).V[v-1].deg++;

    if (constr == FORCED) {
      (*G).V[u-1].deg_forced++;
      (*G).V[v-1].deg_forced++;
    }
    else if (constr == FORBIDDEN) {
      (*G).V[u-1].deg_forbidden++;
      (*G).V[v-1].deg_forbidden++;
    }
  }

  else {
    printf("error: graph_insert_edge: %d %d\n", u, v);
    exit(EXIT_FAILURE);
  }
}

void graph_remove_edge(graph* G, int u, int v) {
  if (u >= 1 && v >= 1 && u <= (*G).n && v <= (*G).n && u != v && graph_adjacent_nodes(G, u, v)) {

    int constr = (u > v) ? (*G).E[ u*(u-1)/2 + v-1 ].constr : (*G).E[ v*(v-1)/2 + u-1].constr;

    (u > v) ? ( (*G).E[ u*(u-1)/2 + v-1 ].flag = 0 ) : ( (*G).E[ v*(v-1)/2 + u-1].flag = 0 );
    (u > v) ? ( (*G).E[ u*(u-1)/2 + v-1 ].cost = 0.0 ) : ( (*G).E[ v*(v-1)/2 + u-1].cost = 0.0 );
    (u > v) ? ( (*G).E[ u*(u-1)/2 + v-1 ].constr = FREE ) : ( (*G).E[ v*(v-1)/2 + u-1].constr = FREE );
    (u > v) ? ( (*G).E[ u*(u-1)/2 + v-1 ].delta = 0.0 ) : ( (*G).E[ v*(v-1)/2 + u-1].delta = 0.0 );

    (*G).V[u-1].deg--;
    (*G).V[v-1].deg--;

    if (constr == FORCED) {
      (*G).V[u-1].deg_forced--;
      (*G).V[v-1].deg_forced--;
    }
    else if (constr == FORBIDDEN) {
      (*G).V[u-1].deg_forbidden--;
      (*G).V[v-1].deg_forbidden--;
    }
  }

  else {
    printf("error: graph_remove_edge: %d %d\n", u, v);
    exit(EXIT_FAILURE);
  }
}

void graph_set_edge_delta(graph* G, int u, int v, double delta) {

  if (u >= 1 && v >= 1 && u <= (*G).n && v <= (*G).n && u != v && graph_adjacent_nodes(G, u, v)) {
    (u > v) ? ( (*G).E[ u*(u-1)/2 + v-1 ].delta = delta ) : ( (*G).E[ v*(v-1)/2 + u-1 ].delta = delta );
  }
  else {
    printf("error: graph_set_edge_delta: %d %d\n", u, v);
    exit(EXIT_FAILURE);
  }
}

void graph_set_edge_cost(graph* G, int u, int v, double cost) {

  if (u >= 1 && v >= 1 && u <= (*G).n && v <= (*G).n && u != v && graph_adjacent_nodes(G, u, v)) {
    (u > v) ? ( (*G).E[ u*(u-1)/2 + v-1 ].cost = cost ) : ( (*G).E[ v*(v-1)/2 + u-1 ].cost = cost );
  }
  else {
    printf("error: graph_set_edge_cost: %d %d\n", u, v);
    exit(EXIT_FAILURE);
  }
}

void graph_set_edge_constr(graph* G, int u, int v, int constr) {

  int old_constr = (u > v) ? (*G).E[ u*(u-1)/2 + v-1 ].constr : (*G).E[ v*(v-1)/2 + u-1 ].constr;

  if (u >= 1 && v >= 1 && u <= (*G).n && v <= (*G).n && u != v && graph_adjacent_nodes(G, u, v)) {
    (u > v) ? ( (*G).E[ u*(u-1)/2 + v-1 ].constr = constr ) : ( (*G).E[ v*(v-1)/2 + u-1 ].constr = constr );

    if (old_constr == FORCED) {
      (*G).V[u-1].deg_forced--;
      (*G).V[v-1].deg_forced--;
    }
    else if (old_constr == FORBIDDEN) {
      (*G).V[u-1].deg_forbidden--;
      (*G).V[v-1].deg_forbidden--;
    }

    if (constr == FORCED) {
      (*G).V[u-1].deg_forced++;
      (*G).V[v-1].deg_forced++;
    }
    else if (constr == FORBIDDEN) {
      (*G).V[u-1].deg_forbidden++;
      (*G).V[v-1].deg_forbidden++;
    }
  }

  else {
    printf("error: graph_set_edge_constr\n");
    exit(EXIT_FAILURE);
  }
}

double graph_get_edge_delta(graph* G, int u, int v) {

  if (u >= 1 && v >= 1 && u <= (*G).n && v <= (*G).n && u != v && graph_adjacent_nodes(G, u, v)) {
    return (u > v) ? (*G).E[ u*(u-1)/2 + v-1 ].delta : (*G).E[ v*(v-1)/2 + u-1 ].delta;
  }
  else {
    printf("error: graph_get_edge_delta\n");
    exit(EXIT_FAILURE);
  }
}

double graph_get_edge_cost(graph* G, int u, int v) {

  if (u >= 1 && v >= 1 && u <= (*G).n && v <= (*G).n && u != v && graph_adjacent_nodes(G, u, v)) {
    return (u > v) ? (*G).E[ u*(u-1)/2 + v-1 ].cost : (*G).E[ v*(v-1)/2 + u-1 ].cost;
  }
  else {
    printf("error: graph_get_edge_cost: %d %d\n", u, v);
    exit(EXIT_FAILURE);
  }
}

int graph_get_edge_constr(graph* G, int u, int v) {

  if (u >= 1 && v >= 1 && u <= (*G).n && v <= (*G).n && u != v && graph_adjacent_nodes(G, u, v)) {
    return (u > v) ? (*G).E[ u*(u-1)/2 + v-1 ].constr : (*G).E[ v*(v-1)/2 + u-1 ].constr;
  }
  else {
    printf("error: graph_get_edge_constr\n");
    exit(EXIT_FAILURE);
  }
}

int graph_get_node_deg(graph* G, int v) {
  if (v >= 1 && v <= (*G).n) {
    return (*G).V[v-1].deg;
  }
  else {
    printf("error: graph_get_node_deg: %d\n", v);
    exit(EXIT_FAILURE);
  }
}

int graph_get_node_deg_forced(graph* G, int v) {
  if (v >= 1 && v <= (*G).n) {
    return (*G).V[v-1].deg_forced;
  }
  else {
    printf("error: graph_get_node_deg_forced\n");
    exit(EXIT_FAILURE);
  }
}

int graph_get_node_deg_forbidden(graph* G, int v) {
  if (v >= 1 && v <= (*G).n) {
    return (*G).V[v-1].deg_forbidden;
  }
  else {
    printf("error: graph_get_node_deg_forbidden\n");
    exit(EXIT_FAILURE);
  }
}

int graph_adjacent_nodes(graph* G, int u, int v) {
  if (u >= 1 && v >= 1 && u <= (*G).n && v <= (*G).n && u != v) {
    return (u > v) ? (*G).E[ u*(u-1)/2 + v-1 ].flag : (*G).E[ v*(v-1)/2 + u-1 ].flag;
  }
  else {
    printf("error: graph_adjacent_nodes: %d %d\n", u, v);
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


int graph_remove_fat_edges(graph *G, onetree *OT, double ub) {

  int i, j, removed = 0;

  int n = G->n;

  compute_deltas(G, OT);

  double lb = onetree_get_cost(OT);

  for (i = 1; i <= n; i++) {
    for (j = i+1; j <= n; j++) {
      if (lb + graph_get_edge_delta(G, i, j) > ub) {
        graph_set_edge_constr(G, i, j, FORBIDDEN);
        removed++;
      }
    }
  }

  return removed;
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
