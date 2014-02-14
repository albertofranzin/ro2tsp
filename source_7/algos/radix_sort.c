#include "../algos/radix_sort.h"


int radix_sort(int *input_edges, int *output_edges, int num_edges, int* weights, int wmin, int wmax, int b) {


	int i, j, p, temp, total;
	int k 		= wmax - wmin + 1; 				/* keys 0, 1, ..., k - 1 */
	int d 		= (int)ceil(log2(k) / log2(b)); /* max number of digits using base b notation */
	int *count 	= (int*)malloc(b * sizeof(int));
	int *pt;

	p = 1;
	for (i = 1; i <= d; i++) {

		memset(count, '\0', b * sizeof(int));

		for (j = 0; j < num_edges; j++) {
			count[ ((weights[input_edges[j]] - wmin) / p) % b ] += 1;
		}

		total = 0;
		for (j = 0; j < b; j++) {
			temp 		= count[j];
			count[j] 	= total;
			total	   += temp;
		}

		for (j = 0; j < num_edges; j++) {
			output_edges[ count[ ((weights[input_edges[j]] - wmin) / p) % b ] ] = input_edges[j];
						  count[ ((weights[input_edges[j]] - wmin) / p) % b ]  += 1;
		}


		pt 				= input_edges;
		input_edges		= output_edges;
		output_edges	= pt;


		p = p * b;

	}

	if (d % 2 == 0) {
		for (i = 0; i < num_edges; i++) {
			output_edges[i] = input_edges[i];
		}
	}

	free(count);
	return 0;

}
