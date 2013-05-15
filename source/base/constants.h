#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define FAILURE 0
#define SUCCESS 1
#define TRUE    1
#define FALSE   0
#define Y 1
#define N 0

// some math for computing 'GEO' distances
#define PI           3.141592
#define EARTH_RADIUS 6378.388

// valori convenzionali per i costi +infinito e -infinito dei lati 
#define BIG    500000000.0
#define SMALL -500000000.0

// per tentare di eliminare effetti errori arrotondamento
#define EPSILON 0.0
//#define EPSILON 0.0000001

// parametri usati per algoritmo subgradiente - lagrange
#define MAX_NUM_ITERATIONS 30000
#define MAX_NUM_ITERATIONS_NO_IMPROV 2000
#define ALPHA 2.0
#define ALPHA_HALVING_TIME 100

// solvers available
#define BRANCH_AND_BOUND 1
#define CPLEX            2

// heuristic algorithms available for the the upper bound:
#define NEAREST_NEIGHBOUR        1
#define NEAREST_NEIGHBOUR_2_OPT  2
#define RANDOM_CYCLE             3

// parametri usati in utils.c (es. getParameters) o main.c ecc ecc
#define FILE_CONFIG             "config"
#define DEFAULT_NUMBER_OF_NODES 10

#define DEFAULT_SOLVER BRANCH_AND_BOUND

#define DEFAULT_RANDOM_INSTANCE_OPTION 1 // -> TRUE
#define DEFAULT_RANDOM_SEED_OPTION     0 // -> FALSE
#define DEFAULT_SEED                   0

#define DEFAULT_TSP_FILE_OPTION 0 // -> FALSE
#define DEFAULT_TSP_FILE        NULL

#define DEFAULT_TSP_FILE_FORMAT 0 // non-valid, I can't assign a fixed one

#define DEFAULT_HEURISTIC_ALGO   NEAREST_NEIGHBOUR_2_OPT
#define DEFAULT_HEURISTIC_TRIALS 1

// dimensioni grafo gnuplot

// random instance area
#define X_MIN -0.010
#define X_MAX  1.010
#define Y_MIN -0.010
#define Y_MAX  1.010

#endif
