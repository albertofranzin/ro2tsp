#include "lagrangian.h"

/*
 * initializer
 * -n : number of nodes in the graph
 *
 * default values are initialized here
 * Later may be parameters...
 */
lagrangian* initLagrange(int n) {
	lagrangian *l = malloc(sizeof(lagrangian));

	// we need |V| multipliers, initially set to 0
	l->mult = malloc(sizeof(double) * n);
	// memset(l->mult, 0, sizeof(double) * n);
	int i;
	for (i = 0; i < n; ++i) {
		l->mult[i] = 0.0;
	}

	// 100 steps is the suggested value
	l->decreased_ago = 0;
	l->wait_for = 100;

	// alpha spans from 0 to 2, according to suggestions
	l->alpha = 0.9;

	l->lastbest = 1000000000.0; // conventional value, just to try

	return l;
}

/*
 * update lagrangian coefficients
 * - G : the graph
 * - l : the lagrangian setup
 *
 */
void updateLagrange(graph *G, lagrangian *l) {
	int n = G->n,
		i, j;

	/*printf("wtf\n");
	printf("%d\n", G->n);
	char ch = getchar();*/

	// apply correponding weight to all of the edges insisisting to it
	for (i = 1; i <= n; ++i) {
		for (j = 1 ; j < i ; j++) {
			//printf("%f %f\t###\t%f ", l->mult[i-1], l->mult[j-1], get_edge_cost(G, i, j));fflush(stdout);
			set_edge_cost(G, i, j, get_edge_cost(G, i, j) + l->mult[i-1] + l->mult[j-1]);
			//printf("%f\n", get_edge_cost(G, i, j));
		}
	}

}

/*
 * compute cost of a graph when lagrangian multipliers are applied
 * - G : graph
 * - l : the lagrangian setup
 * - ub : an upper bound to the solution
 * - rs : the value of the relaxed solution
 *
 * compute the cost of the graph and then subtract the cost imposed
 * by lagrangian multipliers.
 * At the same time, update lagrangian multipliers.
 *
 * return : cost of the graph. If cost < 0, it means that |cost| is the optimal value
 */
double getLagrangeGraphCost(graph *ONE_TREE, graph *G, lagrangian **ll, double ub, double rs) {
	double oldcost = get_graph_cost(ONE_TREE), s = 0., denom = 0., tmp, step_size, cost;
	int i, nd;
	lagrangian *l = (lagrangian *)(ll);

	// compute contribution of lagrangian multipliers
	for (i = 0; i < ONE_TREE->n; ++i) {
		// tmp = l->mult[i];
		s += l->mult[i];
	}
	s *= 2;

	// if s == 0, then we have found an optimal solution
	// therefore, we can terminate our quest for the optimal solution
	// Return solution value, according to the convention that a negative cost
	// means that |cost| is the optimum.
	/*if (s == 0) {
		return -cost + s;
	}*/

	// remove following 2 lines if necessary
	// graph H;
	// initGraph(&H, 1);

	rs = oldcost;
	//ub = heuristicBound(G, &H, 10);
	/*printf("%d\n", G->n);
	char ch = getchar();*/

	// update lagrangian multipliers
	//printf("setup : %f, %f\n", ub, rs);
	for (i = 1; i <= G->n; ++i) {
		nd = get_node_deg(ONE_TREE, i);

		if (nd == 2) continue;

		denom = nd - 2;
		step_size = 1.0 * l->alpha * (ub - rs) / (denom * denom);

		//printf("%f ::: %f -> ", step_size, l->mult[i-1]);fflush(stdout);

		if (nd > 2) {
			l->mult[i-1] += step_size * denom;
		} else {
			l->mult[i-1] -= step_size * denom;
		}
		//printf("%f\n", l->mult[i-1]);
	}
	//printf("graph G before updating costs\n");
	//print_graph(G);
	updateLagrange(G, l);
	//printf("updated graph G\n");
	//print_graph(G);
	//printf("\n");

	// remove following line if necessary
	// deleteGraph(&H);

	cost = get_graph_cost(ONE_TREE);
	//printf("cost with lg : %f \t | \t", cost);fflush(stdout);
	// subtract the lagrangian contribution from computed graph cost
	if (cost >= l->lastbest) {
		printf("to mare\n");
		l->decreased_ago = l->decreased_ago + 1;
		printf("to sorea\n");
		if (l->decreased_ago == l->wait_for) {
			l->alpha = l->alpha / 2.0;
			printf("halving alpha\n");
			l->decreased_ago = 0;
		}
	} else {
		l->decreased_ago = 0;
		l->lastbest = cost;
	}
	//printf("cost w/o lg : %f \t | \t ", oldcost);fflush(stdout);

	//printf("difference from last round : %f ... %f\n", s, ub);

	return cost;
}
