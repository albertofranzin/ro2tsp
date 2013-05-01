/* Name:       tsp.c
** Author:     Leo Liberti
** Purpose:    solving the TSP with cycle breaking constraint generation:
**             code skeleton for didactical use
** Source:     C
** History:    060311 work started
*/

// standard include files
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ilcplex/cplex.h>
#include <assert.h>

// maximum length of variable/constraint names
#define MAXNAME 64
#define INFINITY 1e30
#define EPSILON 1e-6
#define MAXITN 10
#define TRUE 1
#define FALSE 0

// sparsify constraint format and add it to CPLEX problem
void AddConstraint(CPXENVptr env, CPXLPptr lp, int nz, 
       double* coeffs, int* ind, char sense, double rhs) {
  int status = 0;
  // add constraints in row-wise sparse format
  int rmatbeg[2];
  // nonzeroes stored in rmatval starting at position:
  rmatbeg[0] = 0;
  // nonzeroes stored in rmatval ending at position:
  rmatbeg[1] = nz-1;
  // create the constraints
  printf("aaaaa\n");
  status = CPXaddrows(env, lp, 0, 1, nz, &rhs, &sense, rmatbeg, ind, coeffs, NULL, NULL);
  printf("uuuuu\n");
  if (status != 0) {
    fprintf(stderr, "pooling-slp: could not create constraints, error %d\n", 
      status);
    exit(3);
  }
}

// take row, column and width and produce a one-dimensional index
int Flatten2DIndices(int i, int j, int m) {
  return i*m + j;
}



int main(int argc, char** argv) {

  // return code
  int ret = 0;
  // status code returned by cplex callable library functions
  int status = 0;
  // cplex environment to pass to/from cplex callable library
  CPXENVptr env = NULL;
  // partial formulation
  CPXLPptr  ip = NULL;
  // use for storing error messages returned by CPLEX
  char errmsg[1024];
  // counters
  int i, j, k;
  // number of vertices in the graph
  int m;  
  // number of variables in the problem (=m^2)
  int n;

  // read std input
  fprintf(stdout, "input number of vertices: ");
  fscanf(stdin, "%d", &m);
  fprintf(stdout, "\n");
  n = m*m;
  if (m < 2) {
    fprintf(stdout, "error: empty graph\n");
    exit(2);
  }

  // distance matrix
  double** D = (double**) malloc(m * sizeof(double*));
  float v;
  for(i = 0; i < m-1; i++) {
    fprintf(stdout, "input distances (%d,j) with j>%d\n", i, i);
    D[i] = (double*) malloc(m * sizeof(double));
    D[i][i] = INFINITY;
    for(j = i + 1; j < m; j++) {
      fscanf(stdin, "%f", &v);
      D[i][j] = (double) v;
    }
  }
  D[m-1] = (double*) malloc(m * sizeof(double));
  // make distance matrix symmetric
  for(i = 0; i < m - 1; i++) {
    for(j = i + 1; j < m; j++) {
      D[j][i] = D[i][j];
    }
  }

  printf("yo\n");

#ifndef DEBUG
  // print distance matrix
  fprintf(stderr, "distance matrix is:\n");
  for(i = 0; i < m; i++) {
    for(j = 0; j < m; j++) {
      if (i == j) {
        fprintf(stderr, " inf");
      } else {
        fprintf(stderr, " %.2f", D[i][j]);
      }
    }
    fprintf(stderr, "\n");
  }
#endif

  printf("yo yo\n");

  // flatten to costs
  double* c = (double*) malloc(n * sizeof(double));
  k = 0;
  for(i = 0; i < m; i++) {
    for(j = 0; j < m; j++) {
      c[k] = D[i][j];
      k++;
    }
  }

  printf("yo yo ma\n");

  // initialize the CPLEX environment 
  env = CPXopenCPLEX(&status);
  if (env == NULL) {
    fprintf(stderr, "%s: could not open CPLEX environment\n", argv[0]);
    CPXgeterrorstring (env, status, errmsg);
    fprintf (stderr, "%s", errmsg);
    exit(2);
  }
  // turn on output to screen
  status = CPXsetintparam(env, CPX_PARAM_SCRIND, CPX_OFF);
  status = CPXsetintparam(env, CPX_PARAM_MIPDISPLAY, 2);
  // create the problem
  ip = CPXcreateprob(env, &status, "tsp");
  if (ip == NULL) {
    fprintf(stderr, "%s, failed to create problem\n", argv[0]);
    exit(4);
  }

  printf("ma ma yo\n");

  // variable lower bounds 
  double* xL = (double*) malloc(n * sizeof(double));
  // variable upper bounds
  double* xU = (double*) malloc(n * sizeof(double));
  // variable types
  char* ctype = (char*) malloc(n * sizeof(char));
  for(j = 0; j < n; j++) {
    xL[j] = 0;
    xU[j] = 1;
    ctype[j] = 'B';  // variables are binary
  }

  printf("ma yo\n");

  // TODO:
  // create the variables

  // add constraints in ip
  double* coeffs = (double*) malloc(n * sizeof(double));
  int* indices = (int*) malloc(n * sizeof(int));
  char sense;
  double rhs;
  int nz = 0;

  printf("nz\n");

  // constraints 1: for all i, sum_j x_{ij} = 1
  for(i = 0; i < m; i++) {
    nz = 0;
    for(j = 0; j < m; j++) {
      if (i != j) {
        coeffs[nz] = 1.0;
        printf("%d %d\n", i, j);
        indices[nz] = Flatten2DIndices(i,j,m);
        nz++;
      }
    }
    sense = 'E';  // equation
    rhs = 1.0;    // =1
    printf("adding constraint %d\n", i);
    AddConstraint(env, ip, nz, coeffs, indices, sense, rhs);
    printf("constraint added %d\n", i);
  }

  printf("yayo\n");

  // TODO:
  // constraints 2: for all j, sum_i x_{ij} = 1
  for(j = 0; j < m; j++) {
    // (compute nz, coeffs, indices, sense, rhs)
    AddConstraint(env, ip, nz, coeffs, indices, sense, rhs);
  }

  printf("ya\n");

  // select LP optimization algorithm
  status = CPXsetintparam(env, CPX_PARAM_LPMETHOD, CPX_ALG_AUTOMATIC);
  if (status != 0) {
    fprintf(stderr, "%s: could not select optimization algorithm, error %d\n", 
      argv[0], status);
    exit(3);
  }

  printf("hey\n");

  // do it!
  fprintf(stdout, "************ TSP: algorithmic progress ****************\n");
  int termination = FALSE;
  double objval;
  double* x = (double*) malloc(n * sizeof(double));
  int currentvertex;
  int* successor = (int*) malloc(m * sizeof(int));
  int* cycle = (int*) malloc(m * sizeof(int));
  int solstat = 0;
  int itncount = 1;
  while(!termination) {

    // optimize 
    status = CPXmipopt(env, ip);
    if (status != 0) {
      fprintf(stderr, "%s: failed to call optimization algorithm on ip\n", 
	      argv[0]);
      exit(10);
    }
    solstat = CPXgetstat(env, ip);
    if (solstat != 101) {
      fprintf(stdout, "  couldn't find optimal solution, status = %d\n", 
	      solstat);
      termination = TRUE;
      break;
    }
    // get ip solution
    for(i = 0; i < n; i++) {
      x[i] = 0;
    }
    CPXgetmipx(env, ip, x, 0, n - 1);
    CPXgetmipobjval(env, ip, &objval);
    for(i = 0; i < n; i++) {
      x[i] = rint(x[i]);
    }
    fprintf(stdout, "iteration %d: current solution with cost %.2f:\n", 
	    itncount, objval);

    // TODO:
    // verify if there are disjoint cycles
    // (compute successors)
    // (find a cycle, let k be its cardinality)

    if (k >= m) {
      // cycle is hamiltonian, optimal solution
      fprintf(stdout, " )\n    is the optimal solution\n");
      termination = TRUE;
    } else {
      // solution is union of disjoint cycles, break the cycle
      fprintf(stdout, " )\n    is a disjoint cycle, breaking\n");

      // TODO:
      // add the constraint sum_{i!=j in cycle} x[i][j] <= k - 1
      // (compute nz, coeffs, indices, sense, rhs)
      AddConstraint(env, ip, nz, coeffs, indices, sense, rhs);
    }
    itncount++;
  }
  fprintf(stdout, "*******************************************************\n");

  // free storage
  free(cycle);
  free(successor);
  free(x);
  CPXfreeprob(env, &ip);
  free(coeffs);
  free(indices);
  free(ctype);
  free(xU);
  free(xL);
  free(c);
  for(i = m-1; i >= 0; i--) {
    free(D[i]);
  }
  free(D);
  return ret;
}

