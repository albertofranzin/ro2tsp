#include "../data/tree.h"


int tree_init(tree *t) {

    t->vrtx_num		= -1;
    t->edge_num		= -1;
    t->vrtx_deg		= NULL;
    t->vrtx_pred	= NULL;
    t->edges		= NULL;
    return 0;

}


int tree_delete(tree *t) {

    t->vrtx_num		= -1;
    t->edge_num		= -1;
    if (t->vrtx_deg != NULL) {
        free(t->vrtx_deg);
        t->vrtx_deg = NULL;
    }
    if (t->vrtx_pred != NULL) {
        free(t->vrtx_pred);
        t->vrtx_pred = NULL;
    }
    if (t->edges != NULL) {
        free(t->edges);
        t->edges = NULL;
    }
    return 0;

}

int tree_empty(tree *t) {

    int i;
    int n 			= t->vrtx_num;
    t->edge_num		= 0;

    for (i = 0; i < n; i++ ) {
    	t->vrtx_deg[i]		= 0;
    	t->vrtx_pred[i] 	= -1;
        t->edges[i]			= -1;
    }
    t->edges[n]	= -1;

    return 0;

}



int tree_setup(tree *t, int n) {

#ifdef DEBUG
    if (n < 1) {
        fprintf(stderr, "Error in /data/tree.c:\n"
                        "function: tree_setup:\n"
                        "tree_setup : errcode %d\n", 1);
        return -1;
    }
#endif

    if (t->vrtx_deg != NULL) {
        free(t->vrtx_deg);
    }
    if (t->vrtx_pred != NULL) {
        free(t->vrtx_pred);
    }
    if (t->edges != NULL) {
        free(t->edges);
    }

    int i;
    t->vrtx_num		= n;
    t->edge_num		= 0;
    t->vrtx_deg		= (int*)malloc(n * sizeof(int));
    t->vrtx_pred	= (int*)malloc(n * sizeof(int));
    t->edges		= (int*)malloc((n+1) * sizeof(int));

    for (i = 0; i < n; i++ ) {
    	t->vrtx_deg[i]		= 0;
    	t->vrtx_pred[i] 	= -1;
        t->edges[i]			= -1;
    }
    t->edges[n]	= -1;
    return 0;

}


int tree_copy(tree *from, tree *to) {

    if (to->vrtx_deg != NULL) {
        free(to->vrtx_deg);
    }
    if (to->vrtx_pred != NULL) {
        free(to->vrtx_pred);
    }
    if (to->edges != NULL) {
        free(to->edges);
    }


    int i;
    int n = from->vrtx_num;
    to->vrtx_num		= n;
    to->edge_num		= from->edge_num;
    to->vrtx_deg 		= (int*)malloc(n * sizeof(int));
    to->vrtx_pred 		= (int*)malloc(n * sizeof(int));
    to->edges			= (int*)malloc((n + 1) * sizeof(int));
    for (i = 0; i < n; i++) {
        to->vrtx_deg[i]		= from->vrtx_deg[i];
        to->vrtx_pred[i]	= from->vrtx_pred[i];
        to->edges[i]		= from->edges[i];
    }
    to->edges[n]	= from->edges[n];
    return 0;

}


int tree_insert_edge(tree *t, int ie) {

	int n = t->vrtx_num;

#ifdef DEBUG
    if (ie < 0 || ie >= (n * (n - 1)) / 2) {
        fprintf(stderr, "Error in /data/tree.c:\n"
                        "function: tree_insert_edge:\n"
                        "tree_insert_edge : errcode %d\n", 1);
        fprintf(stderr, "Info:\n"
        				"ie = %d\n", ie);
        return -1;
    }
    if (t->edge_num >= n+1) {
        fprintf(stderr, "Error in /data/tree.c:\n"
                        "function: tree_insert_edge:\n"
                        "tree_insert_edge : errcode %d\n", 2);
        exit(1);
        return -2;
    }
#endif

    t->edges[t->edge_num] = ie;
    t->vrtx_deg[get_v1(ie)]++;
    t->vrtx_deg[get_v2(ie)]++;
    t->edge_num++;
    return 0;

}


int tree_remove_edge(tree *t, int ie) {

	return 0;
}


int tree_get_cost(tree *t, graph* g, double *cost) {

	int i;

	*cost = 0.0;
	for (i = 0; i < t->edge_num; i++) {
		*cost += g->edge_cost[t->edges[i]];
	}
	return 0;

}


int onetree_is_cycle(tree *t) {

	int i;
	for (i = 0; i < t->vrtx_num; i++) {
		if (t->vrtx_deg[i] != 2) {
			return FALSE;
		}
	}
	return TRUE;
}


int tree_rooting(tree *t, int root) {

	int i, j, k, v1, v2, etmp, m, e1, e2, my_edge, num_next;
	int n 		 = t->vrtx_num;
	int* pred 	 = (int*)malloc(n * sizeof(int));
	int* next 	 = (int*)malloc(n * sizeof(int));
	int* visited = (int*)calloc(n,  sizeof(int));

	int isonetree = (t->edge_num == n) ? TRUE : FALSE;

	/* case tree is 1-tree */
	if (isonetree == TRUE) {

		/* let e1 = (0, v1) and e2 = (0, v2) be
		 * the two edges incident to 0, such
		 * that v1 < v2: remove e2 */
		e1 = e2 = -1;
		for (i = 0; i < n+1; i++) {
			my_edge = t->edges[i];
			if (get_v1(my_edge) == 0 || get_v2(my_edge) == 0) {
				if 	(e1 < 0) 	e1 = my_edge;
				else			e2 = my_edge;
				if (e2 > 0) break;
			}
		}
		v1 = (get_v1(e1) > 0) ? get_v1(e1) : get_v2(e1);
		v2 = (get_v1(e2) > 0) ? get_v1(e2) : get_v2(e2);
		if (v1 > v2) {
			etmp = e1;
			e1 = e2;
			e2 = etmp;
		}
		/* remove e2 */
		k = 0;
		for (i = 0; i < t->edge_num; i++) {
			if (t->edges[i] != e2) {
				t->edges[k] = t->edges[i];

				k++;
			}
		}
		t->vrtx_deg[get_v1(e2)] -= 1;
		t->vrtx_deg[get_v2(e2)] -= 1;
		t->edge_num--;

	}

	for (i = 0; i < n; i++) t->vrtx_pred[i] = -1;
	next[0]			= root;
	num_next 		= 1;
	visited[root]	= 1;


	while (num_next > 0) {

		m = num_next;
		num_next = 0;

		for (i = 0; i < m; i++) {
			pred[i] = next[i];
		}

		for (i = 0; i < m; i++) {
			for (j = 0; j < t->edge_num; j++) {

				v1 = get_v1(t->edges[j]);
				v2 = get_v2(t->edges[j]);

				if		(v1 == pred[i] && visited[v2] == 0) {
					t->vrtx_pred[v2] = v1;
					visited[v2] 	 = 1;
					next[num_next++] = v2;
				}
				else if (v2 == pred[i] && visited[v1] == 0) {
					t->vrtx_pred[v1] = v2;
					visited[v1] 	 = 1;
					next[num_next++] = v1;
				}

			} /* end inner for */
		} /* end outer for */

	} /* end while loop */


	/* case tree is 1-tree */
	if (isonetree == TRUE) {
		tree_insert_edge(t, e2);
		if (get_v1(e2) > 0) t->vrtx_pred[0] = get_v1(e2);
		else				t->vrtx_pred[0] = get_v2(e2);
	}

	free(pred);
	free(next);
	free(visited);
	return 0;

}
