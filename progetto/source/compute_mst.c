
#include <stdlib.h>
#include "graph.h"
#include "compute_mst.h"

void compute_mst(graph* G, graph* T, int root) {
  int i, j, v;
  double min;

  int n = (*G).n;
  int* flag = (int*)malloc(sizeof(int) * n);
  int* pred = (int*)malloc(sizeof(int) * n);
  double* cost = (double*)malloc(sizeof(double) * n);


  deleteGraph(T);
  initGraph(T, n);

  for (i = 1; i <= n; i++) {
    set_node_x(T, i, get_node_x(G, i));
    set_node_y(T, i, get_node_y(G, i));
    //set_node_deg(T, i, get_node_deg(G, i));
  }

  for (j = 1; j <= n; j++) {
    if (j == root) {
      flag[root-1] = 1;
      pred[root-1] = 0;
    }
    else {
      flag[j-1] = 0;
      if (adjacent(G, root, j)) {
	pred[j-1] = root;
	cost[j-1] = get_edge_cost(G, root, j);
      }
      else {
	pred[j-1] = 0;
      }
    }
  }

  for (i = 1; i <= n-1; i++) {
    for (j = 1; j <= n; j++) {
      if (pred[j-1] > 0 && flag[j-1] == 0) {
	min = cost[j-1];
	v = j;
	break;
      }
    }
    for (j = 1; j <= n; j++) {
      if (pred[j-1] > 0 && flag[j-1] == 0 && cost[j-1] < min) {
	min = cost[j-1];
	v = j;
      }
    }
    flag[v-1] = 1;
    insert_edge(T, pred[v-1], v);
    set_edge_cost(T, pred[v-1], v, get_edge_cost(G, pred[v-1], v));

    for (j = 1; j <= n; j++) {
      if (adjacent(G, v, j) && (pred[j-1] == 0 || (pred[j-1] > 0 && flag[j-1] == 0 && get_edge_cost(G, v, j) < cost[j-1]))) {
	pred[j-1] = v;
	cost[j-1] = get_edge_cost(G, v, j);
      }
    }
  }

  free(flag);
  free(pred);
  free(cost);
}
