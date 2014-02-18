#include "../base/setup_problem.h"


int setup_problem_random(parameters *pars, environment *env) {

	coords_delete(&(env->vertices));
    graph_delete(&(env->main_graph));

    coords_setup(&(env->vertices), pars->num_vertices);
    graph_setup(&(env->main_graph), pars->num_vertices);

	int i, j;

	for (i = 0; i < pars->num_vertices; i++) {

		env->vertices.x[i] = (double)(rand()) / ((double)RAND_MAX + 1.0);
		env->vertices.y[i] = (double)(rand()) / ((double)RAND_MAX + 1.0);

	}

	/* setup index-to-vertices conversion table */
	setup_convtable(pars->num_vertices);

	for (i = 0; i < pars->num_vertices; i++) {
		for (j = i+1; j < pars->num_vertices; j++) {

			graph_insert_edge(&(env->main_graph), get_idx(i, j),
					sqrt(  pow( env->vertices.x[i] - env->vertices.x[j], 2 )
						 + pow( env->vertices.y[i] - env->vertices.y[j], 2 ) ), FREE);

		}
	}

	return 0;

}


int setup_problem_tsplib(parameters *pars, environment *env) {

	FILE *tsplibFile = fopen(pars->tsplib_file_path, "r");
	if (tsplibFile == NULL) perror( pars->tsplib_file_path );
#ifdef DEBUG
    if (tsplibFile == NULL) {
        fprintf(stderr, "Error in /base/setup_problem.c:\n"
                        "function: setup_problem_tsplib:\n"
                        "setup_problem_tsplib : errcode %d\n", 1);
        return -1;
    }
#endif
    char	line[1024];
    char	*parName;  	/* parameter name */
	char	*parValue; 	/* parameter value */
	int		i, j, lineLen;
	int 	haveVertexCoords = FALSE;
	int 	haveEdgeCosts	 = FALSE;


	while(fgets(line, sizeof line, tsplibFile) != NULL) {

		lineLen = strlen(line) - 1;

		/* skip empty lines */
		if(lineLen == 0) continue;
	    if(line[lineLen] == '\n') line[lineLen] = 0;

	    /* skip comments */
	    if(line[0] == '#') continue;

	    // split and strip away the spaces
	    // removing the spaces is necessary in order to use */
	    // the tsp_hash method above */
	    parName 	= strtok(line, ": ");
	    parValue 	= strtok(NULL, ": ");

	    /* manage correctly each parameter */
	    switch (tsp_hash(parName, parValue)) {

	    	case 1000 : /* NAME */
	        break;
	        case 1100 : /* TYPE: TSP */
	        break;
	        case 1200 : /* COMMENT */
	        break;
	        case 1300 : /* DIMENSION */
	        	pars->num_vertices = atoi(parValue);
	        	setup_convtable(pars->num_vertices);
	        break;
	        case 1400 : /* EDGE_WEIGHT_TYPE: EUC_2D */
	        	pars->tsplib_file_format = EUC_2D;
	        break;
	        case 1401 : /* EDGE_WEIGHT_TYPE: MAN_2D */
	        	pars->tsplib_file_format = MAN_2D;
	        break;
	        case 1402 : /* EDGE_WEIGHT_TYPE: CEIL_2D */
	        	pars->tsplib_file_format = CEIL_2D;
	        break;
	        case 1403 : /* EDGE_WEIGHT_TYPE: EXPLICIT */
	        	pars->tsplib_file_format = EXPLICIT;
	        break;
	        case 1404 : /* EDGE_WEIGHT_TYPE: GEO */
	        	pars->tsplib_file_format = GEO;
	        break;
	        case 1405 : /* EDGE_WEIGHT_TYPE: ATT */
	        	pars->tsplib_file_format = ATT;
	        break;
	        case 1500 : /* EDGE_WEIGHT_FORMAT: FULL_MATRIX */
	        	pars->tsplib_matrix_type = FULL_MATRIX;
	        break;
	        case 1501 : /* EDGE_WEIGHT_FORMAT: LOWER_DIAG_ROW */
	        	pars->tsplib_matrix_type = LOWER_DIAG_ROW;
	        break;
	        case 1600 : /* DISPLAY_DATA_TYPE: COORD_DISPLAY */
	        break;
	        case 1700 : /* NODE_COORD_SECTION */
	        case 1800 : /* DISPLAY_DATA_SECTION */
	        {

	        	coords_delete(&(env->vertices));
	            graph_delete(&(env->main_graph));

	            coords_setup(&(env->vertices), pars->num_vertices);
	            graph_setup(&(env->main_graph), pars->num_vertices);

	            const char delimiters[] = " ";
                //char *running;
                char *token1;
                char *token2;
                char *token3;

                while( fgets(line, sizeof line, tsplibFile) != NULL ) {

                	lineLen = strlen(line) - 1;

                    /* skip empty lines */
                    if(lineLen == 0) continue;
                    if(line[lineLen] == '\n') line[lineLen] = 0;

                    /* skip comments */
                    if(line[0] == '#') continue;

                    if (strcmp(line, "EOF") == 0 || strcmp(line, " EOF") == 0) break;

                    token1 = strtok(line, delimiters);
                    token2 = strtok(NULL, delimiters);
                    token3 = strtok(NULL, delimiters);

                    i = atoi(token1) - 1;
                    env->vertices.x[i] = atof(token2);
                    env->vertices.y[i] = atof(token3);

                }

                haveVertexCoords = TRUE;
	        }
            break;
	        case 1900 : /* EDGE_WEIGHT_SECTION */

	        	if (pars->tsplib_matrix_type == FULL_MATRIX) {

	        		// full matrix
                    // each row contains pars->number_of_nodes elements
                    // (or, at least, it should...)
                    // since the assumption is NOT verified, we need
                    // to keep track of the count of read elements
                    // to manually split each row

		        	coords_delete(&(env->vertices));
		            graph_delete(&(env->main_graph));

		            coords_setup(&(env->vertices), pars->num_vertices);
		            graph_setup(&(env->main_graph), pars->num_vertices);

                    int row 				= 0;
                    int cumulative_counter 	= 0;
                    int stop;
                    const char delimiters[] = " ";

                    while (fgets(line, sizeof(line), tsplibFile) != NULL) {
                    	lineLen = strlen(line)-1;

                    	// skip empty lines
                    	if (lineLen == 0) continue;
                    	if (line[lineLen] == '\n') line[lineLen] = 0;

                    	// skip comments
                    	if (line[0] == '#') continue;

                    	if (strcmp(line, "EOF") == 0 ||
                    			strcmp(line, " EOF") == 0) {
                    			break;
                    	}

                    	char *tokens[pars->num_vertices], *tok;
                    	tok = strtok(line, delimiters);

                    	// very dirty
                    	tokens[cumulative_counter] = tok;
                    	cumulative_counter++;
                    	stop = 0;
                    	while (!stop) {
                    		tok = strtok(NULL, delimiters);
                    		if (tok == NULL) {
                    			stop = 1;
                    		}
                    		else {
                    			tokens[cumulative_counter] = tok;
                    			cumulative_counter++;
                    		}
                    	}

                    	if (cumulative_counter >= pars->num_vertices) {
                    		row++;
                    		cumulative_counter %= pars->num_vertices;

                    		for (j = 0; j < row; ++j) {
                    			if (row-1 != j && row <= pars->num_vertices &&
                    					j < pars->num_vertices) {
                    				graph_insert_edge(&(env->main_graph), get_idx(row-1, j), atof(tokens[j]), FREE);
                    			}
                    		}
                    	}
                    }

                    haveVertexCoords = FALSE;
                    break;
	        	}
	        	if (pars->tsplib_matrix_type == LOWER_DIAG_ROW) {

	        		// lower diagonal row matrix
                    // things are (surprisingly) easier here,
                    // since we have to count the read elements,
                    // and manually create the rows.

		        	coords_delete(&(env->vertices));
		            graph_delete(&(env->main_graph));

		            coords_setup(&(env->vertices), pars->num_vertices);
		            graph_setup(&(env->main_graph), pars->num_vertices);

                    int row = 0;
                    int pos = 0;
                    const char delimiters[] = " ";

                    while( fgets(line, sizeof line, tsplibFile) != NULL ) {

                    	lineLen = strlen(line)-1;

                    	// skip empty lines
                    	if(lineLen == 0) continue;
                    	if(line[lineLen] == '\n') line[lineLen] = 0;

                    	// skip comments
                    	if(line[0] == '#') continue;

                    	if (strcmp(line, "EOF") == 0 ||
                    		strcmp(line, " EOF") == 0) {
                    		break;
                    	}

                    	char *tok;
                    	tok = strtok(line, delimiters);

                    	while (tok != NULL) {
                    		if (row != pos && row < pars->num_vertices &&
                    				pos < pars->num_vertices) {
                    				// <- eliminare i controlli row,pos<number_of_nodes ?
                    				graph_insert_edge(&(env->main_graph), get_idx(row, pos), atof(tok), FREE);
                    		}
                    		pos++;
                    		if (atof(tok) == 0.0) {
                    			row++;
                    			pos = 0;
                    		}
                    		tok = strtok(NULL, delimiters);
                    	}
                    }

                    haveVertexCoords = FALSE;
                    break;

	        	}

	        	haveEdgeCosts = TRUE;

	        break;
	        default :
#ifdef DEBUG
	        	fprintf(stderr, "Error in /base/setup_problem.c:\n"
	        			"function: setup_problem_tsplib:\n"
                        "setup_problem_tsplib : errcode %d\n", 2);
		        fprintf(stderr, "Info:\n"
		        		"No support for this case\n"
		        		"parName = %s : parValue = %s\n", parName, parValue);
	        	fclose(tsplibFile);
	        	return -2;
#endif
        	break;

	    } /* end switch */
	} /* end while loop */

    fclose(tsplibFile);


	if (haveVertexCoords == TRUE && haveEdgeCosts == FALSE) {

		double x1, y1, x2, y2;

		/* intermediate values for the 'ATT' distances */
		double xd, yd, rij, tij;

		/* intermediate values for the 'GEO' distances */
		double q1, q2, q3, fract, latitude1, longitude1, latitude2, longitude2;

		for (i = 0; i < pars->num_vertices; i++) {

	        x1 = env->vertices.x[i];
	        y1 = env->vertices.y[i];

	        if (pars->tsplib_file_format == GEO) {
	        	fract      = x1 - (int)x1;
	        	latitude1  = PI * ((int)x1 + 5.0 * fract / 3.0) / 180.0;
	        	fract      = y1 - (int)y1;
	        	longitude1 = PI * ((int)y1 + 5.0 * fract / 3.0) / 180.0;
	        }

	        for (j = i+1; j < pars->num_vertices; j++) {

	        	x2 = env->vertices.x[j];
		        y2 = env->vertices.y[j];

		        if (pars->tsplib_file_format == GEO) { 		/* GEOGRAPHIC */
		        	fract      = x2 -  (int)x2;
		            latitude2  = PI * ((int)x2 + 5.0 * fract / 3.0) / 180.0;
		            fract      = y2 -  (int)y2;
		            longitude2 = PI * ((int)y2 + 5.0 * fract / 3.0) / 180.0;
		            q1 = cosf(longitude1 - longitude2);
		            q2 = cosf(latitude1  - latitude2);
		            q3 = cosf(latitude1  + latitude2);
		        	graph_insert_edge(&(env->main_graph), get_idx(i, j),
		        			(EARTH_RADIUS * acosf(0.5*((1.0+q1)*q2 - (1.0-q1)*q3) ) + 1.0),
		        			FREE);
		        }


		        if (pars->tsplib_file_format == EUC_2D) {	/* EUCLIDEAN 2D */
		        	graph_insert_edge(&(env->main_graph), get_idx(i, j),
		        			round(sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2))),
		        			FREE);

		        	/* or 'nearbyint' instead of 'round' */

		        }
		        if (pars->tsplib_file_format == MAN_2D) {	/* MANHATTAN 2D */
		        	graph_insert_edge(&(env->main_graph), get_idx(i, j),
		        			(fabs(x1 - x2) + fabs(y1 - y2)),
		        			FREE);
		        }
		        if (pars->tsplib_file_format == CEIL_2D) {	/* CEIL 2D */
		        	graph_insert_edge(&(env->main_graph), get_idx(i, j),
		        			ceil(sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2))),
		        			FREE);
		        }
		        if (pars->tsplib_file_format == ATT) {		/* ATT PSEUDO-EUCLIDEAN 2D */
		        	xd  = x1 - x2;
		        	yd  = y1 - y2;
		        	rij = sqrt((xd*xd + yd*yd) / 10.0);
		        	tij = nearbyint(rij);

		            if (tij < rij) tij = tij + 1;

		        	graph_insert_edge(&(env->main_graph), get_idx(i, j),
		        			tij,
		        			FREE);
		        }

	        } /* end for j */
		} /* end for i */
	}
	else if (haveVertexCoords == FALSE && haveEdgeCosts == TRUE ) {
		;
	}
#ifdef DEBUG
	if (haveVertexCoords == FALSE && haveEdgeCosts == FALSE) {
		fprintf(stderr, "Error in /base/setup_problem.c:\n"
                        "function: setup_problem_tsplib:\n"
                        "setup_problem_tsplib : errcode %d\n", 3);
		return -3;
	}
#endif

	return 0;

}

































