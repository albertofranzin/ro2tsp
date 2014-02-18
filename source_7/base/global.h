#ifndef GLOBAL_H_
#define GLOBAL_H_

#define VERBOSE
//#define DEBUG
extern int verbosity;

/* boolean values */
#define TRUE		1
#define FALSE		0

/* subgradient mode */
#define INITASCENT  0
#define GENASCENT	1

/* weights type */
#define INT		0
#define FLO 	1
#define DOUBLE 	2

#define EPSILON 0.0001

/* some maths */
#define PI				3.141592
#define EARTH_RADIUS	6378.388
#define INTEGER_OPTION	TRUE		/* integer rounding option */

/* constraints */
#define	FREE		0	/* edge with no constraints */
#define FORCED		1	/* forced edge */
#define FORBIDDEN	2	/* forbidden edge */

/* verbosity levels */
#define SILENT		0	/* say nothing */
#define ESSENTIAL	1	/* say something */
#define USEFUL		2	/* say a lot, save log file */
#define ANNOYING	3	/* getchars here and there */

/* TSPLIB file format */
#define EUC_2D			0
#define MAN_2D			1
#define CEIL_2D			2
#define EXPLICIT		3
#define GEO				4
#define ATT				5


/* TSPLIB matrix format */
#define FULL_MATRIX		0
#define LOWER_DIAG_ROW	1

/* heuristic algorithms	*/
#define ALL			0	/* all algos */
#define NN			1	/* nearest neighbour */

/* lagrange method */
#define ALPHA		2.0

/* solvers */
#define BB			0	/* branch and bound */
#define CPLEX		1	/* cplex */

/* default values of problem parameters */
#define DEFAULT_RANDOM_INSTANCE_OPTION	TRUE
#define	DEFAULT_RANDOM_SEED_OPTION 		FALSE
#define	DEFAULT_NUM_VERTICES			10
#define	DEFAULT_SEED 					0

#define	DEFAULT_TSPLIB_INSTANCE_OPTION 	FALSE
#define	DEFAULT_TSPLIB_FILE_PATH		NULL
#define	DEFAULT_TSPLIB_FILE_FORMAT		EUC_2D
#define	DEFAULT_TSPLIB_MATRIX_TYPE		FULL_MATRIX

#define	DEFAULT_HEUR_ALGO		 		NN
#define	DEFAULT_NUM_HEUR_TRIALS 		1

#define	DEFAULT_SOLVER					BB
#define	DEFAULT_NUM_THREADS 			1
#define	DEFAULT_VERBOSITY 				SILENT

#define	DEFAULT_CALLBAKS_OPTION			FALSE
#define	DEFAULT_PROXIMITY_OPTION		FALSE
#define	DEFAULT_LOCALBRANCHING_OPTION	FALSE
#define	DEFAULT_HARDFIXING_OPTION		FALSE
#define	DEFAULT_RINSPOLISHING_OPTION	FALSE


/* configuration file */
#define CONFIG_FILE	"config"


#endif /* GLOBAL_H_ */
