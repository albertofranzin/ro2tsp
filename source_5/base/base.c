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
  int         i;
  char       *opt;
  parameters *pars = getParameters();

  // parse command line input
  for (i = 1; i < argc; i++) {

    opt = argv[i];

    if (strcmp(opt, "-h") == 0 || strcmp(opt, "--help") == 0) {
      // print help and exit
      print_helper_menu();
      exit(0);
    }

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

    if (strcmp(opt, "--heur") == 0) {
      if (strcmp(argv[i+1], "ALL") == 0 ||
          strcmp(argv[i+1], "all") == 0                  ) {
        pars->heuristic_algo = ALL;
      } else if (strcmp(argv[i+1], "NEAREST_NEIGHBOUR") == 0 ||
                 strcmp(argv[i+1], "nn")                == 0   ) {
        pars->heuristic_algo = NEAREST_NEIGHBOUR_2_OPT;
      } else if (strcmp(argv[i+1], "RANDOM_CYCLE") == 0 ||
                 strcmp(argv[i+1], "rc")           == 0   ) {
        pars->heuristic_algo = RANDOM_CYCLES_2OPT;
      }
      i++;
    }

    if (strcmp(opt, "--solver") == 0) {
      if (strcmp(argv[i+1], "BRANCH_AND_BOUND") == 0 ||
          strcmp(argv[i+1], "branch_and_bound") == 0 ||
          strcmp(argv[i+1], "bb") == 0                 ) {
        pars->solver = BRANCH_AND_BOUND;
      } else if (strcmp(argv[i+1], "CPLEX") == 0 ||
                 strcmp(argv[i+1], "cplex") == 0   ) {
        pars->solver = CPLEX;
      }
      i++;
    }

    if (strcmp(opt, "--verb") == 0) {
      if (strcmp(argv[i+1], "SILENT") == 0 ||
          strcmp(argv[i+1], "silent") == 0 ||
          strcmp(argv[i+1], "0") == 0        ) {
        pars->verbosity = SILENT;
      } else if (strcmp(argv[i+1], "ESSENTIAL") == 0 ||
                 strcmp(argv[i+1], "essential") == 0 ||
                 strcmp(argv[i+1], "1") == 0           ) {
        pars->verbosity = ESSENTIAL;
      } else if (strcmp(argv[i+1], "USEFUL") == 0 ||
                 strcmp(argv[i+1], "useful") == 0 ||
                 strcmp(argv[i+1], "2") == 0        ) {
        pars->verbosity = USEFUL;
      } else if (strcmp(argv[i+1], "VERBOSE") == 0 ||
                 strcmp(argv[i+1], "verbose") == 0 ||
                 strcmp(argv[i+1], "3") == 0         ) {
        pars->verbosity = VERBOSE;
      } else if (strcmp(argv[i+1], "ANNOYING") == 0 ||
                 strcmp(argv[i+1], "annoying") == 0 ||
                 strcmp(argv[i+1], "4") == 0          ) {
        pars->verbosity = ANNOYING;
      }
      i++;
    }

    if (strcmp(opt, "--cplex_callbacks") == 0 ||
        strcmp(opt, "--callbacks")       == 0 ||
        strcmp(opt, "--cb")              == 0 ||
        strcmp(opt, "-cb")               == 0   ) {
      pars->cplex_callbacks = TRUE;
    }

    if (strcmp(opt, "--proxy") == 0)
      pars->use_proximity = TRUE;

    if (strcmp(opt, "--rins") == 0)
      pars->use_rinspolishing = TRUE;

    if (strcmp(opt, "--hf") == 0 ||
        strcmp(opt, "-hf")  == 0   )
      pars->use_hardfixing = TRUE;

    if (strcmp(opt, "--lb") == 0 ||
        strcmp(opt, "-lb")  == 0   )
      pars->use_localbranching = TRUE;

    if (strcmp(opt, "-nt") == 0 || strcmp(opt, "--threads") == 0)
      pars->threads = atoi(argv[++i]);

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
  printf("  authors: Alberto Franzin\n           Ludovico Minto\n\n");
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
  printf("  -f [--file] x   : x (valid file/path) is the TSPLIB-formatted file of the instance.\n");
  printf("                    If this option is chosen, then the previous ones\n");
  printf("                    will be ignored, if present.\n");
  printf("  --solver x      : choose the solver method for the problem\n");
  printf("                    x can be:\n");
  printf("                    - BRANCH_AND_BOUND | branch_and_bound | bb\n");
  printf("                    - CPLEX | cplex\n");
  printf("  --heur x        : choose the heuristic algorithm for an upper bound\n");
  printf("                    x can be:\n");
  printf("                    - ALL | all\n");
  printf("                    - NEAREST_NEIGHBOUR | nn | nn\n");
  printf("                    - RANDOM_CYCLE | rc\n");
  printf("  --verb x        : set the verbosity level of infos when debugging\n");
  printf("                    Has no effect in release mode.\n");
  printf("                    x can be:\n");
  printf("                    - SILENT | silent | 0       : no infos printed\n");
  printf("                    - ESSENTIAL | essential | 1 : some infos\n");
  printf("                    - USEFUL | useful | 2       : more infos\n");
  printf("                    - VERBOSE | verbose | 3     : log files\n");
  printf("                    - ANNOYING | annoying | 4   : getchars too\n");
  printf("  --cb            : use cplex callbacks\n");
  printf("  --proxy         : use proximity search matheuristic (uses CPLEX)\n");
  printf("  --lb            : use local branching matheuristic (uses CPLEX)\n");
  printf("  --hf            : use hard fixing\n");
  printf("  --rins          : use cplex RINS+polishing (uses CPLEX)\n");
  printf("  -h [--help]     : printf this menu and exit.\n");
  printf("\n\n");
}
