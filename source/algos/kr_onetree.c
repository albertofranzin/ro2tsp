#include "../algos/kr_onetree.h"

int kr_onetree(int *mstedges, int num_mstedges, int *oneedges, int num_oneedges, int *weights, set *vertexset, tree* min1t, int *status) {

	*status = 0;
	int i, st;

	/* consider the edges incident to 0 and take
	 * the two less-cost ones among them */
	int e1, e2;
	int min;
	e1 = e2 = -1;

	for (i = 0; i < num_oneedges; i++) {
		if (e1 < 0 || weights[oneedges[i]] < min) {
			e1 	= oneedges[i];
			min = weights[e1];
		}
	}
	for (i = 0; i < num_oneedges; i++) {
		if (oneedges[i] != e1 && (e2 < 0 || weights[oneedges[i]] < min)) {
			e2	= oneedges[i];
			min = weights[e2];
		}
	}

	/* insert the two edges */
	if ((min1t->vrtx_deg[0] == 0 && (e1 < 0 || e2 < 0)) ||
		(min1t->vrtx_deg[0] == 1 &&  e1 < 0          )) {
		*status = 1;
		return 0;
	}

	if (min1t->vrtx_deg[0] == 0) {
		if (e1 >= 0) tree_insert_edge(min1t, e1);
		if (e2 >= 0) tree_insert_edge(min1t, e2);
	}
	else if (min1t->vrtx_deg[0] == 1) {
		if (e1 >= 0) tree_insert_edge(min1t, e1);
	}

	/*
	if (min1t->vrtx_deg[0] != 2) {
		printf("deg = %d\n", min1t->vrtx_deg[0]);
		printf("errore 2\n");
		exit(1);
	}
	*/


	/* compute a tree of minimum cost spanning
	 * over all vertices other than 0
	 */
	kr_mst(mstedges, num_mstedges, vertexset, min1t, &st);

	if (st != 0) {
		*status = 1;
		return 0;
	}

	/*
	if (min1t->edge_num != min1t->vrtx_num) {
		printf("errore status 3\n");
		exit(1);
	}
	*/

	return 0;
}



int kr_onetree2(graph *g, arraylist *edgelist, set *part_vs, tree *part_1t, tree* min1t, int *status) {

	*status 	= 0;
	int n		= g->vrtx_num;
	int i, st;

	/* consider the edges incident to 0 and take
	 * the two less-cost ones among them */
	int e1, e2, my_edge;
	double min;
	e1 = e2 = -1;

	for (i = 1; i < n; i++) {
		my_edge = get_idx(0, i);
		if (arraylist_isin(edgelist, my_edge) && (e1 < 0 || g->edge_cost[my_edge] < min)) {
			e1 	= my_edge;
			min = g->edge_cost[my_edge];
		}
	}
	for (i = 1; i < n; i++) {
		if (i != e1) {
			my_edge = get_idx(0, i);
			if (arraylist_isin(edgelist, my_edge) && (e2 < 0 || g->edge_cost[my_edge] < min)) {
				e2 	= my_edge;
				min = g->edge_cost[my_edge];
			}
		}
	}

	tree_copy(part_1t, min1t);

	/* insert the two edges */
	if ((min1t->vrtx_deg[0] == 0 && (e1 < 0 || e2 < 0)) ||
		(min1t->vrtx_deg[0] == 1 &&  e1 < 0          )) {
		*status = 1;
		return 0;
	}

	if (min1t->vrtx_deg[0] == 0) {
		if (e1 >= 0) tree_insert_edge(min1t, e1);
		if (e2 >= 0) tree_insert_edge(min1t, e2);
	}
	else if (min1t->vrtx_deg[0] == 1) {
		if (e1 >= 0) tree_insert_edge(min1t, e1);
	}

	/*
	if (min1t->vrtx_deg[0] != 2) {
		printf("errore 2\n");
		exit(1);
	}
	*/

	set tmp_part_vs;
	set_init(&tmp_part_vs);
	set_copy(part_vs, &tmp_part_vs);

	/* compute a tree of minimum cost spanning
	 * over all vertices other than 0
	 */
	kr_mst2(edgelist, &tmp_part_vs, min1t, &st);

	if (st != 0) {
		*status = 1;
		set_delete(&tmp_part_vs);
		return 0;
	}

	/*
	if (min1t->edge_num != min1t->vrtx_num) {
		printf("errore status 3\n");
		exit(1);
	}
	*/

	set_delete(&tmp_part_vs);
	return 0;
}
