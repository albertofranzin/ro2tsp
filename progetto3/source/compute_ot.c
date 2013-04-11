#include "graph.h"
#include "onetree.h"
#include "compute_mst.h"
#include "compute_ot.h"

void compute_ot(graph* G, onetree* OT) {
  int i, u, v;
  double min;

  int n = (*G).n;
  double cost_vect[n];

  onetree_delete(OT);
  onetree_init(OT, n);

  /* cerca il lato con costo minore tra tutti i lati incidenti nel nodo 1;
   * memorizza il secondo estremo u del lato;
   */
  u = 2;
  min = graph_get_edge_cost(G, 1, u);
  for (i = 2; i <= n; i++) {
    if (graph_get_edge_cost(G, 1, i) < min) {
      u = i;
      min = graph_get_edge_cost(G, 1, i);
    }
  }

  /* cerca il secondo lato con costo minore tra tutti i lati incidenti nel nodo 1;
   * memorizza il secondo estremo v del lato;
   */
  v = (u == 2) ? 3 : 2;
  min = graph_get_edge_cost(G, 1, v);
  for (i = 2; i <= n; i++) {
    if (i != u && graph_get_edge_cost(G, 1, i) < min) {
      v = i;
      min = graph_get_edge_cost(G, 1, i);
    }
  }

  /* memorizza i costi dei lati incidenti in 1;
   */
  for (i = 2; i <= n; i++)
    cost_vect[i-1] = graph_get_edge_cost(G, 1, i);

  /* isola il nodo 1
   */
  for (i = 2; i <= n; i++)
    graph_remove_edge(G, 1, i);

  /* calcola albero ricoprente minimo di G con radice 2;
   */
  compute_mst(G, &(*OT).tree, 2);

  /* aggiungi i due lati di costo minimo incidenti in 1;
   */
  onetree_insert_edge(OT, 1, u, cost_vect[u-1]);
  onetree_insert_edge(OT, 1, v, cost_vect[v-1]);

  /* ripristina i lati di G incidenti in 1;
   */
  for (i = 2; i <= n; i++)
    graph_insert_edge(G, 1, i, cost_vect[i-1]);

}

