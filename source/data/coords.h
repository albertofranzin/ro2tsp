#ifndef COORDS_H_
#define COORDS_H_


#include <stdlib.h>
#include <stdio.h>
#include "../base/global.h"


typedef struct _coords {

	int 	num;	/* number of couples of 2D-coordinates */
	double	*x;		/* list of x-coordinates */
	double	*y;		/* list of y-coordinates */

} coords;


/**
* @brief		Initialize a coords structure.
* @param[in] c	coords
* @return   	= 0 if no error
* 				< 0 if error
* @pre			-
* @post			-
* @note			-
*/
int coords_init(coords *c);


/**
* @brief		Delete coords.
* @param[in] c	coords
* @return   	= 0 if no error
* 				< 0 if error
* @pre			c is supposed to be initialized
* @post			-
* @note			-
*/
int coords_delete(coords *c);


/**
* @brief		Setup coords, allocate memory
*           	space for it.
* @param[in] v	coords
* @param[in] n  number of coords
* @return   	= 0 if no error
* 				< 0 if error
* @pre      	v is supposed to be initialized
* @pre      	n >= 1
* @post			-
* @note			-
*/
int coords_setup(coords *c, int n);


/**
* @brief        	Copy points.
* @param[in]  from	coords to be copied
* @param[out] to	copied coords
* @return   		= 0 if no error
* 					< 0 if error
* @pre		  		from is supposed to be initialized
* @pre		  		to is supposed to be initialized
* @post				-
* @note				-
*/
int coords_copy(coords *from, coords *to);


#endif /* COORDS_H_ */
