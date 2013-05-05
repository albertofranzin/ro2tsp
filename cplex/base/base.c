#include "base.h"

/*
 * base_problem_setup
 * - int     : number of arguments passed via command line
 * - char ** : arguments passed via command line
 *
 * read parameters and obtain the info needed to build the graph
 *
 * return : pointer parameters object
 */
parameters *base_problem_setup(int argc, char **argv) {
  int        i;
  char       *opt;
  parameters *pars = getParameters();

  // parse command line input
  for (i = 1; i < argc; i++) {

    opt = argv[i];

    if (strcmp(opt, "-n") == 0 || strcmp(opt, "--number") == 0)
      pars->number_of_nodes = atoi(argv[++i]);

    if (strcmp(opt, "-s") == 0 || strcmp(opt, "--seed") == 0)
      pars->seed = atol(argv[++i]);

    if (strcmp(opt, "-f") == 0 || strcmp(opt, "--file") == 0) {
      // will ignore other options
      pars->tsp_file_option = TRUE;
      pars->random_instance_option = FALSE;
      pars->tsp_file = argv[++i];
    }

    if (strcmp(opt, "-h") == 0 || strcmp(opt, "--heur") == 0) {
    }

    if (strcmp(opt, "--solver") == 0) {
    }

    if (strcmp(opt, "-h") == 0 || strcmp(opt, "--help") == 0) {
      // print help and exit
      print_helper_menu();
      return 0;
    }

  }

  if (pars->random_seed_option == FALSE || pars->seed >= 0) {
      srand(pars->seed);
  } else {
      srand(time(NULL));
  }

  return pars;
}

/*
 * print_helper_menu
 *
 * print a menu with the list of all the parameters
 * and how to use the sw
 */
void print_helper_menu() {
  printf("\n");
  printf("  TSP solver for the course of Ricerca Operativa 2\n");
  printf("  a.y. 2012/13, taught by prof. M. Fischetti\n\n");
  printf("  authors: Alberto Franzin\n         Ludovico Minto\n\n");
  printf("  Helper menu\n");
  printf("  This software can solve two kind on TSP instances:\n");
  printf("  - instances in TSPLIB format;\n");
  printf("  - random instances.\n\n");
  printf("  There are two ways to use it: set the desidered parameters\n");
  printf("  in the 'config' file provided with the package,\n");
  printf("  or append the desidered parameters to the command:\n");
  printf("  -s [--seed] x   : x (integer) selects the desidered seed\n");
  printf("                    for the random instance, such that:\n");
  printf("                      if x >= 0, then x is the seed;\n");
  printf("                      if x < 0, then a pseudorandom seed will be used.\n");
  printf("  -n [--number] x : x (positive integer) is the number of nodes to be generated\n");
  printf("                    in the random instance.\n");
  printf("  -f [--file]   x : x (valid file/path) is the TSPLIB-formatted file of the instance.\n");
  printf("                    If this option is chosen, then the previous ones\n");
  printf("                    will be ignored, if present.\n");
  printf("  -h [--help]     : printf this menu and exit.\n");
  printf("\n\n");
}
