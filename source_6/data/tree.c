#include "../data/tree.h"


int tree_init(tree *t) {

    t->vrtx_num		= -1;
    t->vrtx_root	= -1;
    t->vrtx_pred	= NULL;
    t->vrtx_deg		= NULL;
    return 0;

}


int tree_delete(tree *t) {

    t->vrtx_num		= -1;
    t->vrtx_root	= -1;
    if (t->vrtx_pred != NULL) {
        free(t->vrtx_pred);
        t->vrtx_pred = NULL;
    }
    if (t->vrtx_deg != NULL) {
        free(t->vrtx_deg);
        t->vrtx_deg = NULL;
    }
    return 0;

}


int tree_setup(tree *t, int n, int r) {

#ifdef DEBUG
    if (n < 1) {
        fprintf(stderr, "Error in /data/tree.c:\n"
                        "function: tree_setup:\n"
                        "tree_setup : errcode %d\n", 1);
        return -1;
    }
    if (r < 0 || r >= n) {
        fprintf(stderr, "Error in /data/tree.c:\n"
                        "function: tree_setup:\n"
                        "tree_setup : errcode %d\n", 2);
        return -2;
    }
#endif

    if (t->vrtx_pred != NULL) {
        free(t->vrtx_pred);
    }
    if (t->vrtx_deg != NULL) {
        free(t->vrtx_deg);
    }

    int i;
    t->vrtx_num		= n;
    t->vrtx_root	= r;
    t->vrtx_pred	= (int*)malloc(n * sizeof(int));
    t->vrtx_deg		= (int*)malloc(n * sizeof(int));
    for (i = 0; i < n; i++ ) {
        t->vrtx_pred[i]	= -1;
        t->vrtx_deg[i]  = -1;
    }
    return 0;

}


int tree_copy(tree *from, tree *to) {

    if (to->vrtx_pred != NULL) {
        free(to->vrtx_pred);
    }
    if (to->vrtx_deg != NULL) {
        free(to->vrtx_deg);
    }

    int i;
    to->vrtx_num	= from->vrtx_num;
    to->vrtx_root 	= from->vrtx_root;
    to->vrtx_pred 	= (int*)malloc(from->vrtx_num * sizeof(int));
    to->vrtx_deg  	= (int*)malloc(from->vrtx_num * sizeof(int));
    for (i = 0; i < from->vrtx_num; i++) {
        to->vrtx_pred[i] = from->vrtx_pred[i];
        to->vrtx_deg[i]  = from->vrtx_deg[i];
    }
    return 0;

}


int tree_is_edge(tree* t, int u, int v) {

#ifdef DEBUG
    if (u < 0 || u >= t->vrtx_num) {
        fprintf(stderr, "Error in /data/tree.c:\n"
                        "function: tree_is_edge:\n"
                        "tree_is_edge : errcode %d\n", 1);
        return -1;
    }
    if (v < 0 || v >= t->vrtx_num) {
        fprintf(stderr, "Error in /data/tree.c:\n"
                        "function: tree_is_edge:\n"
                        "tree_is_edge : errcode %d\n", 2);
        return -2;
    }
    if (u == v) {
        fprintf(stderr, "Error in /data/tree.c:\n"
                        "function: tree_is_edge:\n"
                        "tree_is_edge : errcode %d\n", 3);
        return -3;
    }
#endif

    return (t->vrtx_pred[v] == u) || (t->vrtx_pred[u] == v) ;

}


int tree_insert_edge(tree *t, int p, int s) {

#ifdef DEBUG
    if (p < 0 || p >= t->vrtx_num) {
        fprintf(stderr, "Error in /data/tree.c:\n"
                        "function: tree_insert_edge:\n"
                        "tree_insert_edge : errcode %d\n", 1);
        fprintf(stderr, "Info:\n"
        				"p = %d\n", p);
        return -1;
    }
    if (s < 0 || s >= t->vrtx_num) {
        fprintf(stderr, "Error in /data/tree.c:\n"
                        "function: tree_insert_edge:\n"
                        "tree_insert_edge : errcode %d\n", 2);
        fprintf(stderr, "Info:\n"
        				"s = %d\n", s);
        return -2;
    }
    if (p == s) {
        fprintf(stderr, "Error in /data/tree.c:\n"
                        "function: tree_insert_edge:\n"
                        "tree_insert_edge : errcode %d\n", 3);
        fprintf(stderr, "Info:\n"
        				"p = s = %d\n", p);
        return -3;
    }
    if ( t->vrtx_pred[s] >= 0 || tree_is_edge(t, p, s) ) {
        fprintf(stderr, "Error in /data/tree.c:\n"
                        "function: tree_insert_edge:\n"
                        "tree_insert_edge : errcode %d\n", 4);
        return -4;
    }
#endif

    t->vrtx_pred[s] = p;
    t->vrtx_deg[p]++;
    t->vrtx_deg[s]++;
    return 0;

}


int tree_remove_edge(tree *t, int u, int v) {

#ifdef DEBUG
    if (u < 0 || v >= t->vrtx_num) {
        fprintf(stderr, "Error in /data/tree.c:\n"
                        "function: tree_remove_edge:\n"
                        "tree_remove_edge : errcode %d\n", 1);
        return -1;
    }
    if (u < 0 || v >= t->vrtx_num) {
        fprintf(stderr, "Error in /data/tree.c:\n"
                        "function: tree_remove_edge:\n"
                        "tree_insert_edge : errcode %d\n", 2);
        return -2;
    }
    if (u == v) {
        fprintf(stderr, "Error in /data/tree.c:\n"
                        "function: tree_remove_edge:\n"
                        "tree_insert_edge : errcode %d\n", 3);
        return -3;
    }
    if (tree_is_edge(t, u, v) == FALSE) {
        fprintf(stderr, "Error in /data/tree.c:\n"
                        "function: tree_remove_edge:\n"
                        "tree_insert_edge : errcode %d\n", 4);
        return -4;
    }
#endif

    if (t->vrtx_pred[v] == u) {
        t->vrtx_pred[v] = -1;
        t->vrtx_deg[u]--;
        t->vrtx_deg[v]--;
    }
    else if (t->vrtx_pred[u] == v) {
        t->vrtx_pred[u] = -1;
        t->vrtx_deg[u]--;
        t->vrtx_deg[v]--;
    }
    return 0;

}


int tree_get_cost(tree *t, graph* g, double *cost) {

	int i;
	double w;

	*cost = 0.0;
	for (i = 0; i < t->vrtx_num; i++) {
		if (t->vrtx_pred[i] >= 0) {
			graph_get_edge_cost(g, t->vrtx_pred[i], i, &w);
			*cost += w;
		}
	}
	return 0;

}

