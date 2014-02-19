#include "../base/setup_parameters.h"


int setup_parameters_default(parameters *pars) {

	pars->random_instance_option	= DEFAULT_RANDOM_INSTANCE_OPTION;
	pars->random_seed_option 		= DEFAULT_RANDOM_SEED_OPTION;
	pars->num_vertices 				= DEFAULT_NUM_VERTICES;
	pars->seed 						= DEFAULT_SEED;

	pars->tsplib_instance_option	= DEFAULT_TSPLIB_INSTANCE_OPTION;
	pars->tsplib_file_path			= DEFAULT_TSPLIB_FILE_PATH;
	pars->tsplib_file_format 		= DEFAULT_TSPLIB_FILE_FORMAT;
	pars->tsplib_matrix_type 		= DEFAULT_TSPLIB_MATRIX_TYPE;

	pars->heur_algo 				= DEFAULT_HEUR_ALGO;
	pars->num_heur_trials 			= DEFAULT_NUM_HEUR_TRIALS;

	pars->solver					= DEFAULT_SOLVER;
	pars->num_threads 				= DEFAULT_NUM_THREADS;
	verbosity 						= DEFAULT_VERBOSITY;

	pars->callbacks_option 			= DEFAULT_CALLBAKS_OPTION;
	pars->proximity_option 			= DEFAULT_PROXIMITY_OPTION;
	pars->localbranching_option 	= DEFAULT_LOCALBRANCHING_OPTION;
	pars->hardfixing_option 		= DEFAULT_HARDFIXING_OPTION;
	pars->rinspolishing_option 		= DEFAULT_RINSPOLISHING_OPTION;

	return 0;

}


int setup_parameters_config(char *configPath, parameters *pars) {

	FILE	*configFile = fopen(configPath, "r");

	if (configFile == NULL) {
		perror( configPath );
	}
#ifdef DEBUG
    if (configFile == NULL) {
        fprintf(stderr, "Error in /base/setup_parameters.c:\n"
                        "function: setup_parameters_config:\n"
                        "setup_parameters_config : errcode %d\n", 1);
        return -1;
    }
#endif
	char	line[1024];
	char	*parName; 	/* parameter name */
	char	*parValue;	/* parameter value */
	int		lineLen;

	/* beginning file scan */
	while (fgets(line, sizeof line, configFile) != NULL) {
		lineLen = strlen(line) - 1;

		/* skip empty lines */
		if (lineLen == 0)	continue;
		if(line[lineLen] == '\n') line[lineLen] = 0;

		/* skip comments */
		if(line[0] == '#') 	continue;

		/* split and strip away the spaces */
        /* removing the spaces is necessary in order to use */
        /* the par_hash() method above */
		parName		= strtok(line, "= ");
		parValue 	= strtok(NULL, "= ");
		if(parValue == NULL)	continue;

		/* manage correctly each parameter */
		switch (par_hash(parName, parValue)) {


			case 1000 : /* RANDOM_INSTANCE_OPTION = TRUE */
				pars->random_instance_option = TRUE;
			break;
			case 1001 : /* RANDOM_INSTANCE_OPTION = FALSE */
				pars->random_instance_option = FALSE;
			break;
			case 1100 : /* RANDOM_SEED_OPTION = TRUE */
				if (pars->random_instance_option != TRUE) 	break;
				pars->random_seed_option = TRUE;
			break;
			case 1101 : /* RANDOM_SEED_OPTION = FALSE */
				if (pars->random_instance_option != TRUE) 	break;
				pars->random_seed_option = FALSE;
			break;
			case 1200 : /* NUM_VERTICES = x */
				if (pars->random_instance_option != TRUE) 	break;
				pars->num_vertices = atoi(parValue);
			break;
			case 1300 : /* SEED = x */
				if (pars->random_instance_option != TRUE) 	break;
				if (pars->random_seed_option != TRUE) 		break;
				pars->seed = atol(parValue);
			break;
			case 1400 : /* TSPLIB_INSTANCE_OPTION = TRUE */
				if (pars->random_instance_option != FALSE) 	break;
				pars->tsplib_instance_option = TRUE;
			break;
			case 1401 : /* TSPLIB_INSTANCE_OPTION = FALSE */
				if (pars->random_instance_option != FALSE) 	break;
				pars->tsplib_instance_option = FALSE;
			break;
			case 1500 : /* TSPLIB_FILE_PATH = path */
				if (pars->random_instance_option != FALSE) 	break;
				if (pars->tsplib_instance_option != TRUE) 	break;
				pars->tsplib_file_path = (char*)malloc(sizeof(char) * strlen(parValue));
				strcpy(pars->tsplib_file_path, parValue);
			break;
			case 1600 : /* HEUR_ALGO = ALL */
				pars->heur_algo = ALL;
			break;
			case 1601 : /* HEUR_ALGO = NN */
				pars->heur_algo = NN;
			break;
			case 1700 : /* NUM_HEUR_TRIALS = x */
				pars->num_heur_trials = atoi(parValue);
			break;
			case 1800 : /* SOLVER = BB */
				pars->solver = BB;
			break;
			case 1801 : /* SOLVER = CPLEX */
				pars->solver = CPLEX;
			break;
			case 1900 : /* NUM_THREADS = x */
				pars->num_threads = atoi(parValue);
			break;
			case 2000 : /* CALLBACKS_OPTION = TRUE */
				if (pars->solver != CPLEX) 					break;
				pars->callbacks_option = TRUE;
			break;
			case 2001 : /* CALLBACKS_OPTION = FALSE */
				if (pars->solver != CPLEX) 					break;
				pars->callbacks_option = FALSE;
			break;
			case 2100 : /* PROXIMITY_OPTION = TRUE */
				if (pars->solver != CPLEX) 					break;
				pars->proximity_option = TRUE;
			break;
			case 2101 : /* PROXIMITY_OPTION = FALSE */
				if (pars->solver != CPLEX) 					break;
				pars->proximity_option = FALSE;
			break;
			case 2200 : /* LOCALBRANCHING_OPTION = TRUE */
				if (pars->solver != CPLEX) 					break;
				pars->localbranching_option = TRUE;
			break;
			case 2201 : /* LOCALBRANCHING_OPTION = FALSE */
				if (pars->solver != CPLEX) 					break;
				pars->localbranching_option = FALSE;
			break;
			case 2300 : /* HARDFIXING_OPTION = TRUE */
				if (pars->solver != CPLEX) 					break;
				pars->hardfixing_option = TRUE;
			break;
			case 2301 : /* HARDFIXING_OPTION = FALSE */
				// if (pars->solver != CPLEX) 					break;
				pars->hardfixing_option = FALSE;
			break;
			case 2400 : /* RINSPOLISHING_OPTION = TRUE */
				if (pars->solver != CPLEX) 					break;
				pars->rinspolishing_option = TRUE;
			break;
			case 2401 : /* RINSPOLISHING_OPTION = FALSE */
				if (pars->solver != CPLEX) 					break;
				pars->rinspolishing_option = FALSE;
			break;

			case 2500 : /* VERBOSITY = SILENT */
				verbosity = SILENT;
			break;
			case 2501 : /* VERBOSITY = ESSENTIAL */
				verbosity = ESSENTIAL;
			break;
			case 2502 : /* VERBOSITY = USEFUL */
				verbosity = USEFUL;
			break;
			//break;
			case 2503 : /* VERBOSITY = ANNOYING */
				verbosity = ANNOYING;
			break;
			default:
			{
#ifdef DEBUG
				fprintf(stderr, "Error in /base/setup_parameters.c:\n"
						"function: setup_parameters_config:\n"
                        "setup_parameters_config : errcode %d\n", 2);
		        fprintf(stderr, "Info:\n"
		        		"No support for this case\n"
		        		"parName = %s : parValue = %s\n", parName, parValue);
				fclose(configFile);
				return -2;
#endif
			}
			break;

		} /* end switch */

	} /* end while loop (line-by-line file scan) */

	
	fclose(configFile);

	return 0;

}


int setup_parameters_commandline(int argc, char **argv, parameters *pars) {

	int 	i;
	char 	*opt;

	/* parse command line input */
   	for (i = 1; i < argc; i++) {

   	   	opt = argv[i];

   	   	/* number of vertices */
	    if (strcmp(opt, "-n") == 0 ||
	    	strcmp(opt, "--number") == 0) {
	    	pars->random_instance_option = TRUE;
	    	pars->num_vertices = atoi(argv[++i]);
	    }

	    /* seed */
	    if (strcmp(opt, "-s") == 0 ||
	    	strcmp(opt, "--seed") == 0) {
#ifdef DEBUG
	    	if (pars->random_instance_option != TRUE) {
	    		fprintf(stderr, "Error in /base/setup_parameters.c:\n"
	    				"function: setup_parameters_commandline:\n"
                        "setup_parameters_commandline : errcode %d\n", 1);
	    		return -1;
	    	}
#endif
	    	pars->random_seed_option = FALSE;
	    	pars->seed = atol(argv[++i]);
	    }

	    /* tsplib file path/name */
	    if (strcmp(opt, "-f") == 0 ||
	    	strcmp(opt, "--file") == 0) {
	    	/* will ignore other options */
	    	pars->random_instance_option = FALSE;
	    	pars->tsplib_instance_option = TRUE;
	    	pars->tsplib_file_path = argv[++i];
	    }

	    /* heuristic algorithm */
	    if (strcmp(opt, "--heur") == 0) {
	    	if (strcmp(argv[i+1], "ALL") == 0 ||
	    		strcmp(argv[i+1], "all") == 0) {
			   	pars->heur_algo = ALL;
	    	}
	    	if (strcmp(argv[i+1], "NN") == 0 ||
				strcmp(argv[i+1], "nn") == 0) {
			   	pars->heur_algo = NN;
	    	}
	    	i++;
	    }

	    /* solver */
	    if (strcmp(opt, "--solver") == 0) {
	    	if (strcmp(argv[i+1], "BB") == 0 ||
				strcmp(argv[i+1], "bb") == 0) {
			   	pars->solver = BB;
	    	}
	    	if (strcmp(argv[i+1], "CPLEX") == 0 ||
				strcmp(argv[i+1], "cplex") == 0) {
			   	pars->solver = CPLEX;
	    	}
	    	i++;
	    }

	    if (strcmp(opt, "-nt") == 0 ||
	    	strcmp(opt, "--threads") == 0) {
	    	pars->num_threads = atoi(argv[++i]);
	    }

	    /* callbacks option */
	    if (strcmp(opt, "--cplex_callbacks") == 0 ||
	    	strcmp(opt, "--callbacks") == 0 ||
	    	strcmp(opt, "-cb") == 0) {
	    	pars->callbacks_option = TRUE;
	    }

	    if (strcmp(opt, "--proxy") == 0) {
	    	pars->proximity_option = TRUE;
	    }

	    if (strcmp(opt, "--lb") == 0 ||
	        strcmp(opt, "-lb")  == 0   ) {
	    	pars->localbranching_option = TRUE;
	    }

	    if (strcmp(opt, "--hf") == 0 ||
	        strcmp(opt, "-hf")  == 0   ) {
	    	pars->hardfixing_option = TRUE;
	    }

	    if (strcmp(opt, "--rins") == 0) {
	    	pars->rinspolishing_option = TRUE;
	    }

	    /* verbosity level */
	    if (strcmp(opt, "--verb") == 0) {
	    	if (strcmp(argv[i+1], "SILENT") == 0 ||
	    		strcmp(argv[i+1], "silent") == 0 ||
	    		strcmp(argv[i+1], "0") == 0) {
	    		verbosity = SILENT;
	    	}
	    	if (strcmp(argv[i+1], "ESSENTIAL") == 0 ||
				strcmp(argv[i+1], "essential") == 0 ||
				strcmp(argv[i+1], "1") == 0) {
			   	verbosity = ESSENTIAL;
	    	}
	    	if (strcmp(argv[i+1], "USEFUL") == 0 ||
				strcmp(argv[i+1], "useful") == 0 ||
				strcmp(argv[i+1], "2") == 0) {
			   	verbosity = USEFUL;
	    	}
	    	if (strcmp(argv[i+1], "ANNOYING") == 0 ||
				strcmp(argv[i+1], "annoying") == 0 ||
				strcmp(argv[i+1], "4") == 0) {
			   	verbosity = ANNOYING;
	    	}
	    	i++;
	    }

   	} /* end for (parse command line input ) */

	if (pars->random_instance_option == TRUE &&
			pars->random_seed_option == FALSE) {
		srand(pars->seed);
	}
	else {
		srand(time(NULL));
	}
	return 0;

}


/*
 * print_helper_menu
 *
 * print a menu with the list of all the parameters
 * and how to use the sw
 */
void print_helper_menu() {
  printf("\n");
  printf("  TSP solver for the course of Ricerca Operativa 2\n");
  printf("  a.y. 2012/13, taught by prof. M. Fischetti\n\n");
  printf("  authors: Alberto Franzin\n           Ludovico Minto\n\n");
  printf("  Helper menu\n");
  printf("  This software can solve two kind on TSP instances:\n");
  printf("  - instances in TSPLIB format;\n");
  printf("  - random instances.\n\n");
  printf("  There are two ways to use it: set the desidered parameters\n");
  printf("  in the 'config' file provided with the package,\n");
  printf("  or append the desidered parameters to the command:\n");
  printf("  -s [--seed] x   : x (integer) selects the desidered seed\n");
  printf("                    for the random instance, such that:\n");
  printf("                      if x >= 0, then x is the seed;\n");
  printf("                      if x < 0, then a pseudorandom seed will be used.\n");
  printf("  -n [--number] x : x (positive integer) is the number of nodes to be generated\n");
  printf("                    in the random instance.\n");
  printf("  -f [--file] x   : x (valid file/path) is the TSPLIB-formatted file of the instance.\n");
  printf("                    If this option is chosen, then the previous ones\n");
  printf("                    will be ignored, if present.\n");
  printf("  --solver x      : choose the solver method for the problem\n");
  printf("                    x can be:\n");
  printf("                    - BRANCH_AND_BOUND | branch_and_bound | bb\n");
  printf("                    - CPLEX | cplex\n");
  printf("  --heur x        : choose the heuristic algorithm for an upper bound\n");
  printf("                    x can be:\n");
  printf("                    - ALL | all\n");
  printf("                    - NEAREST_NEIGHBOUR | nn | nn\n");
  printf("                    - RANDOM_CYCLE | rc\n");
  printf("  --verb x        : set the verbosity level of infos when debugging\n");
  printf("                    Has no effect in release mode.\n");
  printf("                    x can be:\n");
  printf("                    - SILENT | silent | 0       : no infos printed\n");
  printf("                    - ESSENTIAL | essential | 1 : some infos\n");
  printf("                    - USEFUL | useful | 2       : more infos\n");
  printf("                    - VERBOSE | verbose | 3     : log files\n");
  printf("                    - ANNOYING | annoying | 4   : getchars too\n");
  printf("  --cb            : use cplex callbacks\n");
  printf("  --proxy         : use proximity search matheuristic (uses CPLEX)\n");
  printf("  --lb            : use local branching matheuristic (uses CPLEX)\n");
  printf("  --hf            : use hard fixing\n");
  printf("  --rins          : use cplex RINS+polishing (uses CPLEX)\n");
  printf("  -h [--help]     : printf this menu and exit.\n");
  printf("\n\n");
}
