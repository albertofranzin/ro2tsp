#include <stdio.h>
#include <stdlib.h>
#include "./base/global.h"
#include "./data/graph.h"
#include "./data/tree.h"
#include "./data/coords.h"
#include "./base/plot.h"
#include "./data/parameters.h"
#include "./data/environment.h"
#include "./base/setup_parameters.h"
#include "./base/setup_problem.h"
#include "./algos/prim_mst.h"
#include "./algos/prim_1tree.h"
#include "./algos/lagrange.h"

int verbosity = SILENT; /* amount of infos when running the code */

int main() {

	printf("Welcome to ro2tsp project!\n");

	parameters pars;
	environment env;

	parameters_init(&pars);
	environment_init(&env);

	setup_parameters_default(&pars);

	setup_parameters_config(CONFIG_FILE, &pars);

	setup_problem_tsplib(&pars, &env);

	//plot_graph(&(env.init_graph), &(env.vertices), NULL);

	tree mst;
	tree_init(&mst);

	/*
	graph_set_edge_cstr(&(env.init_graph), 30, 22, FORCED);
	graph_set_edge_cstr(&(env.init_graph), 22, 0, FORCED);
	graph_set_edge_cstr(&(env.init_graph), 30, 21, FORCED);
	graph_set_edge_cstr(&(env.init_graph), 0, 21, FORCED);
	 */

	//prim_1tree(&(env.init_graph), &mst);

	//plot_tree(&mst, &(env.vertices), NULL);

	double lb;
	tree m1tree;
	tree_init(&m1tree);
	lagrange(&(env.init_graph), 42100, 50000, 100, 30, &m1tree, &lb);


	plot_tree(&m1tree, &(env.vertices), NULL);

	printf("End\n");

	return EXIT_SUCCESS;

}
