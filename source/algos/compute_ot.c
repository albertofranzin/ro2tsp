#include "compute_ot.h"

int compute_ot(graph* G, onetree* OT) {
  int i, j, v, v1, v2, flag, some_forced_edges;
  double min1, min2, c;

  int n = (*G).n;



  onetree_delete(OT);
  onetree_init(OT, n);

  // ----------------------------------------------------------------------------------------------------------------------------------------
  // ----------------------------------------------------------------------------------------------------------------------------------------

  // Select two vertices v1 and v2 such that (1, v1) and (1, v2) are the two edges with lowest cost among all free or forced edges incident to vertex 1.
  // Note that forced edges take priority on any free edge, independently of their costs.
  // If there are more than n-3 forbidden edges incident to 1, than the procedures fails (it's impossibile to build the 1-tree without invalidating some forbidden-edge-constraint).

  // Find the first node v1.
  some_forced_edges = 0;
  for (v = 1; v <= n; v++) {
    if (v != 1 && graph_get_edge_constr(G, 1, v) == FORCED) {
      some_forced_edges = 1;
      break;
    }
  }

  v1 = 0;
  if (some_forced_edges == 0) {
    flag = 0;
    for (v = 1; v <= n; v++) {
      if (v != 1 && graph_get_edge_constr(G, 1, v) != FORBIDDEN) {
	c = graph_get_edge_cost(G, 1, v);
	if (flag == 0) {
	  v1 = v;
	  min1 = c;
	  flag = 1;
	}
	else if (flag == 1 && c < min1) {
	  v1 = v;
	  min1 = c;
	}
      }
    }
  }

  else {
    flag = 0;
    for (v = 1; v <= n; v++) {
      if (v != 1 && graph_get_edge_constr(G, 1, v) == FORCED) {
	c = graph_get_edge_cost(G, 1, v);
	if (flag == 0) {
	  v1 = v;
	  min1 = c;
	  flag = 1;
	}
	else if (flag == 1 && c < min1) {
	  v1 = v;
	  min1 = c;
	}
      }
    }
  }

  if (v1 == 0) {
    return FAILURE;
  }

  // Find the second node v2.
  some_forced_edges = 0;
  for (v = 1; v <= n; v++) {
    if (v != 1 && v != v1 && graph_get_edge_constr(G, 1, v) == FORCED) {
      some_forced_edges = 1;
      break;
    }
  }

  v2 = 0;
  if (some_forced_edges == 0) {
    flag = 0;
    for (v = 1; v <= n; v++) {
      if (v != 1 && v != v1 && graph_get_edge_constr(G, 1, v) != FORBIDDEN) {
	c = graph_get_edge_cost(G, 1, v);
	if (flag == 0) {
	  v2 = v;
	  min2 = c;
	  flag = 1;
	}
	else if (flag == 1 && c < min2) {
	  v2 = v;
	  min2 = c;
	}
      }
    }
  }

  else {
    flag = 0;
    for (v = 1; v <= n; v++) {
      if (v != 1 && v != v1 && graph_get_edge_constr(G, 1, v) == FORCED) {
	c = graph_get_edge_cost(G, 1, v);
	if (flag == 0) {
	  v2 = v;
	  min2 = c;
	  flag = 1;
	}
	else if (flag == 1 && c < min2) {
	  v2 = v;
	  min2 = c;
	}
      }
    }
  }

  if (v2 == 0) {
    return FAILURE;
  }

  // ----------------------------------------------------------------------------------------------------------------------------------------
  // ----------------------------------------------------------------------------------------------------------------------------------------

  // The following portion of the code is no more than an easy adaptation of the code written for the computation of the minimung spanning tree of a graph (take a look to compute_mst.c).
  // The only differences can be found in that we compute a spanning tree on the nodes 2, 3, ..., n instead than a spanning tree on the whole graph, 
  // and in that we interrupt the precedure in case we can't build a tree spanning across all the nodes 2, 3, ..., n due to the presence of too many forbidden edges (-> return FAILURE). 


  int k, v_min, new_constr, old_constr;
  double cost_min, new_cost, old_cost; 
  list_node* node_curr;
  list_node* node_min;



  int pred[n-1];      
		    
		    

  double cost[n-1];



  list not_visited;



  list_init(&not_visited);


  // ------------------------------------------

  // We choose the root of the tree spanning on vertices 2, 3, ..., n.
  // It is chosen as the vertex with minimum index between v1 and v2. In this way, the internal organization of the 1-tree (in particular the pointers to the predecessor of each vertex, see onetree.h)
  // will not rely on the costs of the edges of G, but only on the topological structure of the result. In particular, two 1-trees having the same topology, will have the same internal organization.
  // i.e. vertices with the same index in both 1-trees will have the same predecessor.
  // This can be useful when computing and comparing the costs of two 1-trees; note in particular that two 1-trees built respectively from two graph with the same topology but different edge-costs, have the same topology:
  // if we assign to the two 1-trees the same edge-costs and we compute their costs, we will get the same result despite of rounding errors.

  int tmp;
  if (v2 < v1) {
    tmp = v2;
    v2 = v1;
    v1 = tmp;
  }
  int root = v1;


  // ------------------------------------------

  // Initial operations.

  for (v = 2; v <= n; v++) {

    if (v == root) {

      pred[root-2] = 0;

    }
    else {

      list_push_last(&not_visited, v);

      if (graph_get_edge_constr(G, root, v) != FORBIDDEN) {
	pred[v-2] = root;
	cost[v-2] = graph_get_edge_cost(G, root, v);
      }

      else {
        pred[v-2] = 0;
      }

    }

  }


  // ------------------------------------------

  // Main loop.

  for (k = 0; k < n-2; k++) {


    // ------------------------------------------

    // We try to extend the region of visited vertices. 

    some_forced_edges = 0;
    node_curr = list_get_first(&not_visited);
    for (i = 0; i < n-k-2; i++) {

      v = (*node_curr).data;
      if (pred[v-2] >  0 && graph_get_edge_constr(G, pred[v-2], v) == FORCED) {
	some_forced_edges = 1;
	break;
      }
      node_curr = list_get_next(&not_visited, node_curr);
    }

    if (some_forced_edges == 0) {

      flag = 0;
      v_min = 0;
      node_curr = list_get_first(&not_visited);
      for (i = 0; i < n-k-2; i++) {
	v = (*node_curr).data;

	if (flag == 0 && pred[v-2] > 0) {
	  v_min = v;
	  cost_min = cost[v-2];
	  node_min = node_curr;
	  flag = 1;
	}

	else if (flag == 1 && pred[v-2] > 0 && cost[v-2] < cost_min) {
	  v_min = v;
	  cost_min = cost[v-2];
	  node_min = node_curr;
	}

	node_curr = list_get_next(&not_visited, node_curr);
      }

    }
      
    else {

      flag = 0;
      v_min = 0;
      node_curr = list_get_first(&not_visited);
      for (i = 0; i < n-k-2; i++) {
	v = (*node_curr).data;

	if (flag == 0 && pred[v-2] > 0 && graph_get_edge_constr(G, pred[v-2], v) == FORCED) {
	  v_min = v;
	  cost_min = cost[v-2];
	  node_min = node_curr;
	  flag = 1;
	}

	else if (flag == 1 && pred[v-2] > 0 && graph_get_edge_constr(G, pred[v-2], v) == FORCED && cost[v-2] < cost_min) {
	  v_min = v;
	  cost_min = cost[v-2];
	  node_min = node_curr;
	}

	node_curr = list_get_next(&not_visited, node_curr);
      }

    }

    if (v_min == 0) { 
      return FAILURE;
    }

    // ------------------------------------------

    // Remove the selected vertex from the list of not-visited vertices and insert the new edge in the solution.

    list_remove(&not_visited, node_min);
    onetree_insert_edge(OT, pred[v_min-2], v_min, graph_get_edge_cost(G, pred[v_min-2], v_min), graph_get_edge_constr(G, pred[v_min-2], v_min));


    // ------------------------------------------

    // For each not-visited vertex v, update pred[v] and cost[v].

    if (n-k-3 > 0) { // The list contains one less vertex than before, and it may be empty if we are done.

      node_curr = list_get_first(&not_visited);
      for (i = 0; i < n-k-3; i++) {
	v = (*node_curr).data;
	new_cost = graph_get_edge_cost(G, v_min, v);
	new_constr = graph_get_edge_constr(G, v_min, v);

	if (new_constr != FORBIDDEN) {

	  if (pred[v-2] == 0) { // Vertex v has no predecessor.

	    pred[v-2] = v_min;
	    cost[v-2] = new_cost;

	  }

	  else { // Vertex v has already a predecessor pred[v].

	    old_cost = graph_get_edge_cost(G, pred[v-2], v);
	    old_constr = graph_get_edge_constr(G, pred[v-2], v);

	    if ( (new_constr == FORCED && old_constr == FREE) ||
		 (new_constr == FREE && old_constr == FREE && new_cost < old_cost) ||
		 (new_constr == FORCED && old_constr == FORCED && new_cost < old_cost) ) {

        	      pred[v-2] = v_min;
 	              cost[v-2] = new_cost;
	      
	    }
	  }
	}

	node_curr = list_get_next(&not_visited, node_curr);
      }

    }
  }


  // ----------------------------------------------------------------------

  // We add to the solution the two edges (1, v1), (1, w2).
  onetree_insert_edge(OT, 1, v1, min1, graph_get_edge_constr(G, 1, v1)); // In practice what we do here is to insert the edge(1, v1) setting 1 as the predecessor of v1 (see onetree_insert_edge procedure);
                                        // we can do this safely since up to now  v1 has been considered as the root of the tree spanning on vertices 2, 3, ..., n, so its predecessor pointer has nevere been reserved.
 
  onetree_insert_edge(OT, v2, 1, min2, graph_get_edge_constr(G, v2, 1));

  (*OT).v1 = v1;
  (*OT).v2 = v2;

  return SUCCESS;
}

