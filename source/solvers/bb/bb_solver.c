/* Forward checking included */

#include "bb_solver.h"

void bb_solver(bb_env* env, bb_stats* stats) {



  double ub, lb;
  int i, j, status; 
  int n = (*env).G_INPUT.n;

  cycle CYCLE;
  cycle_init(&CYCLE, 0);

  /*
  graph G_FAT;
  */

  // To easily access some frequently used pointer...
  graph* G_CURR = &(*env).G_CURR;
  onetree* OT_CURR = &(*env).OT_CURR;







  // -----------------------------------------------------------------------------------------------------------------------------------------
  // Update some variables...

  (*stats).num_of_calls++;

  (*env).curr_call = (*stats).num_of_calls;

  (*env).curr_level++;

  (*stats).num_of_levels = ((*env).curr_level > (*stats).num_of_levels) ? (*env).curr_level : (*stats).num_of_levels;








  // -----------------------------------------------------------------------------------------------------------------------------------------
  // Things to do at the root node of the recursione tree.

  if ((*env).curr_call == 1) {

  	// Set G_CURR to be a copy of the input graph.
	graph_copy(&(*env).G_INPUT, &(*env).G_CURR);

	// Compute a naive upper bound that will be used when our heuristic is not able to find any upper bound 
	// and any tour with no forbidden edges. This upper bound will be a valid upper bound to the cost 
	// of an optimal tour (with no forbidden edges) for every configuration of constraints on the edges 
	// of the graph (in fact, it is the sum of the first n edges of the input graph with highest costs).

	// Naive.
	status = compute_upper_bound(G_CURR, NULL, DUMB, &ub);

	(*stats).dumb_ub = ub;
	printf("# node %d : upper bound : dumb           = %f\n", (*env).curr_call, (*stats).dumb_ub);

	// Compute an upper bound of the cost of the optimal tour. The procedure will always successfully returns, since
	// the graph is complete and no forbidden (nor forced) edges are currently present.
	
	// Random Cycles.
	status = compute_upper_bound(G_CURR, &CYCLE, RANDOM_CYCLES, &ub);

	(*stats).rc_ub = ub; 
	printf("# node %d : upper bound : heur. rc        = %f\n", (*env).curr_call, (*stats).rc_ub);

	// Random Cycles + 2OPT.
	status = compute_upper_bound(G_CURR, &CYCLE, RANDOM_CYCLES_2OPT, &ub);

	(*stats).rc2opt_ub = ub;
	printf("# node %d : upper bound : heur. rc + 2opt = %f\n", (*env).curr_call, (*stats).rc2opt_ub);

	// Nearest Neighbour.
	status = compute_upper_bound(G_CURR, &CYCLE, NEAREST_NEIGHBOUR, &ub);

	(*stats).nn_ub = ub;
	printf("# node %d : upper bound : heur. nn        = %f\n", (*env).curr_call, (*stats).nn_ub);

	// Nearest Neighbour + 2OPT.
	status = compute_upper_bound(G_CURR, &CYCLE, NEAREST_NEIGHBOUR_2_OPT, &ub);

	(*stats).nn2opt_ub = ub;
	printf("# node %d : upper bound : heur. nn + 2opt = %f\n", (*env).curr_call, (*stats).nn2opt_ub);

	// Set the upper-bound(s)
	(*env).dumb_ub = (*stats).dumb_ub;
	ub = (*stats).nn2opt_ub < (*stats).rc2opt_ub ? (*stats).nn2opt_ub : (*stats).rc2opt_ub;

	// We assume that, in the case in which compute_upper_bound successfully returns, then OT_CURR is a tour and ub is the cost of that tour.
	// We set the initial incumbent equal to the upper bound...
	(*env).incumbent = (*env).z_opt = ub;

	(*stats).init_ub = ub;
	printf("# node %d : initial upper bound           = %f\n", (*env).curr_call, (*stats).init_ub);

	//printf("# node %d : initial incumbent             = %f\n", (*env).curr_call, (*stats).init_ub);

	// ... and the optimal solution TOUR_OPT equal to the tour just computed.
	cycle_copy(&CYCLE, &(*env).TOUR_OPT);

	// The procedure will always successfully returns, since the graph is complete and no forbidden 
	// (nor forced) edges are currently present.
	status = compute_lagrange(G_CURR, OT_CURR, ub, &lb);

	//onetree_plot(OT_CURR, &(*env).EG_INPUT, "Lagr. 1-tree");


	// Discard fat edges.
	if (FAT_EDGES_OPT == ENABLED) {
	  bb_remove_fat_edges(env, stats, ub);
	}

	if (INTEGER_ROUNDING_OPT == ENABLED) {

		if (lb - floor(lb) > SAFE_ROUNDING_THRESHOLD) lb = ceil(lb);
		else (*stats).num_fails_integer_rounding++;

	}

	(*env).z_curr = lb;

	(*stats).init_lb = lb;
	printf("# node %d : initial lower bound           = %f\n", (*env).curr_call, (*stats).init_lb);


	// It may be the case that the solution computed by compute_lagrange is in fact the optimal solution...
	if (onetree_is_cycle(OT_CURR)) {

		(*env).incumbent = (*env).z_opt = (*env).z_curr;
		onetree_to_cycle(OT_CURR, &(*env).TOUR_OPT);

		printf("# node %d : updated incumbent = %f : node = %d : level = %d\n", (*env).curr_call, (*env).incumbent, (*env).curr_call, (*env).curr_level);

		return;

	}
  }


  else if (FORWARD_CHECKING_OPT == DISABLED) {

	// Things at all the other nodes. In case forward checking is enabled, then the computation of 
	// the current 1-tree will be performed in the forward-cheking part.

	status = bb_compute_current_ot(env, stats);

	if (status == FAILURE) { (*stats).num_fails_compute_ot++; return; }
   
  }



  // =========================================================================================================================================
  // START NO-FC SECTION

  if (FORWARD_CHECKING_OPT == DISABLED) {

	// -----------------------------------------------------------------------------------------------------------------------------------------
	// Check if we can prune the current branch.

	if ((*env).z_curr >= (*env).incumbent) return;

	// -----------------------------------------------------------------------------------------------------------------------------------------
	// Check if the current solution is the optimal one for the current subtree.

	if (onetree_is_cycle(OT_CURR)) {

		// The 1-tree is in fact a tour. Moreover, it has been computed using either the subgradient method or Prim's algorithm, so its cost is guardanteed
		// to be less or equal to the cost of an optimal tour of G_CURR: thus, it is an optimal tour of G_CURR.
		// Note that, in general, the cost of the optimal tour of G_CURR may be greater or equal to the cost of the optimal tour
		// of the original graph G_INPUT. Thus, we cannot say if the tour just found is the optimal one for the original graph G_INPUT.
		// However, in the previous if-condition we checked, in fact, that the cost of the tour just found is less than the cost of the best tour
		// found up to know, so we save it as a temporary optimal solution (and we return: we will not get any better solution in the subtree rooted at
		// the current node of the recursion tree).

		(*env).incumbent = (*env).z_opt = (*env).z_curr;

		onetree_to_cycle(OT_CURR, &(*env).TOUR_OPT);

		printf("# node %d : updated incumbent = %f : node = %d : level = %d\n", (*env).curr_call, (*env).incumbent, (*env).curr_call, (*env).curr_level);

		return;

	}
 
	// -----------------------------------------------------------------------------------------------------------------------------------------
	// Select vertex and edges for branching.

	int w, v, u;

	// Select the vertex where to perform branching.
	status = bb_select_node(env, &w, SELECT_NODE_OPT);

	if (status == FAILURE) { // All the edges (of the 1-tree) incident to the nodes with degree greater or equal to 3 are forced or forbidden.

		(*stats).num_fails_select_node++; return;

	}

	// Select two edges which are incident to the vertex just found, and are not forbidden nor forced:
	// branching will be done with respect to these two edges.
	// If there not exist such two edges, then look if there is at least one edge incident to the vertex which is not forbidden nor forced:
	// branching will be done with respect to this edge.

	status = bb_select_edges(env, &w, &v, &u, SELECT_EDGES_OPT); // The procedure will always successfully return, since at this point we are guaranteed 
	                                           // that vertex w has at least one edge incident to it which is not forced nor forbidden.

	// -----------------------------------------------------------------------------------------------------------------------------------------
	// Branching.

	// (w, v) and (w, u) are two edges of OT_CURR incident to w, which are not forced nor forbidden: we can proceed with 3-way branching.
	if (v > 0 && u > 0) {

		(*stats).num_3_ways_branching++;

		bb_bkp bkp[3];
		for (i = 0; i < 3; i++) { bb_bkp_init(&bkp[i], BASIC); }

		for (i = 0; i < 3; i++) {
			
			bb_bkp_save(&bkp[i], env);
	
			switch (i) {
			case 0: status = bb_propagate_and_check(w, v, 0, FORBIDDEN, 0, env, &bkp[i], NULL, NOFC_MODE);      break;  // Branch #1
			case 1: status = bb_propagate_and_check(w, v, u, FORCED, FORBIDDEN, env, &bkp[i], NULL, NOFC_MODE); break; // Branch #2
			case 2: status = bb_propagate_and_check(w, v, u, FORCED, FORCED, env, &bkp[i], NULL, NOFC_MODE);    break; // Branch #3
			default: break;
			}

			if (status == SUCCESS) bb_solver(env, stats);
			else (*stats).num_fails_propagate++;

			bb_bkp_restore(&bkp[i], env);
			bb_bkp_delete(&bkp[i]);
	
		}
	}

	// There exists only one edge of OT_CURR incident to w, which is not forced nor forbidden: we proceed with 2-way branching.
	else if (v > 0) {

		(*stats).num_2_ways_branching++;

		bb_bkp bkp[2];
		for (i = 0; i < 2; i++) { bb_bkp_init(&bkp[i], BASIC); }

		for (i = 0; i < 2; i++) {

			bb_bkp_save(&bkp[i], env);
		
		switch (i) {
		case 0: status = bb_propagate_and_check(w, v, 0, FORBIDDEN, 0, env, &bkp[i], NULL, NOFC_MODE);  break;  // Branch #1
		case 1: status = bb_propagate_and_check(w, v, u, FORCED, 0, env, &bkp[i], NULL, NOFC_MODE);     break;  // Branch #2
		default: break;
		}

		if (status == SUCCESS) bb_solver(env, stats);
		else (*stats).num_fails_propagate++;

		bb_bkp_restore(&bkp[i], env);
		bb_bkp_delete(&bkp[i]);
	
		}
	}
    
  }
  // END NO-FC SECTION
  // =========================================================================================================================================





  // =========================================================================================================================================
  // START FC SECTION
  if (FORWARD_CHECKING_OPT == ENABLED) {

	// -----------------------------------------------------------------------------------------------------------------------------------------
	// Select vertex and edges for branching.

	int w, v, u;

	// Select the vertex where to perform branching.
	status = bb_select_node(env, &w, SELECT_NODE_OPT);

	// All the edges (of the 1-tree) incident to the nodes with degree greater or equal to 3 are forced or forbidden.
	if (status == FAILURE) { (*stats).num_fails_select_node++; return; }


	// Select two edges which are incident to the vertex just found, and are not forbidden nor forced:
	// branching will be done with respect to these two edges.
	// If there not exist such two edges, then look if there is at least one edge incident to the vertex which is not forbidden nor forced:
	// branching will be done with respect to this edge.

	status = bb_select_edges(env, &w, &v, &u, SELECT_EDGES_OPT); // The procedure will always successfully return, since at this point we are guaranteed 
		                                   // that vertex w has at least one edge incident to it which is not forced nor forbidden.

	// -----------------------------------------------------------------------------------------------------------------------------------------
	// Forward checking and branching.

	// (w, v) and (w, u) are two edges of OT_CURR incident to w, which are not forced nor forbidden: we can proceed with 3-way branching.
	if (v > 0 && u > 0) {

		(*stats).num_3_ways_branching++;

		int k;
		int branch_selection[3];
		double z[3];
		bb_bkp bkp[3];
		bb_bkp update[3];
		for (i = 0; i < 3; i++) { bb_bkp_init(&bkp[i], BASIC); bb_bkp_init(&update[i], SAVE_OT); }

		for (i = 0; i < 3; i++) {

			bb_bkp_save(&bkp[i], env);
	
			switch (i) {
			case 0: status = bb_propagate_and_check(w, v, 0, FORBIDDEN, 0, env, &bkp[i], &update[i], FC_MODE);      break; // Branch #1
			case 1: status = bb_propagate_and_check(w, v, u, FORCED, FORBIDDEN, env, &bkp[i], &update[i], FC_MODE); break; // Branch #2
			case 2: status = bb_propagate_and_check(w, v, u, FORCED, FORCED, env, &bkp[i], &update[i], FC_MODE);    break; // Branch #3
			default: break;
			}

			if (status == SUCCESS) {
				status = bb_compute_current_ot(env, stats);
				if (status == SUCCESS) { z[i] = (*env).z_curr;       bb_bkp_save(&update[i], env);    }
				else                   { z[i] = (*env).incumbent;    (*stats).num_fails_compute_ot++; } // This is a little trick. In this way, in the following, the branch will not be entered because the condition z1 < (*env).incumbent will never be verified.
			}
			else {
				(*stats).num_fails_propagate++;
				z[i] = (*env).incumbent;
			}

			// We want to update the incumbent as soon as possible!
			if (z[i] < (*env).incumbent && onetree_is_cycle(OT_CURR)) {
				(*env).incumbent = (*env).z_opt = z[i];
				onetree_to_cycle(OT_CURR, &(*env).TOUR_OPT);
				printf("# node %d : updated incumbent = %f : node = %d : level = %d\n", (*env).curr_call, (*env).incumbent, (*env).curr_call, (*env).curr_level);
			}

			// Restore the old configuration.
			bb_bkp_restore(&bkp[i], env);

		}


		if (z[0] >= z[1] && z[1] >= z[2]) 	{ branch_selection[0] = 0; branch_selection[1] = 1; branch_selection[2] = 2; }
		else if (z[0] >= z[2] && z[2] >= z[1]) 	{ branch_selection[0] = 0; branch_selection[1] = 2; branch_selection[2] = 1; }
		else if (z[1] >= z[0] && z[0] >= z[2]) 	{ branch_selection[0] = 1; branch_selection[1] = 0; branch_selection[2] = 2; }
		else if (z[1] >= z[2] && z[2] >= z[0]) 	{ branch_selection[0] = 1; branch_selection[1] = 2; branch_selection[2] = 0; }
		else if (z[2] >= z[0] && z[0] >= z[1])	{ branch_selection[0] = 2; branch_selection[1] = 0; branch_selection[2] = 1; }
		else					{ branch_selection[0] = 2; branch_selection[1] = 1; branch_selection[2] = 0; }


		for (k = 0; k < 3; k++) {

			switch (branch_selection[k]) {

			case 0:
			if (z[0] < (*env).incumbent) {
				bb_bkp_restore(&update[0], env);
				bb_solver(env, stats);
				bb_bkp_restore(&bkp[0], env);
			}
			bb_bkp_delete(&bkp[0]);
			bb_bkp_delete(&update[0]);
			break;

			case 1:
			if (z[1] < (*env).incumbent) {
				bb_bkp_restore(&update[1], env);
				bb_solver(env, stats);
				bb_bkp_restore(&bkp[1], env);
			}
			bb_bkp_delete(&bkp[1]);
			bb_bkp_delete(&update[1]);
			break;

			case 2:
			if (z[2] < (*env).incumbent) {
				bb_bkp_restore(&update[2], env);
				bb_solver(env, stats);
				bb_bkp_restore(&bkp[2], env);
			}
			bb_bkp_delete(&bkp[2]);
			bb_bkp_delete(&update[2]);
			break;

			default:
			break;

			}
		}
  	}

 	// There exists only one edge of OT_CURR incident to w, which is not forced nor forbidden: we proceed with 2-way branching.
 	 else if (v > 0) {

		(*stats).num_2_ways_branching++;

		int k;
		int branch_selection[2];
		double z[2];
		bb_bkp bkp[2];
		bb_bkp update[2];
		for (i = 0; i < 2; i++) { bb_bkp_init(&bkp[i], BASIC); bb_bkp_init(&update[i], SAVE_OT); }

		for (i = 0; i < 2; i++) {

			bb_bkp_save(&bkp[i], env);
	
			switch (i) {
			case 0: status = bb_propagate_and_check(w, v, 0, FORBIDDEN, 0, env, &bkp[i], &update[i], FC_MODE);    break; // Branch #1
			case 1: status = bb_propagate_and_check(w, v, 0, FORCED, 0, env, &bkp[i], &update[i], FC_MODE); break; // Branch #2
			default: break;
			}

			if (status == SUCCESS) {
				status = bb_compute_current_ot(env, stats);
				if (status == SUCCESS) {       z[i] = (*env).z_curr;       bb_bkp_save(&update[i], env);       }
				else                   {       z[i] = (*env).incumbent;    (*stats).num_fails_compute_ot++;  } // This is a little trick. In this way, in the following, the branch will not be entered because the condition z1 < (*env).incumbent will never be verified.
			}
			else {
				(*stats).num_fails_propagate++;
				z[i] = (*env).incumbent;
			}

			// We want to update the incumbent as soon as possible!
			if (z[i] < (*env).incumbent && onetree_is_cycle(OT_CURR)) {
				(*env).incumbent = (*env).z_opt = z[i];
				onetree_to_cycle(OT_CURR, &(*env).TOUR_OPT);
				printf("# node %d : updated incumbent = %f : node = %d : level = %d\n", (*env).curr_call, (*env).incumbent, (*env).curr_call, (*env).curr_level);
			}

			// Restore the old configuration.
			bb_bkp_restore(&bkp[i], env);
		}


		if (z[0] >= z[1])	{ branch_selection[0] = 1; branch_selection[1] = 2; }
		else 			{ branch_selection[0] = 2; branch_selection[1] = 1; }


		for (k = 0; k < 2; k++) {

			switch (branch_selection[k]) {

			case 1:
			if (z[0] < (*env).incumbent) {
				bb_bkp_restore(&update[0], env);
				bb_solver(env, stats);
				bb_bkp_restore(&bkp[0], env);
			}
			bb_bkp_delete(&bkp[0]);
			bb_bkp_delete(&update[0]);
			break;

			case 2:
			if (z[1] < (*env).incumbent) {
				bb_bkp_restore(&update[1], env);
				bb_solver(env, stats);
				bb_bkp_restore(&bkp[1], env);
			}
			bb_bkp_delete(&bkp[1]);
			bb_bkp_delete(&update[1]);
			break;

			default:
			break;

			}
		}
 	 }


  }
// END FC SECTION
// =========================================================================================================================================

return;
} // end bb_solver







// Compute a forbidden-edges-free 1-tree for the problem graph with the current configuration of edge-constraints. Save the 1-tree in the specific environnement variable env.OT_CURR,
// while save in env.z_curr the correspondig lower bound of the cost of the optimal tour. The 1-tree will be computed using the subgradient method (see function
// compute_lagrange) or using some simple implementation based on Prim's algorithm for finding minimum spanning trees (see function compute_ot).
// The procedure fails in computing a 1-tree if and only if no 1-tree without forbidden edges exists for the current graph. In this case the function returns FAILURE.
int bb_compute_current_ot(bb_env* env, bb_stats* stats) {

	double ub, lb;
	int status; 

	graph* G_CURR = &(*env).G_CURR;
	onetree* OT_CURR = &(*env).OT_CURR;

	int n = (*G_CURR).n;
	cycle CYCLE;
	cycle_init(&CYCLE, 0);

	// Compute an upper bound of the cost of the optimal tour in G_CURR. Note that, the optimal tour of the current graph has not necessarily 
	// to be equal (nor it has to have the same cost) to the optimal tour of the original input graph; if so, this is due to the presence 
	// of some constraints (of the type forced/forbidden) on the edges.

	status = compute_upper_bound(G_CURR, &CYCLE, NEAREST_NEIGHBOUR, &ub);


	if (status == SUCCESS) {

		// Since compute_upper_bound returned successfully, then some tour containing only not-forbidden edges exists (it has been discovered!);
		// thus, compute_lagrange will successfully compute the 1-tree at its first iteration, and it surely will return with success.

		status = compute_lagrange(G_CURR, OT_CURR, ub, &lb);

		(*env).z_curr = lb;

	}
    

	else {

	        (*stats).num_fails_compute_ub++;

		/* Decomment this block if you want to use compute_ot when the heuristic fails in computing an upper bound. Otherwise comment it. */
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
		
		/* Deselect this block if you want to use compute_lagrange with the dumb upper bound when the heuristic fails in computing an upper bound. Otherwise comment it. */
		
		// compute_upper_bound does not succeed in finding any tour of not-forbidden edges; if we want to use the subgradient method as we have done so far (it relies
		// on the prior knowledge of some upper bound), then we have to compute an upper bound in some other way. We may sum the costs of all the edges of the graph... this is not a good idea!
		// We can instead compute another better (well, still dumb...) upper bound by summing the first n edges with highest cost: since the costs are never modified throughout the execution, we can
		// pre-compute this value once for all at the beginning, and save it in a specific variable of the environnement, say it .dumb_ub.
		
	        printf("trying optimizing the lagrangean function using the dumb upper bound...\n");
		status = compute_lagrange(G_CURR, OT_CURR, (*env).dumb_ub, &lb);

		if (status == SUCCESS) {

			(*env).z_curr = lb;
			
		}
			  
		else {

			// It may be the case that no 1-tree with not-forbidden edges exists; this is due to the presence of too many forbidden edges.
			// Since a tour (and in particular the optimal tour) is a 1-tree, then we can deduce that no tour satisfying the constraints exists:
			// thus we can safely prune the branch.

			printf("# call %d : return: no spanning tree exists satisfying the constraints\n", (*env).curr_call);

			return FAILURE;

		}

	}
  
	// z_curr is in fact a lower bound to the value of the optimal solution. Since we know that the optimal solution has integer value,
	// then we can safely replace z_curr with its ceiling. However, we should also take into account the possibile presence of rounding errors: so we
	// actually do not replace z_curr with its ceiling if the difference z_curr - floor(z_curr) is not above some threshold.
	if (INTEGER_ROUNDING_OPT == ENABLED) {
	  if (lb - floor(lb) > SAFE_ROUNDING_THRESHOLD) lb = ceil(lb);
	  else (*stats).num_fails_integer_rounding++;
	}
       
	return SUCCESS;

}


