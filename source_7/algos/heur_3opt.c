#include "../algos/heur_3opt.h"

int heur_3opt(graph *g, cycle *cyc, double* ub) {


	int n 			= g->vrtx_num;
	int i, j, l, h, k;
	double ccost, delta;

	int a, b, c, d, e, f;

	ccost = 0.0;
	for (i = 0; i < n; i++) {
		ccost += g->edge_cost[ get_idx(cyc->vrtx_idx[i], cyc->vrtx_idx[(i+1)%n]) ];
	}


	int *vertices = (int*)malloc(n * sizeof(int));
	for (i = 0; i < n; i++) {
		vertices[i] = cyc->vrtx_idx[i];
	}

	int changed = TRUE;
	int counter = 0;

	double *deltas = (double*)malloc(7 * sizeof(double));

	double oldweight;


	while (changed == TRUE) {


	    counter++;
	    changed = FALSE;


	    for (i = 0; i < n; i++) {
	    	for (j = i+2; j < n; j++) {
	    		for (k = j+2; k < n; k++) {


	    			//printf("i = %d, j = %d, k = %d\n", i, j, k);

	    			a = vertices[i];
	    			b = vertices[(i+1)%n];
	    			c = vertices[j];
	    			d = vertices[(j+1)%n];
	    			e = vertices[k];
	    			f = vertices[(k+1)%n];

	    			if (a == 0 && f == 0) {
	    				continue;
	    			}


	    			oldweight = g->edge_cost[get_idx(a, b)] +
	    						g->edge_cost[get_idx(c, d)] +
	    						g->edge_cost[get_idx(e, f)];


	    			/* case 0: adecbf */
	    			deltas[0] = g->edge_cost[get_idx(a, d)] +
	    						g->edge_cost[get_idx(e, c)] +
	    						g->edge_cost[get_idx(b, f)] -
	    						oldweight;
	    			/* case 0: adebcf */
	    			deltas[1] = g->edge_cost[get_idx(a, d)] +
	    						g->edge_cost[get_idx(e, b)] +
	    						g->edge_cost[get_idx(c, f)] -
	    						oldweight;
	    			/* case 0: aedbcf */
	    			deltas[2] = g->edge_cost[get_idx(a, e)] +
	    						g->edge_cost[get_idx(d, b)] +
	    						g->edge_cost[get_idx(c, f)] -
	    						oldweight;
	    			/* case 0: acbedf */
	    			deltas[3] = g->edge_cost[get_idx(a, c)] +
	    						g->edge_cost[get_idx(b, e)] +
	    						g->edge_cost[get_idx(d, f)] -
	    						oldweight;
	    			/* case 0: aedcbf */
	    			deltas[4] = g->edge_cost[get_idx(a, e)] +
	    						g->edge_cost[get_idx(d, c)] +
	    						g->edge_cost[get_idx(b, f)] -
	    						oldweight;
	    			/* case 0:  acbdef */
	    			deltas[5] = g->edge_cost[get_idx(a, c)] +
	    						g->edge_cost[get_idx(b, d)] +
	    						g->edge_cost[get_idx(e, f)] -
	    						oldweight;
	    			/* case 0: abcedf */
	    			deltas[6] = g->edge_cost[get_idx(a, b)] +
	    						g->edge_cost[get_idx(c, e)] +
	    						g->edge_cost[get_idx(d, f)] -
	    						oldweight;


	    			delta = 0.0;
	    			for (l = 0; l < 7; l++) {
	    				if (deltas[l] < 0.0) {
	    				//if (deltas[l] < delta ) {
	    					delta = deltas[l];
	    					h = l;
	    					//break;
	    				}
	    			}

	    			if (delta < 0.0) {

	    				ccost += delta;


	    				if (h == 0) {
	    					reverse(vertices, j+1, k);
	    					reverse(vertices, i+1, k);
	    				}
	    				if (h == 1) {
	    					reverse(vertices, i+1, j);
	    					reverse(vertices, j+1, k);
	    					reverse(vertices, i+1, k);

	    				}
	    				if (h == 2) {
	    					reverse(vertices, i+1, j);
	    					reverse(vertices, i+1, k);
	    				}
	    				if (h == 3) {
	    					reverse(vertices, i+1, j);
	    					reverse(vertices, j+1, k);
	    				}
	    				if (h == 4) { /* change 2 edges only */
	    					reverse(vertices, i+1, k);
	    				}
	    				if (h == 5) { /* change 2 edges only */
	    					reverse(vertices, i+1, j);
	    				}
	    				if (h == 6) { /* change 2 edges only */
	    					reverse(vertices, j+1, k);
	    				}


	    				changed = TRUE;
	    				i = j = k = n;
	    			} /* end if */

	    		} /* end for k */
	    	} /* end for j */
	    } /* end for i */

	} /* end while loop */

	for (i = 0; i < n; i++) {
		cyc->vrtx_idx[i] = vertices[i];
	}
	*ub = ccost;

	free(vertices);
	return 0;

}
