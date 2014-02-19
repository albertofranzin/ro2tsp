#include "../algos/compute_ub.h"

int compute_ub(graph *g, int algo, cycle *best_c, double *best_ub, int *ones, int *zeros) {

	int n = g->vrtx_num;
	int i;
	double ub;
	cycle temp_c;
	cycle_init(&temp_c);

	if (algo == NN) {
		for (i = 0; i < n; i++) {
			heur_nn(g, &temp_c, i, &ub);
			if (i == 0 || ub < *best_ub) {
				*best_ub = ub;
				cycle_copy(&temp_c, best_c);
			}
		}
	}

	if (algo == RC) {
		for (i = 0; i < NUM_TRIALS_RC; i++) {
			srand(i);
			heur_rc(g, &temp_c, &ub);
			if (i == 0 || ub < *best_ub) {
				*best_ub = ub;
				cycle_copy(&temp_c, best_c);
			}
		}
	}

	if (algo == NN2OPT) {
		heur_nn2opt(g, best_c, best_ub);
	}

	if (algo == RC2OPT) {
		heur_rc2opt(g, best_c, best_ub, ones, zeros);
	}

	if (algo == RC23OPT) {
		heur_rc2opt(g, best_c, best_ub, ones, zeros);
	    heur_3opt(g, best_c, best_ub);

	}

	cycle_delete(&temp_c);

	return 0;
}


