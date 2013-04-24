#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define FAILURE 0
#define SUCCESS 1
#define TRUE 1
#define FALSE 0

// valori convenzionali per i costi +infinito e -infinito dei lati 
#define BIG 50000.0
#define SMALL -50000.0

// per tentare di eliminare effetti errori arrotondamento
//#define EPSILON 0.000000001
#define EPSILON 0.0000001

// parametri usati per algoritmo subgradiente - lagrange
#define VERY_SMALL -1000000000.0
#define ALPHA 2.0
#define MAX_NUM_OF_ITERATIONS 30000
#define SLACK_TIME 2000
#define ALPHA_HALVING_TIME 100

// parametri usati in utils.c (es. getParameters) o main.c ecc ecc
#define FILE_CONFIG "config"
#define DEFAULT_NUMBER_OF_NODES 10

#define DEFAULT_RANDOM_INSTANCE_OPTION 1 // -> TRUE
#define DEFAULT_RANDOM_SEED_OPTION 0 // -> FALSE
#define DEFAULT_SEED 0

#define DEFAULT_TSP_FILE_OPTION 0 // -> FALSE
#define DEFAULT_TSP_FILE NULL

#define DEFAULT_HEURISTIC_TRIALS 1

// dimensioni grafo gnuplot

// random instance
/**/
#define X_MIN -0.010
#define X_MAX 1.010
#define Y_MIN -0.010
#define Y_MAX 1.010
/**/

// ulysses16
/*
#define X_MIN 30.010
#define X_MAX 42.010
#define Y_MIN -6.010
#define Y_MAX 28.010
*/

// dantzig42

/*
#define X_MIN 0.010
#define X_MAX 180.010
#define Y_MIN 10.010
#define Y_MAX 110.010
*/

// att48
/*
#define X_MIN 0.010
#define X_MAX 8000.010
#define Y_MIN 0.010
#define Y_MAX 8000.010
*/

// bays29
/*
#define X_MIN 0.010
#define X_MAX 2500.010
#define Y_MIN 0.010
#define Y_MAX 2500.010
*/

// berlin52
/** /
#define X_MIN 0.010
#define X_MAX 1800.010
#define Y_MIN 0.010
#define Y_MAX 1200.010
/ **/


// eil51
/*
#define X_MIN 0.010
#define X_MAX 80.010
#define Y_MIN 0.010
#define Y_MAX 80.010
*/

// eil76
/*
#define X_MIN 0.010
#define X_MAX 80.010
#define Y_MIN 0.010
#define Y_MAX 80.010
*/

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
/** /


// bier127
/*
#define X_MIN 5000.000
#define X_MAX 20000.010
#define Y_MIN 5000.010
#define Y_MAX 20000.010
*/
#endif
