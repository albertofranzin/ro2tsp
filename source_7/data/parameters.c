#include "../data/parameters.h"


int parameters_init(parameters *pars) {

	pars->random_instance_option	= -1;
	pars->random_seed_option		= -1;
	pars->num_vertices				= -1;
	pars->seed						= -1;

	pars->tsplib_instance_option	= -1;
	pars->tsplib_file_path			= NULL;
	pars->tsplib_file_format		= -1;
	pars->tsplib_matrix_type		= -1;

	pars->heur_algo					= -1;
	pars->num_heur_trials			= -1;

	pars->solver					= -1;
	pars->num_threads				= -1;
	pars->callbacks_option			= -1;
	return 0;

}


int parameters_delete(parameters *pars) {

	pars->random_instance_option	= -1;
	pars->random_seed_option		= -1;
	pars->num_vertices				= -1;
	pars->seed						= -1;

	pars->tsplib_instance_option	= -1;
	if (pars->tsplib_file_path!= NULL) {
		free(pars->tsplib_file_path);
		pars->tsplib_file_path = NULL;
	}
	pars->tsplib_file_format		= -1;
	pars->tsplib_matrix_type		= -1;

	pars->heur_algo					= -1;
	pars->num_heur_trials			= -1;

	pars->solver					= -1;
	pars->num_threads				= -1;
	pars->callbacks_option			= -1;
	return 0;

}
