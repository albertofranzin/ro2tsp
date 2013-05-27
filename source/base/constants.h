#ifndef CONSTANTS_H_
#define CONSTANTS_H_

//------------------------------------------------------

#define SUCCESS 1
#define FAILURE 0

#define FALSE   0
#define TRUE    1

#define N 0
#define Y 1

#define DISABLED 0
#define ENABLED  1

#define DEFAULT 0

#define EPSILON 0.0

#define BIG    5000000000.0
#define SMALL -5000000000.0
//------------------------------------------------------

// Branch and Bound

// - edge constraints.
#define FREE       0
#define FORBIDDEN -1
#define FORCED     1

// - backup modes
#define BASIC   0
#define SAVE_OT 1

// - propagate modes
#define NOFC_MODE 0
#define FC_MODE   1

// - parameter used when rounding the current solution to the ceiling 
#define SAFE_ROUNDING_THRESHOLD 0.00001


#define INTEGER_ROUNDING_OPT ENABLED
#define FORWARD_CHECKING_OPT ENABLED
#define FAT_EDGES_OPT        DISABLED
#define SELECT_NODE_OPT      DEFAULT
#define SELECT_EDGES_OPT     DEFAULT




//------------------------------------------------------

// compute_lagrange

#define MAX_NUM_ITERATIONS           30000
#define MAX_NUM_ITERATIONS_NO_IMPROV 100
#define ALPHA                        2.0
#define ALPHA_HALVING_TIME           10

/*
#define MAX_NUM_ITERATIONS 30000
#define MAX_NUM_ITERATIONS_NO_IMPROV 2000
#define ALPHA 2.0
#define ALPHA_HALVING_TIME 50
*/




//------------------------------------------------------

// solvers available
#define BRANCH_AND_BOUND 1
#define CPLEX            2




//------------------------------------------------------

// heuristic algorithms available for the the upper bound:
#define DUMB                     0
#define NEAREST_NEIGHBOUR        1
#define NEAREST_NEIGHBOUR_2_OPT  2
#define RANDOM_CYCLES            3
#define RANDOM_CYCLES_2OPT       4

#define NUM_TRIALS_RANDOM_CYCLES          1000000
#define NUM_TRIALS_RANDOM_CYCLES_2OPT     1000

//------------------------------------------------------

// some math for computing 'GEO' distances
#define PI           3.141592
#define EARTH_RADIUS 6378.388

//------------------------------------------------------

// utils.c,  main.c, ...

#define FILE_CONFIG             "config"
#define DEFAULT_NUMBER_OF_NODES 10

#define DEFAULT_SOLVER BRANCH_AND_BOUND

#define DEFAULT_RANDOM_INSTANCE_OPTION 1 // -> TRUE
#define DEFAULT_RANDOM_SEED_OPTION     0 // -> FALSE
#define DEFAULT_SEED                   0

#define DEFAULT_TSP_FILE_OPTION 0 // -> FALSE
#define DEFAULT_TSP_FILE        NULL

#define DEFAULT_TSP_FILE_FORMAT 41

#define DEFAULT_HEURISTIC_ALGO   NEAREST_NEIGHBOUR_2_OPT
#define DEFAULT_HEURISTIC_TRIALS 1

//------------------------------------------------------

// plotting graphs

// random instance
#define X_MIN -0.010
#define X_MAX  1.010
#define Y_MIN -0.010
#define Y_MAX  1.010

// ----------------------------------------------------------

// when debugging, show the desidered amount of informations
// fatal errors are shown always
#define SILENT     0 // say nothing
#define ESSENTIAL  1 // say something
#define USEFUL     2 // say a lot
#define VERBOSE    3 // save log files
#define ANNOYING   4 // getchars here and there

#endif
