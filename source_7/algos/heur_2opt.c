#include "../algos/heur_2opt.h"

int heur_2opt(graph *g, cycle *cyc, double* ub) {


	int n 			= g->vrtx_num;
	//double *costs	= (double*)malloc(n * sizeof(double));
	int i, j;
	double ccost, delta;

	int v, z, h, k;



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

	double oldweight;

	while (changed == TRUE) {

	    counter++;
	    changed = FALSE;


	    for (i = 0; i < n; i++) {
	    	for (j = i+2; j < n; j++) {

	    		v = vertices[i];
	    		z = vertices[(i+1)%n];
	    		h = vertices[j];
	    		k = vertices[(j+1)%n];

	    		if (v == 0 && k == 0) {
	    		   continue;
	    		}

	    	    // We are guaranteed that v, z, h, k are all different vertices.
	    		oldweight = g->edge_cost[get_idx(v, z)] +
	    					g->edge_cost[get_idx(h, k)];


	    		// case: vhzk
	    		delta = g->edge_cost[get_idx(v, h)] +
	    				g->edge_cost[get_idx(z, k)] -
	    				oldweight;


	    		if (delta < 0.0) {

	    			ccost += delta;

	    			// reverse(z, h)
	    			reverse(vertices, i+1, j);

	    			changed = TRUE;
	    			i = j = n;

	    		} // end if

	    	} // end for j
	    } // end for i

	} // end while loop

	for (i = 0; i < n; i++) {
		cyc->vrtx_idx[i] = vertices[i];
	}
	*ub = ccost;

	free(vertices);
	return 0;

}


/*
int heur_2opt(graph *g, cycle *c, double* ub) {


	int n 			= g->vrtx_num;
	double *costs	= (double*)malloc(n * sizeof(double));
	int i, j, v, z, h, k, l, tmp;
	double ccost, delta;



	ccost = 0.0;
	for (i = 0; i < n; i++) {
		costs[i] = g->edge_cost[ get_idx(c->vrtx_idx[i], c->vrtx_idx[(i+1)%n]) ];
		ccost	+= costs[i];
	}

	int changed = TRUE;
	int counter = 0;

	while (changed == TRUE) {

	    counter++;
	    changed = FALSE;

	    for (i = 0; i < n ; i++) {
	    	for (j = i+2; j < n; j++) {
		    // i+2: We don't want something like i-->i+1=j-->j+1

	    		if ((j+1) % n != i) {
	    		// We don't want something like j-->j+1=i-->i+1
	    		// this happens only if i=0 and j=n-1 so that n-1-->0-->1

	    			v = c->vrtx_idx[i];
	    			z = c->vrtx_idx[(i+1) % n];
	    			h = c->vrtx_idx[j];
	    			k = c->vrtx_idx[(j+1) % n];


	    			if((h == 0 && v == 0) ||
	    			   (z == 0 && k == 0)  ) {
	    				continue;
	    			}

	    			// We are guaranteed that v, z, h, k are all different vertices.
	    			delta = g->edge_cost[get_idx(v, h)] +
	    					g->edge_cost[get_idx(z, k)] -
	    									   costs[i] -
	    									   costs[j];

	    			if (delta < 0.0) {

	    				ccost += delta;

	    				// Reverse the portion of the cycle which goes from
	    				// index i+1 to index j;
	    				// From ..., node[i]=v,  node[i+1]=z,  node[i+2]=v_1,  ...,
	    				// node[j-1]=v_s,  node[j]=h,  node[j+1]=k, ...
	    				// To   ..., node[i]=v,  node[i+1]=h,  node[i+2]=v_s,  ...,
	    				// node[j-1]=v_1,  node[j]=z,  node[j+1]=k, ...
	    				//
	    				k = (i+1) % n;


	    				for (l = 0; l <= (j - k) / 2; ++l) {
	    					tmp = c->vrtx_idx[k + l];
	    					c->vrtx_idx[k + l] 	= c->vrtx_idx[j - l];
	    					c->vrtx_idx[j - l] 	= tmp;
	    					costs[i + l] = g->edge_cost[get_idx(c->vrtx_idx[i + l],
	    										          c->vrtx_idx[(i+l+1)%n])];

	    					costs[j - l] = g->edge_cost[get_idx(c->vrtx_idx[j - l],
	    			                                      c->vrtx_idx[(j-l+1)%n])];

	    				}

	    				changed = TRUE;
	    			    i = j = n;
	    			}
	    		}

	    	} // end for j
	    } // end for i
	} // end while


	*ub = ccost;
	return 0;

}
*/
