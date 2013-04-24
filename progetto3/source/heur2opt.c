#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "graph.h"
#include "egraph.h"
#include "tree.h"
#include "onetree.h"
#include "compute_mst.h"
#include "compute_ot.h"
#include "compute_nearest_neighbour.h"
#include "compute_upper_bound.h"
#include "compute_lagrange.h"
#include "solve_tsp.h"
#include "cycle.h"

#include "list.h"
#include "edge.h"
#include "edgelist.h"

#include "utils.h"

int main(int argc, char *argv[]) {
    int i;
    char* opt;

    parameters* pars = getParameters();

    /* ======================== */
    /* parse command line input */
    /* ======================== */


    for (i = 1; i < argc; i++) {
    opt = argv[i];
    if (strcmp(opt, "-n") == 0)
      pars->number_of_nodes = atoi(argv[++i]);
    if (strcmp(opt, "-s") == 0)
      pars->seed = atol(argv[++i]);
    }







    /* ================= */
    /* inizializza grafo */
    /* ================= */

    graph G;
    graph_init(&G, 1);

    egraph EG;
    egraph_init(&EG, pars->number_of_nodes);

    if (pars->tsp_file_option == TRUE) {
        read_tsp_from_file(&EG, pars);
        egraph_to_graph(&EG, &G);
        printf("@ Euclidean TSP\n# tsplib instance\n# number of nodes = %d\n# tsplib file = %s\n\n", pars->number_of_nodes, pars->tsp_file);
    }
    else if (pars->random_instance_option == TRUE) {
        if (pars->random_seed_option == FALSE) {
          srand(pars->seed);
          egraph_random(&EG);
          egraph_to_graph(&EG, &G);
          printf("@ Euclidean TSP\n# random instance\n# number of nodes = %d\n# seed = %ld\n\n", pars->number_of_nodes, pars->seed);
        }
        else {
          ; // aggiungi caso generazione pseudocasuale del seed
        }
    }


    /* ========================================= */
    /* Algoritmo Euristico - ricerca upper bound */
    /* ========================================= */

    tree T;
    tree_init(&T, 1);

    printf("creating a cycle\n");
    cycle C;
    cycle_init(&C, 1);
    printf("cycle created\n");

    double heuristic_upper_bound;
    heuristic_upper_bound = compute_upper_bound(&G, &C);
    printf("@ Nearest Neighbour Heuristic\n# upper bound = %f\n", heuristic_upper_bound);

    double incumbent = heuristic_upper_bound;

    cycle_print(&C);

    onetree ONE_TREE;
    onetree_init(&ONE_TREE, 1);
    double z = compute_lagrange(&G, &ONE_TREE, incumbent);

    printf("\n@ Branch and Bound\n# initial incumbent = %f, lower bound = %f\n",
        incumbent, z);

    return 0;
}
