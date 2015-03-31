#ifndef CYCLE_H_
#define CYCLE_H_
#include <stdlib.h>
#include <stdio.h>
#include "../base/global.h"

typedef struct _cycle {
	int 	vrtx_num;	/* number of distinct vertices in the cycle; */
						/* cycle closes on first node, without need */
						/* to store it twice */
	int		*vrtx_idx;	/* list of vertex indexes */

} cycle;


/**
* @brief		Initialize a cycle structure.
* @param[in] c	cycle
* @return   	= 0 if no error
* 				< 0 if error
* @pre			-
* @post			-
* @note			-
*/
int cycle_init(cycle *c);


/**
* @brief		Delete a cycle.
* @param[in] c	cycle
* @return   	= 0 if no error
* 				< 0 if error
* @pre			c is supposed to be initialized
* @post			-
* @note			-
*/
int cycle_delete(cycle *c);


/**
* @brief		Setup a cycle, allocate memory
*           	space for it.
* @param[in] c	cycle
* @param[in] n  number of vertices
* @return   	= 0 if no error
* 				< 0 if error
* @pre      	c is supposed to be initialized
* @pre      	n >= 1
* @post			-
* @note			-
*/
int cycle_setup(cycle *c, int n);


/**
* @brief        	Copy a cycle.
* @param[in]  from	cycle to be copied
* @param[out] to	copied cycle
* @return   		= 0 if no error
* 					< 0 if error
* @pre		  		from is supposed to be initialized
* @pre		  		to is supposed to be initialized
* @post				-
* @note				-
*/
int cycle_copy(cycle *from, cycle *to);


#endif /* CYCLE_H_ */
