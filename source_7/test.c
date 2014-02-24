#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "./base/global.h"

#include "./data/arraylist.h"
#include "./data/coords.h"
#include "./data/environment.h"
#include "./data/graph.h"
#include "./data/cycle.h"
#include "./data/list.h"
#include "./data/parameters.h"
#include "./data/statistics.h"
#include "./data/tree.h"

#include "./base/plot.h"
#include "./base/setup_parameters.h"
#include "./base/setup_problem.h"
#include "./base/utils.h"

#include "./algos/counting_sort.h"
#include "./algos/radix_sort.h"
#include "./algos/reduce.h"

#include "./algos/pr_mst.h"
#include "./algos/kr_mst.h"

#include "./algos/pr_onetree.h"
#include "./algos/kr_onetree.h"

#include "./algos/kr_lagrange_vj.h"
#include "./algos/pr_lagrange_hk.h"
#include "./algos/pr_lagrange_vj.h"

#include "./algos/heur_nn.h"
#include "./algos/heur_rc.h"
#include "./algos/heur_2opt.h"
#include "./algos/heur_3opt.h"
#include "./algos/heur_nn2opt.h"
#include "./algos/heur_rc2opt.h"
#include "./algos/compute_ub.h"


#include "./solvers/bb/pr_bb.h"
#include "./solvers/bb/kr_bb.h"

#include "./solvers/cpx/cpx_solve_iterative.h"

int* idx_to_v1	= NULL;
int* idx_to_v2	= NULL;
int verbosity	= SILENT;

int main(int argc, char **argv) {

	parameters pars;
	parameters_init(&pars);

	environment env;
	environment_init(&env);

	statistics stats;
	statistics_init(&stats);

	setup_parameters_default(&pars);
	
	setup_parameters_config(CONFIG_FILE, &pars);
	setup_parameters_commandline(argc, argv, &pars);

	setup_problem_tsplib(&pars, &env);

	/* START CPLEX */


	cpx_solve_iterative(&env, &pars, &stats);
	plot_tree(&(env.global_1t), &(env.vertices), NULL);
	exit(1);

	/* END CPLEX */

	//double opt = 3323;	// burma14x*
	//double opt = 6859;	// ulysses16
	//double opt = 7013;	// ulysses22
	//double opt = 10628;	// att48
	//double opt = 1610;	// bayg29
	//double opt = 2020;	// bays29
	//double opt = 55209;	// gr96
	//double opt = 2085;	// gr17
	//double opt = 2707;	// gr21
	//double opt = 1272;	// gr24
	//double opt = 5046;	// gr48
	//double opt = 11461;	// hk48
	//double opt = 937;		// fri26
	//double opt = 25395;	// brazil58x
	//double opt = 699;		// dantzig42
	//double opt = 1273;	// swiss42x
	//double opt = 7542;	// berlin52
	//double opt = 426; 	// eil51
	//double opt = 675; 	// st70
	//double opt = 538; 	// eil76
	//double opt = 108159; 	// pr76
	//double opt = 1211; 	// rat99
	//double opt = 7910; 	// rd100
	//double opt = 21282; 	// kroA100x
	//double opt = 22141; 	// kroB100x
	//double opt = 20749;	// kroC100x
	//double opt = 21294; 	// kroD100x
	//double opt = 22068; 	// kroE100x
	//double opt = 629; 	// eil101
	//double opt = 14379; 	// lin105
	//double opt = 44303; 	// pr107
	//double opt = 59030;	// pr124
	//double opt = 118282;	// bier127x
	//double opt = 6110;	// ch130
	//double opt = 96772;	// pr136
	//double opt = 69853; // gr137
	//double opt = 58537;	// pr144
	//double opt = 6528;	// ch150
	//double opt = 26524;	// kroA150
	//double opt = 26130;	// kroB150
	//double opt = 73682;	// pr152
	//double opt = 42080;	// u159
	//double opt = 2323;	// rat195
	//double opt = 15780;	// d198
	double opt = 40160;// gr202
	//double opt = 3919;	// tsp225
	//double opt = 126643;	// ts225
	//double opt = 80369;	// pr226
	//double opt = 42029;	// lin318
	//double opt = 35002;	// d493
	//double opt = 36905; 	// u574
	//double opt = 6773;	// rat575
	//double opt = 34643;	// p654
	//double opt = 259045;	// pr1002
	//double opt = 224094;	// u1060

	/*
	int n			= pars.num_vertices;
	int i;

	// **
	int *ones	= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));
	int *zeros	= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));

	double temp_ub, best_ub;

	clock_t t1, t2;

	cycle best_c;
	cycle_init(&best_c);
	cycle temp_c;
	cycle_init(&temp_c);
	t1 = clock();
	compute_ub(&(env.main_graph), NN, &best_c, &best_ub, ones, zeros);
	t2 = clock();
	//plot_cycle(&best_c, &(env.vertices), NULL);
	printf("%.2f %.2f\n", best_ub, (double)(t2-t1)/CLOCKS_PER_SEC);
	free(ones);
	free(zeros);

	// ***********************************

	ones	= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));
	zeros	= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));

	//cycle best_c;
	cycle_init(&best_c);
	//cycle temp_c;
	cycle_init(&temp_c);
	t1 = clock();
	compute_ub(&(env.main_graph), NN2OPT, &best_c, &best_ub, ones, zeros);
	t2 = clock();
	//plot_cycle(&best_c, &(env.vertices), NULL);
	printf("%.2f %.2f\n", best_ub, (double)(t2-t1)/CLOCKS_PER_SEC);
	free(ones);
	free(zeros);

	// ***********************************

	ones	= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));
	zeros	= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));

	//cycle best_c;
	//cycle_init(&best_c);
	//cycle temp_c;
	//cycle_init(&temp_c);
	t1 = clock();
	//compute_ub(&(env.main_graph), NN23OPT, &best_c, &best_ub, ones, zeros);
	heur_3opt(&(env.main_graph), &best_c, &best_ub);
	t2 = clock();
	//plot_cycle(&best_c, &(env.vertices), NULL);
	printf("%.2f %.2f\n", best_ub, (double)(t2-t1)/CLOCKS_PER_SEC);
	free(ones);
	free(zeros);

	// **************************

	// ones	= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));
	// zeros	= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));


	// //cycle best_c;
	// cycle_init(&best_c);
	// //cycle temp_c;
	// cycle_init(&temp_c);
	// t1 = clock();
	// //compute_ub(&(env.main_graph), RC, &best_c, &best_ub, ones, zeros);
	// t2 = clock();
	// //plot_cycle(&best_c, &(env.vertices), NULL);
	// //printf("%.2f %.2f\n", best_ub, (double)(t2-t1)/CLOCKS_PER_SEC);
	// free(ones);
	// free(zeros);

	// ***********************************

	ones	= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));
	zeros	= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));

	//cycle best_c;
	cycle_init(&best_c);
	//cycle temp_c;
	cycle_init(&temp_c);
	t1 = clock();
	compute_ub(&(env.main_graph), RC2OPT, &best_c, &best_ub, ones, zeros);
	t2 = clock();
	//plot_cycle(&best_c, &(env.vertices), NULL);
	printf("%.2f %.2f\n", best_ub, (double)(t2-t1)/CLOCKS_PER_SEC);
	free(ones);
	free(zeros);

	// **************************/

	int i, n			= pars.num_vertices;
	double best_ub;
	int *ones	= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));
	int *zeros	= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));
	clock_t t1 = 0, t2 = 0;

	cycle best_c;
	cycle_init(&best_c);
	cycle temp_c;
	cycle_init(&temp_c);

	//t1 = clock();
	compute_ub(&(env.main_graph), RC23OPT, &best_c, &best_ub, ones, zeros);
	//heur_3opt(&(env.main_graph), &best_c, &best_ub);
	//t2 = clock();
	//plot_cycle(&best_c, &(env.vertices), NULL);
	printf("%.2f %.2f\n", best_ub, (double)(t2-t1)/CLOCKS_PER_SEC);

	int num_edges = n * (n-1) / 2;
	int ksjdfh = 0;
	for (i = 0 ; i < num_edges ; i++) {
		if (zeros[i] == 1){
			graph_set_edge_cstr(&(env.main_graph), i, FORBIDDEN);
			ksjdfh++;
		}
		else if (ones[i] == 1) {
			graph_set_edge_cstr(&(env.main_graph), i, FORCED);
			ksjdfh++;
		}
	}
	printf("removed by zeros and ones %d\n", ksjdfh);
	//getchar();
	//free(ones);
	//free(zeros);

	//exit(1);

	// **
	/**/

	//int i;

	int st;
	double ub		= best_ub;//40885; //ceil(opt * 1.01);
	//double ub		= ceil(opt * 1.01);
	env.global_ub	= ub; //40885; //ceil(opt * 1.01);
	//env.global_ub	= ceil(opt * 1.01);

	clock_t start, end;

	double time_interval;

	/* sort edges */

	arraylist edgelist;
	arraylist_init(&edgelist);
	arraylist_setup(&edgelist, (n * (n - 1)) / 2);

	int //num_edges, 
	wmin, wmax;
	int *unsrt_edges	= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));
	int *srt_edges		= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));
	int *weights		= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));

	num_edges = 0;
	wmin = wmax = (int)rint((env.main_graph).edge_cost[0]);
	for (i = 0; i < (n * (n - 1)) / 2; i++) {
		if (env.main_graph.edge_cstr[i] == FREE) {
			unsrt_edges[num_edges++]	= i;
			weights[i]		= (int)rint((env.main_graph).edge_cost[i]);
			if (weights[i] < wmin) wmin = weights[i];
			if (weights[i] > wmax) wmax = weights[i];
		}
	}
	counting_sort(unsrt_edges, srt_edges, num_edges, weights, wmin, wmax);

	int tail = edgelist.capacity;
	for (i = 0; i < num_edges; i++) {
		arraylist_insert_pred(&edgelist, tail, srt_edges[i]);
	}

	/* compute 1-tree */

	double  prhk_lb, prvj_lb, prvjgen_lb, krvj_lb, krvjgen_lb, best_lb;

	tree prhk_1t;
	tree_init(&prhk_1t);
	tree_setup(&prhk_1t, n);

	tree prvj_1t;
	tree_init(&prvj_1t);
	tree_setup(&prvj_1t, n);

	tree prvjgen_1t;
	tree_init(&prvjgen_1t);
	tree_setup(&prvjgen_1t, n);


	tree krvj_1t;
	tree_init(&krvj_1t);
	tree_setup(&krvj_1t, n);

	tree krvj_part_1t;
	tree_init(&krvj_part_1t);
	tree_setup(&krvj_part_1t, n);

	set krvj_part_vs;
	set_init(&krvj_part_vs);
	set_setup(&krvj_part_vs, n);

	tree krvjgen_1t;
	tree_init(&krvjgen_1t);
	tree_setup(&krvjgen_1t, n);

	tree krvjgen_part_1t;
	tree_init(&krvjgen_part_1t);
	tree_setup(&krvjgen_part_1t, n);

	set krvjgen_part_vs;
	set_init(&krvjgen_part_vs);
	set_setup(&krvjgen_part_vs, n);


	tree best_1t;
	tree_init(&best_1t);
	tree_setup(&best_1t, n);


	double *prhk_mults	= (double*)malloc(n * sizeof(double));
	double *prvj_mults	= (double*)malloc(n * sizeof(double));
	double *krvj_mults	= (double*)malloc(n * sizeof(double));
	double *best_mults	= (double*)malloc(n * sizeof(double));


	pr_lagrange_hk(&env, ub, &prhk_1t, prhk_mults, &prhk_lb, &st);

	pr_lagrange_vj(&env, INITASCENT, ub, &krvj_1t, prvj_mults, &prvj_lb, &st);

	pr_lagrange_vj(&env, GENASCENT, ub, &krvjgen_1t, prvj_mults, &prvjgen_lb, &st);

	kr_lagrange_vj(&env, INITASCENT, ub, &edgelist, &krvj_part_1t, &krvj_part_vs, &krvj_1t, krvj_mults, &krvj_lb, &st);

	kr_lagrange_vj(&env, GENASCENT, ub, &edgelist, &krvjgen_part_1t, &krvjgen_part_vs, &krvjgen_1t, krvj_mults, &krvjgen_lb, &st);

	/*printf("pr hk     lower bound = %.2f : %.2f\n", prhk_lb / opt * 100.0, prhk_lb);
	printf("pr vj     lower bound = %.2f : %.2f\n", prvj_lb / opt * 100.0, prvj_lb);
	printf("pr vj gen lower bound = %.2f : %.2f\n", prvjgen_lb / opt * 100.0, prvjgen_lb);
	printf("kr vj     lower bound = %.2f : %.2f\n", krvj_lb / opt * 100.0, krvj_lb);
	printf("kr vj gen lower bound = %.2f : %.2f\n", krvjgen_lb / opt * 100.0, krvjgen_lb);*/

	int best 	= 0;
	best_lb 	= prhk_lb;
	if (prvj_lb > best_lb) {
		best_lb	= prvj_lb;
		best 	= 1;
	}
	/**/if (krvj_lb > best_lb) {
		best_lb	= krvj_lb;
		best 	= 2;
	}

	if (best == 0) {
		tree_copy(&prhk_1t, &best_1t);
		for (i = 0; i < n; i++) {
			best_mults[i] = prhk_mults[i];
		}
		best_lb = prhk_lb;
	}/**/
	if (best == 1) {
		tree_copy(&prvj_1t, &best_1t);
		for (i = 0; i < n; i++) {
			best_mults[i] = prvj_mults[i];
		}
		best_lb = prvj_lb;
	}

	/**/if (best == 2) {
		for (i = 0; i < n; i++) {
			best_mults[i] = krvj_mults[i];
		}
		tree_copy(&krvj_1t, &best_1t);
		best_lb = krvj_lb;
	}/**/

	env.global_lb = best_lb;

	//printf("best lower bound = %.2f : %.2f\n", best_lb / opt * 100.0, best_lb);

	int num_rmvedges;
	int *rmvedges  	= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));

	tree_rooting(&best_1t, 0);
	reduce(&(env.main_graph), &best_1t, best_mults, best_lb, ub, rmvedges, &num_rmvedges);

	printf("num edges = %d : num removed edges = %d\n", (n * (n - 1)) / 2, num_rmvedges);
	printf("ratio = %.2f\n", num_rmvedges / (double)((n * (n - 1)) / 2) * 100.0);
	printf("lower bound = %.2f\n", best_lb);
	/*printf("ratio = %.2f\n", best_lb / opt * 100.00);*/

	for (i = 0; i < num_rmvedges; i++) {
		arraylist_remove(&edgelist, rmvedges[i]);
		graph_set_edge_cstr(&(env.main_graph), rmvedges[i], FORBIDDEN);
	}


	/* branch & bound */

	double gap;

	tree part_1t;
	tree_init(&part_1t);
	tree_setup(&part_1t, n);

	set part_vs;
	set_init(&part_vs);
	set_setup(&part_vs, n);

	stats.num_nodes 	= 0;
	stats.num_levels 	= 0;
	stats.curr_node 	= 0;
	stats.curr_level 	= 0;

	tree_copy(&best_1t, &(env.global_1t)); /* if lagrange 1-tree is optimal.. */

	printf("initial gap : %f%%\n", 100.0* (env.global_ub - env.global_lb)/env.global_lb);

	start = clock();
	env.start_time = start;
	kr_bb(&env, &stats, &edgelist, &part_1t, &part_vs, &gap);
	end = clock();

	time_interval = (end - start) / (double)CLOCKS_PER_SEC;
	printf("num edges           = %d\n", (n * (n - 1)) / 2);
	printf("removed             = %d\n", num_rmvedges);
	printf("ratio               = %.2f\n", num_rmvedges / (double)((n * (n - 1)) / 2) * 100.0);
	printf("initial lower bound = %.2f\n", best_lb);
	printf("initial upper bound = %.2f\n", ub);
	printf("final upper bound   = %.2f\n", env.global_ub);
	printf("time                = %.2f\n", time_interval);
	//plot_tree(&(env.global_1t), &(env.vertices), NULL);

	printf("End\n");



	parameters_delete(&pars);
	environment_delete(&env);
	statistics_delete(&stats);
	delete_convtable();


	return EXIT_SUCCESS;


}
