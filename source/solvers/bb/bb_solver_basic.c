/* Plain version */

#include "bb_solver.h"

void bb_solver(bb_env* env, bb_stats* stats) {

  double ub, lb;
  int i, j, status; 
  int n = (*env).G_CURR.n;

  // To easily access some frequently used pointer...
  graph* G_CURR = &(*env).G_CURR;
  onetree* OT_CURR = &(*env).OT_CURR;








  // -----------------------------------------------------------------------------------------------------------------------------------------

  // Update some variables...

  (*stats).number_of_calls++;

  (*env).curr_call = (*stats).number_of_calls;

  (*env).curr_level++;

  (*stats).number_of_levels = ((*env).curr_level > (*stats).number_of_levels) ? (*env).curr_level : (*stats).number_of_levels;

  printf("node = %d\n", (*env).curr_call);






  // -----------------------------------------------------------------------------------------------------------------------------------------

  // Do this if we have just started execution in the root node of the recursione tree...
  if ((*env).curr_call == 1) {

    	// Set G_CURR to be a copy of the input graph.
    	graph_copy(&(*env).G_INPUT, &(*env).G_CURR);

	// Compute a naive upper bound that will be used when our heuristic is not able to find any upper bound and any tour with no forbidden edges.
	// This upper bound will be a valid upper bound to the cost of an optimal tour (with no forbidden edges) for every configuration of constraints
	// on the edges of the graph.
	status = compute_upper_bound(&(*env).G_INPUT, NULL, NAIVE, &(*env).naive_ub);

	// Compute an upper bound of the cost of the optimal tour. The procedure will always successfully returns, since
	// the graph is complete and no forbidden (nor forced) edges are currently present.
	status = compute_upper_bound(G_CURR, OT_CURR, NEAREST_NEIGHBOUR, &ub);

	// We assume that, in the case in which compute_upper_bound successfully returns, then OT_CURR is a tour and ub is the cost of that tour.
	// We set the initial incumbent equal to the upper bound...
	(*env).incumbent = (*env).z_opt = ub;

	// ... and the optimal solution G_OPT equal to the tour just computed.
	onetree_to_graph(OT_CURR, &(*env).TOUR_OPT);

	// The procedure will always successfully returns, since the graph is complete and no forbidden 
	// (nor forced) edges are currently present.
	status = compute_lagrange(G_CURR, OT_CURR, ub, &lb);

	(*env).z_curr = lb;

  }

  // ...otherwise we have just started execution in some other deeper node, so do this:
  else {

	// Compute an upper bound of the cost of the optimal tour in G_CURR. Note that, the optimal tour of the current graph has not necessarily 
	// to be equal (nor it has to have the same cost) to the optimal tour of the original input graph; in case, this is due to the presence 
	// of some constraints (of the type forced/forbidden) on the edges.
	status = compute_upper_bound(G_CURR, OT_CURR, NEAREST_NEIGHBOUR, &ub);


	if (status == SUCCESS) {

		// Since compute_upper_bound returned successfully, then some tour containing only not-forbidden edges exists (it has been discovered!);
		// thus, compute_lagrange will successfully compute the 1-tree at its first iteration, and it surely will return with success.
		status = compute_lagrange(G_CURR, OT_CURR, ub, &lb);

		(*env).z_curr = lb;

	}
    

	else {

	  /*
		// compute_upper_bound does not succeed in finding any tour of not-forbidden edges; since we do not know any upper bound 
		// to the cost of the optimal tour (well, we can find an upper bound in some other way, e.g. summing the costs of all the edges.. 
		// however this is not a good upper bound!) then we can't use the subgradient method as we have done so far (it relies on the prior knowledge of some upper bound).
		// Thus, we decide to compute the 1-tree using the simple procedure compute_ot, 
		// which is based on a simple application of Prim's algorithm, without looking for some better lower-bounding 1-tree. 
	        
		status = compute_ot(G_CURR, OT_CURR);

		if (status == SUCCESS) {

			(*env).z_curr = onetree_get_cost(OT_CURR);

		}
	  */
		
		// compute_upper_bound does not succeed in finding any tour of not-forbidden edges; if we want to use the subgradient method as we have done so far (it relies
		// on the prior knowledge of some upper bound), then we have to compute an upper bound in some other way. We may sum the costs of all the edges of the graph... this is not a good idea!
		// We can instead compute another naive upper bound by summing the first n edges with highest cost: since the costs are never modified throughout the execution, we can
		// pre-compute this value once for all at the beginning, and save it in a specific variable of the environnement, say it .naive_ub. 

		status = compute_lagrange(G_CURR, OT_CURR, (*env).naive_ub, &lb);

		if (status == SUCCESS) {

			(*env).z_curr = lb;

		}
		
	 
		else {

			// It may be the case that no 1-tree with not-forbidden edges exists; this is due to the presence of too many forbidden edges.
			// Since a tour (and in particular the optimal tour) is a 1-tree, then we can deduce that no tour satisfying the constraints exists:
			// thus we can safely prune the branch.

			return;

		}

	}

  }








  // -----------------------------------------------------------------------------------------------------------------------------------------
  
  // *** Decomment this block only if you know that the value of the optimal solution is an integer *** //
  /*
  // z_curr is in fact a lower bound to the value of the optimal solution. Since we know that the optimal solution has integer value,
  // then we can safely replace z_curr with its ceiling. However, we should also take into account the possibile presence of rounding errors: so we
  // actually do not replace z_curr with its ceiling if the difference z_curr - floor(z_curr) is not above some threshold.
  if ((*env).z_curr - floor((*env).z_curr) > SAFE_ROUNDING_THRESHOLD) {

    (*env).z_curr = ceil((*env).z_curr);

  }
  */
  

  // If the cost of the current solution (which can be feasible or not) is greater or equal to the cost of the current best feasible solution,
  // then we cut the branch. In fact, we know that some new constraint (or maybe none) will be added in each node of the subtree rooted at the current node 
  // of the recusion tree, so for each node of the subtree the cost of the solution will always be greater or equal to the cost of the current solution.
  if ((*env).z_curr >= (*env).incumbent) {

    return;

  }








  // -----------------------------------------------------------------------------------------------------------------------------------------

  if (onetree_is_cycle(OT_CURR)) {

	// The 1-tree is in fact a tour. Moreover, it has been computed using either the subgradient method or Prim's algorithm, so its cost is guardanteed
   	// to be less or equal to the cost of an optimal tour of G_CURR: thus, it is an optimal tour of G_CURR.

	// Note that, in general, the cost of the optimal tour of G_CURR may be greater or equal to the cost of the optimal tour
	// of the original graph G_INPUT. Thus, we cannot say if the tour just found is the optimal one for the original graph G_INPUT.
	// However, in the previous if-condition we checked, in fact, that the cost of the tour just found is less than the cost of the best tour
	// found up to know, so we save it as a temporary optimal solution (and we return: we will not get any better solution in the subtree rooted at
	// the current node of the recursion tree).

	(*env).incumbent = (*env).z_opt = (*env).z_curr;

	onetree_to_graph(OT_CURR, &(*env).TOUR_OPT);

	printf("# updated incumbent = %f : node = %d : level = %d\n", (*env).incumbent, (*env).curr_call, (*env).curr_level);

	return;

  }








  // -----------------------------------------------------------------------------------------------------------------------------------------

  int w, v, u;

  // Select the vertex where to perform branching.
  status = bb_select_node(env, &w);

  if (status == FAILURE) { // All the edges (of the 1-tree) incident to the nodes with degree greater or equal to 3 are forced or forbidden.

    return;

  }

  // Select two edges which are incident to the vertex just found, and are not forbidden nor forced:
  // branching will be done with respect to these two edges.
  // If there not exist such two edges, then look if there is at least one edge incident to the vertex which is not forbidden nor forced:
  // branching will be done with respect to this edge.

  status = bb_select_edges(env, &w, &v, &u); // The procedure will always successfully return, since at this point we are guaranteed 
                                             // that vertex w has at least one edge incident to it which is not forced nor forbidden.


  // -----------------------------------------------------------------------------------------------------------------------------------------

  // (w, v) and (w, u) are two edges of OT_CURR incident to w, which are not forced nor forbidden: we can proceed with 3-way branching.
  if (v > 0 && u > 0) {

	bb_bkp bkp1, bkp2, bkp3;
	bb_bkp_init(&bkp1, DEFAULT_MODE); bb_bkp_init(&bkp2, DEFAULT_MODE); bb_bkp_init(&bkp3, DEFAULT_MODE);


	// Branch #1: forbid to use (w, v) in the solution.

	bb_bkp_save(&bkp1, env);
	bb_bkp_save_constraint(&bkp1, w, v, graph_get_edge_constr(G_CURR, w, v));
	graph_set_edge_constr(G_CURR, w, v, FORBIDDEN);
	bb_solver(env, stats);
	bb_bkp_restore(&bkp1, env);
	bb_bkp_delete(&bkp1);


	// Branch #2: force to use (w, v) in the solution, while forbid to use (w, u).
 
	bb_bkp_save(&bkp2, env);
	bb_bkp_save_constraint(&bkp2, w, v, graph_get_edge_constr(G_CURR, w, v));
	bb_bkp_save_constraint(&bkp2, w, u, graph_get_edge_constr(G_CURR, w, u));
	graph_set_edge_constr(G_CURR, w, v, FORCED);
	graph_set_edge_constr(G_CURR, w, u, FORBIDDEN);
	bb_solver(env, stats);
	bb_bkp_restore(&bkp2, env);
	bb_bkp_delete(&bkp2);


	// Branch #3: force to use both (w, v) and (w, u) in the solution.	

	bb_bkp_save(&bkp3, env);
	bb_bkp_save_constraint(&bkp3, w, v, graph_get_edge_constr(G_CURR, w, v));
	bb_bkp_save_constraint(&bkp3, w, u, graph_get_edge_constr(G_CURR, w, u));
	graph_set_edge_constr(G_CURR, w, v, FORCED);
	graph_set_edge_constr(G_CURR, w, u, FORCED);
	bb_solver(env, stats);
	bb_bkp_restore(&bkp3, env);
	bb_bkp_delete(&bkp3);
  

  }



  // There exists only one edge of OT_CURR incident to w, which is not forced nor forbidden: we proceed with 2-way branching.
  else if (v > 0) {

	bb_bkp bkp1, bkp2;
	bb_bkp_init(&bkp1, DEFAULT_MODE); bb_bkp_init(&bkp2, DEFAULT_MODE);

    
	// Branch #1: forbid to use (w, v) in the solution.
 
	bb_bkp_save(&bkp1, env);
	bb_bkp_save_constraint(&bkp1, w, v, graph_get_edge_constr(G_CURR, w, v));
	graph_set_edge_constr(G_CURR, w, v, FORBIDDEN);
	bb_solver(env, stats);
	bb_bkp_restore(&bkp1, env);
	bb_bkp_delete(&bkp1);


	// Branch #1: force to use (w, v) in the solution.
	 
	bb_bkp_save(&bkp2, env);
	bb_bkp_save_constraint(&bkp2, w, v, graph_get_edge_constr(G_CURR, w, v));
	graph_set_edge_constr(G_CURR, w, v, FORCED);
	bb_solver(env, stats);
	bb_bkp_restore(&bkp2, env);
	bb_bkp_delete(&bkp2);
	
   
  }

  return;
}

