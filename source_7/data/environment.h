#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_


#include <stdlib.h>
#include <stdio.h>
#include "../base/global.h"

#include "../data/coords.h"
#include "../data/graph.h"
#include "../data/tree.h"

/**
* @brief    Problem environment.
*/
typedef struct _environment {

	coords 	vertices;		/* coordinates of the vertices */
	graph 	main_graph;		/* initial graph */
	tree	global_1t;		/* best 1-tree (tour) */
	double  global_ub;		/* best upper-bound */
	double	global_lb;		/* best lower-bound */

	double 	gen_step;		/* base step to be used in general ascent */

	int		 num_rednodes;	/* number of active red-nodes */
	int 	 redindex;		/* deepest red-node index */
	double 	*redlb;			/* red-lower-bound associated to each red-node */
	tree 	*redtrees;		/* red-treees associated to each red-node */
	int		*rededges;		/* list of red-edges */
	int		*redcstrs;		/* list of constraints of red-edges
							 * before their are forbidden */
	int		*redpreds;		/* list of predecessors of red--edges
							 * before their removal from edgelist */
	int		*redbegin;		/* begin index of a set of red-edges in the list;
	 	 	 	 	 	 	 * each set is associated to a red-node */

	double	*genascent_mults;

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
