#include "compute_lagrange.h"

int compute_lagrange(graph* G, onetree* OT, double ub, double* lb) {
  int i, j, v, status, num_of_iterations, time_since_improvement;
  double alpha, square_norm, step_size, z, z_best;


  int n = (*G).n;



  double* multipliers = (double*)calloc(n, sizeof(double));   // multipliers[v] is the lagrangean multiplier associated to the vertex v.



  double* subgradient = (double*)calloc(n, sizeof(double)); // subgradient[i] is the i-th component of the subgradient vector.
                                                            // Remember that we are moving in the space of lagrangean multipliers, which is R^(n), n being the number of vertices of the graph.
 

  
  onetree_delete(OT);
  onetree_init(OT, n);



  graph G_tmp;
  onetree OT_tmp;
  onetree_init(&OT_tmp, n);
  graph_init(&G_tmp, 1);
  graph_copy(G, &G_tmp);



  int flag = 0;


  alpha = ALPHA;
  num_of_iterations = 0;
  time_since_improvement = 0;

  while (num_of_iterations < MAX_NUM_ITERATIONS) {
    num_of_iterations++;


    // Computes the 1-tree associated to the current graph G_tmp.
    // Note that, at each iteration, G_tmp is a copy of the original graph G where the costs of the edges have been modified to take into account of the effect of the lagrangean multipliers.
    // If the compute_ot fails, then also compute_lagrange will fail; in fact, this means that there is no 1-tree satisfying the constraints on the edges;
    // in parctice, this is due to the fact that G has too many forbidden edges.
    status = compute_ot(&G_tmp, &OT_tmp);
    if (status == FAILURE) {
      return FAILURE; // If it doesn't fails at the firt iteration, then compute_ot will never fails because no new constraints are added during compute_lagrange (this will not be true in compute_incremental_lagrange)
    }


    // Computes the cost of the solution.
    z = onetree_get_cost(&OT_tmp);
    for (i = 2; i <= n; i++) {
      z += 2 * multipliers[i-1];
    }

    // If needed, update the cost of the best solution found up to now; save the 1-tree.
    if (z > z_best || flag == 0) {
      flag = 1;
      z_best = z;
      onetree_copy(&OT_tmp, OT); // l'albero OT avrà costi credo diversi da quelli presenti nei corrispondenti lati di G

      time_since_improvement = 0; 
    }
    else {
      time_since_improvement++;
    }



    // If the cost of the solution has not imporved while the last SLACK_TIME iterations, then exit the loop.
    if (time_since_improvement >= MAX_NUM_ITERATIONS_NO_IMPROV)
      break;



    // If the cost of the solution has not imporved while the last ALPHA_HALVING_TIME iterations, then halve the coefficent alpha.
    if (time_since_improvement >= ALPHA_HALVING_TIME)
      alpha /= 2.0;

    
    square_norm = 0.0;
    for (i = 2; i <= n; i++) {

      // Compute the subgradient vector.
      subgradient[i-1] = 2 - onetree_get_node_deg(&OT_tmp, i); // You have to take into account the presence of forced edges and forbidden edges..?! Note however that no forbidden edges are incident to any node of the 1-tree,
                                                             // since compute_ot does not chose any time a forbidden edge as a new edge for the spanning tree (at last, it terminates returning a failure code).
      // Compute the square norm of the subgradient vector.
      square_norm += subgradient[i-1] * subgradient[i-1];
    }
    

    
    // If the subgradient vector is the null vector, then exit the loop.
    // We have reached the maximum of the lagrangean function, so our solution is the optimal one.
    if (square_norm == 0.0) {// Question: may this condition be more subject to rounding errors than checking if all the components of the subgradient vector are equal to zero?

      // A causa di errori di arrotondamento il valore z_best potrebbe essere un po' più grande o un po' più piccolo
      // del valore teorico. Quando si è all'ottimo inoltre, il valore z_best corrisponde anche al valore del costo dell'1-albero
      // con costi originari (quelli di G); se si risolvono istanze ad esempio tsplib con valori dei lati interi, allora possiamo tranquillamente arontondare
      // z_best all'intero più vicino
      if (INTEGER_ROUNDING_OPT == ENABLED) z_best = rint(z_best);

      break;
    }
    


    // Compute the step width.
    step_size = alpha * (ub - z) / square_norm; // Held-Karp.


  
    // Compute the new lagrangean multipliers.
    for (i = 2; i <= n; i++) {
      multipliers[i-1] += step_size * (subgradient[i-1] / sqrt(square_norm));
    }
  

    // Set the costs of the edges to take into account of the lagrangean multipliers.
    for (i = 1; i <= n; i ++) {
      for (j = i+1; j <= n; j++) {
        graph_set_edge_cost(&G_tmp, i, j, graph_get_edge_cost(G, i, j) - multipliers[i-1] - multipliers[j-1]); // Note that when i = 1, multipliers[i] is equal to zero.
      }
    }
 
  } 



  *lb = z_best;

  
  onetree_delete(&OT_tmp);
  graph_delete(&G_tmp);
  free(subgradient);
  free(multipliers);
  
  return SUCCESS;
}
