#ifndef SET_H_
#define SET_H_

#include <stdlib.h>
#include <stdio.h>
#include "../base/global.h"

/**
* @brief    A partition of integers 0, 1, ..., n-1 into
* 			some number of subsets. Two different subsets
* 			can be joined into one single subset, and for
* 			each integer, it is possible to determine the
* 			subset to which it belongs.
*/
typedef struct _set {

	int num;	/* number of integers */
	int *list;	/* name or id of the list (the subset)
				 * to which each integer belongs; the first
				 * integer of the list is chosen as the
				 * representative of the list, i.e. its id */
	int *last;	/* last integer in the list */
	int *size;	/* size of the list */
	int *next;	/* next integer in the list */


} set;


/**
* @brief		Initialize a set structure.
* @param[in] s	set
* @return   	= 0 if no error
* 				< 0 if error
* @pre			-
* @post			-
* @note			-
*/
int set_init(set *s);


/**
* @brief		Delete a set.
* @param[in] s	set
* @return   	= 0 if no error
* 				< 0 if error
* @pre			s is supposed to be initialized
* @post			-
* @note			-
*/
int set_delete(set *s);


int set_empty(set *s);


/**
* @brief		Setup a set, allocate memory
*           	space for it.
* @param[in] s	set
* @param[in] n  number of integers
* @return   	= 0 if no error
* 				< 0 if error
* @pre      	s is supposed to be initialized
* @pre      	n >= 1
* @post			-
* @note			-
*/
int set_setup(set *s, int n);


/**
* @brief			Copy a set.
* @param[in] from	set to be copied
* @param[in] to  	new copy
* @return   		= 0 if no error
* 					< 0 if error
* @pre      		from is supposed to be initialized
* @pre      		to is supposed to be initialized
* @post				-
* @note				-
*/
int set_copy(set *from, set *to);



/**
* @brief			Get the id of the subset or list
*					to which an integer belongs.
* @param[in]	s	set
* @param[in]	z	integer
* @param[out]	x	id of the subset
* @return			= 0 if no error
* 					< 0 if error
* @pre      		s is supposed to be initialized
* @pre      		0 <= z < num
* @post			-
* @note			-
*/
int set_find(set *s, int z, int *x);


/**
* @brief		Join two different subsets.
* @param[in] s	set
* @param[in] x  id of the first subset
* @param[in] x  id of the second subset
* @return   	= 0 if no error
* 				< 0 if error
* @pre      	s is supposed to be initialized
* @pre      	x and y are supposed to be valid ids,
* 				and they should have been obtained
* 				using the find operation.
* @post			-
* @note			-
*/
int set_union(set *s, int x, int y);


#endif /* SET_H_ */
