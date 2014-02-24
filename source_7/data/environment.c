#include "../data/environment.h"


int environment_init(environment *env) {

	coords_init(&(env->vertices));
	graph_init(&(env->main_graph));
	tree_init(&(env->global_1t));
	env->global_ub 	= -1.0;
	env->global_lb 	= -1.0;
	env->gen_step	= -1.0;

	env->num_rednodes	= -1;
	env->redindex 		= -1;
	env->redlb 			= NULL;
	env->redtrees 		= NULL;
	env->rededges 		= NULL;
	env->redcstrs 		= NULL;
	env->redpreds 		= NULL;
	env->redbegin 		= NULL;

	env->genascent_mults = NULL;

	/* cplex */

	env->mylp			= NULL;

	env->start_time		= 0;

	return 0;
}


int environment_delete(environment *env) {

	coords_init(&(env->vertices));
	graph_init(&(env->main_graph));
	tree_init(&(env->global_1t));
	env->global_ub	= -1.0;
	env->global_lb	= -1.0;
	env->gen_step	= -1.0;

	env->num_rednodes	= -1;
	env->redindex 		= -1;
	env->redlb 			= NULL;
	env->redtrees 		= NULL;
	env->rededges 		= NULL;
	env->redcstrs 		= NULL;
	env->redpreds 		= NULL;
	env->redbegin 		= NULL;

	env->genascent_mults = NULL;

	/* cplex */

	env->mylp			= NULL;

	env->start_time		= 0;


	return 0;
}
