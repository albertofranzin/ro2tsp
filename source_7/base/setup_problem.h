#ifndef SETUP_PROBLEM_H_
#define SETUP_PROBLEM_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../base/global.h"
#include "../base/utils.h"

#include "../data/coords.h"
#include "../data/environment.h"
#include "../data/graph.h"
#include "../data/parameters.h"


/**
* @brief				Setup the problem by loading in the environment
* 						data from a random instance.
* @param[in]	pars	problem parameters
* @param[out]	env		problem environment
* @return   			= 0 if no error
* 						< 0 if error
* @pre					pars is supposed to be initialized
* @pre					env is supposed to be initialized
* @post					-
* @note					-
*/
int setup_problem_random(parameters *pars, environment *env);


/**
* @brief				Setup the problem by loading in the environment
* 						data from a TSPLIB instance.
* @param[in]	pars	problem parameters
* @param[out]	env		problem environment
* @return   			= 0 if no error
* 						< 0 if error
* @pre					pars is supposed to be initialized
* * @pre				env is supposed to be initialized
* @post					-
* @note					-
*/
int setup_problem_tsplib(parameters *pars, environment *env);


#endif /* SETUP_PROBLEM_H_ */
