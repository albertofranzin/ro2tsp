#include "../../solvers/bb/kr_bb.h"


int kr_bb(environment *env, statistics *stats, arraylist *edgelist, tree *part_1t, set *part_vs, double *gap) {

	stats->curr_node++;

	int n				= (env->main_graph).vrtx_num;
	double *curr_mults 	= (double*)malloc(n * sizeof(double));

	/*if (stats->curr_node % 100 == 0) printf("NODE = %d, LEV = %d\n",
			stats->curr_node, stats->curr_level);*/
	//printf("active edges = %.2f\n", (double)(edgelist->size) / ((n * (n - 1)) / 2) * 100.0);


	int i, k, st;
	int v;
	int e1, e2;
	double curr_lb, curr_gap;
	double delta = 0.0;
	int reduction;

	tree curr_1t;
	tree_init(&curr_1t);
	tree_setup(&curr_1t, n);

	tree subproblem_part_1t;
	tree_init(&subproblem_part_1t);

	set subproblem_part_vs;
	set_init(&subproblem_part_vs);

	/* compute 1-tree */
	if (stats->curr_node == 1) {
		/*
		kr_onetree2(&(env->main_graph), edgelist, part_vs, part_1t, &curr_1t, &st);
		if (st != 0) {
			curr_lb = 0.0;
				for (i = 0; i < n; i++) {
					curr_lb += env->main_graph.edge_cost[curr_1t.edges[i]];
				}
		}
		for (i = 0; i < n; i++) {
			curr_mults[i] = 0.0;
		}
		*/
		kr_lagrange_vj(env, INITASCENT, env->global_ub, edgelist, part_1t, part_vs, &curr_1t, curr_mults, &curr_lb, &st);
		//kr_lagrange_vj(env, INITASCENT, env->global_ub, edgelist, part_1t, part_vs, &curr_1t, curr_mults, &curr_lb, &st);
		/*
		for (i = 0; i < n; i++) {
			env->genascent_mults[i] = curr_mults[i];
		}
		*/
	}
	else {
		kr_lagrange_vj(env, INITASCENT, env->global_ub, edgelist, part_1t, part_vs, &curr_1t, curr_mults, &curr_lb, &st);
		/*
		kr_onetree2(&(env->main_graph), edgelist, part_vs, part_1t, &curr_1t, &st);
		if (st != 0) {
			curr_lb = 0.0;
				for (i = 0; i < n; i++) {
					curr_lb += env->main_graph.edge_cost[curr_1t.edges[i]];
				}
		}
		for (i = 0; i < n; i++) {
			curr_mults[i] = 0.0;
		}
		*/
		//printf("general lagrange\n");
		/*
		if (stats->curr_level < 5) {
			tree_delete(&curr_1t);
			tree_setup(&curr_1t, n);
			pr_lagrange_hk(env, env->global_ub, 20000, 20000, 100, &curr_1t, curr_mults, &curr_lb, &st);
		}
		else {
			kr_lagrange_vj(env, INITASCENT, env->global_ub, edgelist, part_1t, part_vs, &curr_1t, curr_mults,  &curr_lb, &st);
		}
		*/
		//printf("end general lagrange\n");
		/*
		for (i = 0; i < n; i++) {
			curr_mults[i] = env->genascent_mults[i];
		}
		kr_lagrange_vj(env, GENASCENT, env->global_ub, edgelist, part_1t, part_vs, &curr_1t, curr_mults, &curr_lb, &st);
		*/
	}

	/* no spanning tree exists */
	if (st != 0) {
		free(curr_mults);
		tree_delete(&curr_1t);
		tree_delete(&subproblem_part_1t);
		set_delete(&subproblem_part_vs);
		return 0;
	}

	if (curr_lb - floor(curr_lb) > EPSILON) {
		curr_lb = ceil(curr_lb);
	}

	//if (ceil(curr_lb) >= env->global_ub) {
	if (curr_lb >= env->global_ub) {
		free(curr_mults);
		tree_delete(&curr_1t);
		tree_delete(&subproblem_part_1t);
		set_delete(&subproblem_part_vs);
		return 0;
	}

	if (onetree_is_cycle(&curr_1t)) {
		//printf("update! ub = %.2f\n", ceil(curr_lb));

		tree_copy(&curr_1t, &(env->global_1t));
		if (ceil(curr_lb) - curr_lb > EPSILON) {
			env->global_ub = floor(curr_lb);
		}
		else {
			env->global_ub = curr_lb;
		}
		free(curr_mults);
		tree_delete(&curr_1t);
		tree_delete(&subproblem_part_1t);
		set_delete(&subproblem_part_vs);
		return 0;
	}

	int *rmvedges = (int*)malloc((n * (n - 1)) / 2 * sizeof(int));
	int *rmvpreds = (int*)malloc((n * (n - 1)) / 2 * sizeof(int));
	int *rmvcstrs = (int*)malloc((n * (n - 1)) / 2 * sizeof(int));
	int  num_rmvedges;

	int *modedges	= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));
	int *modcstrs	= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));
	int *tmppreds	= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));
	int nm;

	curr_gap = env->global_ub - curr_lb;

	/* reduce */
	//printf("reduce\n");
	reduction = FALSE;
	if (curr_gap < *gap - delta) {
		reduction = TRUE;

		tree_rooting(&curr_1t, 0);
		reduce(&(env->main_graph), &curr_1t, curr_mults, curr_lb, env->global_ub, rmvedges, &num_rmvedges);
		//printf("num_rmvedges = %d\n", num_rmvedges);

		for (i = 0; i < num_rmvedges; i++) {
			rmvcstrs[i] = env->main_graph.edge_cstr[rmvedges[i]];
			graph_set_edge_cstr(&(env->main_graph), rmvedges[i], FORBIDDEN);
			if (rmvcstrs[i] == FREE) {
				rmvpreds[i] = edgelist->pred[rmvedges[i]];
				arraylist_remove(edgelist, rmvedges[i]);
			}
		}
	}

	/* vertex selection*/
	//printf("vertex selection\n");
	vertexsel(env, &curr_1t, &v);

	/* edge selection */
	//printf("edge selection\n");
	edgesel(env, &curr_1t, v, &e1, &e2);


	clock_t curr_time = clock();

	if ((double)(curr_time - env->start_time) / CLOCKS_PER_SEC > 2000) {
		printf("time limit reached, stopping. CURRENT BEST SOLUTION : %f ; GAP : %f%% ; NODES : %d\n",
				env->global_ub, 100.0* (env->global_ub - env->global_lb)/env->global_lb, stats->curr_node);
		exit(1);
	}

	if (e1 >= 0 && e2 >= 0) { /* the case e2 < 0 can't be applied here:
							   * if e2 < 0 then there are exactly 3
	 	 	 	 	 	 	   * edges incident to v: of them, 2 are
	 	 	 	 	 	 	   * forced, while the other one, e1, is free
	 	 	 	 	 	 	   */

		/* check if feasible */
		//printf("first branch\n");
		tree_copy(part_1t, &subproblem_part_1t);
		set_copy(part_vs, &subproblem_part_vs);

		modedges[0] = e1;
		modedges[1] = e2;
		modcstrs[0] = FORCED;
		modcstrs[1] = FORCED;
		graph_set_edge_cstr(&(env->main_graph), e1, FORCED);
		graph_set_edge_cstr(&(env->main_graph), e2, FORCED);
		nm = 2;
		propagate(env, v, modedges, modcstrs, &nm);
		for (k = 0; k < nm; k++) {
			graph_set_edge_cstr(&(env->main_graph), modedges[k], FREE);
		}
		setncheck(env, edgelist, &subproblem_part_1t, &subproblem_part_vs, modedges, modcstrs, tmppreds, &nm, &st);

		/* recursive call */
		if (st == 0) {
			//printf("Hi, I'm node %d, level %d, and I go down...\n", stats->curr_node, stats->curr_level);
			stats->curr_level++;
			kr_bb(env, stats, edgelist, &subproblem_part_1t, &subproblem_part_vs, &curr_gap);
			stats->curr_level--;
			//printf("Hi, I'm node %d, level %d, and I'm back!...\n", stats->curr_node, stats->curr_level);
		}

		/* rollback propagate */
		//printf("rollback propagate\n");
		for (i = nm - 1; i >= 0; i--) {
			arraylist_insert_next(edgelist, tmppreds[i], modedges[i]);
			graph_set_edge_cstr(&(env->main_graph), modedges[i], FREE);
		}

		/* FORCE e1, FORBID e2 */

		/* check if feasible */
		//printf("second branch\n");
		tree_copy(part_1t, &subproblem_part_1t);
		set_copy(part_vs, &subproblem_part_vs);

		modedges[0] = e1;
		modedges[1] = e2;
		modcstrs[0] = FORCED;
		modcstrs[1] = FORBIDDEN;
		graph_set_edge_cstr(&(env->main_graph), e1, FORCED);
		graph_set_edge_cstr(&(env->main_graph), e2, FORBIDDEN);
		nm = 2;
		propagate(env, v, modedges, modcstrs, &nm);
		for (k = 0; k < nm; k++) {
			graph_set_edge_cstr(&(env->main_graph), modedges[k], FREE);
		}
		setncheck(env, edgelist, &subproblem_part_1t, &subproblem_part_vs, modedges, modcstrs, tmppreds, &nm, &st);

		/* recursive call */
		if (st == 0) {
			//printf("Hi, I'm node %d, level %d, and I go down...\n", stats->curr_node, stats->curr_level);
			stats->curr_level++;
			kr_bb(env, stats, edgelist, &subproblem_part_1t, &subproblem_part_vs, &curr_gap);
			stats->curr_level--;
			//printf("Hi, I'm node %d, level %d, and I'm back!...\n", stats->curr_node, stats->curr_level);
		}

		/* rollback propagate */
		//printf("rollback propagate\n");
		for (i = nm - 1; i >= 0; i--) {
			arraylist_insert_next(edgelist, tmppreds[i], modedges[i]);
			graph_set_edge_cstr(&(env->main_graph), modedges[i], FREE);
		}

	}

	if (e1 >= 0) {

		/* FORBID e1 */

		/* check if feasible */
		tree_copy(part_1t, &subproblem_part_1t);
		set_copy(part_vs, &subproblem_part_vs);

		modedges[0] = e1;
		modcstrs[0] = FORBIDDEN;
		graph_set_edge_cstr(&(env->main_graph), e1, FORBIDDEN);
		nm = 1;
		propagate(env, v, modedges, modcstrs, &nm);
		for (k = 0; k < nm; k++) {
			graph_set_edge_cstr(&(env->main_graph), modedges[k], FREE);
		}
		setncheck(env, edgelist, &subproblem_part_1t, &subproblem_part_vs, modedges, modcstrs, tmppreds, &nm, &st);

		if (st == 0) {
			//printf("Hi, I'm node %d, level %d, and I go down...\n", stats->curr_node, stats->curr_level);
			stats->curr_level++;
			kr_bb(env, stats, edgelist, &subproblem_part_1t, &subproblem_part_vs, &curr_gap);
			stats->curr_level--;
			//printf("Hi, I'm node %d, level %d, and I'm back!...\n", stats->curr_node, stats->curr_level);
		}

		/* rollback propagate */
		//printf("rollback propagate\n");
		for (i = nm - 1; i >= 0; i--) {
			arraylist_insert_next(edgelist, tmppreds[i], modedges[i]);
			graph_set_edge_cstr(&(env->main_graph), modedges[i], FREE);
		}

	}

	/* rollback reduce */
	//printf("rollback reduce\n");
	if (reduction == TRUE) {
		for (i = num_rmvedges - 1; i >= 0; i--) {
			graph_set_edge_cstr(&(env->main_graph), rmvedges[i], rmvcstrs[i]);
			if (rmvcstrs[i] == FREE) {
				arraylist_insert_next(edgelist, rmvpreds[i], rmvedges[i]);
			}
		}
	}

	free(curr_mults);
	free(rmvedges);
	free(rmvcstrs);
	free(rmvpreds);
	free(modedges);
	free(modcstrs);
	free(tmppreds);
	tree_delete(&curr_1t);
	tree_delete(&subproblem_part_1t);
	set_delete(&subproblem_part_vs);

	return 0;
}
