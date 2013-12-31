#include "../data/environment.h"


int environment_init(environment *env) {

	coords_init(&(env->vertices));

	graph_init(&(env->init_graph));
	graph_init(&(env->curr_graph));

	cycle_init(&(env->init_tour));
	cycle_init(&(env->curr_tour));
	cycle_init(&(env->best_tour));

	tree_init(&(env->init_1t));
	tree_init(&(env->curr_1t));
	tree_init(&(env->best_1t));

	env->init_ub = -1.0;
	env->init_lb = -1.0;
	env->best_ub = -1.0;
	env->best_lb = -1.0;
	return 0;

}


int environment_delete(environment *env) {

	coords_delete(&(env->vertices));

	graph_delete(&(env->init_graph));
	graph_delete(&(env->curr_graph));

	cycle_delete(&(env->init_tour));
	cycle_delete(&(env->curr_tour));
	cycle_delete(&(env->best_tour));

	tree_delete(&(env->init_1t));
	tree_delete(&(env->curr_1t));
	tree_delete(&(env->best_1t));

	env->init_ub = -1.0;
	env->init_lb = -1.0;
	env->best_ub = -1.0;
	env->best_lb = -1.0;
	return 0;

}
