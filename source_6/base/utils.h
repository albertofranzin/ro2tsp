#ifndef UTILS_H_
#define UTILS_H_


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../base/global.h"


/**
* @brief				Map each valid combination of parameter
* 						name and value (parName, parValue)
* 						into a unique integer code.
* @param[in] parName	parameter name
* @param[in] parValue	parameter value
* @return   			the code associated to the couple, if no error
* 						< 0 if error
* 						parameter name and value (parName, parValue)
* @pre					-
* @post					-
* @note					-
*/
short par_hash(char* parName, char* parValue);


/**
* @brief				Map each valid combination of parameter
* 						name and value (parName, parValue)
* 						into a unique integer code.
* @param[in] parName	parameter name
* @param[in] parValue	parameter value
* @return   			the code associated to the couple, if no error
* 						< 0 if error
* 						parameter name and value (parName, parValue)
* @pre					-
* @post					-
* @note					-
*/
short tsp_hash(char* parName, char* parValue);


#endif /* UTILS_H_ */
