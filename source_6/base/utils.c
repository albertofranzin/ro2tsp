#include "../base/utils.h"


short par_hash(char* parName, char* parValue) {


	if (strcmp(parName, "RANDOM_INSTANCE_OPTION") == 0 &&
		strcmp(parValue, "TRUE") == 0)
		return 1000;
	if (strcmp(parName, "RANDOM_INSTANCE_OPTION") == 0 &&
		strcmp(parValue, "FALSE") == 0)
		return 1001;


	if (strcmp(parName, "RANDOM_SEED_OPTION") == 0 &&
		strcmp(parValue, "TRUE") == 0)
		return 1100;
	if (strcmp(parName, "RANDOM_SEED_OPTION") == 0 &&
		strcmp(parValue, "FALSE") == 0)
		return 1101;


	if (strcmp(parName, "NUM_VERTICES") == 0)
		return 1200;


	if (strcmp(parName, "SEED") == 0)
		return 1300;


	if (strcmp(parName, "TSPLIB_INSTANCE_OPTION") == 0 &&
		strcmp(parValue, "TRUE") == 0)
		return 1400;
	if (strcmp(parName, "TSPLIB_INSTANCE_OPTION") == 0 &&
		strcmp(parValue, "FALSE") == 0)
		return 1401;


	if (strcmp(parName, "TSPLIB_FILE_PATH") == 0)
		return 1500;


	if (strcmp(parName, "HEUR_ALGO") == 0 &&
		strcmp(parValue, "ALL") == 0)
		return 1600;
	if (strcmp(parName, "HEUR_ALGO") == 0 &&
		strcmp(parValue, "NN") == 0)
		return 1601;


	if (strcmp(parName, "NUM_HEUR_TRIALS") == 0)
		return 1700;


	if (strcmp(parName, "SOLVER") == 0 &&
		strcmp(parValue, "BB") == 0)
		return 1800;
	if (strcmp(parName, "SOLVER") == 0 &&
		strcmp(parValue, "CPLEX") == 0)
		return 1801;


	if (strcmp(parName, "NUM_THREADS") == 0)
		return 1900;


	if (strcmp(parName, "CALLBACKS_OPTION") == 0 &&
		strcmp(parValue, "TRUE") == 0)
		return 2000;
	if (strcmp(parName, "CALLBACKS_OPTION") == 0 &&
		strcmp(parValue, "FALSE") == 0)
		return 2001;


	if (strcmp(parName, "VERBOSITY") == 0 &&
		strcmp(parValue, "SILENT") == 0)
		return 2100;
	if (strcmp(parName, "VERBOSITY") == 0 &&
		strcmp(parValue, "ESSENTIAL") == 0)
		return 2101;
	if (strcmp(parName, "VERBOSITY") == 0 &&
		strcmp(parValue, "USEFUL") == 0)
		return 2102;
	if (strcmp(parName, "VERBOSITY") == 0 &&
		strcmp(parValue, "ANNOYING") == 0)
		return 2103;


	return 0;

}



short tsp_hash(char* parName, char* parValue) {

	if (strcmp(parName, "NAME") == 0)
		return 1000;

	if (strcmp(parName, "TYPE") == 0 &&
		strcmp(parValue, "TSP") == 0) /* only TYPE : TSP is allowed here */
		return 1100;

	if (strcmp(parName, "COMMENT") == 0)
		return 1200;

	if (strcmp(parName, "DIMENSION") == 0)
		return 1300;

	if (strcmp(parName, "EDGE_WEIGHT_TYPE") == 0 &&
		strcmp(parValue, "EUC_2D") == 0)
		return 1400;
	if (strcmp(parName, "EDGE_WEIGHT_TYPE") == 0 &&
		strcmp(parValue, "MAN_2D") == 0)
		return 1401;
	if (strcmp(parName, "EDGE_WEIGHT_TYPE") == 0 &&
		strcmp(parValue, "CEIL_2D") == 0)
		return 1402;
	if (strcmp(parName, "EDGE_WEIGHT_TYPE") == 0 &&
		strcmp(parValue, "EXPLICIT") == 0)
		return 1403;
	if (strcmp(parName, "EDGE_WEIGHT_TYPE") == 0 &&
		strcmp(parValue, "GEO") == 0)
		return 1404;
	if (strcmp(parName, "EDGE_WEIGHT_TYPE") == 0 &&
		strcmp(parValue, "ATT") == 0)
		return 1405;

	if (strcmp(parName, "EDGE_WEIGHT_FORMAT") == 0 &&
		strcmp(parValue, "FULL_MATRIX") == 0)
		return 1500;
	if (strcmp(parName, "EDGE_WEIGHT_FORMAT") == 0 &&
		strcmp(parValue, "LOWER_DIAG_ROW") == 0)
		return 1501;

	if (strcmp(parName, "DISPLAY_DATA_TYPE") == 0 &&
		strcmp(parValue, "COORD_DISPLAY") == 0)
		return 1600;

	if (strcmp(parName, "NODE_COORD_SECTION") == 0)
		return 1700;

	if (strcmp(parName, "DISPLAY_DATA_SECTION") == 0)
		return 1800;

	if (strcmp(parName, "EDGE_WEIGHT_SECTION") == 0)
		return 1900;

	return 0;

}

