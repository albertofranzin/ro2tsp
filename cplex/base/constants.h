#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define FAILURE 0
#define SUCCESS 1
#define TRUE    1
#define FALSE   0

// valori convenzionali per i costi +infinito e -infinito dei lati 
#define BIG    50000.0
#define SMALL -50000.0

// per tentare di eliminare effetti errori arrotondamento
//#define EPSILON 0.000000001
#define EPSILON 0.0000001

// parametri usati per algoritmo subgradiente - lagrange
#define VERY_SMALL            -1000000000.0
#define ALPHA                  2.0
#define MAX_NUM_OF_ITERATIONS  50000
#define SLACK_TIME             2000
#define ALPHA_HALVING_TIME     150

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

#define DEFAULT_HEURISTIC_ALGO   NEAREST_NEIGHBOUR_2_OPT
#define DEFAULT_HEURISTIC_TRIALS 1

// dimensioni grafo gnuplot

// random instance - and default starting values
/**/
#define X_MIN -0.010
#define X_MAX  1.010
#define Y_MIN -0.010
#define Y_MAX  1.010
/**/

// ulysses16
/** /
#define X_MIN 33.010
#define X_MAX 42.010
#define Y_MIN -6.010
#define Y_MAX 28.010
/ **/

// dantzig42
/** /
#define X_MIN 0.010
#define X_MAX 180.010
#define Y_MIN 10.010
#define Y_MAX 110.010
/ **/

// att48
/** /
#define X_MIN 0.010
#define X_MAX 8000.010
#define Y_MIN 0.010
#define Y_MAX 8000.010
/ **/

// bay{g,s}29
/** /
#define X_MIN 0.010
#define X_MAX 2500.010
#define Y_MIN 0.010
#define Y_MAX 2500.010
/ **/

// berlin52
/** /
#define X_MIN 0.010
#define X_MAX 1800.010
#define Y_MIN 0.010
#define Y_MAX 1200.010
/ **/


// eil51
/** /
#define X_MIN 0.010
#define X_MAX 80.010
#define Y_MIN 0.010
#define Y_MAX 80.010
/ **/

// eil76
/** /
#define X_MIN 0.010
#define X_MAX 80.010
#define Y_MIN 0.010
#define Y_MAX 80.010
/ **/

// eil101
/*
#define X_MIN 0.010
#define X_MAX 80.010
#define Y_MIN 0.010
#define Y_MAX 80.010
*/


// pr107
/** /
#define X_MIN 0.010
#define X_MAX 20000.010
#define Y_MIN 0.010
#define Y_MAX 20000.010
/ **/


// bier127
/*
#define X_MIN 5000.000
#define X_MAX 20000.010
#define Y_MIN 5000.010
#define Y_MAX 20000.010
*/


// kro*
/** /
#define X_MIN 0.000
#define X_MAX 4000.010
#define Y_MIN 0.010
#define Y_MAX 4000.010
/ **/


// burma14
/** /
#define X_MIN 14.000
#define X_MAX 26.010
#define Y_MIN 90.010
#define Y_MAX 100.010
/ **/


// fri26
/** /
#define X_MIN .000
#define X_MAX .010
#define Y_MIN .010
#define Y_MAX .010
/ **/


// sst70
/** /
#define X_MIN 0.000
#define X_MAX 101.010
#define Y_MIN 0.010
#define Y_MAX 101.010
/ **/


// rat99
/** /
#define X_MIN 0.010
#define X_MAX 100.010
#define Y_MIN 0.010
#define Y_MAX 220.010
/ **/


// rd100
/** /
#define X_MIN 0.010
#define X_MAX 1000.010
#define Y_MIN 0.010
#define Y_MAX 1000.010
/ **/

#endif
