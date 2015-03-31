#include "../algos/heur_rc2opt.h"


int heur_rc2opt(graph  *g, cycle  *best_c, double *best_ub, int *ones, int *zeros) {

	int i, j;
	int n = g->vrtx_num;

    int num_total_trials 	= NUM_TRIALS_RC2OPT / n;
    int num_threads			= NUM_THREADS;

    //printf("random cycles: %d threads, each with up to %d trials\n",
    //num_threads, (int)(NUM_TRIALS_RC2OPT / n / num_threads));

    pthread_t *thread	= (pthread_t*)malloc(num_threads * sizeof(pthread_t));
    rc_params *rcp		= (rc_params*)malloc(num_threads * sizeof(rc_params));
    cycle     *cycles	= (cycle*)malloc(num_threads * sizeof(cycle));

    for (i = 0; i < num_threads; ++i) {
    	cycle_init(&cycles[i]);
    }


    int **local_ones 	= (int**)malloc(num_threads * sizeof(int *));
    int **local_zeros 	= (int**)malloc(num_threads * sizeof(int *));
    int  *local_n_ones  = (int*) malloc(num_threads * sizeof(int));
    int  *local_n_zeros = (int*) malloc(num_threads * sizeof(int));

    memset(local_n_ones, 0, num_threads);
    memset(local_n_zeros, 0, num_threads);

    for (i = 0; i < num_threads; ++i) {
    	local_zeros[i] = malloc((n * (n - 1)) / 2 *sizeof(int));
    	for (j = 0; j < n*(n-1)/2; ++j) {
    		local_zeros[i][j] = 1;
    	}
    	local_ones[i]  = malloc((n * (n - 1)) / 2 *sizeof(int));
    }

    int *zs = (int*)malloc((n * (n - 1)) / 2 *sizeof(int));
    int *os = (int*)malloc((n * (n - 1)) / 2 *sizeof(int));

    // initially, all zeros - just saves time later
    for (i = 0; i < n*(n-1)/2; ++i) {
    	zeros[i] = 1;
    }

    memset(os, 0, (n * (n - 1)) / 2 *sizeof(int));

    for (i = 0; i < num_threads; ++i) {

    	rcp[i].th_no            = i;
    	rcp[i].g                = g;
    	rcp[i].c                = &cycles[i];
    	rcp[i].num_cycles		= num_total_trials / num_threads;
    	rcp[i].zeros            = local_zeros[i];
    	rcp[i].ones             = local_ones[i];
    	rcp[i].n_zeros          = local_n_zeros[i];
    	rcp[i].n_ones           = local_n_ones[i];


    	if (pthread_create(&thread[i], NULL, rc_thread, (void *)&rcp[i])) {
    		fprintf(stderr, "Fatal error in compute_upper_bound.c :: \n");
    		fprintf(stderr, "error while creating thread %d\n", i);
    		exit(1);
    	}
    } /* end thread creation */


    for (i = 0; i < num_threads; ++i) {

    	if (pthread_join(thread[i], NULL)) {
    		fprintf(stderr, "Fatal error in compute_upper_bound.c :: \n");
    		fprintf(stderr, "error while joining thread %d\n", i);
    		exit(1);
    	}

    	//if (min >= rcp[i].ub) { ???
    	if (i == 0 || rcp[i].ub < *best_ub) {
    		cycle_copy(rcp[i].c, best_c);
    		*best_ub = rcp[i].ub;
    	}

    	for (j = 0; j < n*(n-1)/2; ++j) {
    		if(rcp[i].zeros[j] == 0) zeros[j] = 0;
    		if(rcp[i].ones[j] == 1)  os[j]++;
        }

    } /* end thread joining */

    for (j = 0; j < n*(n-1)/2; ++j) {
        if(os[j] == num_threads) {
            ones[j] = 1;
    	}
    	else {
    		ones[j] = 0;
    	}
    }

    for (i = 0; i < num_threads; ++i) {
    	free(local_ones[i]);
    	free(local_zeros[i]);
    }
    free(local_ones);
    free(local_zeros);
    free(local_n_ones);
    free(local_n_zeros);

    free(thread);
    free(rcp);
    free(cycles);
    return 0;
}


void *rc_thread(void *p) {

	rc_params *rcp = (rc_params *) p;

	//better trying different random seeds?
	//srand(time(NULL) * rcp->th_no);
	srand(1 * rcp->th_no);

	int i;
	int n      		= rcp->g->vrtx_num;
	int num_trials 	= rcp->num_cycles;
	double ccost, min;

	//int count_opt 		= 0;
	int count_onepercent 	= 0;
	int cc					= 0;

	int *zs = (int*)malloc((n * (n - 1)) / 2 *sizeof(int));
	int *os = (int*)malloc((n * (n - 1)) / 2 *sizeof(int));

	// initially, all zeros - just saves time later
	for (i = 0; i < (n * (n - 1)) / 2; ++i) {
		zs[i] = 1;
	}
	memset(os, 0, (n * (n - 1)) / 2 *sizeof(int));

	cycle_delete(rcp->c);
	cycle_setup(rcp->c, n);

	cycle temp_c;
	cycle_init(&temp_c);
	cycle_setup(&temp_c, n);

	for (i = 0; i < num_trials; i++) {

		//printf("trial = %d\n", i);
		cc++;
		heur_rc(rcp->g, &temp_c, &ccost);
		heur_2opt(rcp->g, &temp_c, &ccost);

		if (i == 0 || ccost < min) {
			// printf("update! ub = %.2f\n", ccost);
			cycle_copy(&temp_c, rcp->c);
			min = ccost;

			int j, idx, x, y;
			for (j = 0; j < n; ++j) {
				x = rcp->c->vrtx_idx[j];
				y = rcp->c->vrtx_idx[(j+1)%n];
				if (x < y) {
					idx = y * (y - 1) / 2 + x;
				}
				else {
					idx = x * (x - 1) / 2 + y;
				}
				zs[idx] = 0;
				os[idx] += 1;
			}
		}

		if (ccost == min) {
			count_onepercent++;
			// last_1pc = i;
			//getchar();
		}
	}

	int nzs = 0;
	int nos = (n * (n - 1)) / 2;

	for (i = 0; i < (n * (n - 1)) / 2; ++i) {

		if (os[i] == num_trials) {
			rcp->n_ones++;
			rcp->ones[i] = 1;
			nos++;
		}
		if (zs[i] == 0) {
			rcp->zeros[i] = 0;
			nzs--;
		}
	}

	rcp->n_ones  	= nos;
	rcp->n_zeros 	= nzs;
	rcp->ub			= min;

	/*printf("++ %f . ratio = %f\n", min, ((double)cc)/i);
  	  printf("recap : count_opt = %d, count_onepercent = %d\n", count_opt, count_onepercent);
  	  printf("last_best = %d, last_1pc = %d\n", last_best, last_1pc);
  	  getchar(); */

	cycle_delete(&temp_c);
	return NULL;
}


