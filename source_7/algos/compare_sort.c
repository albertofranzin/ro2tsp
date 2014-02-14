#include "../algos/compare_sort.h"


int compare_sort(int *input_edges, int *output_edges, int num_edges, double* weights) {

	int i;

	pair *edges = (pair*)malloc(num_edges * sizeof(pair));
	for (i = 0; i < num_edges; i++) {
		edges[i].idx = input_edges[i];
		edges[i].val = weights[input_edges[i]];
	}

	qsort(edges, num_edges, sizeof(pair), compare_pairs);

	for (i = 0; i < num_edges; i++) {
		output_edges[i] = edges[i].idx;
	}

	return 0;

}
