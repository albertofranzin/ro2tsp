#ifndef __UTILS_H
#define __UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <math.h>

/* constants and variables */

typedef struct _parameters {
	unsigned int no_of_nodes;
	unsigned int seed;
	unsigned short plot;
	unsigned short plotOnlyTree;
} parameters;

// config file
#define FILE_CONFIG "config"

// define infinity for double
#define DOUBLE_INFINITY DBL_MAX

// global declaration for incumbent solution
extern double incumbent;

/* methods */

// this was just too long to stay in that line with all the names there
#define distance(x1, y1, x2, y2) \
	sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));

void init();

void initializeRandom(long seed);
void initializeRandomWithTime();

short parHash(char *);
parameters *getParameters();

#endif