#ifndef STATISTICS_H_
#define STATISTICS_H_


#include <stdlib.h>
#include <stdio.h>
#include "../base/global.h"

/**
* @brief    Problem statistics.
*/
typedef struct _statistics {

	int bb_nod_num; /* number of nodes */
	int bb_lev_num; /* deepest level */
	int reduce_nr; 	/* number of removed edges */

} statistics;


/**
* @brief			Initialize a statistics structure.
* @param[in] stats	statistics
* @return  	 		= 0 if no error
* 					< 0 if error
* @pre				-
* @post				-
* @note				-
*/
int statistics_init(statistics *stats);


/**
* @brief			Delete a statistics.
* @param[in] stats	statistics
* @return   		= 0 if no error
* 					< 0 if error
* @pre				stats is supposed to be initialized
* @post				-
* @note				-
*/
int statistics_delete(statistics *stats);


#endif /* STATISTICS_H_ */
