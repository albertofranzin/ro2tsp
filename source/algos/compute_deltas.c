// Note: we assume G be a complete graph.
// Moreover, we assume that OT is a MINIMUM 1-tree of G.

#include "compute_deltas.h"

int compute_deltas(graph* G, onetree* OT) { // Linear time complexity!!!

  int n = G->n;
  int i, j, k, count, root, curr, n_leaves;


  int leaves[n];
  int path[n];
  int ordered_vertices[n];
  int* flags = (int*)calloc(n, sizeof(int));

  // --------------------------------------------------------------------------------------------------------------------------------------
  // We write the vertices of the 1-tree in a list: we set 1 to be the first vertex in the list, and we sort the remaining vertices {2, ..., n} in such a way that the precedence relation 
  // induced by the rooted spanning-tree portion of the 1-tree is always verified. 
  // Specifically, let ordered_vertices[] be a vector of length n, then:
  // ordered_vertices[0] = 1, 
  // ordered_vertices[1] = root, where root is the root of the spanning tree portion of the 1-tree.
  // {ordered_vertices[i] : i = 1, 2, ... n-1} = {2, ..., n} such that if ordered_vertices[i] = u, ordered_vertices[j] = v and u = ancestor(v), then i < j.
  // Def: if u = pred(v) then u = ancestor(v); if u = pred(z) and z = ancestor(v) then u = ancestor(v).


  // Let T be the spanning-tree portion of the 1-tree. T is a spanning tree on vertices {2, 3, ..., n}.
  // List in a vector leaves[] the leaves of T.
  n_leaves = 0;
  leaves[n_leaves++] = OT->V[0].pred; // Note that T can be obtained from the 1-tree removing the two edges incidents to 1, let be edges (1, v1) and (1, v2). Let v1 be the root of T, then
                                                // v2 has to be considered as a leave (even if deg(v2) = 2, since in the 1-tree v2 is adjacent to 1 and to some other vertex of T). Remember also that
                                                // the 1-tree is build in such a way that, pred(v1)=1 and pred(1)=v2, where v1 is the root of T.

  for (i = 0; i < n; i++) {
    if (OT->V[i].deg == 1)

      leaves[n_leaves++] = i;

  }

  // Get the root of T.
  root = OT->nn1;

 
  ordered_vertices[0] = 0;  flags[0] = 1;
  ordered_vertices[1] = root; flags[root] = 1;
  k = 2;
  // Starting from each leave, walk from that leave backward toward the root, following the chain of predecessors.
  // Flag each vertex in the path as visited. If some vertex is reached which is already flagged, than stop the walk
  // and do the same starting from another leave. Since at the beginning the only flagged vertex is the root,
  // then the first path will be a path from the selected leave to the root. 
  for (i = 0; i < n_leaves; i++) {

    count = 0;
    curr = leaves[i];

    while (flags[curr] != 1) {

      path[count++] = curr;
      flags[curr] = 1;
      curr = OT->V[curr].pred;

    }

    for (j = 0; j < count; j++) { // Write the vertices in reverese order.
      ordered_vertices[k++] = path[count-1-j];
    }

  }

  // --------------------------------------------------------------------------------------------------------------------------------------
  // For each edge, compute the associated delta. 
  // Let (i, j) be an edge of the complete graph which does belongs to the 1-tree. Let also i, j != 1. Adding (i, j) to 
  // the 1-tree will create a new cycle C. Let e_max be the edge with maximum cost in C\{(i, j)}, and let c(i, j), c(e_max) be the costs
  // of (i, j) and e_max respectively. Then: delta(i, j) = c(i, j) - c(e_max).
  // If (i, j) is such that i or j is equal to 1, and the edge does not belong to the 1-tree. Let e_max be the edge with maximum costs
  // between the two edges incident to 1. Then delta(i, j) = c(i, j) - c(e_max).
  // If (i, j) belongs to the 1-tree, then delta(i, j) = 0.

  // Since OT is a minimum 1-tree of G, then delta(i, j) >= 0 for each edge (i, j).


  // First step: for each edge (i, j), i,j != 1, compute the cost of the edge of maximum cost in C\{(i, j)}, where C is the cycle formed by adding (i, j) to the 1-tree;
  // if (i, j) does not belong to the 1-tree then use as computed value the cost of the edge itself.
  // Important: the computation if performed selecting the edges by visiting the vertices in the appropriate order (the order in which they are written in the vector ordered_vertices).

  int u, v, pred_v;
  double delta;

  for (i = 1; i < n; i++) {
    for (j = i+1; j < n; j++) {

      if (i != 0 && j != 0) {

      u = ordered_vertices[i];
      v = ordered_vertices[j];
      pred_v = OT->V[v].pred;

      if (pred_v == u) { // The edge belongs to the 1-tree.

	graph_set_edge_delta(G, u, v, graph_get_edge_cost(G, u, v));

      }
      else {

	delta = ((graph_get_edge_delta(G, u, pred_v) > graph_get_edge_cost(G, v, pred_v))) ? 
	  graph_get_edge_delta(G, u, pred_v) : 
	  graph_get_edge_cost(G, v, pred_v);
	
	graph_set_edge_delta(G, u, v, delta); // Now delta is not the true delta, it is the cost of the edge of maximum cost in C\{(i, j)} !!!

      }
      }

    }
  }
  

  // Second step: for each edge compute the associated delta: delta(i, j) = c(i, j) - c(e_max).

  double cost_1v, cost_1u, max_cost;
  v = root;
  u = OT->nn2;
  cost_1v = graph_get_edge_cost(G, 0, v);
  cost_1u = graph_get_edge_cost(G, 0, u);
  max_cost = (cost_1v > cost_1u) ? cost_1v : cost_1u;

  for (i = 0; i < n; i++) {
    for (j = i+1; j < n; j++) {

      graph_set_edge_delta(G, i, j, graph_get_edge_cost(G, i, j) - graph_get_edge_delta(G, i, j));

    }
  }

  for (j = 1; j < n; j++) {

    graph_set_edge_delta(G, 0, j, graph_get_edge_cost(G, 0, j) - max_cost);
    
  }

  graph_set_edge_delta(G, 0, v, 0.0);
  graph_set_edge_delta(G, 0, u, 0.0);

  free(flags);

}
