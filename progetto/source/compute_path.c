#include "compute_path.h"

int compute_path(graph* G, graph* P, int from, int to, int flag) {
  int i, j, pred;

  int n = (*G).n;

  /* inizializza P;
   * - l'inizializzazione viene fatta solamente alla prima chiamata della funzione;
   * - per non inizializzare P ad ogni chiamata ricorsiva viene utilizzato il parametro flag (nota: la funzione dev'essere inizialmente chiamata con flag = 0);
   */
  if (flag == 0) {
    deleteGraph(P);
    initGraph(P, n);
    for (i = 1; i <= n; i++) {
      set_node_x(P, i, get_node_x(G, i));
      set_node_y(P, i, get_node_y(G, i));
    }
    flag = 1;
  }

  if (from == to)
    return 1;

  /* trova il predecessore del nodo from lungo il percorso fin'ora calcolato (e salvato in P);
   */
  pred = from;
  for (i = 1; i <= n; i++) {
    if (i != from && adjacent(P, from, i)) {
      pred = i;
      break;
    }
  }

  /* cerca ricorsivamente nei nodi figli, eccetto il nodo from stesso e il nodo predecessore;
   */
  for (i = 1; i <= n; i++) {
    if (i != from && i != pred && adjacent(G, from, i)) {
      insert_edge(P, from, i);
      if (compute_path(G, P, i, to, flag)) {
	return 1;
      }
      else {
	remove_edge(P, from, i); // roll back nel caso il percorso tentato non abbia portato a destinazione;
      }
    }
  }

  return 0;
}
