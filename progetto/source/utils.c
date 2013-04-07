#include "utils.h"

/*inline int atPosition(int i, int j) {
	if (i > j) {
		return (i * (i-1)) / 2 + j;
	} else {
		return (j * (j-1)) / 2 + i;
	}
}*/

unsigned long initializeRandom(unsigned int seed) {
	unsigned long sd = seed;
	if (sd == 0) {
		time_t now = time ( 0 ); 
		unsigned char *p = (unsigned char *)&now;
		sd = 0;
		size_t i;

		for ( i = 0; i < sizeof now; i++ ) 
			sd = sd * ( UCHAR_MAX + 2U ) + p[i];
	}
	srand(sd);
	//printf("utils.c :: initializeRandom :: srand done\n");
	return sd;
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
	if (strcmp(parName, "HEURISTIC_TRIALS") == 0)
		return 4;
	if (strcmp(parName, "TSP_FILE") == 0) {
		return 5;
	}
	return -1;
}

// read parameters from config file
// default values for parameters are set here
parameters *getParameters() {
	parameters *pars = malloc(sizeof(parameters));
	//memset(pars, 0, sizeof(parameters));

	pars->no_of_nodes = 10;
	pars->seed = 0;
	pars->plot = 1;
	pars->plotOnlyTree = 0;
	pars->heuristic_trials = 1;
	pars->tsp_file = NULL;

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
					case 4 : pars->heuristic_trials = atoi(p2);
							 break;
					case 5 : if (strcmp(p2, "NULL") != 0) {
								printf("%s\n", p2);
								pars->tsp_file = malloc(sizeof(char) * strlen(p2));
								strcpy(pars->tsp_file, p2);
								printf("%s\n", pars->tsp_file);
							 }
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

short tspHash(char *parName, char *parValue) {
	if (strcmp(parName, "NAME") == 0)
		return 0;

	if (strcmp(parName, "TYPE") == 0) {
		// only TYPE: TSP is allowed here
		if (strcmp(parValue, "TSP") == 0) {
			return 1;
		} else {
			return -1;
		}
	}

	if (strcmp(parName, "COMMENT") == 0)
		return 2;

	if (strcmp(parName, "DIMENSION") == 0)
		return 3;

	if (strcmp(parName, "EDGE_WEIGHT_TYPE") == 0) {
		// only these 4 kinds of 2D distances are considered
		if (strcmp(parValue, "EUC_2D") == 0) {
			return 41;
		} else if (strcmp(parValue, "MAN_2D") == 0) {
			return 42;
		} else if (strcmp(parValue, "CEIL_2D") == 0) {
			return 43;
		} else if (strcmp(parValue, "GEO") == 0) {
			return 44;
		} else {
			return -1;
		}
	}

	if (strcmp(parName, "DISPLAY_DATA_TYPE") == 0) {
		if (strcmp(parValue, "COORD_DISPLAY") == 0) {
			return 51;
		} else {
			return -1;
		}
	}

	if (strcmp(parName, "NODE_COORD_SECTION") == 0)
		return 61;

	if (strcmp(parName, "DISPLAY_DATA_SECTION") == 0)
		return 62;

	return -1;
}

/*
 * read_tsp_from_file
 * - G : graph to be filled in
 * - ppars : parameters (some may be overridden)
 *
 * will override parameters, if needed
 *
 * Assumes file is formatted in the right way.
 * Does not guarantee correct working if this is not true.
 * Flexibility = 0!
 */
void read_tsp_from_file(graph *G, parameters *pars) {

	//parameters *pars = (parameters *)ppars;

	printf("--> %s\n", pars->tsp_file);

	FILE *tspFile = fopen(pars->tsp_file, "r");
	long i = 0, j;
	char line[128];
	char *p1, *p2;
	int lineLen;
	short read = 1;

	deleteGraph(G);

	printf("oooh shit\n");

	if (tspFile != NULL) {
		while( fgets(line, sizeof line, tspFile) != NULL ) {
			lineLen = strlen(line)-1;

			// skip empty lines
			if(lineLen == 0) continue;
			if(line[lineLen] == '\n') line[lineLen] = 0;

			// skip comments
			if(line[0] == '#') continue;

			// split and strip away the spaces
			// removing the spaces is necessary in order to use
			// the parHash() method above
			p1 = strtok(line, ": ");
			p2 = strtok(NULL, ": ");

			// manage correctly each parameter
			switch(tspHash(p1, p2)) {
				case 0 : break;
				case 1 : break;
				case 2 : break;
				case 3 : pars->no_of_nodes = atoi(p2);
						 printf("updated no_of_nodes : %d\n", pars->no_of_nodes);
						 break;

				case 41: // ?
				case 42: // what to do?
				case 43: // nothing, for now
				case 44: break;

				case 51: break;

				case 61:
				case 62: initGraph(G, pars->no_of_nodes);
						 //i = 0;
						 printf("graph initialized\n");
						 while( fgets(line, sizeof line, tspFile) != NULL ) {
						 	printf("doin' anything?\n");
							lineLen = strlen(line)-1;

							// skip empty lines
							if(lineLen == 0) continue;
							if(line[lineLen] == '\n') line[lineLen] = 0;

							// skip comments
							if(line[0] == '#') continue;

							if (strcmp(line, "EOF") == 0 ||
								strcmp(line, " EOF") == 0) {
								printf("get out!\n");
								break;
							}

							char *running, *token1, *token2, *token3;
							const char delimiters[] = " ";

							// token = strtok(&running, delimiters);
							//printf("abababab\n");
							//strcpy(running, line);
							//printf("cdcdcdcd\n");
							token1 = strtok(line, delimiters);
							//printf("efefefef\n");
							token2 = strtok(NULL, delimiters);
							//printf("ghghghgh\n");
							token3 = strtok(NULL, delimiters);

							printf("%s %s %s\n", token1, token2, token3);

							j = atoi(token1) - 1;
							G->V[j].x = atof(token2);
							G->V[j].y = atof(token3);

						 }
						 printf("here?\n");
						 break;

				default: break;
			}
		}
		fclose(tspFile);
	} else {
		perror(pars->tsp_file);
	}

	printf("graph filled, exiting\n");

	for (i = 0; i < pars->no_of_nodes; i++) {
		for (j = i+1; j < pars->no_of_nodes; j++) {
			(*G).E[ j*(j+1) / 2 + i].flag = 1;
			(*G).E[ j*(j+1) / 2 + i].cost = sqrt(pow( (*G).V[i].x - (*G).V[j].x, 2 ) + pow( (*G).V[i].y - (*G).V[j].y, 2 ));
		}
	}
}

/*
 * snbdComp
 *const void * a,b : the two elements to be compared
 *
 * comparison function to be passed to qsort()
 *
 * return: -1,0,1 depending on the comparison of the two elements
 */
//int snbdComp(const void *aa, const void *bb) {
int snbdComp(node *a, node *b) {
	/*node a = aa,
		 b = bb;*/

	if (a->deg < b->deg) { return -1; }
	if (a->deg > b->deg) { return 1; }
	return 0;
}

// sort edges by weight
/*int sebwComp(const void *aa, const void *bb) {
	edge **a = (edge **)aa,
		 **b = (edge **)bb;

	if ((*a)->weight < (*b)->weight) { return -1; }
	if ((*a)->weight > (*b)->weight) { return 1; }
	return 0;
}*/

/*void appendDouble(double **nnl, double n, int pos) {
	double *nl = (double *)(*nnl);

	double *tmp = realloc(nl, sizeof(nl) + sizeof(double)); 
	if (tmp != NULL) {
		memcpy(&(tmp), &(nl), sizeof(nl));
		// beware : 'pos' correctness is not checked!
		tmp[pos] = n;
		nl = tmp;
	} else {
		fprintf(stderr, "memory allocation failed - smthg very close to a segfault [evil laugh] \n");
		exit(1);
	}
}*/
