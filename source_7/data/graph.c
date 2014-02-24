#include "../data/graph.h"



int graph_init(graph *g) {

	g->vrtx_num 	= -1;
	g->vrtx_deg 	= NULL;
	g->vrtx_frc 	= NULL;
	g->vrtx_frb 	= NULL;
	g->edge_cost	= NULL;
	g->edge_cstr	= NULL;
	g->edge_flag	= NULL;
	g->edge_delta   = NULL;
	return 0;

}


int graph_delete(graph *g) {

	g->vrtx_num = -1;
	if (g->vrtx_deg != NULL) {
		free(g->vrtx_deg);
		g->vrtx_deg = NULL;
	}
	if (g->vrtx_frc != NULL) {
		free(g->vrtx_frc);
		g->vrtx_frc = NULL;
	}
	if (g->vrtx_frb != NULL) {
		free(g->vrtx_frb);
		g->vrtx_frb = NULL;
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
	if (g->edge_delta != NULL) {
		free(g->edge_delta);
		g->edge_delta = NULL;
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

	if (g->vrtx_deg != NULL) {
		free(g->vrtx_deg);
	}
	if (g->vrtx_frc != NULL) {
		free(g->vrtx_frc);
	}
	if (g->vrtx_frb != NULL) {
		free(g->vrtx_frb);
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
	if (g->edge_delta != NULL) {
		free(g->edge_delta);
	}

	int i;
	int ne = (n * (n - 1)) / 2;
	g->vrtx_num 	= n;
	g->vrtx_deg 	= (int*)malloc(n * sizeof(int));
	g->vrtx_frc 	= (int*)malloc(n * sizeof(int));
	g->vrtx_frb 	= (int*)malloc(n * sizeof(int));
	g->edge_cost 	= (double*)malloc(ne * sizeof(double));
	g->edge_cstr 	= (int*)malloc(ne * sizeof(int));
	g->edge_flag 	= (int*)malloc(ne * sizeof(int));
	g->edge_delta 	= (double*)malloc(ne * sizeof(double));

    for (i = 0; i < n; i++ ) {
        g->vrtx_deg[i]	= 0;
        g->vrtx_frc[i]  = 0;
        g->vrtx_frb[i]  = 0;
    }

    for (i = 0; i < ne; i++) {
        g->edge_cost[i]	= 0.0;
        g->edge_cstr[i]	= FREE;
        g->edge_flag[i]	= FALSE;
        g->edge_delta[i]= 0.0;
    }
    return 0;

}


int graph_copy(graph *from, graph *to) {

	if (to->vrtx_deg != NULL) {
		free(to->vrtx_deg);
	}
	if (to->vrtx_frc != NULL) {
		free(to->vrtx_frc);
	}
	if (to->vrtx_frb != NULL) {
		free(to->vrtx_frb);
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
	if (to->edge_delta != NULL) {
		free(to->edge_delta);
	}


	int i;
	int n			= from->vrtx_num;
	int ne			= (n * (n - 1)) / 2;
	to->vrtx_num 	= n;
	to->vrtx_deg 	= (int*)malloc(n * sizeof(int));
	to->vrtx_frc 	= (int*)malloc(n * sizeof(int));
	to->vrtx_frb 	= (int*)malloc(n * sizeof(int));
	to->edge_cost	= (double*)malloc(ne * sizeof(double));
	to->edge_cstr 	= (int*)malloc(ne * sizeof(int));
	to->edge_flag 	= (int*)malloc(ne * sizeof(int));
	to->edge_delta 	= (double*)malloc(ne * sizeof(double));

	for (i = 0; i < n; i++) {
		to->vrtx_deg[i]	= from->vrtx_deg[i];
		to->vrtx_frc[i] = from->vrtx_frc[i];
		to->vrtx_frb[i] = from->vrtx_frb[i];
	}
	for (i = 0; i < ne; i++) {
		to->edge_cost[i] = from->edge_cost[i];
		to->edge_cstr[i] = from->edge_cstr[i];
		to->edge_flag[i] = from->edge_flag[i];
		to->edge_delta[i] = from->edge_delta[i];
	}
	return 0;

}


int graph_insert_edge(graph *g, int ie, double w, int c) {

	int n = g->vrtx_num;

#ifdef DEBUG
    if (ie < 0 || ie >= (n * (n - 1)) / 2) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_insert_edge:\n"
                        "graph_insert_edge : errcode %d\n", 1);
        fprintf(stderr, "Info:\n"
        				"args: ie = %d", ie);
        return -1;
    }
    if (g->edge_flag[ie] == TRUE) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_insert_edge:\n"
                        "graph_insert_edge : errcode %d\n", 2);
        return -2;
    }
#endif

    g->edge_flag[ie] = TRUE;
    g->edge_cost[ie] = w;
    g->edge_cstr[ie] = c;

    g->vrtx_deg[get_v1(ie)]++;
    g->vrtx_deg[get_v2(ie)]++;


    if (c == FORCED) {
      g->vrtx_frc[get_v1(ie)]++;
      g->vrtx_frc[get_v2(ie)]++;
    }
    else if (c == FORBIDDEN) {
      g->vrtx_frb[get_v1(ie)]++;
      g->vrtx_frb[get_v2(ie)]++;
    }
   return 0;

}


int graph_remove_edge(graph *g, int ie) {

	int n = g->vrtx_num;

#ifdef DEBUG
    if (ie < 0 || ie >= (n * (n - 1)) / 2) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_remove_edge:\n"
                        "graph_remove_edge : errcode %d\n", 1);
        return -1;
    }
    if (g->edge_flag[ie] == FALSE) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_remove_edge:\n"
                        "graph_remove_edge : errcode %d\n", 2);
        fprintf(stderr, "Info:\n"
        				"args: u = %d, v = %d\n", get_v1(ie), get_v2(ie));
        return -2;
    }
#endif

    int c = g->edge_cstr[ie];
    g->edge_flag[ie] = FALSE;
    g->edge_cost[ie] = 0.0;
    g->edge_cstr[ie] = FREE;


    g->vrtx_deg[get_v1(ie)]--;
    g->vrtx_deg[get_v2(ie)]--;
    if (c == FORCED) {
      g->vrtx_frc[get_v1(ie)]--;
      g->vrtx_frc[get_v2(ie)]--;
    }
    else if (c == FORBIDDEN) {
      g->vrtx_frb[get_v1(ie)]--;
      g->vrtx_frb[get_v2(ie)]--;
    }
   return 0;

}


int graph_set_edge_cstr(graph *g, int ie, int c) {

	int n = g->vrtx_num;

#ifdef DEBUG
    if (ie < 0 || ie >= (n * (n - 1)) / 2) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_set_edge_cstr:\n"
                        "graph_set_edge_cstr : errcode %d\n", 1);
        fprintf(stderr, "Info:\n"
        				"ie = %d\n", ie);
        return -1;
    }
    if (g->edge_flag[ie] == FALSE) {
        fprintf(stderr, "Error in /data/graph.c:\n"
                        "function: graph_set_edge_cstr:\n"
                        "graph_set_edge_cstr : errcode %d\n", 2);
        fprintf(stderr, "Info:\n"
        				"args: u = %d, v = %d\n", get_v1(ie), get_v2(ie));
        return -2;
    }
#endif

    int c_old = g->edge_cstr[ie];
    g->edge_cstr[ie] = c;

    if (c_old == FORCED) {
      g->vrtx_frc[get_v1(ie)]--;
      g->vrtx_frc[get_v2(ie)]--;
    }
    else if (c_old == FORBIDDEN) {
      g->vrtx_frb[get_v1(ie)]--;
      g->vrtx_frb[get_v2(ie)]--;
    }
    if (c == FORCED) {
      g->vrtx_frc[get_v1(ie)]++;
      g->vrtx_frc[get_v2(ie)]++;
    }
    else if (c == FORBIDDEN) {
      g->vrtx_frb[get_v1(ie)]++;
      g->vrtx_frb[get_v2(ie)]++;
    }
    return 0;

}
