#include "utils.h"

void initializeRandom(unsigned int seed) {
	unsigned int sd = seed;
	if (sd == 0) {
		time_t now = time ( 0 ); 
		unsigned char *p = (unsigned char *)&now;
		sd = 0;
		size_t i;

		for ( i = 0; i < sizeof now; i++ ) 
			sd = sd * ( UCHAR_MAX + 2U ) + p[i];
	}
	srand(sd);
	printf("utils.c :: initializeRandom :: srand done\n");
}

/*
 * parHash
 * parName : the string
 *
 * 'manual' hashing for the parameter name, to fill in the parameter list
 * the values are hard-coded here
 * Maybe ugly, but easy.
 *
 * return : an integer associated to the parameter
 */
short parHash(char *parName) {
	if (strcmp(parName, "NUMBER_OF_NODES") == 0)
		return 0;
	if (strcmp(parName, "SEED") == 0)
		return 1;
	if (strcmp(parName, "PLOT") == 0)
		return 2;
	if (strcmp(parName, "PLOT_ONLY_1TREE") == 0)
		return 3;
	return -1;
}

// read parameters from config file
// default values for parameters are set here
parameters *getParameters() {
	parameters *pars = malloc(sizeof(parameters));
	memset(pars, 0, sizeof(parameters));

	pars->no_of_nodes = 10;
	pars->seed = 0;
	pars->plot = 1;
	pars->plotOnlyTree = 0;

	FILE *parFile = fopen(FILE_CONFIG, "r");
	long i = 0, j;
	char line[128];
	char *p1, *p2;
	int lineLen;

	if ( parFile != NULL ) {
		while( fgets(line, sizeof line, parFile) != NULL ){
			lineLen = strlen(line)-1;

			// skip empty lines
			if(lineLen == 0) continue;
			if(line[lineLen] == '\n') line[lineLen] = 0;

			// skip comments
			if(line[0] == '#') continue;

			// split and strip away the spaces
			// removing the spaces is necessary in order to use
			// the parHash() method above
			p1 = strtok(line, "= ");
			p2 = strtok(NULL, "= ");

			if(p2 != NULL) {
				// manage correctly each parameter
				switch(parHash(p1)) {
					case 0 : pars->no_of_nodes = atoi(p2);
							 break;
					case 1 : pars->seed = atoi(p2);
							 break;
					case 2 : if (p2[0] == 'y' || p2[0] == 'Y')
							 	pars->plot = 1;
							 else
							 	pars->plot = 0;
							 break;
					case 3 : if (p2[0] == 'y' || p2[0] == 'Y')
							 	pars->plotOnlyTree = 1;
							 else
							 	pars->plotOnlyTree = 0;
							 break;

					default: break;
				}
			}
		}
		fclose(parFile);
	} else {
		/* errore nell'apertura del file */
		perror( FILE_CONFIG );
	}

	return pars;
}
