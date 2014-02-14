#include "../algos/reduce.h"

int reduce(graph *g, tree *min1t, double lb, double ub, int *rmvedges, int *num_rmvedges)  {

	int n	= g->vrtx_num;			/* number of vertices */
	int ne	= (n * (n - 1)) / 2;	/* number of edges */
	int nl	= 0;					/* number of leaves */
	int i, j, k, u, v, p, my_edge, edge_pu, edge_pv, edge_uv, tmp_edge, count, curr, delta_pu;
	double cost_pv;

	int *leaves		= (int*)malloc(n * sizeof(int));
	int *temp		= (int*)malloc(n * sizeof(int));
	int *path		= (int*)malloc(n * sizeof(int));
	int *pred		= (int*)malloc(n * sizeof(int));
	int *flag 		= (int*)malloc(n * sizeof(int));
	double *deltas	= (double*)malloc(ne * sizeof(double));

	/*
	tree min1t;
	tree_init(&min1t);
	tree_setup(&min1t, n);
	prim_1tree(g, &min1t);
	*/

	/*
	int small_edge; // edge incident to 0 of smaller cost
	int big_edge;	// edge incident to 0 with bigger cost
	small_edge	= -1;
	big_edge 	= -1;
	for (i = 0; i < n+1; i++) {
		my_edge = hk_min1t->edges[i];
		if (get_v1(my_edge) == 0 || get_v2(my_edge) == 0) {
			if (small_edge < 0) {
				small_edge	= my_edge;
			}
			else {
				big_edge	= my_edge;
				break;
			}
		}
	}
	if (g->edge_cost[small_edge] > g->edge_cost[big_edge]) {
		tmp_edge	= small_edge;
		small_edge 	= big_edge;
		big_edge 	= tmp_edge;
	}
	*/

	/* NO(find the two vertices adjacent to 0, and choose as v1
	 * the vertex with the smaller index;) v1 was NO(also) chosen by prim_mst
	 * as the root of the mst spanning over 1, 2, ..., n-1
	 * */
	int v1 = -1;
	int v2 = -1;
	int vtmp;
	for (i = 0; i < n+1; i++) {
		my_edge = min1t->edges[i];
		if (get_v1(my_edge) == 0 || get_v2(my_edge) == 0) {
			if (v1 < 0) {
				v1 = (get_v1(my_edge) != 0) ? get_v1(my_edge) : get_v2(my_edge);
			}
			else {
				v2 = (get_v1(my_edge) != 0) ? get_v1(my_edge) : get_v2(my_edge);
				break;
			}
		}
	}

	/*
	if (v1 < v2) {
		vtmp = v1;
		v1 = v2;
		v2 = vtmp;
	}
	*/


	if (min1t->vrtx_pred[v1] != 0) {
		vtmp = v1;
		v1 = v2;
		v2 = vtmp;
	}

	//printf("v1 = %d v2 = %d\n", v1, v2);

	/* search for the leaves of the tree rooted in 0
	 * obtained by removing (0, v2) from min1t
	 * */
	nl = 0;
	for (i = 1; i < n; i++) {
		//if (get_idx(0, i) == big_edge) {
		if (i == v2) {
			if (min1t->vrtx_deg[i] == 2) {
				leaves[nl++] = i;
				//continue;
			}
		}
		else {
			if (min1t->vrtx_deg[i] == 1) {
				leaves[nl++] = i;
			}
		}

	}

	//printf("deg = %d\n", min1t->vrtx_deg[0]);

	//printf("nl = %d\n", nl);

	//printf("leaves = %d\n", leaves[0]);


	/* compute predecessors */
	/* we assume t computed using Prim's algorithm */
	for (i = 0; i < n; i++) {
		pred[i] = min1t->vrtx_pred[i];
	}

	/* compute unique path */
	for (i = 0; i < n; i++) {
		flag[i] = 0;
	}
	k = 0;
	//v = (get_v1(small_edge) != 0) ? get_v1(small_edge) : get_v2(small_edge);
	path[k++]	= 0;
	//path[k++]	= v;
	path[k++]	= v1;
	flag[0]		= 1;
	flag[v1]	= 1;

	for (i = 0; i < nl; i++) {
		count	= 0;
		curr	= leaves[i];

		while (flag[curr] != 1) {
			temp[count++]	= curr;
		    flag[curr]		= 1;
		    curr 			= pred[curr];
		}

		for (j = 0; j < count; j++) { /* Write the vertices in reverese order */
			path[k++] = temp[count-1-j];
		}
	}

	/* compute marginal costs */
	for (i = 0; i < ne; i++) {
		deltas[i] = 0.0;
	}

	for (i = 1; i < n; i++) {
		for (j = i+1; j < n; j++) {
			//printf("i = %d, j = %d\n", i, j);


			u = path[i];
			v = path[j];
			//if (i == 1 && j == 2) printf("v = %d\n", path[j]);

			p = pred[v];

			edge_pu = get_idx(p, u);
			edge_pv = get_idx(p, v);
			edge_uv = get_idx(u, v);

			//delta_pu = (p == u) ? 0.0 : deltas[edge_pu];

			cost_pv  = g->edge_cost[edge_pv];


			/*
			if (p == u) { // the edge belongs to the tree
				if (g->edge_cstr[edge_pv] == FREE) {
					deltas[edge_uv] = cost_pv;
				}
				else {
					deltas[edge_uv] = 0.0;
				}
			}
			*/

			if (p == u) { // the edge belongs to the tree
					deltas[edge_uv] = cost_pv;
			}
			else {

				delta_pu = deltas[edge_pu];
				/*
				if (g->edge_cstr[edge_pv] == FREE) {
					if (delta_pu > cost_pv) {
						deltas[edge_uv] = delta_pu;
					}
					else {
						deltas[edge_uv] = cost_pv;
					}
				}
				else {
					deltas[edge_uv] = delta_pu;
				}
				*/
				if (delta_pu > cost_pv) {
					deltas[edge_uv] = delta_pu;
				}
				else {
					deltas[edge_uv] = cost_pv;
				}


			}
		}
	}

	/*
	double big;
	if (g->edge_cstr[get_idx(0, v1)] == FREE && g->edge_cstr[get_idx(0, v2)] == FREE) {
		big = (g->edge_cost[get_idx(0, v1)] > g->edge_cost[get_idx(0, v2)]) ?
			   g->edge_cost[get_idx(0, v1)] : g->edge_cost[get_idx(0, v2)];
		for (i = 1; i < n; i++) {
			deltas[get_idx(0, i)] = big;
		}
		deltas[get_idx(0, v1)]	= g->edge_cost[get_idx(0, v1)];
		deltas[get_idx(0, v2)]	= g->edge_cost[get_idx(0, v2)];
	}
	if (g->edge_cstr[get_idx(0, v1)] == FREE && g->edge_cstr[get_idx(0, v2)] != FREE) {
		big = g->edge_cost[get_idx(0, v1)];
		for (i = 1; i < n; i++) {
			deltas[get_idx(0, i)] = big;
		}
		deltas[get_idx(0, v1)]	= g->edge_cost[get_idx(0, v1)];
		deltas[get_idx(0, v2)]	= 0.0;
	}
	if (g->edge_cstr[get_idx(0, v1)] != FREE && g->edge_cstr[get_idx(0, v2)] == FREE) {
		big = g->edge_cost[get_idx(0, v2)];
		for (i = 1; i < n; i++) {
			deltas[get_idx(0, i)] = big;
		}
		deltas[get_idx(0, v1)]	= 0.0;
		deltas[get_idx(0, v2)]	= g->edge_cost[get_idx(0, v2)];
	}
	if (g->edge_cstr[get_idx(0, v1)] != FREE && g->edge_cstr[get_idx(0, v2)] != FREE) {
		big = 0.0;
		for (i = 1; i < n; i++) {
			deltas[get_idx(0, i)] = big;
		}
		deltas[get_idx(0, v1)]	= 0.0;
		deltas[get_idx(0, v2)]	= 0.0;
	}
	*/
	double big = (g->edge_cost[get_idx(0, v1)] > g->edge_cost[get_idx(0, v2)]) ?
				  g->edge_cost[get_idx(0, v1)] : g->edge_cost[get_idx(0, v2)];
	for (i = 1; i < n; i++) {
		//deltas[get_idx(0, i)] = g->edge_cost[big_edge];
		deltas[get_idx(0, i)] = big;
	}
	//deltas[small_edge]	= g->edge_cost[small_edge];
	deltas[get_idx(0, v1)]	= g->edge_cost[get_idx(0, v1)];
	//deltas[big_edge] 	= g->edge_cost[big_edge];
	deltas[get_idx(0, v2)]	= g->edge_cost[get_idx(0, v2)];


	*num_rmvedges = 0;
	for (my_edge = 0; my_edge < ne; my_edge++) {
		/*
		if (deltas[my_edge] > 0.0 && g->edge_cstr[my_edge] == FREE) deltas[my_edge] = g->edge_cost[my_edge] - deltas[my_edge];
		*/
		deltas[my_edge] = g->edge_cost[my_edge] - deltas[my_edge];
		if (lb + deltas[my_edge] > ub && g->edge_cstr[my_edge] == FREE) {
			//g->edge_cstr[my_edge] = FORBIDDEN; <- devi usare set_edge_cstr!!!
			rmvedges[*num_rmvedges] = my_edge;
			*num_rmvedges += 1;
		}
	}

	free(leaves);
	free(temp);
	free(path);
	free(pred);
	free(flag);
	free(deltas);
	return 0;
}

