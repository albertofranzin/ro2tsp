#include "onetree.h"
#include "graph.h"



void onetree_init(onetree* OT, int n) {
  int i;

  if (n == 0) {
    OT->n = 0;
    OT->V = NULL;
    OT->nn1 = -1;
    OT->nn2 = -1;
  }
  else if (n >= 2) {
    OT->n = n;
    OT->V = (onetree_node*)calloc(n, sizeof(onetree_node));
    for (i = 0; i < n; i++) OT->V[i].pred = -1;
    OT->nn1 = -1;
    OT->nn2 = -1;
  }

#ifdef DEBUG
  if (n < 2 && n != 0) {
    printf("error: onetree_init\n");
    exit(1);
  }
#endif

}



void onetree_delete(onetree* OT) {

  free(OT->V);
  OT->V = NULL;
  OT->n = 0;
  OT->nn1 = -1;
  OT->nn2 = -1;

}



void onetree_copy(onetree* FROM, onetree* TO) {

  int i;
  int n = FROM->n;

  onetree_delete(TO);
  onetree_init(TO, n);
  TO->nn1 = FROM->nn1;
  TO->nn2 = FROM->nn2;
  for (i = 0; i < n; i++) {
    TO->V[i].pred = FROM->V[i].pred;
    TO->V[i].deg = FROM->V[i].deg;
  }

}



void onetree_insert_edge(onetree* OT, int u, int v) {

#ifdef DEBUG
  if (u < 0 || v < 0 || u >= OT->n || v >= OT->n || u == v || onetree_adjacent_nodes(OT, u, v) || OT->V[v].pred >= 0) {
    printf("error: onetree_insert_edge: %d %d\n", u, v);
    exit(1);
  }
#endif

  OT->V[v].pred = u;
  OT->V[v].deg++;
  OT->V[u].deg++;

}



void onetree_remove_edge(onetree* OT, int u, int v) {

#ifdef DEBUG
  if (u < 0 || v < 0 || u >= OT->n || v >= OT->n || u == v || !onetree_adjacent_nodes(OT, u, v)) {
    printf("error: onetree_remove_edge: %d %d\n", u, v);
    exit(1);
  }
#endif

  if (OT->V[v].pred == u) {

    OT->V[v].pred = -1;
    OT->V[v].deg--;
    OT->V[u].deg--;
    
  }
  else if (OT->V[u].pred == v) {

    OT->V[u].pred = -1;
    OT->V[v].deg--;
    OT->V[u].deg--;

  }

}



int onetree_adjacent_nodes(onetree* OT, int u, int v) {

#ifdef DEBUG
  if (u < 0 || v < 0 || u >= OT->n || v >= OT->n || u == v) {
    printf("error: onetree_adjacent_nodes: %d %d\n", u, v);
    exit(1);
  }
#endif

  return ((OT->V[v].pred == u) || OT->V[u].pred == v);

}




double onetree_get_cost(onetree* OT, graph* G) {

  int i;
  double c = 0.0;
  int n = OT->n;

  for (i = 0; i < n; i++) {
    if (OT->V[i].pred >= 0)
      c += graph_get_edge_cost(G, OT->V[i].pred, i);
  }
  return c;

}



int onetree_is_cycle(onetree* OT) {

  int i;
  int n = OT->n; 

  for (i = 0; i < n; i++) {
    if (OT->V[i].deg != 2)
      return 0;
  }
  return 1;

}



void onetree_to_cycle(onetree* OT, cycle* C) {

  int i;
  int v;
  int n = OT->n;

  cycle_delete(C);
  cycle_init(C, n);

  v = 0;
  for (i = 0; i < n; i++) {

    C->nodes[i] = v;
    v = OT->V[v].pred;
  
  }

}
