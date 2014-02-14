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
	pars->callbacks_option 			= DEFAULT_CALLBAKS_OPTION;
	verbosity 						= DEFAULT_VERBOSITY;
	return 0;

}


int setup_parameters_config(char *configPath, parameters *pars) {

	FILE	*configFile = fopen(configPath, "r");
	if (configFile == NULL)
		perror( configPath );
#ifdef DEBUG
    if (configFile == NULL) {
        fprintf(stderr, "Error in /base/setup_parameters.c:\n"
                        "function: setup_parameters_config:\n"
                        "setup_parameters_config : errcode %d\n", 1);
        return -1;
    }
#endif
	char	line[256];
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
			case 2100 : /* VERBOSITY = SILENT */
				verbosity = SILENT;
			break;
			case 2101 : /* VERBOSITY = ESSENTIAL */
				verbosity = ESSENTIAL;
			break;
			case 2102 : /* VERBOSITY = USEFUL */
				verbosity = USEFUL;
			break;
			break;
			case 2103 : /* VERBOSITY = ANNOYING */
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

	    /* callbacks option */
	    if (strcmp(opt, "--cplex_callbacks") == 0 ||
	    	strcmp(opt, "--callbacks") == 0 ||
	    	strcmp(opt, "-cb") == 0) {
	    	pars->callbacks_option = TRUE;
	    }

	    if (strcmp(opt, "-nt") == 0 ||
	    	strcmp(opt, "--threads") == 0) {
	    	pars->num_threads = atoi(argv[++i]);
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
