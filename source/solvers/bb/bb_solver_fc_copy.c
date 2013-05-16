/* Forward checking included */

#include "bb_solver.h"

void bb_solver(bb_env* env, bb_stats* stats) {

  double ub, lb;
  int i, j, status; 
  int n = (*env).G_CURR.n;

  cycle CYCLE;
  cycle_init(&CYCLE, 0);

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

  // Do this if we have just started execution in the root node of the recursione tree.
  if ((*env).curr_call == 1) {

    	// Set G_CURR to be a copy of the input graph.
    	graph_copy(&(*env).G_INPUT, &(*env).G_CURR);

	// Compute a naive upper bound that will be used when our heuristic is not able to find any upper bound and any tour with no forbidden edges.
	// This upper bound will be a valid upper bound to the cost of an optimal tour (with no forbidden edges) for every configuration of constraints
	// on the edges of the graph (in fact, it is the sum of the first n edges of the input graph with highest costs).
       status = compute_upper_bound(G_CURR, NULL, NAIVE, &ub);
	(*env).naive_ub = ub;
	(*stats).naive_ub = ub;
	printf("# node %d : upper bound : naive           = %f\n", (*env).curr_call, (*stats).naive_ub);

	// Compute an upper bound of the cost of the optimal tour. The procedure will always successfully returns, since
	// the graph is complete and no forbidden (nor forced) edges are currently present.
	status = compute_upper_bound(G_CURR, &CYCLE, NEAREST_NEIGHBOUR, &ub);
	(*stats).nn_ub = ub;
	printf("# node %d : upper bound : heur. nn        = %f\n", (*env).curr_call, (*stats).nn_ub);

	// Compute an upper bound of the cost of the optimal tour. The procedure will always successfully returns, since
	// the graph is complete and no forbidden (nor forced) edges are currently present.
	status = compute_upper_bound(G_CURR, &CYCLE, NEAREST_NEIGHBOUR_2_OPT, &ub);
	(*stats).nn2opt_ub = ub;
	printf("# node %d : upper bound : heur. nn + 2opt = %f\n", (*env).curr_call, (*stats).nn2opt_ub);

	// We assume that, in the case in which compute_upper_bound successfully returns, then OT_CURR is a tour and ub is the cost of that tour.
	// We set the initial incumbent equal to the upper bound...
	(*env).incumbent = (*env).z_opt = ub;
	(*stats).init_ub = ub;
	printf("# node %d : initial incumbent             = %f\n", (*env).curr_call, (*stats).init_ub);

	// ... and the optimal solution TOUR_OPT equal to the tour just computed.
	cycle_copy(&CYCLE, &(*env).TOUR_OPT);

	// The procedure will always successfully returns, since the graph is complete and no forbidden 
	// (nor forced) edges are currently present.
	status = compute_lagrange(G_CURR, OT_CURR, ub, &lb);

	// *** Decomment this if-statement only if you know that the value of the optimal solution is an integer *** //
	/*
	if (lb - floor(lb) > SAFE_ROUNDING_THRESHOLD) lb = ceil(lb);
	*/

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









  // -----------------------------------------------------------------------------------------------------------------------------------------

  int w, v, u;

  // Select the vertex where to perform branching.
  status = bb_select_node(env, &w);

  if (status == FAILURE) { // All the edges (of the 1-tree) incident to the nodes with degree greater or equal to 3 are forced or forbidden.

    //printf("# node %d : select_node: FAILURE -> no vertex exists where to perform branching\n", (*env).curr_call);
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

        int k;
        int branch_selection[3];
        double z1, z2, z3;

	bb_bkp bkp1, bkp2, bkp3;
	bb_bkp_init(&bkp1, BASIC); bb_bkp_init(&bkp2, BASIC); bb_bkp_init(&bkp3, BASIC);

	bb_bkp update1, update2, update3;
	bb_bkp_init(&update1, SAVE_OT); bb_bkp_init(&update2, SAVE_OT); bb_bkp_init(&update3, SAVE_OT);




	// Forward checking of branch #1: forbid to use (w, v) in the solution.

	bb_bkp_save(&bkp1, env);
	//status = bb_apply_and_check(w, v, 0, FORBIDDEN, 0, env, &bkp1, &update1, FC_MODE);
	status = bb_propagate_and_check(w, v, 0, FORBIDDEN, 0, env, &bkp1, &update1, FC_MODE);

	if (status == SUCCESS) { 
	  status = bb_compute_current_ot(env, stats);

	  if (status == SUCCESS) {       z1 = (*env).z_curr;       bb_bkp_save(&update1, env);       }
	  else                   {       z1 = (*env).incumbent;    (*stats).num_fails_compute_ot++;  } // This is a little trick. In this way, in the following, the branch will not be entered because the condition z1 < (*env).incumbent will never be verified.

	}
	else {
	  (*stats).num_fails_propagate++;
	  z1 = (*env).incumbent;
	}

	// We want to update the incumbent as soon as possible!
	if (z1 < (*env).incumbent && onetree_is_cycle(OT_CURR)) {

	  (*env).incumbent = (*env).z_opt = z1;
	  onetree_to_cycle(OT_CURR, &(*env).TOUR_OPT);
	  printf("# node %d : updated incumbent = %f : node = %d : level = %d\n", (*env).curr_call, (*env).incumbent, (*env).curr_call, (*env).curr_level);

	}

	// Restore the old configuration, which was active before perform forward checking.
	bb_bkp_restore(&bkp1, env);




 	// Forward checking of branch #2: force to use (w, v) in the solution, while forbid to use (w, u).

	bb_bkp_save(&bkp2, env);
	//status = bb_apply_and_check(w, v, u, FORCED, FORBIDDEN, env, &bkp2, &update2, FC_MODE);
	status = bb_propagate_and_check(w, v, u, FORCED, FORBIDDEN, env, &bkp2, &update2, FC_MODE);

	if (status == SUCCESS) { 
	  status = bb_compute_current_ot(env, stats);

	  if (status == SUCCESS) {       z2 = (*env).z_curr;       bb_bkp_save(&update2, env);       }
	  else                   {       z2 = (*env).incumbent;    (*stats).num_fails_compute_ot++;  } // This is a little trick. In this way, in the following, the branch will not be entered because the condition z1 < (*env).incumbent will never be verified.
      
	}
	else {
	  (*stats).num_fails_propagate++;
	  z2 = (*env).incumbent;
	}

	// We want to update the incumbent as soon as possible!
	if (z2 < (*env).incumbent && onetree_is_cycle(OT_CURR)) {

	  (*env).incumbent = (*env).z_opt = z2;
	  onetree_to_cycle(OT_CURR, &(*env).TOUR_OPT);
	  printf("# node %d : updated incumbent = %f : node = %d : level = %d\n", (*env).curr_call, (*env).incumbent, (*env).curr_call, (*env).curr_level);

	}

	// Restore the old configuration, which was active before perform forward checking.
	bb_bkp_restore(&bkp2, env);






 	// Forward checking of branch #3: force to use both (w, v) and (w, u) in the solution.

	bb_bkp_save(&bkp3, env);
	//status = bb_apply_and_check(w, v, u, FORCED, FORCED, env, &bkp3, &update3, FC_MODE);
	status = bb_propagate_and_check(w, v, u, FORCED, FORCED, env, &bkp3, &update3, FC_MODE);

	if (status == SUCCESS) { 
	  status = bb_compute_current_ot(env, stats);

	  if (status == SUCCESS) {       z3 = (*env).z_curr;       bb_bkp_save(&update3, env);       }
	  else                   {       z3 = (*env).incumbent;    (*stats).num_fails_compute_ot++;  } // This is a little trick. In this way, in the following, the branch will not be entered because the condition z1 < (*env).incumbent will never be verified.
      
	}
	else {
	  (*stats).num_fails_propagate++;
	  z3 = (*env).incumbent;
	}

	// We want to update the incumbent as soon as possible!
	if (z3 < (*env).incumbent && onetree_is_cycle(OT_CURR)) {

	  (*env).incumbent = (*env).z_opt = z3;
	  onetree_to_cycle(OT_CURR, &(*env).TOUR_OPT);
	  printf("# node %d : updated incumbent = %f : node = %d : level = %d\n", (*env).curr_call, (*env).incumbent, (*env).curr_call, (*env).curr_level);

	}

	// Restore the old configuration, which was active before perform forward checking.
	bb_bkp_restore(&bkp3, env);


	if (z1 >= z2 && z2 >= z3)
	  { branch_selection[0] = 1; branch_selection[1] = 2; branch_selection[2] = 3; }
	else if (z1 >= z3 && z3 >= z2)
	  { branch_selection[0] = 1; branch_selection[1] = 3; branch_selection[2] = 2; }
	else if (z2 >= z1 && z1 >= z3)
	  { branch_selection[0] = 2; branch_selection[1] = 1; branch_selection[2] = 3; }
	else if (z2 >= z3 && z3 >= z1)
	  { branch_selection[0] = 2; branch_selection[1] = 3; branch_selection[2] = 1; }
	else if (z3 >= z1 && z1 >= z2)
	  { branch_selection[0] = 3; branch_selection[1] = 1; branch_selection[2] = 2; }
	else
	  { branch_selection[0] = 3; branch_selection[1] = 2; branch_selection[2] = 1; }


	for (k = 0; k < 3; k++) {
	  switch (branch_selection[k]) {

	  case 1:
	    if (z1 < (*env).incumbent) {

	      bb_bkp_restore(&update1, env);
	      bb_solver(env, stats);
	      bb_bkp_restore(&bkp1, env);
	    }
	    bb_bkp_delete(&bkp1);
	    bb_bkp_delete(&update1);
	    break;

	  case 2:
	    if (z2 < (*env).incumbent) {

	      bb_bkp_restore(&update2, env);
	      bb_solver(env, stats);
	      bb_bkp_restore(&bkp2, env);
	    }
	    bb_bkp_delete(&bkp2);
	    bb_bkp_delete(&update2);
	    break;

	  case 3:
	    if (z3 < (*env).incumbent) {

	      bb_bkp_restore(&update3, env);
	      bb_solver(env, stats);
	      bb_bkp_restore(&bkp3, env);
	    }
	    bb_bkp_delete(&bkp3);
	    bb_bkp_delete(&update3);
	    break;

	  default:
	    break;

	  }
	}

  }





  // There exists only one edge of OT_CURR incident to w, which is not forced nor forbidden: we proceed with 2-way branching.
  else if (v > 0) {

        int k;
        int branch_selection[2];
        double z1, z2;

	bb_bkp bkp1, bkp2;
	bb_bkp_init(&bkp1, BASIC); bb_bkp_init(&bkp2, BASIC);

	bb_bkp update1, update2;
	bb_bkp_init(&update1, SAVE_OT); bb_bkp_init(&update2, SAVE_OT);




	// Forward checking of branch #1: forbid to use (w, v) in the solution.

	bb_bkp_save(&bkp1, env);
	//status = bb_apply_and_check(w, v, 0, FORBIDDEN, 0, env, &bkp1, &update1, FC_MODE);
	status = bb_propagate_and_check(w, v, 0, FORBIDDEN, 0, env, &bkp1, &update1, FC_MODE);

	if (status == SUCCESS) { 
	  status = bb_compute_current_ot(env, stats);

	  if (status == SUCCESS) {       z1 = (*env).z_curr;       bb_bkp_save(&update1, env);       }
	  else                   {       z1 = (*env).incumbent;    (*stats).num_fails_compute_ot++;  } // This is a little trick. In this way, in the following, the branch will not be entered because the condition z1 < (*env).incumbent will never be verified.
     
	}
	else {
	  (*stats).num_fails_propagate++;
	  z1 = (*env).incumbent;
	}

	// We want to update the incumbent as soon as possible!
	if (z1 < (*env).incumbent && onetree_is_cycle(OT_CURR)) {

	  (*env).incumbent = (*env).z_opt = z1;
	  onetree_to_cycle(OT_CURR, &(*env).TOUR_OPT);
	  printf("# node %d : updated incumbent = %f : node = %d : level = %d\n", (*env).curr_call, (*env).incumbent, (*env).curr_call, (*env).curr_level);

	}

	// Restore the old configuration, which was active before perform forward checking.
	bb_bkp_restore(&bkp1, env);




 	// Forward checking of branch #2: force to use (w, v) in the solution.

	bb_bkp_save(&bkp2, env);
	//status = bb_apply_and_check(w, v, 0, FORCED, 0, env, &bkp2, &update2, FC_MODE);
	status = bb_propagate_and_check(w, v, 0, FORCED, 0, env, &bkp2, &update2, FC_MODE);

	if (status == SUCCESS) { 
	  status = bb_compute_current_ot(env, stats);

	  if (status == SUCCESS) {       z2 = (*env).z_curr;       bb_bkp_save(&update2, env);       }
	  else                   {       z2 = (*env).incumbent;    (*stats).num_fails_compute_ot++;  } // This is a little trick. In this way, in the following, the branch will not be entered because the condition z1 < (*env).incumbent will never be verified.
      
	}
	else {
	  (*stats).num_fails_propagate++;
	  z2 = (*env).incumbent;
	}

	// We want to update the incumbent as soon as possible!
	if (z2 < (*env).incumbent && onetree_is_cycle(OT_CURR)) {

	  (*env).incumbent = (*env).z_opt = z2;
	  onetree_to_cycle(OT_CURR, &(*env).TOUR_OPT);
	  printf("# node %d : updated incumbent = %f : node = %d : level = %d\n", (*env).curr_call, (*env).incumbent, (*env).curr_call, (*env).curr_level);

	}

	// Restore the old configuration, which was active before perform forward checking.
	bb_bkp_restore(&bkp2, env);






	if (z1 >= z2)
	  { branch_selection[0] = 1; branch_selection[1] = 2; }
	else
	  { branch_selection[0] = 2; branch_selection[1] = 1; }






	for (k = 0; k < 2; k++) {
	  switch (branch_selection[k]) {

	  case 1:
	    if (z1 < (*env).incumbent) {

	      bb_bkp_restore(&update1, env);
	      bb_solver(env, stats);
	      bb_bkp_restore(&bkp1, env);
	    }
	    bb_bkp_delete(&bkp1);
	    bb_bkp_delete(&update1);
	    break;

	  case 2:
	    if (z2 < (*env).incumbent) {

	      bb_bkp_restore(&update2, env);
	      bb_solver(env, stats);
	      bb_bkp_restore(&bkp2, env);
	    }
	    bb_bkp_delete(&bkp2);
	    bb_bkp_delete(&update2);
	    break;

	  default:
	    break;

	  }
	}

  }

  return;
}







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

		/* Deselect this block if you want to use compute_ot when the heuristic fails in computing an upper bound */
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
		
		/* Deselect this block if you want to use compute_lagrange with the naive upper bound when the heuristic fails in computing an upper bound */
		
		// compute_upper_bound does not succeed in finding any tour of not-forbidden edges; if we want to use the subgradient method as we have done so far (it relies
		// on the prior knowledge of some upper bound), then we have to compute an upper bound in some other way. We may sum the costs of all the edges of the graph... this is not a good idea!
		// We can instead compute another naive upper bound by summing the first n edges with highest cost: since the costs are never modified throughout the execution, we can
		// pre-compute this value once for all at the beginning, and save it in a specific variable of the environnement, say it .naive_ub.

	        printf("trying optimizing the lagrangean function using the naive upper bound...\n");
		status = compute_lagrange(G_CURR, OT_CURR, (*env).naive_ub, &lb);

		if (status == SUCCESS) {

			(*env).z_curr = lb;
			
		}
			  
		else {

			// It may be the case that no 1-tree with not-forbidden edges exists; this is due to the presence of too many forbidden edges.
			// Since a tour (and in particular the optimal tour) is a 1-tree, then we can deduce that no tour satisfying the constraints exists:
			// thus we can safely prune the branch.

			//printf("# call %d : return: no spanning tree exists satisfying the constraints\n", (*env).curr_call);

			return FAILURE;

		}

	}


	// *** Decomment this if-statement only if you know that the value of the optimal solution is an integer *** //
  
	// z_curr is in fact a lower bound to the value of the optimal solution. Since we know that the optimal solution has integer value,
	// then we can safely replace z_curr with its ceiling. However, we should also take into account the possibile presence of rounding errors: so we
	// actually do not replace z_curr with its ceiling if the difference z_curr - floor(z_curr) is not above some threshold.
	/*
	if ((*env).z_curr - floor((*env).z_curr) > SAFE_ROUNDING_THRESHOLD) (*env).z_curr = ceil((*env).z_curr);
	else (*stats).num_fails_integer_rounding++;
	*/
       
	return SUCCESS;

}


