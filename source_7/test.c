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


int* idx_to_v1	= NULL;
int* idx_to_v2	= NULL;
int verbosity	= SILENT;

int main() {

	printf("Welcome to ro2tsp project!\n");

	/*
	int q;
	int *a = (int*)malloc(10 * sizeof(int));

	a[0] = 0;
	a[1] = 1;
	a[2] = 2;
	a[3] = 3;
	a[4] = 4;
	a[5] = 5;
	a[6] = 6;
	a[7] = 7;
	a[8] = 8;
	a[9] = 9;

	for (q = 0; q < 10; q++) {
		printf("%d\n", a[q]);
	}

	move_back(a, 0, 4, 8);

	printf("*************\n");

	for (q = 0; q < 10; q++) {
		printf("%d\n", a[q]);
	}

	exit(1);
	*/

	parameters pars;
	parameters_init(&pars);

	environment env;
	environment_init(&env);

	statistics stats;
	statistics_init(&stats);

	setup_parameters_default(&pars);

	setup_parameters_config(CONFIG_FILE, &pars);

	setup_problem_tsplib(&pars, &env);

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
	//double opt = 58537;	// pr144
	//double opt = 6528;	// ch150
	double opt = 26524;	// kroA150
	//double opt = 26130;	// kroB150
	//double opt = 73682;	// pr152
	//double opt = 42080;	// u159
	//double opt = 2323;	// rat195
	//double opt = 15780;	// d198
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


	int n			= pars.num_vertices;
	int i;
	/*
	int *ones	= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));
	int *zeros	= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));

	double temp_ub, best_ub;

	cycle best_c;
	cycle_init(&best_c);

	cycle temp_c;
	cycle_init(&temp_c);


	for (i = 0; i < n; i++) {

		srand(i);
		heur_rc(&(env.main_graph), &temp_c, &temp_ub);
		//heur_nn(&(env.main_graph), &temp_c, i, &temp_ub);
		heur_2opt(&(env.main_graph), &temp_c, &temp_ub);
		heur_3opt(&(env.main_graph), &temp_c, &temp_ub);

		printf("i = %d : temp ub = %.2f : percent = %.2f\n", i, temp_ub, (temp_ub / opt - 1.0) * 100.0);

		if (i == 0 || temp_ub < best_ub) {
			best_ub = temp_ub;
			cycle_copy(&temp_c, &best_c);
		}

	}



	//heur_2opt(&(env.main_graph), &best_c, &best_ub);
	//heur_3opt(&(env.main_graph), &best_c, &best_ub);

	compute_ub(&(env.main_graph), RC23OPT, &best_c, &best_ub, ones, zeros);

	plot_cycle(&best_c, &(env.vertices), NULL);
	printf("best ub = %.2f\n", best_ub);

	free(ones);
	free(zeros);

	exit(1);

	*/



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
	//double opt = 58537;	// pr144
	//double opt = 6528;	// ch150
	//double opt = 26524;	// kroA150
	//double opt = 26130;	// kroB150
	//double opt = 73682;	// pr152
	//double opt = 42080;	// u159
	//double opt = 2323;	// rat195
	//double opt = 15780;	// d198
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

	/* TEST: PLOT MAIN GRAPH ***************************************/
	/*
	plot_graph(&(env.main_graph), &(env.vertices), NULL);
	*/



	/* TEST: TREE ROOTING *****************************************/
	/*
	int st, i;
	int n = pars.num_vertices;

	tree my1t;
	tree_init(&my1t);
	tree_setup(&my1t, n);
	onetree_prim(&(env.main_graph), &my1t, &st);
	for (i = 0; i < n; i++) my1t.vrtx_pred[i] = -1;

	tree_rooting(&my1t, 0);
	plot_tree(&my1t, &(env.vertices), NULL);
	for (i = 0; i < n; i++) {
		printf("vertex = %d : pred = %d\n", i, my1t.vrtx_pred[i]);
	}
	printf("\n");

	tree myt;
	tree_init(&myt);
	tree_setup(&myt, n);
	mst_prim(&(env.main_graph), &myt, 0, &st);
	for (i = 0; i < n; i++) myt.vrtx_pred[i] = -1;

	tree_rooting(&myt, 0);
	plot_tree(&myt, &(env.vertices), NULL);
	for (i = 0; i < n; i++) {
		printf("vertex = %d : pred = %d\n", i, myt.vrtx_pred[i]);
	}
	printf("\n");

	tree_delete(&myt);
	tree_delete(&my1t);
	*/

	/* TEST: LAGRANGE PRIM HK *************************************/
	/*
	int i, st, nr;
	double lb, gen_step, time_interval;
	double ub		= opt * 1.05;
	int n			= pars.num_vertices;
	clock_t start, end;

	tree best1t;
	tree_init(&best1t);
	tree_setup(&best1t, n);

	start = clock();
	pr_lagrange_hk(&env, ub, 20000, 20000, 100, &best1t, &lb, &st);
	end = clock();
	time_interval = (end - start) / (double)CLOCKS_PER_SEC;

	plot_tree(&best1t, &(env.vertices), NULL);
	printf("time = %.2f\n", time_interval);

	tree_delete(&best1t);
	*/

	/* TEST: LAGRANGE PRIM VJ *************************************/
	/*
	int i, st, nr;
	double lb, gen_step, time_interval;
	int n			= pars.num_vertices;
	clock_t start, end;

	tree best1t;
	tree_init(&best1t);
	tree_setup(&best1t, n);

	start = clock();
	pr_lagrange_vj(&env, &lb, &gen_step, &best1t, &st);
	end = clock();
	time_interval = (end - start) / (double)CLOCKS_PER_SEC;

	plot_tree(&best1t, &(env.vertices), NULL);
	printf("time = %.2f\n", time_interval);

	tree_delete(&best1t);
	*/


	/* TEST: LAGRANGE KRUSKAL VJ **********************************/
	/*
	int i, st, nr;
	double lb, gen_step, time_interval;
	int n			= pars.num_vertices;
	clock_t start, end;

	tree best1t;
	tree_init(&best1t);
	tree_setup(&best1t, n);

	int num_edges 	= (n * (n - 1)) / 2;
	int *unsrtedges	= (int*)malloc(num_edges * sizeof(int));
	int *srtedges	= (int*)malloc(num_edges * sizeof(int));
	int	*weights	= (int*)malloc(num_edges * sizeof(int));
	int wmin 		= (int)(env.main_graph).edge_cost[0];
	int wmax 		= (int)(env.main_graph).edge_cost[0];

	for (i = 0; i < num_edges; i++) {
		unsrtedges[i]	= i;
		weights[i]		= (int)(env.main_graph).edge_cost[i];
		if (weights[i] < wmin) wmin = weights[i];
		if (weights[i] > wmax) wmax = weights[i];
	}

	counting_sort(unsrtedges, srtedges, num_edges, weights, wmin, wmax);

	arraylist edgelist;
	arraylist_init(&edgelist);
	arraylist_setup(&edgelist, num_edges);

	int tail = num_edges;
	for (i = 0; i < num_edges; i++) {
		arraylist_insert_pred(&edgelist, tail, srtedges[i]);
	}

	start = clock();
	kr_lagrange_vj(&env, &edgelist, &best1t, &lb, &gen_step, &st);
	end = clock();
	time_interval = (end - start) / (double)CLOCKS_PER_SEC;

	plot_tree(&best1t, &(env.vertices), NULL);
	printf("time = %.2f\n", time_interval);

	tree_delete(&best1t);
	arraylist_delete(&edgelist);
	*/

	/* TEST: REDUCE ***********************************************/
	/*
	int i, st, nr;
	int n			= pars.num_vertices;
	double ub		= opt * 1.01;
	double lb;

	tree best1t;
	tree_init(&best1t);
	tree_setup(&best1t, n);

	pr_lagrange_hk(&env, ub, 20000, 20000, 100, &best1t, &lb, &st);

	reduce(&(env.main_graph), &best1t, lb, ub, &nr);

	//plot_graph(&(env.main_graph), &(env.vertices), NULL);

	printf("num edges = %d : num removed edges = %d\n", (n * (n - 1)) / 2, nr);
	printf("ratio = %.2f\n", nr / (double)((n * (n - 1)) / 2) * 100.0);
	tree_delete(&best1t);
	*/

	/* TEST: LAGRANGE KRUSKAL + REDUCE ****************************/
	/*
	int i, st, nr;
	double lb, gen_step, time_interval;
	double ub		= opt * 1.01;
	int n			= pars.num_vertices;
	clock_t start, end;

	tree best1t;
	tree_init(&best1t);
	tree_setup(&best1t, n);

	//pr_lagrange_hk(&env, ub, 20000, 20000, 100, &best1t, &lb, &st);
	//reduce(&(env.main_graph), &best1t, lb, ub, &nr);

	int num_edges;
	int *unsrtedges	= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));
	int *srtedges	= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));
	int	*weights	= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));
	int wmin 		= (int)(env.main_graph).edge_cost[0];
	int wmax 		= (int)(env.main_graph).edge_cost[0];

	num_edges = 0;
	for (i = 0; i < (n * (n - 1)) / 2; i++) {
		if (env.main_graph.edge_cstr[i] == FREE) {
			unsrtedges[num_edges++]	= i;
			weights[i]		= (int)(env.main_graph).edge_cost[i];
			if (weights[i] < wmin) wmin = weights[i];
			if (weights[i] > wmax) wmax = weights[i];
		}
	}

	counting_sort(unsrtedges, srtedges, num_edges, weights, wmin, wmax);

	arraylist edgelist;
	arraylist_init(&edgelist);
	arraylist_setup(&edgelist, (n * (n - 1)) / 2);

	int tail = edgelist.capacity;
	for (i = 0; i < num_edges; i++) {
		arraylist_insert_pred(&edgelist, tail, srtedges[i]);
	}

	start = clock();
	kr_lagrange_vj(&env, &edgelist, &best1t, &lb, &gen_step, &st);
	end = clock();
	time_interval = (end - start) / (double)CLOCKS_PER_SEC;

	plot_tree(&best1t, &(env.vertices), NULL);
	printf("num edges = %d : num removed edges = %d\n", (n * (n - 1)) / 2, nr);
	printf("ratio = %.2f\n", nr / (double)((n * (n - 1)) / 2) * 100.0);
	printf("time = %.2f\n", time_interval);

	tree_delete(&best1t);
	arraylist_delete(&edgelist);
	*/

	/* TEST: BB PRIM **********************************************/
	/*
	double time_interval;
	clock_t start, end;

	env.global_ub = opt * 1.05;

	start = clock();
	pr_bb(&env, &stats);
	end = clock();
	time_interval = (end - start) / (double)CLOCKS_PER_SEC;

	printf("time = %.2f\n", time_interval);
	printf("upper bound = %.2f\n", env.global_ub);
	plot_tree(&(env.global_1t), &(env.vertices), NULL);
	*/

	/* TEST: BB KRUSKAL *******************************************/

	int st;
	double best_lb, time_interval;
	double ub		= 26633; //ceil(opt * 1.01);
	env.global_ub	= 26633; //ceil(opt * 1.01);
	//int n			= pars.num_vertices;
	clock_t start, end;

	tree best_1t;
	tree_init(&best_1t);
	tree_setup(&best_1t, n);

	double *best_mults = (double*)malloc(n * sizeof(double));

	pr_lagrange_hk(&env, ub, 20000, 20000, 100, &best_1t, best_mults, &best_lb, &st);

	/*
	printf("percent = %.2f\n", best_lb / opt * 100.0);
	exit(1);
	*/


	int *rmvedges  		= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));
	int num_rmvedges;

	reduce(&(env.main_graph), &best_1t, best_mults, best_lb, ub, rmvedges, &num_rmvedges);

	printf("num edges = %d : num removed edges = %d\n", (n * (n - 1)) / 2, num_rmvedges);
	printf("ratio = %.2f\n", num_rmvedges / (double)((n * (n - 1)) / 2) * 100.0);
	printf("lower bound = %.2f\n", best_lb);
	printf("ratio = %.2f\n", best_lb / opt * 100.00);

	for (i = 0; i < num_rmvedges; i++) {
		graph_set_edge_cstr(&(env.main_graph), rmvedges[i], FORBIDDEN);
	}

	int num_edges;
	int *unsrtedges		= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));
	int *srtedges		= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));
	double	*weights	= (double*)malloc((n * (n - 1)) / 2 * sizeof(double));
	double wmin 		= (env.main_graph).edge_cost[0];
	double wmax 		= (env.main_graph).edge_cost[0];

	num_edges = 0;
	for (i = 0; i < (n * (n - 1)) / 2; i++) {
		if (env.main_graph.edge_cstr[i] == FREE) {
			unsrtedges[num_edges++]	= i;
			weights[i]		= (env.main_graph).edge_cost[i];
			if (weights[i] < wmin) wmin = weights[i];
			if (weights[i] > wmax) wmax = weights[i];
		}
	}

	counting_sort(unsrtedges, srtedges, num_edges, weights, wmin, wmax);

	arraylist edgelist;
	arraylist_init(&edgelist);
	arraylist_setup(&edgelist, (n * (n - 1)) / 2);

	int tail = edgelist.capacity;
	for (i = 0; i < num_edges; i++) {
		arraylist_insert_pred(&edgelist, tail, srtedges[i]);
	}

	tree part1t;
	tree_init(&part1t);
	tree_setup(&part1t, n);
	set partvs;
	set_init(&partvs);
	set_setup(&partvs, n);

	/*********************************************************/
	/*
	env.path_node = -1;
	env.path_tree 	= (tree*)malloc( n * sizeof(tree));
	for (i = 0; i < n; i++) {
		tree_init(&(env.path_tree[i]));
		tree_setup(&(env.path_tree[i]), n);
	}
	env.path_lb	= (double*)malloc( n * sizeof(double));
	env.rmvedges	= (int*)   malloc((n * (n - 1)) / 2 * sizeof(int));
	env.begin		= (int*)   malloc( n * sizeof(int));
	*/
	/*********************************************************/

	double new_ub;


	stats.num_nodes 	= 0;
	stats.num_levels 	= 0;
	stats.curr_node 	= 0;
	stats.curr_level 	= 0;

	start = clock();
	//pr_bb(&env, &stats);
	env.genascent_mults = (double*)malloc(n * sizeof(double));

	kr_bb(&env, &stats, &edgelist, &part1t, &partvs, &new_ub);
	end = clock();
	time_interval = (end - start) / (double)CLOCKS_PER_SEC;

	printf("num edges = %d : num removed edges = %d\n", (n * (n - 1)) / 2, num_rmvedges);
	printf("ratio = %.2f\n", num_rmvedges / (double)((n * (n - 1)) / 2) * 100.0);

	printf("time = %.2f\n", time_interval);
	printf("upper bound = %.2f\n", env.global_ub);

	plot_tree(&(env.global_1t), &(env.vertices), NULL);



	/* TEST: KRUSKAL **********************************************/
	/*
	int i, st, nr;
	double lb, gen_step, time_interval;
	double ub		= opt * 1.01;
	env.global_ub	= opt * 1.01;
	int n			= pars.num_vertices;
	clock_t start, end;

	tree best1t;
	tree_init(&best1t);
	tree_setup(&best1t, n);

	//pr_lagrange_hk(&env, ub, 20000, 20000, 100, &best1t, &lb, &st);
	//reduce(&(env.main_graph), &best1t, lb, ub, &nr);

	int num_edges;
	int *unsrtedges		= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));
	int *srtedges		= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));

	double *weights		= (double*)malloc((n * (n - 1)) / 2 * sizeof(double));
	double  wmin 		= (int)(env.main_graph).edge_cost[0];
	double  wmax 		= (int)(env.main_graph).edge_cost[0];

	num_edges = 0;
	for (i = 0; i < (n * (n - 1)) / 2; i++) {
		if (env.main_graph.edge_cstr[i] == FREE) {
			unsrtedges[num_edges++]	= i;
			weights[i]		= (env.main_graph).edge_cost[i];
			if (weights[i] < wmin) wmin = weights[i];
			if (weights[i] > wmax) wmax = weights[i];
		}
	}

	counting_sort(unsrtedges, srtedges, num_edges, weights, wmin, wmax);

	arraylist edgelist;
	arraylist_init(&edgelist);
	arraylist_setup(&edgelist, (n * (n - 1)) / 2);

	int tail = edgelist.capacity;
	for (i = 0; i < num_edges; i++) {
		arraylist_insert_pred(&edgelist, tail, srtedges[i]);
	}

	tree part1t;
	tree_init(&part1t);
	tree_setup(&part1t, n);
	set partvs;
	set_init(&partvs);
	set_setup(&partvs, n);

	start = clock();
	//kr_lagrange_vj(&env, &edgelist, &part1t, &partvs, INITASCENT, ub, &best1t, &lb, &gen_step, &st);
	pr_lagrange_vj(&env, INITASCENT, ub, &lb, &gen_step, &best1t, &st);
	end = clock();
	time_interval = (end - start) / (double)CLOCKS_PER_SEC;

	printf("num edges = %d : num removed edges = %d\n", (n * (n - 1)) / 2, nr);
	printf("ratio = %.2f\n", nr / (double)((n * (n - 1)) / 2) * 100.0);
	printf("time = %.2f\n", time_interval);

	printf("lower bound = %.2f\n", lb);
	plot_tree(&best1t, &(env.vertices), NULL);
	*/


	printf("End\n");



	parameters_delete(&pars);
	environment_delete(&env);
	statistics_delete(&stats);
	delete_convtable();


	return EXIT_SUCCESS;

}
