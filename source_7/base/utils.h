#ifndef UTILS_H_
#define UTILS_H_


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../base/global.h"


extern int* idx_to_v1;
extern int* idx_to_v2;


typedef struct _pair {

	int		idx;
	double	val;

} pair;


int merge(int *a1, int n1, int *a2, int n2, int *a, double *w);


/**
* @brief		Comparator function, it compares two
* 				pairs (idx, val).
* @param[in]	a first pair
* @param[in]	b second pair
* @return  		1  if a.val > b.val
* 				0  if a.val = b.val
* 				-1 if a.val < b.val
* @pre			p1 and p2 are assumed to be pairs
* @post					-
* @note					-
*/
int compare_pairs(const void *p1, const void *p2);

int compare_ints(const void *i1, const void *i2);

void setup_convtable(int n);

void delete_convtable(void);

int get_idx(int u, int v);

int get_v1(int e);

int get_v2(int e);

int mergelist(int *l1, int n1, int *l2, int n2, int *l, int *n, double *w);

/**
* @brief				Map each valid combination of parameter
* 						name and value (parName, parValue)
* 						into a unique integer code.
* @param[in] parName	parameter name
* @param[in] parValue	parameter value
* @return   			the code associated to the couple, if no error
* 						< 0 if error
* 						parameter name and value (parName, parValue)
* @pre					-
* @post					-
* @note					-
*/
short par_hash(char* parName, char* parValue);


/**
* @brief				Map each valid combination of parameter
* 						name and value (parName, parValue)
* 						into a unique integer code.
* @param[in] parName	parameter name
* @param[in] parValue	parameter value
* @return   			the code associated to the couple, if no error
* 						< 0 if error
* 						parameter name and value (parName, parValue)
* @pre					-
* @post					-
* @note					-
*/
short tsp_hash(char* parName, char* parValue);


#endif /* UTILS_H_ */
