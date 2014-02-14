#include "../solvers/pr_bb.h"


int pr_bb(environment *env, statistics *stats) {

	int i, j, st, v, e1, e2, x, y;
	double gen_step, z;

	int n	= (env->main_graph).vrtx_num;

	stats->bb_nod_num++;
	int curr_node = stats->bb_nod_num;

	if (curr_node % 1 == 0) printf("NODE = %d, LEV = %d\n", stats->bb_nod_num, stats->bb_lev_num);

	tree temp1t;
	tree_init(&temp1t);

	/* compute 1-tree */
	if (curr_node == 1) {
		pr_lagrange_vj(env, INITASCENT, 0.0, &z, &gen_step, &temp1t, &st);
		env->gen_step = gen_step;
	}
	else {
		pr_lagrange_vj(env, INITASCENT, 0.0, &z, &gen_step, &temp1t, &st);
		//pr_lagrange_vj(env, GENASCENT, env->global_ub, &z, &(env->gen_step), &temp1t, &st);
	}

	//printf("z = %.2f\n", z);

	/* no spanning tree exists */
	if (st != 0) {
		tree_delete(&temp1t);
		return 0;
	}

	if (ceil(z) >= env->global_ub) {
		tree_delete(&temp1t);
		return 0;
	}

	if (onetree_is_cycle(&temp1t)) {
		tree_copy(&temp1t, &(env->global_1t));
		env->global_ub = ceil(z);
		printf("update! ub = %.2f\n", ceil(z));
		tree_delete(&temp1t);
		return 0;
	}


	/* vertex selection*/
	vertexsel(env, &temp1t, &v);

	/* edge selection */
	edgesel(env, &temp1t, v, &e1, &e2);

	if (e1 >= 0 && e2 >= 0) { /* the case e2 < 0 can't be applied here:
							   * if e2 < 0 then there are exactly 3
	 	 	 	 	 	 	   * edges incident to v: of them, 2 are
	 	 	 	 	 	 	   * forced, while the other one, e1, is free
	 	 	 	 	 	 	   */

		/* force both e1 and e2 */
		graph_set_edge_cstr(&(env->main_graph), e1, FORCED);
		graph_set_edge_cstr(&(env->main_graph), e2, FORCED);

		/* recursive call */
		//printf("Hi, I'm node %d, level %d, and I go down...\n", curr_node, stats->bb_lev_num);
		stats->bb_lev_num++;
		pr_bb(env, stats);
		stats->bb_lev_num--;
		//printf("Hi, I'm node %d, level %d, and I'm back!...\n", curr_node, stats->bb_lev_num);

		/* remove constraint on e1 and e2 */
		graph_set_edge_cstr(&(env->main_graph), e1, FREE);
		graph_set_edge_cstr(&(env->main_graph), e2, FREE);


		/* force e1 and forbid e2 */
		graph_set_edge_cstr(&(env->main_graph), e1, FORCED);
		graph_set_edge_cstr(&(env->main_graph), e2, FORBIDDEN);

		/* recursive call */
		//printf("Hi, I'm node %d, level %d, and I go down...\n", curr_node, stats->bb_lev_num);
		stats->bb_lev_num++;
		pr_bb(env, stats);
		stats->bb_lev_num--;
		//printf("Hi, I'm node %d, level %d, and I'm back!...\n", curr_node, stats->bb_lev_num);

		/* remove constraint on e1 and e2 */
		graph_set_edge_cstr(&(env->main_graph), e1, FREE);
		graph_set_edge_cstr(&(env->main_graph), e2, FREE);

	}

	if (e1 >= 0) {

		/* forbid e1 */
		graph_set_edge_cstr(&(env->main_graph), e1, FORBIDDEN);

		/* recursive call */
		//printf("Hi, I'm node %d, level %d, and I go down...\n", curr_node, stats->bb_lev_num);
		stats->bb_lev_num++;
		pr_bb(env, stats);
		stats->bb_lev_num--;
		//printf("Hi, I'm node %d, level %d, and I'm back!...\n", curr_node, stats->bb_lev_num);

		/* remove constraint on e1 and e2 */
		graph_set_edge_cstr(&(env->main_graph), e1, FREE);

	}

	tree_delete(&temp1t);
	return 0;
}
