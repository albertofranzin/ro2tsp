#include "../algos/heur_2opt.h"

int heur_2opt(graph *g, cycle *cyc, double* ub) {

	cycle *C = cyc;

	int n 			= g->vrtx_num;
	//double *costs	= (double*)malloc(n * sizeof(double));
 	int i, j, v, z, h, k, l;
	double *costs	= (double*)malloc(n * sizeof(double));
 	double ccost = 0.0;
	for (i = 0; i < n; i++) {
		//printf("%d ", i);
		costs[i] = g->edge_cost[ get_idx(C->vrtx_idx[i], C->vrtx_idx[(i+1)%n]) ];
		ccost	+= costs[i];
		//printf("%f\n", ccost);
	}

	double delta;
	double cost = ccost;

	//int n = C->n;
	int changed = 1;

	//int MAX_ITER = 100;
	int counter = 0;

	while (changed /*&& counter < MAX_ITER*/) {
		counter++;
		changed = 0;


		for (i = 0; i < n ; i++) {

		  for (j = i+2; j < n; j++) {
		    // i+2: We don't want something like i-->i+1=j-->j+1

		    if ((j+1) % n != i) {
		      // We don't want something like j-->j+1=i-->i+1
		      // (this happens only if i=0 and j=n-1 so that n-1-->0-->1).

		      v = C->vrtx_idx[i];
		      z = C->vrtx_idx[(i+1) % n];
		      h = C->vrtx_idx[j];
		      k = C->vrtx_idx[(j+1) % n];

		      /*printf("check now\n");
		      printf("%d %d | %d %d %d %d\n", i, j, v, z, h, k);*/

		      if((h == 0 && v == 0) ||
		         (z == 0 && k == 0)   ) {
		        continue;
		       }

		      // We are guaranteed that v, z, h, k are all different vertices.
		      delta = g->edge_cost[get_idx(v,h)] +
		              g->edge_cost[get_idx(z,k)] -
		              //graph_get_edge_cost(G, v, z) -
		              //graph_get_edge_cost(G, h, k);
		              costs[i] -
		              costs[j];


		      if (delta < -0.0000001) {

			      //printf("%d %d %f   ",i, j, delta);
		          ccost += delta;
		          //printf("%f\n", ccost);
		          
		          // Reverse the portion of the cycle which goes from
		          // index i+1 to index j;
		          // From ..., node[i]=v,  node[i+1]=z,  node[i+2]=v_1,  ...,
		          //    node[j-1]=v_s,  node[j]=h,  node[j+1]=k, ...
		          // To   ..., node[i]=v,  node[i+1]=h,  node[i+2]=v_s,  ...,
		          //    node[j-1]=v_1,  node[j]=z,  node[j+1]=k, ...
		          k = (i+1) % n; //0;
		          /*for (l = (i+1)%n; l != (j+1)%n; l = (l+1)%n) {
		            tmp_nodes[k] = C->nodes[l];
		            k++;
		          }

		          k = k-1;
		          for (l = (i+1)%n; l != (j+1)%n; l = (l+1)%n) {
		            C->nodes[l] = tmp_nodes[k];
		            k--;
		          }*/
		          int tmp;
		          for (l = 0; l <= (j - k) / 2; ++l) {

		            tmp             = C->vrtx_idx[k + l];
		            C->vrtx_idx[k + l] = C->vrtx_idx[j - l];
		            C->vrtx_idx[j - l] = tmp;

		            costs[i + l] = g->edge_cost[get_idx(C->vrtx_idx[i + l], C->vrtx_idx[(i+l+1) % n])];
		            costs[j - l] = g->edge_cost[get_idx(C->vrtx_idx[j - l], C->vrtx_idx[(j-l+1) % n])];

		          }

		          changed = 1;
		          i = j = n;

		      }
		    }
		  }
		} // end for

	} // end while

	*ub = cost;

	return SUCCESS;

	/*for (i = 0; i < n; i++) {
		cyc->vrtx_idx[i] = vertices[i];
	}
	*ub = ccost;

	free(vertices);
	return 0;*/

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
