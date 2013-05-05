#ifndef __BASE_H
#define __BASE_H

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "../data/graph.h"
#include "../data/egraph.h"

/*
 * base_problem_setup
 * - int     : number of arguments passed via command line
 * - char ** : arguments passed via command line
 *
 * read parameters and obtain the info needed to build the graph
 *
 * return : pointer parameters object
 */
parameters *base_problem_setup(int, char **);

/*
 * print_helper_menu
 *
 * print a menu with the list of all the parameters
 * and how to use the sw
 */
void print_helper_menu();

#endif
