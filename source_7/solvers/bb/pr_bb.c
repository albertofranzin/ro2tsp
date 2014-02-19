#include "../../solvers/bb/pr_bb.h"


int pr_bb(environment *env, statistics *stats) {

	stats->curr_node++;
	if (stats->curr_node % 1 == 0) printf("NODE = %d, LEV = %d\n", stats->curr_node, stats->curr_level);

	int n				= (env->main_graph).vrtx_num;
	double *best_mults	= (double*)malloc(n * sizeof(double));

	int st, v, e1, e2;
	double curr_lb;

	tree curr_1t;
	tree_init(&curr_1t);

	/* compute 1-tree */
	if (stats->curr_node == 1) {
		pr_lagrange_vj(env, INITASCENT, env->global_ub, &curr_1t, best_mults, &curr_lb, &st);
	}
	else {
		pr_lagrange_vj(env, INITASCENT, env->global_ub, &curr_1t, best_mults, &curr_lb, &st);
		//pr_lagrange_vj(env, GENASCENT, env->global_ub, &z, &(env->gen_step), &temp1t, &st);
	}

	/* no spanning tree exists */
	if (st != 0) {
		free(best_mults);
		tree_delete(&curr_1t);
		return 0;
	}

	if (ceil(curr_lb) >= env->global_ub) {
		free(best_mults);
		tree_delete(&curr_1t);
		return 0;
	}

	if (onetree_is_cycle(&curr_1t)) {
		printf("update! ub = %.2f\n", ceil(curr_lb));
		tree_copy(&curr_1t, &(env->global_1t));
		env->global_ub = ceil(curr_lb);
		free(best_mults);
		tree_delete(&curr_1t);
		return 0;
	}

	/* vertex selection*/
	vertexsel(env, &curr_1t, &v);

	/* edge selection */
	edgesel(env, &curr_1t, v, &e1, &e2);

	if (e1 >= 0 && e2 >= 0) { /* the case e2 < 0 can't be applied here:
							   * if e2 < 0 then there are exactly 3
	 	 	 	 	 	 	   * edges incident to v: of them, 2 are
	 	 	 	 	 	 	   * forced, while the other one, e1, is free
	 	 	 	 	 	 	   */

		/* FORCE e1, FORCE e2 */
		graph_set_edge_cstr(&(env->main_graph), e1, FORCED);
		graph_set_edge_cstr(&(env->main_graph), e2, FORCED);

		/* recursive call */
		//printf("Hi, I'm node %d, level %d, and I go down...\n", curr_node, stats->bb_lev_num);
		stats->curr_level++;
		pr_bb(env, stats);
		stats->curr_level--;
		//printf("Hi, I'm node %d, level %d, and I'm back!...\n", curr_node, stats->bb_lev_num);

		/* remove constraints */
		graph_set_edge_cstr(&(env->main_graph), e1, FREE);
		graph_set_edge_cstr(&(env->main_graph), e2, FREE);


		/* FORCE e1, FORBID e2 */
		graph_set_edge_cstr(&(env->main_graph), e1, FORCED);
		graph_set_edge_cstr(&(env->main_graph), e2, FORBIDDEN);

		/* recursive call */
		//printf("Hi, I'm node %d, level %d, and I go down...\n", curr_node, stats->bb_lev_num);
		stats->curr_level++;
		pr_bb(env, stats);
		stats->curr_level--;
		//printf("Hi, I'm node %d, level %d, and I'm back!...\n", curr_node, stats->bb_lev_num);

		/* remove constraints */
		graph_set_edge_cstr(&(env->main_graph), e1, FREE);
		graph_set_edge_cstr(&(env->main_graph), e2, FREE);

	}

	if (e1 >= 0) {

		/* FORBID e1 */
		graph_set_edge_cstr(&(env->main_graph), e1, FORBIDDEN);

		/* recursive call */
		//printf("Hi, I'm node %d, level %d, and I go down...\n", curr_node, stats->bb_lev_num);
		stats->curr_level++;
		pr_bb(env, stats);
		stats->curr_level--;
		//printf("Hi, I'm node %d, level %d, and I'm back!...\n", curr_node, stats->bb_lev_num);

		/* remove constraints */
		graph_set_edge_cstr(&(env->main_graph), e1, FREE);

	}

	free(best_mults);
	tree_delete(&curr_1t);
	return 0;
}
