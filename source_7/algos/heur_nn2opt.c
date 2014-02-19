#include "../algos/heur_nn2opt.h"

int heur_nn2opt(graph *g, cycle *best_c, double *best_ub) {

    int n = g->vrtx_num;
    int i;
    double ccost;

    cycle temp_c;
    cycle_init(&temp_c);
    cycle_setup(&temp_c, n);


    for (i = 0; i < n; i++) {

    	heur_nn(g, &temp_c, i, &ccost);
    	heur_2opt(g, &temp_c, &ccost);
        //getchar();
        printf("2opt : %d/%d\n",i+1, n);

    	if (i == 0 || ccost < *best_ub) {
    		//printf("update! ub = %.2f\n", *best_ub);
    		*best_ub = ccost;
    		cycle_copy(&temp_c, best_c);
    	}

    }

    return 0;
}
