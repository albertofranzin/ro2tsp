#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_


#include <stdlib.h>
#include <stdio.h>
#include "../base/global.h"
#include "../data/cycle.h"
#include "../data/graph.h"
#include "../data/coords.h"
#include "../data/tree.h"


/**
* @brief    Problem environment.
*/
typedef struct _environment {

	coords 	vertices;		/* coordinates of the vertices */
	graph 	init_graph;		/* initial graph */
	cycle	init_tour;		/* initial hamiltonian cycle */
	tree	init_1t;		/* initial 1-tree */
	double	init_ub;		/* initial upper bound */
	double	init_lb;		/* initial lower bound */

	graph	curr_graph;		/* current graph */
	cycle	curr_tour;		/* current hamiltonian cycle */
	tree	curr_1t;		/* current 1-tree */

	cycle	best_tour;		/* best hamiltonian cycle found */
	tree	best_1t;		/* best 1-tree found */
	double	best_ub;		/* best upper bound */
	double	best_lb;		/* best lower bound */

} environment;


/**
* @brief			Initialize an environment structure.
* @param[in] env	environment
* @return  	 		= 0 if no error
* 					< 0 if error
* @pre				-
* @post				-
* @note				-
*/
int environment_init(environment *env);


/**
* @brief			Delete an environment.
* @param[in] env	environment
* @return  		 	= 0 if no error
* 					< 0 if error
* @pre				env is supposed to be initialized
* @post				-
* @note				-
*/
int environment_delete(environment *env);


#endif /* ENVIRONMENT_H_ */
