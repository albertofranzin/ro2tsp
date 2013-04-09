#include "compute_ot.h"

void compute_ot(graph* G, graph* T) {
  int i, u, v;
  double min;
  int n = (*G).n;

  deleteGraph(T);
  initGraph(T, n);

  /* copia i nodi di G in T;
   */
  for (i = 1; i <= n; i++) {
    set_node_x(T, i, get_node_x(G, i));
    set_node_y(T, i, get_node_y(G, i));
  }

  /* cerca il lato con costo minore tra tutti i lati incidenti nel nodo 1;
   * memorizza il secondo estremo u del lato;
   */
  u = 2;
  min = get_edge_cost(G, 1, u);
  for (i = 2; i <= n; i++) {
    if (get_edge_cost(G, 1, i) < min) {
      u = i;
      min = get_edge_cost(G, 1, i);
    }
  }

  /* cerca il secondo lato con costo minore tra tutti i lati incidenti nel nodo 1;
   * memorizza il secondo estremo v del lato;
   */
  v = (u == 2) ? 3 : 2;
  min = get_edge_cost(G, 1, v);
  for (i = 2; i <= n; i++) {
    if (i != u && get_edge_cost(G, 1, i) < min) {
      v = i;
      min = get_edge_cost(G, 1, i);
    }
  }

  /* memorizza i costi dei lati incidenti in 1;
   */
  double* cost_vect = (double*)malloc(sizeof(double) * n);
  for (i = 2; i <= n; i++)
    cost_vect[i-1] = get_edge_cost(G, 1, i);

  /* isola il nodo 1
   */
  for (i = 2; i <= n; i++)
    remove_edge(G, 1, i);

  /* calcola un albero ricoprente minimo di G con radice 2;
   */
  compute_mst(G, T, 2);

  /* aggiungi a T i due lati di costo minimo incidenti in 1;
   */
  insert_edge(T, 1, u);
  insert_edge(T, 1, v);
  set_edge_cost(T, 1, u, cost_vect[u-1]);
  set_edge_cost(T, 1, v, cost_vect[v-1]);

  /* ripristina i lati di G incidenti in 1;
   */
  for (i = 2; i <= n; i++)
    insert_edge(G, 1, i);

  /* ripristina i costi dei lati di G incidenti in 1;
   */
  for (i = 2; i <= n; i++)
    set_edge_cost(G, 1, i, cost_vect[i-1]);

  free(cost_vect);
}

