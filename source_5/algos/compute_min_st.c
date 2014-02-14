// NOTA: si assume G grafo completo

#include "compute_min_st.h"

int compute_min_st(graph* G, tree* T, int root) {

  int i, k, v, v_min, new_constr, old_constr, flag, some_forced_edges;
  double cost_min, new_cost, old_cost; 
  list_node* node_curr;
  list_node* node_min;
  int n = G->n;



  int pred[n];      // If vertex v has been visited, then pred[v] is the predecessor of v in the solution (spanning tree);
                    // otherwise, pred[v] it is the vertex among all visited vertices connected to v through a free or forced edge, such that the edge from pred[v] to v has minimum cost.
                    // In the presence of forced edges from the set of visited vertices to vertex v, the choice of pred[v] will be done among all visited vertices adjacent to v through a forced edge.

  double cost[n];   // cost[v] is the cost of the edge from pred[v] to v.



  list not_visited; // This list will contain all vertices not yet visited.



  list_init(&not_visited);
  tree_delete(T);
  tree_init(T, n, root);



  // ----------------------------------------------------------------------------------------------------------------------------------------
  // ----------------------------------------------------------------------------------------------------------------------------------------

  // At the beginning, the set of visited vertices contains only the root; the root is marked as visited; its predecessor is set equal to 0 since the root cannot have any predecessor.
  // All other vertices are marked as not-visited and added to the list not_visited.
  // For all vertices v which are not yet visited and are connected to the root through a free or forced edge, pred[v] is set equal to the root.
  // The predecessor of all other vertices that are connected to the root through a forbidden edge, is set equal to 0.
  for (v = 0; v < n; v++) {

    if (v == root) {

      pred[root] = -1;

    }
    else {

      list_push_last(&not_visited, v);

      if (graph_get_edge_constr(G, root, v) != FORBIDDEN) {
        pred[v] = root;
        cost[v] = graph_get_edge_cost(G, root, v);
      }

      else {
        pred[v] = -1;
      }

    }

  }


  // ----------------------------------------------------------------------------------------------------------------------------------------
  // ----------------------------------------------------------------------------------------------------------------------------------------

  // This is the main loop. At each iteration, one vertex is selected and removed from the list not_visited, and an edge is added to the solution (spanning tree).
  // The number of iterations is no more than n-1, where n is the number of vertices of G.
  // If there is a portion of G which is isolated with respect to the rest because of some forbidden edge, then the loop will be exited before the completion of all n-1 iterations;
  // in this case the solution will not be a spanning tree on the entire graph, but a spanning tree on the connected region containing the root.

  for (k = 0; k < n-1; k++) {

    // ------------------------------------------

    // Now we try to extend the region of visited vertices. 
    // Intuitively, it shall be selected the vertex among all vertices not yet visited which is the "closest" to the visited region.
    // Since pred[v] is the vertex among all visited vertices connected to v through a free or forced edge, such that the edge from pred[v] to v has minimum cost cost[v],
    // then it suffices to chose v such that pred[v] > 0 and cost[v] is minimum.
    // Moreover, any forced edge will take priority on any other free edge, independently of their costs; 
    // so if there is some vertex v such that (pred[v], v) is a forced edge, then the search will be restricted to the vertices v such that (pred[v], v) is a forced edge.
    // Eventually, it can be possibile that no free or forced edge exists from the visited region to the not-visited one: in this case we cannot extend the visited region, so we exit the loop.

    // Check if there is some vertex v such that (pred[v], v) is a forced edge.
    some_forced_edges = 0;
    node_curr = not_visited.head.next;
    for (i = 0; i < n-k-1; i++) {
      v = node_curr->data;
      if (pred[v] >=  0 && graph_get_edge_constr(G, pred[v], v) == FORCED) {
        some_forced_edges = 1;
        break;
      }
      node_curr = node_curr->next;
    }


    // Case #1: no vertex v exists such that (pred[v], v) is a forced edge.
    if (some_forced_edges == 0) {

      flag = 0;
      v_min = -1;
      node_curr = not_visited.head.next;
      for (i = 0; i < n-k-1; i++) {
        v = node_curr->data;

        if (flag == 0 && pred[v] >= 0) {
          v_min = v;
          cost_min = cost[v];
          node_min = node_curr;
          flag = 1;
        }

        else if (flag == 1 && pred[v] >= 0 && cost[v] < cost_min) {
          v_min = v;
          cost_min = cost[v];
          node_min = node_curr;
        }

        node_curr = node_curr->next;
      }

    }
      

    // Case #2: at least one vertex v exists such that (pred[v], v) is a forced edge.
    else {

      flag = 0;
      v_min = -1;
      node_curr = not_visited.head.next;
      for (i = 0; i < n-k-1; i++) {
        v = node_curr->data;

        if (flag == 0 && pred[v] >= 0 && graph_get_edge_constr(G, pred[v], v) == FORCED) {
          v_min = v;
          cost_min = cost[v];
          node_min = node_curr;
          flag = 1;
        }

        else if (flag == 1 && pred[v] >= 0 && graph_get_edge_constr(G, pred[v], v) == FORCED && cost[v] < cost_min) {
          v_min = v;
          cost_min = cost[v];
          node_min = node_curr;
        }

        node_curr = node_curr->next;
      }

    }

    if (v_min < 0) { // If no free or forced edge exists from the visited region to the not-visited one, then exit the main loop.
      break;
    }


    // ------------------------------------------

    // Remove the selected vertex from the list of not-visited vertices.
    // Insert the edge (pred[v-1], v) in the solution.
    list_remove(&not_visited, node_min);
    tree_insert_edge(T, pred[v_min], v_min);

 
    // ------------------------------------------

    // For each not-visited vertex v, update pred[v] and cost[v].

    if (n-k-2 > 0) { // The list contains one less vertex than before, and it may be empty if we are done.

      node_curr = not_visited.head.next;
      for (i = 0; i < n-k-2; i++) {
        v = node_curr->data;
        new_cost = graph_get_edge_cost(G, v_min, v);
        new_constr = graph_get_edge_constr(G, v_min, v);

        if (new_constr != FORBIDDEN) {

          if (pred[v] < 0) { // Vertex v has no predecessor.

            pred[v] = v_min;
            cost[v] = new_cost;

          }

          else { // Vertex v has already a predecessor pred[v].

            old_cost = graph_get_edge_cost(G, pred[v], v);
            old_constr = graph_get_edge_constr(G, pred[v], v);

            if ( (new_constr == FORCED && old_constr == FREE) ||
                 (new_constr == FREE && old_constr == FREE && new_cost < old_cost) ||
                 (new_constr == FORCED && old_constr == FORCED && new_cost < old_cost) ) {

                      pred[v] = v_min;
                      cost[v] = new_cost;
              
            }
          }
        }

        node_curr = node_curr->next;
      }

    }



  }
}
