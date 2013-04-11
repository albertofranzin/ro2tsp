#ifndef CONSTANTS_H_
#define CONSTANTS_H_

// valori convenzionali per i costi +infinito e -infinito dei lati 
#define BIG 2000
#define SMALL -2000

// dimensioni grafo gnuplot
#define X_MIN -0.010
#define X_MAX 1.010
#define Y_MIN -0.010
#define Y_MAX 1.010

// parametri main
#define DEFAULT_NUMBER_OF_NODES 10
#define DEFAULT_SEED 0

// per tentare di eliminare effetti errori arrotondamento
#define EPSILON 0.0000000001

// parametri usati per algoritmo subgradiente - lagrange
#define VERY_SMALL -1000000000
#define ALPHA 2.0
#endif
