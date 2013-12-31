#ifndef SETUP_PARAMETERS_H_
#define SETUP_PARAMETERS_H_


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../base/global.h"
#include "../base/utils.h"
#include "../data/parameters.h"


/**
* @brief					Set the parameters of the problem, with reference to
* 							the default values specified in constants.h
* 							to the arguments passed by command line.
* @param[out] 	pars		parameters
* @return   				= 0 if no error
* 							< 0 if error
* @pre						pars is supposed to be initialized
* @post						-
* @note						-
*/
int setup_parameters_default(parameters *pars);


/**
* @brief					Set the parameters of the problem, with reference to
* 							the values specified in the configuration file.
* @param[in]	filepath	configuration file path/filename
* @param[out] 	pars		parameters
* @return  				 	= 0 if no error
* 							< 0 if error
* @pre						pars is supposed to be initialized
* @post						-
* @note						-
*/
int setup_parameters_config(char *configPath, parameters *pars);

/**
* @brief					Set the parameters of the problem, with reference to
* 							to the arguments passed by command line.
* @param[in]	argc		number of arguments passed by command line
* @param[in]	argv		arguments passed by command line
* @param[out] 	pars		parameters
* @return 				  	= 0 if no error
* 							< 0 if error
* @pre						pars is supposed to be initialized
* @post						-
* @note						-
*/
int setup_parameters_commandline(int argc, char **argv, parameters *pars);


#endif /* SETUP_PARAMETERS_H_ */
