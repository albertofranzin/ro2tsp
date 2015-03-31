#ifndef RADIX_SORT_H_
#define RADIX_SORT_H_


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../base/global.h"

int radix_sort(int *input_edges, int *output_edges, int num_edges, int* weights, int wmin, int wmax, int b);

#endif /* RADIX_SORT_H_ */
