#include "compute_deltas.h"

// Note: we assume G be a complete graph.
// Moreover, we assume that OT is a minimum 1-tree of G.
int compute_deltas(graph *G, onetree *OT) {

  int n = G->n;
  int i, j, k, count, curr, numleaves;


  int  leaves[n];
  int  path[n];
  int  sorted_vrtx[n];
  int *flags = (int*)calloc(n, sizeof(int));

  for (i = 0; i < n; i++) {
    for (j = i+1; j < n; j++) {
      graph_set_edge_delta(G, i, j, 0.0);
    }
  }

  numleaves = 0;

  if (OT->V[OT->nn2].deg == 2) {
    leaves[numleaves++] = OT->nn2;
  }
  for (i = 1; i < n; i++) {
    // 1st and 2nd condition are redundant
    if (i != OT->nn1     &&
        i != OT->nn2     &&
        OT->V[i].deg == 1  ) {
      leaves[numleaves++] = i;
    }
  }


  sorted_vrtx[0] = 0;  
  flags[0] = 1;

  sorted_vrtx[1] = OT->nn1; 
  flags[OT->nn1] = 1;

  k = 2;

  for (i = 0; i < numleaves; i++) {
    count = 0;
    curr = leaves[i];

    while (flags[curr] != 1) {
      path[count++] = curr;
      flags[curr] = 1;
      curr = OT->V[curr].pred;
    }

    for (j = 0; j < count; j++) { // Write the vertices in reverese order.
      sorted_vrtx[k++] = path[count-1-j];
    }
  }


  int u, v, p;
  double delta_pu, cost_pv, max;

  /*for (u = 0; u < n; ++u) {
    printf("u = %d, sorted_vrtx[%d] = %d, pred = %d\n",
        u, u, sorted_vrtx[u], OT->V[u].pred);
  }*/

  for (i = 1; i < n; i++) {
    for (j = i+1; j < n; j++) {
      u = sorted_vrtx[i];      
      v = sorted_vrtx[j];
      p = OT->V[v].pred;

      // NOTA: NON CAPISCO L'ORDINE DEI NODI...
      if (p == u) {
        delta_pu = 0.0;//BIG;//SMALL; WHAT'S THE DIFFERENCE?
      } else {
        delta_pu = graph_get_edge_delta(G, p, u);
      }
      cost_pv  = graph_get_edge_cost(G, p, v);

      if (p == u) { // the edge belongs to the 1-tree
        graph_set_edge_delta(G, u, v, cost_pv);
      } else {
        if (delta_pu > cost_pv) {
          graph_set_edge_delta(G, u, v, delta_pu);
        } else {
          graph_set_edge_delta(G, u, v, cost_pv);
        }
      } // end if-else p==u
    } // end for j
  } // end for i


  if (graph_get_edge_cost(G, 0, OT->nn1) > graph_get_edge_cost(G, 0, OT->nn2)) {
    max = graph_get_edge_cost(G, 0, OT->nn1);
  } else {
    max = graph_get_edge_cost(G, 0, OT->nn2);
  }

  for (i = 1; i < n; i++) {
    graph_set_edge_delta(G, 0, i, graph_get_edge_cost(G, 0, i) - max);    
  }

  graph_set_edge_delta(G, 0, OT->nn1, 0.0);
  graph_set_edge_delta(G, 0, OT->nn2, 0.0);

  for (i = 1; i < n; i++) {
    for (j = i+1; j < n; j++) {
      graph_set_edge_delta(G, i, j, 
              graph_get_edge_cost(G, i, j) - graph_get_edge_delta(G, i, j));
    }
  }

  free(flags);
  return 0;
}
