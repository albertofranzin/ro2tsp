#ifndef __BASE_H
#define __BASE_H

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "../data/graph.h"
#include "../data/egraph.h"

/**
 * base_problem_setup
 * read parameters and obtain the info needed to build the graph
 * 
 * @param  argc number of arguments passed via command line
 * @param  argv arguments passed via command line
 * @return      pointer to parameters object
 */
parameters *base_problem_setup(int argc, char **argv);

/**
 * print_helper_menu
 *
 * print a menu with the list of all the parameters
 * and how to use the sw
 */
void print_helper_menu();

#endif

