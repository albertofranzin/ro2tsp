#include "../solvers/kr_bb.h"


int kr_bb(environment *env, statistics *stats, arraylist *edgelist, tree *part1t, set *partvs, double *new_ub) {


	*new_ub = 0.0;

	int i, j, k, st, e1, e2, v, x, y, sv, sx, sy, pred1, pred2, condition1, condition2, condition3, my_edge;
	double gen_step, z;
	//double temp_ub;

	int n = (env->main_graph).vrtx_num;

	stats->bb_nod_num++;
	int curr_node = stats->bb_nod_num;



	int *temp = (int*)malloc( (n * (n - 1)) / 2 * sizeof(int));
	my_edge = edgelist->next[edgelist->capacity];
	k = 0;
	for (i = 0; i < edgelist->size; i++) {
		temp[k++] = my_edge;
		my_edge = edgelist->next[my_edge];
	}

	for (i = 1; i < k; i++) {
		if (env->main_graph.edge_cost[temp[i]] < env->main_graph.edge_cost[temp[i-1]]) {
			printf("errore!\n");
			printf("node = %d\n", curr_node);
			exit(1);
		}
	}
	free(temp);

	if (curr_node % 1 == 0) printf("NODE = %d, LEV = %d\n", stats->bb_nod_num, stats->bb_lev_num);

	tree temp1t;
	tree_init(&temp1t);
	tree_setup(&temp1t, n);

	tree newpart1t;
	tree_init(&newpart1t);

	set newpartvs;
	set_init(&newpartvs);


	/* compute 1-tree */
	if (curr_node == 1) {
		kr_lagrange_vj(env, edgelist, part1t, partvs, INITASCENT, 0.0, &temp1t, &z, &gen_step, &st);
		env->gen_step = gen_step;
	}
	else {
		kr_lagrange_vj(env, edgelist, part1t, partvs, INITASCENT, 0.0, &temp1t, &z, &gen_step, &st);
		//kr_lagrange_vj(env, edgelist, part1t, partvs, GENASCENT, env->global_ub, &temp1t, &z, &(env->gen_step), &st);
	}

	/* no spanning tree exists */
	if (st != 0) {
		tree_delete(&temp1t);
		tree_delete(&newpart1t);
		set_delete(&newpartvs);
		return 0;
	}

    if (ceil(z) >= env->global_ub) {
		tree_delete(&temp1t);
		tree_delete(&newpart1t);
		set_delete(&newpartvs);
		return 0;
	}

	if (onetree_is_cycle(&temp1t)) {
		tree_copy(&temp1t, &(env->global_1t));
		env->global_ub	= ceil(z);
		//*new_ub			= ceil(z);
		printf("update! ub = %.2f\n", ceil(z));
		tree_delete(&temp1t);
		tree_delete(&newpart1t);
		set_delete(&newpartvs);
		return 0;
	}


	/* vertex selection*/
	vertexsel(env, &temp1t, &v);

	/* edge selection */
	edgesel(env, &temp1t, v, &e1, &e2);

	//temp_ub = env->global_ub;
	int *rmvedges = (int*)malloc((n * (n - 1)) / 2 * sizeof(int));
	int *rmvpreds = (int*)malloc((n * (n - 1)) / 2 * sizeof(int));
	int *rmvcstrs = (int*)malloc((n * (n - 1)) / 2 * sizeof(int));
	int  num_rmvedges;
	int reduce1 = FALSE;
	int reduce2 = FALSE;
	double ub1, ub2, ub3;
	ub1 = ub2 = ub3 = 0.0;


	x = y = -1;
	if (e1 >= 0) x = (get_v1(e1) != v) ? get_v1(e1) : get_v2(e1);
	if (e2 >= 0) y = (get_v1(e2) != v) ? get_v1(e2) : get_v2(e2);

	if (e1 >= 0 && e2 >= 0) { /* the case e2 < 0 can't be applied here:
							   * if e2 < 0 then there are exactly 3
	 	 	 	 	 	 	   * edges incident to v: of them, 2 are
	 	 	 	 	 	 	   * forced, while the other one, e1, is free
	 	 	 	 	 	 	   */

		/* force both e1 and e2 */
		tree_copy(part1t, &newpart1t);
		set_copy(partvs, &newpartvs);

		condition3 = TRUE;
		if (env->main_graph.edge_cstr[e1] != FREE || env->main_graph.edge_cstr[e2] != FREE) condition3 = FALSE;

		condition2 = TRUE;
		if (v != 0 && x != 0 && condition3 == TRUE) {
			set_find(&newpartvs, v, &sv);
			set_find(&newpartvs, x, &sx);
			if (sv != sx) set_union(&newpartvs, sv, sx);
			else          condition2 = FALSE;
		}
		if (v != 0 && y != 0 && condition2 == TRUE && condition3 == TRUE) {
			set_find(&newpartvs, v, &sv);
			set_find(&newpartvs, y, &sy);
			if (sv != sy) set_union(&newpartvs, sv, sy);
			else          condition2 = FALSE;
		}

		condition1 = TRUE;    k = 0;
		if (v == 0 || x == 0) k++;
		if (v == 0 || y == 0) k++;
		if (k + newpart1t.vrtx_deg[0] > 2) condition1 = FALSE;

		if (condition1 == FALSE || condition2 == FALSE || condition3 == FALSE) {
			printf("errore\n");
			exit(1);
		}

		if (condition1 == TRUE && condition2 == TRUE && condition3 == TRUE) {
			pred1 = edgelist->pred[e1];
			arraylist_remove(edgelist, e1);
			pred2 = edgelist->pred[e2];
			arraylist_remove(edgelist, e2);
			tree_insert_edge(&newpart1t, e1);
			tree_insert_edge(&newpart1t, e2);
			graph_set_edge_cstr(&(env->main_graph), e1, FORCED);
			graph_set_edge_cstr(&(env->main_graph), e2, FORCED);

			/* recursive call */
			//printf("Hi, I'm node %d, level %d, and I go down...\n", curr_node, stats->bb_lev_num);
			stats->bb_lev_num++;
			kr_bb(env, stats, edgelist, &newpart1t, &newpartvs, &ub1);
			stats->bb_lev_num--;
			//printf("Hi, I'm node %d, level %d, and I'm back!...\n", curr_node, stats->bb_lev_num);

			/* remove constraint on e1 and e2 (in reverse order)*/
			arraylist_insert_next(edgelist, pred2, e2);
			graph_set_edge_cstr(&(env->main_graph), e2, FREE);
			arraylist_insert_next(edgelist, pred1, e1);
			graph_set_edge_cstr(&(env->main_graph), e1, FREE);

		}


		if (ub1 > 0.0) {

			*new_ub = ub1;

			reduce1 = TRUE;
			tree_rooting(&temp1t, 0);
			reduce(&(env->main_graph), &temp1t, z, ub1, rmvedges, &num_rmvedges);

			printf("num_rmvedges = %d\n", num_rmvedges);


			for (i = 0; i < num_rmvedges; i++) {
				rmvcstrs[i] = env->main_graph.edge_cstr[rmvedges[i]];
				graph_set_edge_cstr(&(env->main_graph), rmvedges[i], FORBIDDEN);
				if (rmvcstrs[i] == FREE) {
					rmvpreds[i] = edgelist->pred[rmvedges[i]];
					arraylist_remove(edgelist, rmvedges[i]);
				}
			}
			//temp_ub = env->global_ub;
		}

		/* force e1 and forbid e2 */
		tree_copy(part1t, &newpart1t);
		set_copy(partvs, &newpartvs);

		condition3 = TRUE;
		if (env->main_graph.edge_cstr[e1] != FREE || env->main_graph.edge_cstr[e2] != FREE) condition3 = FALSE;

		condition2 = TRUE;
		if (v != 0 && x != 0 && condition3 == TRUE) {
			set_find(&newpartvs, v, &sv);
			set_find(&newpartvs, x, &sx);
			if (sv != sx) set_union(&newpartvs, sv, sx);
			else          condition2 = FALSE;
		}

		condition1 = TRUE; k = 0;
		if (v == 0 || x == 0) k++;
		if (k + newpart1t.vrtx_deg[0] > 2) condition1 = FALSE;

		if (condition1 == FALSE || condition2 == FALSE || condition3 == FALSE) {
			printf("errore\n");
			exit(1);
		}

		if (condition1 == TRUE && condition2 == TRUE && condition3 == TRUE) {
			pred1 = edgelist->pred[e1];
			arraylist_remove(edgelist, e1);
			pred2 = edgelist->pred[e2];
			arraylist_remove(edgelist, e2);
			tree_insert_edge(&newpart1t, e1);
			graph_set_edge_cstr(&(env->main_graph), e1, FORCED);
			graph_set_edge_cstr(&(env->main_graph), e2, FORBIDDEN);

			/* recursive call */
			//printf("Hi, I'm node %d, level %d, and I go down...\n", curr_node, stats->bb_lev_num);
			stats->bb_lev_num++;
			kr_bb(env, stats, edgelist, &newpart1t, &newpartvs, &ub2);
			stats->bb_lev_num--;
			//printf("Hi, I'm node %d, level %d, and I'm back!...\n", curr_node, stats->bb_lev_num);

			/* remove constraint on e1 and e2 (reverse order)*/
			arraylist_insert_next(edgelist, pred2, e2);
			graph_set_edge_cstr(&(env->main_graph), e2, FREE);
			arraylist_insert_next(edgelist, pred1, e1);
			graph_set_edge_cstr(&(env->main_graph), e1, FREE);

		}

		if (ub2 > 0.0) {
		//if (temp_ub > env->global_ub) {
			*new_ub = ub2;


			//if (reduce1 == TRUE) { /* rollback */
			if (ub1 > 0.0) { /* rollback */


				for (i = num_rmvedges - 1; i >= 0; i--) {

					graph_set_edge_cstr(&(env->main_graph), rmvedges[i], rmvcstrs[i]);
					if (rmvcstrs[i] == FREE) {
						arraylist_insert_next(edgelist, rmvpreds[i], rmvedges[i]);
					}
				}
			}

			reduce2 = TRUE;
			tree_rooting(&temp1t, 0);
			reduce(&(env->main_graph), &temp1t, z, ub2, rmvedges, &num_rmvedges);

			printf("num_rmvedges = %d\n", num_rmvedges);

			for (i = 0; i < num_rmvedges; i++) {
				rmvcstrs[i] = env->main_graph.edge_cstr[rmvedges[i]];
				graph_set_edge_cstr(&(env->main_graph), rmvedges[i], FORBIDDEN);
				if (rmvcstrs[i] == FREE) {
					rmvpreds[i] = edgelist->pred[rmvedges[i]];
					arraylist_remove(edgelist, rmvedges[i]);
				}
			}
			//temp_ub = env->global_ub;
		}


	}

	if (e1 >= 0) {

		/* forbid e1 */
		tree_copy(part1t, &newpart1t);
		set_copy(partvs, &newpartvs);

		condition3 = TRUE;
		if (env->main_graph.edge_cstr[e1] != FREE) condition3 = FALSE;

		condition2 = TRUE;
		condition1 = TRUE;

		if (condition1 == FALSE || condition2 == FALSE || condition3 == FALSE) {
			printf("errore\n");
			exit(1);
		}

		if (condition1 == TRUE && condition2 == TRUE && condition3 == TRUE) {
			pred1 = edgelist->pred[e1];
			arraylist_remove(edgelist, e1);
			graph_set_edge_cstr(&(env->main_graph), e1, FORBIDDEN);

			/* recursive call */
			//printf("Hi, I'm node %d, level %d, and I go down...\n", curr_node, stats->bb_lev_num);
			stats->bb_lev_num++;
			kr_bb(env, stats, edgelist, &newpart1t, &newpartvs, &ub3);
			stats->bb_lev_num--;
			//printf("Hi, I'm node %d, level %d, and I'm back!...\n", curr_node, stats->bb_lev_num);

			/* remove constraint on e1 */
			arraylist_insert_next(edgelist, pred1, e1);
			graph_set_edge_cstr(&(env->main_graph), e1, FREE);

		}

		if (ub3 > 0.0) {
		//if (temp_ub > env->global_ub) {
			*new_ub = ub3;
		}


	}

	//if (reduce1 == TRUE || reduce2 == TRUE) { /* rollback */
	if (ub1 > 0.0 || ub2 > 0.0) { /* rollback */


		for (i = num_rmvedges - 1; i >= 0; i--) {
			graph_set_edge_cstr(&(env->main_graph), rmvedges[i], rmvcstrs[i]);
			if (rmvcstrs[i] == FREE) {
				arraylist_insert_next(edgelist, rmvpreds[i], rmvedges[i]);
			}
		}
	}


	free(rmvedges);
	free(rmvcstrs);
	free(rmvpreds);

	tree_delete(&temp1t);
	tree_delete(&newpart1t);
	set_delete(&newpartvs);

	return 0;
}
