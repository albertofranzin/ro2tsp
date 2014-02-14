#ifndef COUNTING_SORT_H_
#define COUNTING_SORT_H_


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../base/global.h"

int counting_sort(int *input_edges, int *output_edges, int num_edges, double* weights, double wmin, double wmax);

int counting_sort_2pass(int *input_edges, int *output_edges, int num_edges, int* weights, int wmin, int wmax, int block);

#endif /* COUNTING_SORT_H_ */
