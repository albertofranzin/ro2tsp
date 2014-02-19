#include "../algos/counting_sort.h"


int counting_sort(int *input_edges, int *output_edges, int num_edges, double* weights, double wmin, double wmax) {

	printf("initialize\n");
	int i, temp, total;
	int  num_keys 	= (int)(wmax - wmin + 1.0);
	int *count_keys = (int*)calloc(num_keys, sizeof(int));

	printf("first step\n");
	for (i = 0; i < num_edges; i++) {
		count_keys[ (int)(weights[input_edges[i]] - wmin) ] += 1;
	}

	printf("second step\n");
	total 		= 0;
	for (i = 0; i < num_keys; i++) {
		temp 			= count_keys[i];
		count_keys[i] 	= total;
		total	   	   += temp;
	}

	printf("third step\n");
	for (i = 0; i < num_edges; i++) {
		output_edges[ count_keys[ (int)(weights[input_edges[i]] - wmin) ] ] = input_edges[i];
					  count_keys[ (int)(weights[input_edges[i]] - wmin) ]  += 1;
	}

	free(count_keys);
	printf("exit\n");
	return 0;

}


int counting_sort_2pass(int *input_edges, int *output_edges, int  num_edges, int* weights, int wmin, int wmax, int block) {

	int i, j, temp, total, start, end;
	int  num_big_keys 		= (int)ceil((double)(wmax - wmin + 1) / block);
	int  num_small_keys 	= block;
	int *start_points 		= (int*)malloc(num_big_keys * sizeof(int));
	int *big_key_count		= (int*)calloc(num_big_keys, sizeof(int));
	int *small_key_count	= (int*)malloc(num_small_keys * sizeof(int));
	int *temp_edges			= (int*)malloc(num_edges * sizeof(int));

	for (i = 0; i < num_edges; i++) {
		big_key_count[ (weights[input_edges[i]] - wmin) / block ] += 1;
	}

	total 		= 0;
	for (i = 0; i < num_big_keys; i++) {
		temp 				= big_key_count[i];
		big_key_count[i] 	= total;
		start_points[i]		= total;
		total	   			+= temp;
	}

	for (i = 0; i < num_edges; i++) {
		temp_edges[ big_key_count[ (weights[input_edges[i]] - wmin) / block ] ] = input_edges[i];
					big_key_count[ (weights[input_edges[i]] - wmin) / block ]  += 1;
	}

	for (i = 0; i < num_big_keys; i++) {

		start	= start_points[i];
		end		= (i  < num_big_keys - 1) ? start_points[i+1] : num_edges;

		if (start == end) continue;

		memset(small_key_count, '\0', num_small_keys * sizeof(int));

		for (j = start; j < end; j++) {
			small_key_count[ (weights[temp_edges[j]] - wmin) % block ] += 1;
		}

		total = 0;
		for (j = 0; j < num_small_keys; j++) {
			temp 				= small_key_count[j];
			small_key_count[j]	= total;
			total 			   += temp;
		}

		for (j = start; j < end; j++) {
			output_edges[ start + small_key_count[ (weights[temp_edges[j]] - wmin) % block ] ] = temp_edges[j];
						          small_key_count[ (weights[temp_edges[j]] - wmin) % block ]  += 1;
		}
	}

	free(start_points);
	free(big_key_count);
	free(small_key_count);
	free(temp_edges);
	return 0;

}
