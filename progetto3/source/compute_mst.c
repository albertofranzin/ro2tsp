#include "compute_mst.h"

void compute_mst(graph* G, tree* T, int root) {
  int i, j, v;
  double min;

  int n = (*G).n;
  // stessa cosa, ma così facendo si arrangia da solo a deallocare la memoria degli array
  // senza bisogno delle tre free in fondo al metodo
  short flag[n];// = (short*)malloc(sizeof(short) * n);
  int pred[n];// = (int*)malloc(sizeof(int) * n);
  double cost[n];// = (double*)malloc(sizeof(double) * n);


  tree_delete(T);
  tree_init(T, n);
  tree_set_root(T, root);

  /* inizializzazione delle principali strutture;
   * - pongo a root il predecessore dei nodi direttamente connessi alla radice;
   * - pongo a 0 il predecessore dei nodi non direttamente connessi alla radice;
   *   si tratta di nodi di cui al momento non si conosce la raggiungibilità (da root);
   */
  for (j = 1; j <= n; j++) {
    if (j == root) {
      flag[root-1] = 1;
      pred[root-1] = 0;
    }
    else {
      flag[j-1] = 0;
      if (graph_adjacent_nodes(G, root, j)) {
        pred[j-1] = root;
        cost[j-1] = graph_get_edge_cost(G, root, j);
      }
      else {
        pred[j-1] = 0;
      }
    }
  }

  /* loop principale di n-1 cicli;
   * - se G è connesso, allora ad ogni ciclo viene aggiunto un nuovo lato;
   * - se G non è connesso, è possibile che negli ultimi n-k-1 cicli non venga aggiunto
   *   alcun lato; k è il numero di nodi del sottografo connesso al quale appartiene root;
   */ 
  for (i = 1; i <= n-1; i++) {

    // ricerca un primo nodo candidato per estendere la regione dei nodi già ricoperti dall'albero;
    for (j = 1; j <= n; j++) {
      if (pred[j-1] > 0 && flag[j-1] == 0) {
        min = cost[j-1];
        v = j;
        break;
      }
    }

    // a partire dal candidato, cerca il miglior candidato per estendere la regione dei nodi già ricoperti dall'albero;
    for (j = 1; j <= n; j++) {
      if (pred[j-1] > 0 && flag[j-1] == 0 && cost[j-1] < min) {
        min = cost[j-1];
        v = j;
      }
    }

    // segna il nodo come ricoperto;
    flag[v-1] = 1;
    // inserisci un nuovo lato;
    tree_insert_edge(T, pred[v-1], v);
    tree_set_edge_cost(T, pred[v-1], v, graph_get_edge_cost(G, pred[v-1], v));

    // aggiorna i costi di raggiungibilità dei nodi non ancora ricoperti e raggiungibili direttamente dal nodo appena ricoperto;
    for (j = 1; j <= n; j++) {
      if (graph_adjacent_nodes(G, v, j) && (pred[j-1] == 0 || (pred[j-1] > 0 && flag[j-1] == 0 && graph_get_edge_cost(G, v, j) < cost[j-1]))) {
        pred[j-1] = v;
        cost[j-1] = graph_get_edge_cost(G, v, j);
      }
    }
  }
}
