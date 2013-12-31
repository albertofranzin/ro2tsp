#include "../data/graph.h"


int graph_init(graph *g) {

	g->vrtx_num = -1;
	g->vrtx_deg_all = NULL;
	g->vrtx_deg_frc = NULL;
	g->vrtx_deg_frb = NULL;
	g->edge_cost	= NULL;
	g->edge_cstr	= NULL;
	g->edge_flag	= NULL;
	return 0;

}


int graph_delete(graph *g) {

	g->vrtx_num = -1;
	if (g->vrtx_deg_all != NULL) {
		free(g->vrtx_deg_all);
		g->vrtx_deg_all = NULL;
	}
	if (g->vrtx_deg_frc != NULL) {
		free(g->vrtx_deg_frc);
		g->vrtx_deg_frc = NULL;
	}
	if (g->vrtx_deg_frb != NULL) {
		free(g->vrtx_deg_frb);
		g->vrtx_deg_frb = NULL;
	}
	if (g->edge_cost != NULL) {
		free(g->edge_cost);
		g->edge_cost = NULL;
	}
	if (g->edge_cstr != NULL) {
		free(g->edge_cstr);
		g->edge_cstr = NULL;
	}
	if (g->edge_flag != NULL) {
		free(g->edge_flag);
		g->edge_flag = NULL;
	}
	return 0;

}


int graph_setup(graph *g, int n) {

#ifdef DEBUG
    if (n < 1) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_setup:\n"
                        "graph_setup : errcode %d\n", 1);
        return -1;
    }
#endif

	if (g->vrtx_deg_all != NULL) {
		free(g->vrtx_deg_all);
	}
	if (g->vrtx_deg_frc != NULL) {
		free(g->vrtx_deg_frc);
	}
	if (g->vrtx_deg_frb != NULL) {
		free(g->vrtx_deg_frb);
	}
	if (g->edge_cost != NULL) {
		free(g->edge_cost);
	}
	if (g->edge_cstr != NULL) {
		free(g->edge_cstr);
	}
	if (g->edge_flag != NULL) {
		free(g->edge_flag);
	}

	int i;
	g->vrtx_num 	= n;
	g->vrtx_deg_all = (int*)malloc(n * sizeof(int));
	g->vrtx_deg_frc = (int*)malloc(n * sizeof(int));
	g->vrtx_deg_frb = (int*)malloc(n * sizeof(int));
	g->edge_cost 	= (double*)malloc((n * (n + 1) / 2) * sizeof(double));
	g->edge_cstr 	= (int*)malloc((n * (n + 1) / 2) * sizeof(int));
	g->edge_flag 	= (int*)malloc((n * (n + 1) / 2) * sizeof(int));
    for (i = 0; i < n; i++ ) {
        g->vrtx_deg_all[i]	= 0;
        g->vrtx_deg_frc[i]  = 0;
        g->vrtx_deg_frb[i]  = 0;
    }
    for (i = 0; i < n * (n + 1) / 2; i++) {
        g->edge_cost[i]		= 0.0;
        g->edge_cstr[i]		= FREE;
        g->edge_flag[i]		= FALSE;
    }
    return 0;

}


int graph_copy(graph *from, graph *to) {

	if (to->vrtx_deg_all != NULL) {
		free(to->vrtx_deg_all);
	}
	if (to->vrtx_deg_frc != NULL) {
		free(to->vrtx_deg_frc);
	}
	if (to->vrtx_deg_frb != NULL) {
		free(to->vrtx_deg_frb);
	}
	if (to->edge_cost != NULL) {
		free(to->edge_cost);
	}
	if (to->edge_cstr != NULL) {
		free(to->edge_cstr);
	}
	if (to->edge_flag != NULL) {
		free(to->edge_flag);
	}

	int i;
	to->vrtx_num 		= from->vrtx_num;
	to->vrtx_deg_all 	= (int*)malloc(from->vrtx_num * sizeof(int));
	to->vrtx_deg_frc 	= (int*)malloc(from->vrtx_num * sizeof(int));
	to->vrtx_deg_frb 	= (int*)malloc(from->vrtx_num * sizeof(int));
	to->edge_cost		= (double*)malloc
			((from->vrtx_num * (from->vrtx_num + 1) / 2) * sizeof(double));
	to->edge_cstr 		= (int*)malloc
			((from->vrtx_num * (from->vrtx_num + 1) / 2) * sizeof(int));
	to->edge_flag 		= (int*)malloc
			((from->vrtx_num * (from->vrtx_num + 1) / 2) * sizeof(int));
	for (i = 0; i < from->vrtx_num; i++) {
		to->vrtx_deg_all[i] = from->vrtx_deg_all[i];
		to->vrtx_deg_frc[i] = from->vrtx_deg_frc[i];
		to->vrtx_deg_frb[i] = from->vrtx_deg_frb[i];
	}
	for (i = 0; i < from->vrtx_num * (from->vrtx_num + 1) / 2; i++) {
		to->edge_cost[i] = from->edge_cost[i];
		to->edge_cstr[i] = from->edge_cstr[i];
		to->edge_flag[i] = from->edge_flag[i];
	}
	return 0;

}


int graph_is_edge(graph* g, int u, int v) {

#ifdef DEBUG
    if (u < 0 || u >= g->vrtx_num) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_is_edge:\n"
                        "graph_is_edge : errcode %d\n", 1);
        return -1;
    }
    if (v < 0 || v >= g->vrtx_num) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_is_edge:\n"
                        "graph_is_edge : errcode %d\n", 2);
        return -2;
    }
    if (u == v) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_is_edge:\n"
                        "graph_is_edge : errcode %d\n", 3);
        return -3;
    }
#endif

    return (u > v) ? g->edge_flag[ u*(u+1)/2 + v ] : g->edge_flag[ v*(v+1)/2 + u ];

}

int graph_insert_edge(graph *g, int u, int v, double w, int c) {

#ifdef DEBUG
    if (u < 0 || u >= g->vrtx_num) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_insert_edge:\n"
                        "graph_insert_edge : errcode %d\n", 1);
        fprintf(stderr, "Info:\n"
        				"args: u = %d", u);
        return -1;
    }
    if (v < 0 || v >= g->vrtx_num) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_insert_edge:\n"
                        "graph_insert_edge : errcode %d\n", 2);
        fprintf(stderr, "Info:\n"
        				"args: v = %d", v);
        return -2;
    }
    if (u == v) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_insert_edge:\n"
                        "graph_insert_edge : errcode %d\n", 3);
        fprintf(stderr, "Info:\n"
        				"args: u = v = %d", u);
        return -3;
    }
    if (graph_is_edge(g, u, v) == TRUE) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_insert_edge:\n"
                        "graph_insert_edge : errcode %d\n", 4);
        return -4;
    }
#endif

    (u > v) ? ( g->edge_flag[ u*(u+1)/2 + v ] = TRUE ) :
    		( g->edge_flag[ v*(v+1)/2 + u ] = TRUE );
    (u > v) ? ( g->edge_cost[ u*(u+1)/2 + v ] = w ) :
    		( g->edge_cost[ v*(v+1)/2 + u ] = w );
    (u > v) ? ( g->edge_cstr[ u*(u+1)/2 + v ] = c ) :
    		( g->edge_cstr[ v*(v+1)/2 + u ] = c );

    g->vrtx_deg_all[u]++;
    g->vrtx_deg_all[v]++;

    if (c == FORCED) {
      g->vrtx_deg_frc[u]++;
      g->vrtx_deg_frc[v]++;
    }
    else if (c == FORBIDDEN) {
      g->vrtx_deg_frb[u]++;
      g->vrtx_deg_frb[v]++;
    }
   return 0;

}


int graph_remove_edge(graph *g, int u, int v) {

#ifdef DEBUG
    if (u < 0 || u >= g->vrtx_num) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_remove_edge:\n"
                        "graph_remove_edge : errcode %d\n", 1);
        return -1;
    }
    if (v < 0 || v >= g->vrtx_num) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_remove_edge:\n"
                        "graph_remove_edge : errcode %d\n", 2);
        return -2;
    }
    if (u == v) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_remove_edge:\n"
                        "graph_remove_edge : errcode %d\n", 3);
        return -3;
    }
    if (graph_is_edge(g, u, v) == FALSE) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_remove_edge:\n"
                        "graph_remove_edge : errcode %d\n", 4);
        fprintf(stderr, "Info:\n"
        				"args: u = %d, v = %d\n", u, v);
        return -4;
    }
#endif

    int c = (u > v) ? g->edge_cstr[ u*(u+1)/2 + v ] : g->edge_cstr[ v*(v+1)/2 + u];
    (u > v) ? ( g->edge_flag[ u*(u+1)/2 + v ] = FALSE ) :
    		( g->edge_flag[ v*(v+1)/2 + u] = FALSE );
    (u > v) ? ( g->edge_cost[ u*(u+1)/2 + v ] = 0.0 ) :
    		( g->edge_cost[ v*(v+1)/2 + u] = 0.0 );
    (u > v) ? ( g->edge_cstr[ u*(u+1)/2 + v ] = FREE ) :
    		( g->edge_cstr[ v*(v+1)/2 + u] = FREE );

    g->vrtx_deg_all[u]--;
    g->vrtx_deg_all[v]--;
    if (c == FORCED) {
      g->vrtx_deg_frc[u]--;
      g->vrtx_deg_frc[v]--;
    }
    else if (c == FORBIDDEN) {
      g->vrtx_deg_frb[u]--;
      g->vrtx_deg_frb[v]--;
    }
   return 0;

}


int graph_set_edge_cost(graph *g, int u, int v, double w) {

#ifdef DEBUG
    if (u < 0 || u >= g->vrtx_num) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_set_edge_cost:\n"
                        "graph_set_edge_cost : errcode %d\n", 1);
        return -1;
    }
    if (v < 0 || v >= g->vrtx_num) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_set_edge_cost:\n"
                        "graph_set_edge_cost : errcode %d\n", 2);
        return -2;
    }
    if (u == v) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_set_edge_cost:\n"
                        "graph_set_edge_cost : errcode %d\n", 3);
        return -3;
    }
    if (graph_is_edge(g, u, v) == FALSE) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_set_edge_cost:\n"
                        "graph_set_edge_cost : errcode %d\n", 4);
        fprintf(stderr, "Info:\n"
        				"args: u = %d, v = %d\n", u, v);
        return -4;
    }
#endif

    (u > v) ? ( g->edge_cost[ u*(u+1)/2 + v ] = w ) :
    		( g->edge_cost[ v*(v+1)/2 + u ] = w );
    return 0;

}


int graph_get_edge_cost(graph *g, int u, int v, double *w) {

#ifdef DEBUG
    if (u < 0 || u >= g->vrtx_num) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_get_edge_cost:\n"
                        "graph_get_edge_cost : errcode %d\n", 1);
        return -1;
    }
    if (v < 0 || v >= g->vrtx_num) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_get_edge_cost:\n"
                        "graph_get_edge_cost : errcode %d\n", 2);
        return -2;
    }
    if (u == v) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_get_edge_cost:\n"
                        "graph_get_edge_cost : errcode %d\n", 3);
        return -3;
    }
    if (graph_is_edge(g, u, v) == FALSE) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_get_edge_cost:\n"
                        "graph_get_edge_cost : errcode %d\n", 4);
        fprintf(stderr, "Info:\n"
        				"args: u = %d, v = %d\n", u, v);
        return -4;
    }
#endif

    *w = (u > v) ? g->edge_cost[ u*(u+1)/2 + v ] : g->edge_cost[ v*(v+1)/2 + u ];
    return 0;

}


int graph_set_edge_cstr(graph *g, int u, int v, int c) {

#ifdef DEBUG
    if (u < 0 || u >= g->vrtx_num) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_set_edge_cstr:\n"
                        "graph_set_edge_cstr : errcode %d\n", 1);
        return -1;
    }
    if (v < 0 || v >= g->vrtx_num) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_set_edge_cstr:\n"
                        "graph_set_edge_cstr : errcode %d\n", 2);
        return -2;
    }
    if (u == v) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_set_edge_cstr:\n"
                        "graph_set_edge_cstr : errcode %d\n", 3);
        return -3;
    }
    if (graph_is_edge(g, u, v) == FALSE) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_set_edge_cstr:\n"
                        "graph_set_edge_cstr : errcode %d\n", 4);
        fprintf(stderr, "Info:\n"
        				"args: u = %d, v = %d\n", u, v);
        return -4;
    }
#endif

    int c_old = (u > v) ? g->edge_cstr[ u*(u+1)/2 + v ] : g->edge_cstr[ v*(v+1)/2 + u ];
    (u > v) ? ( g->edge_cstr[ u*(u+1)/2 + v ] = c ) :
    		( g->edge_cstr[ v*(v+1)/2 + u ] = c );

    if (c_old == FORCED) {
      g->vrtx_deg_frc[u]--;
      g->vrtx_deg_frc[v]--;
    }
    else if (c_old == FORBIDDEN) {
      g->vrtx_deg_frb[u]--;
      g->vrtx_deg_frb[v]--;
    }
    if (c == FORCED) {
      g->vrtx_deg_frc[u]++;
      g->vrtx_deg_frc[v]++;
    }
    else if (c == FORBIDDEN) {
      g->vrtx_deg_frc[u]++;
      g->vrtx_deg_frc[v]++;
    }
    return 0;

}


int graph_get_edge_cstr(graph *g, int u, int v, int *c) {

#ifdef DEBUG
    if (u < 0 || u >= g->vrtx_num) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_get_edge_cstr:\n"
                        "graph_get_edge_cstr : errcode %d\n", 1);
        return -1;
    }
    if (v < 0 || v >= g->vrtx_num) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_get_edge_cstr:\n"
                        "graph_get_edge_cstr : errcode %d\n", 2);
        return -2;
    }
    if (u == v) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_get_edge_cstr:\n"
                        "graph_get_edge_cstr : errcode %d\n", 3);
        return -3;
    }
    if (graph_is_edge(g, u, v) == FALSE) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_get_edge_cstr:\n"
                        "graph_get_edge_cstr : errcode %d\n", 4);
        fprintf(stderr, "Info:\n"
        				"args: u = %d, v = %d\n", u, v);
        return -4;
    }
#endif

    *c = (u > v) ? g->edge_cstr[ u*(u+1)/2 + v ] : g->edge_cstr[ v*(v+1)/2 + u ];
    return 0;

}



