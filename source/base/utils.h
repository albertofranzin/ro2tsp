#ifndef __UTILS_H
#define __UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include "../data/graph.h"
#include "../data/egraph.h"
#include "../data/onetree.h"

/* constants and variables */

/*
 * parameters
 *
 * unsigned int  solver                  : solver method (cplex or b&b)
 * unsigned int  number_of_nodes         : # of nodes in the graph
 * int           random_instance_option  : generate a random graph?
 * int           random_seed_option      : use a pseudorandom seed?
 * long          seed                    : seed
 * int           tsp_file_option         : use a TSPLIB(-like) instance?
 * char         *tsp_file                : TSPLIB path/filename
 * unsigned int  heuristic_algo          : heuristic algorithm
 * unsigned int  heuristic_trials        : number of trials of the heuristic algo
 */
typedef struct _parameters {

  unsigned int  solver;
  unsigned int  number_of_nodes;
  int           random_instance_option;
  int           random_seed_option;
  long          seed;
  int           tsp_file_option;
  char         *tsp_file;
  unsigned int  heuristic_algo;
  unsigned int  heuristic_trials;

} parameters;

/* methods */

short parHash(char* c);

parameters *getParameters(void);



/*
 * tspHash
 * - parameter name
 * - parameter value
 *
 * recognizes parameters.
 *
 * returns:
 *  0 : parName = 'NAME'
 *  1 : (parName, parValue) = ('TYPE', 'TSP')
 *  2 : parName = 'COMMENT'
 *  3 : parName = 'DIMENSION'
 *
 * 41 : (parName, parValue) = ('EDGE_WEIGHT_TYPE', 'EUD_2D')
 * 42 : (parName, parValue) = ('EDGE_WEIGHT_TYPE', 'MAN_2D')
 * 43 : (parName, parValue) = ('EDGE_WEIGHT_TYPE', 'CEIL_2D')
 * 44 : (parName, parValue) = ('EDGE_WEIGHT_TYPE', 'GEO')
 * 45 : (parName, parValue) = ('EDGE_WEIGHT_TYPE', 'EXPLICIT')
 * 46 : (parName, parValue) = ('EDGE_WEIGHT_FORMAT', 'FULL_MATRIX')
 * 47 : (parName, parValue) = ('EDGE_WEIGHT_FORMAT', 'LOWER_DIAG_ROW')
 *
 * 51 : (parName, parValue) = ('DISPLAY_DATA_TYPE', 'COORD_DISPLAY')
 * 61 : parName = 'NODE_COORD_SECTION'
 * 62 : parName = 'DISPLAY_DATA_SECTION'
 * 63 : parName = 'EDGE_WEIGHT_SECTION'
 *
 * -1 : all the other combinations/values
 */
short tspHash(char*, char*);

/*
 * read_tsp_from_file
 * - G : graph to be filled in
 * - pars : parameters (some may be overridden)
 *
 * will override parameters, if needed
 *
 * Assumes file is formatted in the right way.
 * Does not guarantee correct working if this is not true.
 * Flexibility = 0!
 */
void read_tsp_from_file(egraph *G, parameters *pars);

//typedef int (*compfn)(const void*, const void*);
//int snbdComp(const void *, const void *);
//int snbdComp(node *, node *);

// sort nodes by cost - comparator for qsort
int sebwComp ( const void *, const void *);

/*
 * free_and_null
 * - char ** : pointer to the list
 *
 * empty a char** and set the pointer to NULL
 */
void free_and_null(char **ptr);

#endif
