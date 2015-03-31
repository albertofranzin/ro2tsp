#ifndef PARAMETERS_H_
#define PARAMETERS_H_


#include <stdlib.h>
#include <stdio.h>
#include "../base/global.h"


/**
* @brief    Parameters of the problem.
*/
typedef struct _parameters {

	int				random_instance_option;	/* whether to generate a random instance */
	int				random_seed_option;		/* whether to use a pseudorandom seed */
	unsigned int	num_vertices;			/* number of vertices of the graph */
	long			seed;					/* seed */

	int				tsplib_instance_option;	/* whether to use a TSPLIB instance */
											/* (TSP-formatted file) */
	char			*tsplib_file_path;			/* TSPLIB path/filename */
	int				tsplib_file_format;		/* TSPLIB file format */
	int				tsplib_matrix_type;		/* TSPLIB matrix type */

	unsigned int	heur_algo;				/* chosen heuristic algorithm */
	unsigned int	num_heur_trials;		/* number of trials when using */
											/* the heuristic algorithm */
	unsigned int	solver;					/* chosen solver */
	unsigned int	num_threads;			/* number of threads */

	/* cplex */

	int				callbacks_option;		/* whether to use callbacks */
	int				proximity_option;
	int				localbranching_option;
	int				hardfixing_option;
	int				rinspolishing_option;

	int 			verbosity;

} parameters;


/**
* @brief			Initialize a parameters structure.
* @param[in] pars	parameters
* @return  	 		= 0 if no error
* 					< 0 if error
* @pre				-
* @post				-
* @note				-
*/
int parameters_init(parameters *pars);


/**
* @brief			Delete a parameters.
* @param[in] pars	parameters
* @return   		= 0 if no error
* 					< 0 if error
* @pre				pars is supposed to be initialized
* @post				-
* @note				-
*/
int parameters_delete(parameters *pars);


#endif /* PARAMETERS_H_ */
