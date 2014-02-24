#include "../algos/reduce.h"


int reduce(graph *g, tree *min1t, double *mults, double lb, double ub, int *rmvedges, int *num_rmvedges)  {

	int n	= g->vrtx_num;			/* number of vertices */
	int ne	= (n * (n - 1)) / 2;	/* number of edges */
	int nl	= 0;					/* number of leaves */
	int i, j, k, u, v, p, curr, my_edge, edge_pu, edge_pv, edge_uv, count;

	int *leaves		= (int*)malloc(n * sizeof(int));
	int *temp		= (int*)malloc(n * sizeof(int));
	int *path		= (int*)malloc(n * sizeof(int));
	int *pred		= (int*)malloc(n * sizeof(int));
	int *flag 		= (int*)malloc(n * sizeof(int));
	double *deltas	= (double*)malloc(ne * sizeof(double));

	/* find a value 'min' so that
	 * weight(i, j) - mult(i) - mult(j) > min
	 * for each edge (i, j)
	 */
	double min;
	for (i = 0; i < n; i++) {
		if (i == 0 || mults[i] > min) {
			min = mults[i];
		}
	}
	if (min > 0)	min  = -(min * 3.0);
	else 			min  = 0.0;

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

	if (min1t->vrtx_pred[v1] != 0) {
		vtmp = v1;
		v1 = v2;
		v2 = vtmp;
	}

	/* search for the leaves of the tree rooted in 0
	 * obtained by removing (0, v2) from min1t
	 * */
	nl = 0;
	for (i = 1; i < n; i++) {
		if (i == v2) {
			if (min1t->vrtx_deg[i] == 2) {
				leaves[nl++] = i;
			}
		}
		else {
			if (min1t->vrtx_deg[i] == 1) {
				leaves[nl++] = i;
			}
		}

	}

	/* compute predecessors */
	/* we assume t computed using Prim's algorithm */
	for (i = 1; i < n; i++) {
		pred[i] = min1t->vrtx_pred[i];
	}

	/* compute unique path */
	for (i = 0; i < n; i++) {
		flag[i] = 0;
	}

	k = 0;
	path[k++]	= 0;
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
		for (j = 0; j < count; j++) { /* Write the vertices in reverse order */
			path[k++] = temp[count-1-j];
		}

	}

	for (i = 0; i < ne; i++) {
		deltas[i] = min;
	}

	/* compute marginal costs */
	for (i = 1; i < n; i++) {
		for (j = i+1; j < n; j++) {

			u = path[i];
			v = path[j];
			p = pred[v];

			edge_pu = get_idx(p, u);
			edge_pv = get_idx(p, v);
			edge_uv = get_idx(u, v);

			if (p == u) { /* the edge belongs to the tree */

				if (g->edge_cstr[edge_pv] == FREE) {
					deltas[edge_uv] = g->edge_cost[edge_pv]
					                             - mults[p]
					                             - mults[v];
				}
				else {
					//deltas[edge_uv] = 0.0;
					deltas[edge_uv] = min;
				}

			}
			else {
				if (g->edge_cstr[edge_pv] == FREE) {
					if (deltas[edge_pu] > g->edge_cost[edge_pv]
					                                 - mults[p]
					                                 - mults[v]) {
						deltas[edge_uv] = deltas[edge_pu];
					}
					else {
						deltas[edge_uv] = g->edge_cost[edge_pv]
							                         - mults[p]
							                         - mults[v];
					}
				}
				else {
					deltas[edge_uv] = deltas[edge_pu];
				}
			}

		}
	}

	double cost_0v1 = g->edge_cost[get_idx(0, v1)] - mults[0] - mults[v1];
	double cost_0v2 = g->edge_cost[get_idx(0, v2)] - mults[0] - mults[v2];
	if (g->edge_cstr[get_idx(0, v1)] == FREE &&
		g->edge_cstr[get_idx(0, v2)] == FREE) {

		if (cost_0v1 > cost_0v2) {
			for (i = 1; i < n; i++) {
				deltas[get_idx(0, i)] = cost_0v1;
			}
			deltas[get_idx(0, v2)] = cost_0v2;
		}
		else {
			for (i = 1; i < n; i++) {
				deltas[get_idx(0, i)] = cost_0v2;
			}
			deltas[get_idx(0, v1)] = cost_0v1;
		}
	}
	else if (g->edge_cstr[get_idx(0, v1)] == FREE) {
		for (i = 1; i < n; i++) {
			deltas[get_idx(0, i)] = cost_0v1;
		}
		//deltas[get_idx(0, v2)]	= cost_0v2;
		deltas[get_idx(0, v2)]	= min;
	}
	else if (g->edge_cstr[get_idx(0, v2)] == FREE) {
		for (i = 1; i < n; i++) {
			deltas[get_idx(0, i)] = cost_0v2;
		}
		//deltas[get_idx(0, v1)]	= cost_0v1;
		deltas[get_idx(0, v1)]	= min;
	}
	else {
		for (i = 1; i < n; i++) {
			//deltas[get_idx(0, i)] = 0.0;
			deltas[get_idx(0, i)] = min;
		}
	}

	*num_rmvedges = 0;
	for (my_edge = 0; my_edge < ne; my_edge++) {

		//if (deltas[my_edge] != 0.0) {
		if (deltas[my_edge] > min) {
			g->edge_delta[my_edge] = g->edge_cost[my_edge]
			                      - mults[get_v1(my_edge)]
			                      - mults[get_v2(my_edge)]
			                             - deltas[my_edge];
		}
		else {
			g->edge_delta[my_edge] = 0.0;
		}

		if (lb + g->edge_delta[my_edge] > ub &&
		   g->edge_cstr[my_edge] == FREE) {
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

