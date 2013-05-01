
/* ======================================================================
      tsp.c   David Pisinger, Simon Spoorendonk, Thomas Friis Antonsen
   ====================================================================== */

/* This C program solves the asymetric traveling salesman problem
 *
 *    minimize     sum_{i=1,..,n} sum_{j=1,..,n}      c_ij x_ij
 *    subject to   sum_{j=1,..,n} x_ij = 1,           i = 1,..,n
 *                 sum_{i=1,..,n} x_ij = 1,           j = 1,..,n
 *                 sum_{i,j \in S} x_ij <= |S|-1,     S subset V
 *                 x_ij in {0,1}
 * where
 *   G = (V,E)  is a graph with n nodes and n x n edges
 *   C = (c_ij) is the distance matrix of size n x n
 *   X = (x_ij) is a boolean matrix where x_ij=1 if edge (i,j) is chosen
 *
 * the code is run with the command
 * 
 *   tsp instance model [rootbound] [cplex]
 *
 * where "instance" is a file name, and model is one of the following
 *   ASSIGN        only assignment constraints
 *   MTZ           assignment constraints and MTZ constraints
 *   SUBTOUR       assignment constraints and subtour constraints
 *   CUTTING       cutting plane algorithm
 *   BRANCHING     ip cutting plane algorithm
 *
 * optional [rootbound] solve model to rootbound
 *   LP            rootnode
 *
 * optional [cplex] if CPLEX should be used in ip-cutting
 *   CPLEX         use CPLEX
 */


/* ======================================================================
  	             inclusion of library files
   ====================================================================== */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <values.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <sys/times.h>
#include <unistd.h>
#include "cplex.h"

#include "Graph.h"
#include "GraphAlg.h"

/* ======================================================================
				   defines
   ====================================================================== */

#define MAXV    400     /* max number of vertices */
#define MAXTIME 60      /* time limit in seconds for using CPLEX */
#define MAXLINE 10      /* max number of entries pr. line in infile */
#define MAXCUTS 3000    /* max number of cuts generated */
#define PATH    "/vol/www/undervisning/2004e/datV-optimer/P2"
                        /* directory where all files are found */
#define TMPFILE "/tmp/username.lp"
                        /* name of cplex input file */
#define EPSILON 0.001   /* epsilon for testing integrality */
#define INF     1<<30


/* ======================================================================
				   macros
   ====================================================================== */

#define FALSE 0                       /* logical false */
#define TRUE  1                       /* logical true */

#define STR(x) (x ? "TRUE" : "FALSE") /* logical string */

#define CPX_NOLICENSE 32201           /* no CPLEX license available */


/* ======================================================================
				 type declarations
   ====================================================================== */

typedef int boolean;


/* ======================================================================
				  global variables
   ====================================================================== */

int     n;                     /* number of nodes in problem */
int     c[MAXV][MAXV];         /* cost of edge (i,j) */
int     xip[MAXV][MAXV];       /* ip optimal columns chosen */
double  xlp[MAXV][MAXV];       /* lp optimal columns chosen */
int     t[MAXCUTS][MAXV];      /* table of valid cuts */
int     maxt;                  /* last entry in table of cuts */
int     zip;                   /* ip optimal solution value */
double  zlp;                   /* lp optimal solution value */
int     branches[MAXV][MAXV];  /* table of current branches */
int     bbnodes;               /* Nodes in the branch/and/bound tree */
FILE    *logfile;              /* logfile where additional info is written */
CPXENVptr env = NULL;          /* CPLEX environment pointer */


/* =======================================================================
				  timing routine
   ======================================================================= */

/* The following routine is used for measuring the cpu time used
 *   cpu_time(NULL)    - start timer
 *   cpu_time(&t)      - return the elapsed cpu-time in variable t (double).
 */

void cpu_time(double *t)
{
  static struct tms start, stop;

  if (t == NULL) {
    times(&start);
  } else {
    times(&stop);
    *t = (double) (stop.tms_utime - start.tms_utime) / sysconf(_SC_CLK_TCK);
  }
}


/* =======================================================================
				  error
   ======================================================================= */

/* Write an error message to the screen and terminate 
 */

static void error(char *str, ...)
{
  va_list args;

  va_start(args, str);
  vprintf(str, args);
  printf("\n");
  vfprintf(logfile, str, args);
  va_end(args);
  printf("STOP !!!\n\n");
  exit(-1);
}


/* ======================================================================
				check_objective
   ====================================================================== */

/* Check if the objective function
 *     sum_{i=1,..,n} sum_{j=1,..,n} c_ij x_ij
 * is equal to the parameter z
 * 
 * This is done for either the lp or the ip solution depending on the
 * solution parameter
 */

void check_objective(double z, boolean intsolution)
{
  int i, j, zint, sumint;
  double sum;
  

  printf("check_objective ");
  sum = 0; sumint = 0; zint = (int) (z + 0.5);
  for (i = 1; i <= n; i++) {
    for (j = 1; j <= n; j++) {
      if (i == j) continue;
      if (intsolution) {
	if (xip[i][j]) sumint += c[i][j];
      }
      else if (xlp[i][j]) sum += c[i][j]*xlp[i][j];
    }
  }
  if (intsolution) {
    printf("z %d sum %d\n", zint, sumint);
    if (abs(zint - sumint) > EPSILON ) error("not correct objective\n");
  }
  else {
    printf("z %.3lf sum %.3lf\n", z, sum);
    if (fabs(z - sum) > EPSILON ) error("not correct objective\n");
  }
}


/* ======================================================================
				hamilton
   ====================================================================== */

/* Check if the xip[][] or xlp[][] matrix defines a Hamilton cycle.
 */

boolean hamilton(boolean intsolution)
{
  int i, j, k, v[MAXV];
  boolean ham;

  printf("hamilton ");
  for (i = 1; i <= n; i++) {
    v[i] = 0;
    for (j = 1; j <= n; j++) {
      if (i == j) continue;
      if (intsolution) {
        if (xip[i][j]) {
          if (v[i] != 0) error("more than one edge leaving node %d\n", i);
          v[i] = j;
        }
      }
      else {
        if (xlp[i][j]) {
          if (v[i] != 0) error("more than one edge leaving node %d\n", i);
          v[i] = j;
        }
      }
    }
    if (v[i] == 0) error("no edges leaving %d\n", i);
  }

  k = 1; ham = TRUE;
  for (i = 1; i <= n; i++) {
    printf("%d ", k);
    j = v[k];
    if (j == 0) { ham = FALSE; break; }
    v[k] = 0;
    k = j;
  }
  printf("%s\n", STR(ham));
  return ham;
}

/* ======================================================================
				integral
   ====================================================================== */

/* Check if the xlp[][] matrix only contains integers.
 */

boolean integral(void)
{
  int i, j;
 
  printf("intgral ");
  for (i = 1; i <= n; i++) {
    for (j = 1; j <= n; j++) {
      if (i == j) continue;
      if (xlp[i][j]){
        if ( (fabs(xlp[i][j] - floor(xlp[i][j])) > EPSILON) &&
             (fabs(xlp[i][j] - ceil(xlp[i][j])) > EPSILON )) {
            printf("%s\n", STR(FALSE));
            return FALSE;
        }
        else printf("x%d_%d ",i,j);
      }
    }
  }

  printf("%s\n", STR(TRUE));
  return TRUE;
}


/* ======================================================================
				write_solution
   ====================================================================== */

/* Write the cost matrix c[][] and current solution xip[][]/xlp[][] to 
 * the screen
 */

void write_solution(boolean intsolution)
{
  int i, j, sumi;
  double sumf;

  for (i = 1; i <= n; i++) {
    for (j = 1; j <= n; j++) printf( "%3d ", c[i][j]);
    printf("\n");
  }

  for (i = 1; i <= n; i++) {
    for (j = 1; j <= n; j++) 
      if (intsolution) printf( "%1d ", xip[i][j]);
      else printf( "%.3lf ", xlp[i][j]);
    printf("\n");
  }
  sumf = sumi = 0;
  for (i = 1; i <= n; i++) {
    for (j = 1; j <= n; j++)       
      if (intsolution) {
        if (xip[i][j]) sumi += c[i][j];
      }
      else if (xlp[i][j]) sumf += c[i][j]*xlp[i][j];
  }
  if (intsolution) printf("solution value %d\n", sumi);
  else printf("solution value %.3lf\n", sumf);
}


/* ======================================================================
		               read_instance
   ====================================================================== */

/* Read an instance to matrix c and variable n.
 * The instance is read from the file "name" with path equal to
 * the macro PATH defined in the heading.
 */

void read_instance(char *name)
{
  register int i, j;
  int v;
  FILE *in;
  char s[100], d;

  /* konstruer filnavn og aaben fil */
  sprintf(s, "%s/%s", PATH, name);
  printf("reading instance from '%s'\n", s);
  in = fopen(s, "r"); if (in == NULL) error("%s no such file", s);

  /* skip en eventuel kommentar i foerste linie */
  d = getc(in); 
  if (d == '#') { fgets(s, 100, in); printf("#%s", s); } else ungetc(d, in);

  /* laes data */
  fscanf(in, "%d", &v); n = v; 
  for (i = 1; i <= n; i++) {
    for (j = 1; j <= n; j++) { fscanf(in, "%d", &v); c[i][j] = v; }
  }

  /* kontroller at instansen overholder antagne egenskaber */
  if (n <= 0) error("empty instance");
  for (i = 1; i <= n; i++) {
    /* if (c[i][i] != 0) error("not zero diagonal"); */
  }
}


/* ======================================================================
				separate_cuts_ip
   ====================================================================== */

/* Routine for separating valid inequalities. Useful variables are
 *   xip[][]   current ip solution matrix 
 *   maxt      number of cuts in table t[][].
 *   t[][]     table of cuts. Each line t[] defines a valid inequality.
 *             t[i][j] = TRUE means that node j is part of inequality i. 
 */
 
void separate_cuts_ip(void)
{
  int i, j, h, k, v[MAXV];

  printf("separate_cuts_ip ");
  for (i = 1; i <= n; i++) {
    for (j = 1; j <= n; j++) {
      if ((i != j) && (xip[i][j])) v[i] = j;
    }
  }

  for (;;) {
    for (k = 1; k <= n; k++) if (v[k] != 0) break;
    if (k > n) break;

    maxt++;
    for (i = 1; i <= n; i++) t[maxt][i] = 0;

    for (h = k;;) {
      t[maxt][h] = TRUE;
      j = v[h];
      v[h] = 0;
      h = j;
      if (h == k) break;
    }
  }
  printf("\n");
}


/* ======================================================================
				separate_cuts_lp
   ====================================================================== */

/* Routine for separating valid inequalities. Useful variables are
 *   xlp[][]   current lp solution matrix 
 *   maxt      number of cuts in table t[][].
 *   t[][]     table of cuts. Each line t[] defines a valid inequality.
 *             t[i][j] = TRUE means that node j is part of inequality i. 
 */
 
void separate_cuts_lp(void)
{
  /* ##### to be filled out ##### */


}


/* ======================================================================
			      write_objective
   ====================================================================== */

/* Write the objective function to a CPLEX file of format ".lp"
 */

void write_objective(FILE *out)
{
  int i, j, k;

  k = 0;
  fprintf(out, " ");
  for (i = 1; i <= n; i++) {
    for (j = 1; j <= n; j++) {
      if (j % MAXLINE == 0) fprintf(out, "\n  "); 
      if (i == j) {
        fprintf(out, "          ");
      } else {
        if (k > 0) fprintf(out, "+");
        fprintf(out, "%3dx%02d_%02d", c[i][j], i, j); 
        k++;
      }
    }
    fprintf(out, "\n");
  }
}


/* ======================================================================
			      write_cutting_constraints
   ====================================================================== */

/* Write the generated cuts to a CPLEX file of format ".lp"
 */

void write_cutting_constraints(FILE *out)
{
  int i, j, k, h, m;

  for (h = 1; h <= maxt; h++) {
    m = 0;
    for (i = 1; i <= n; i++) if (t[h][i] != 0) m++;

    k = 0;
    for (i = 1; i <= n; i++) {
      if (t[h][i] == 0) continue;
      for (j = 1; j <= n; j++) {
        if (t[h][j] == 0) continue;
        if (i == j) continue;
        if ((k+1) % MAXLINE == 0) fprintf(out,"\n  ");
        if (k > 0) fprintf(out, "+");
        fprintf(out, "x%02d_%02d", i, j);
        k++;
      }
    }
    fprintf(out, "<=%d\n", m-1);
  }
}
 
 
/* ======================================================================
			      write_subtour_constraints
   ====================================================================== */

/* Write the subtour constraints to a CPLEX file of format ".lp"
 */

void write_subtour_constraints(FILE *out, int *S, int i)
{
  int j, k, m;

  if (i == n+1) {
    m = 0;
    for (k = 1; k <= n; k++) if (S[k]) m++;
    if ((m < 2) || (m == n)) return;

    k = 0;
    for (i = 1; i <= n; i++) {
      if (S[i] == 0) continue;
      for (j = 1; j <= n; j++) {
        if (S[j] == 0) continue;
        if (i == j) continue;
        if ((k+1) % MAXLINE == 0) fprintf(out,"\n  ");
        if (k > 0) fprintf(out, "+");
        fprintf(out, "x%02d_%02d", i, j);
        k++;
      }
    }
    fprintf(out, "<=%d\n", m-1);
  } else {
    S[i] = TRUE;  write_subtour_constraints(out, S, i+1); 
    S[i] = FALSE; write_subtour_constraints(out, S, i+1); 
  }
}
 
 
/* ======================================================================
			      write_assignment_constraints
   ====================================================================== */

/* Write the assignment constraints to a CPLEX file of format ".lp"
 */

void write_assignment_constraints(FILE *out)
{
  int i, j, k;

  for (i = 1; i <= n; i++) {
    k = 0;
    for (j = 1; j <= n; j++) {
      if (j % MAXLINE == 0) fprintf(out, "\n  "); 
      if (i == j) { 
        fprintf(out,"       ");
      } else {
        if (k > 0) fprintf(out, "+"); 
        fprintf(out, "x%02d_%02d", i, j);
        k++;
      }
    }
    fprintf(out, "=1\n");
  }
  fprintf(out, "\n");

  for (i = 1; i <= n; i++) {
    k = 0;
    for (j = 1; j <= n; j++) {
      if (j % MAXLINE == 0) fprintf(out, "\n  "); 
      if (i == j) { 
        fprintf(out,"       ");
     } else {
        if (k > 0) fprintf(out, "+"); 
        fprintf(out, "x%02d_%02d", j, i);
        k++;
      }
    }
    fprintf(out, "=1\n");
  }
  fprintf(out, "\n");
}


/* ======================================================================
			      write_mtz_constraints
   ====================================================================== */

/* Write the MTZ constraints to a CPLEX file of format ".lp"
 */

void write_mtz_constraints(FILE *out)
{
  /* ##### to be filled out ##### */


}


/* ======================================================================
			      write_x_bounds
   ====================================================================== */

/* Write bounds on the x variables to a CPLEX file of format ".lp"
 */

void write_x_bounds(FILE *out)
{
  int i, j;

  for (i = 1; i <= n; i++) {
    for (j = 1; j <= n; j++) {
      if (i != j) fprintf(out, "0<=x%02d_%02d<=1\n", i, j);
    }
  }
}

/* ======================================================================
			      write_u_bounds
   ====================================================================== */

/* Write bounds on the u variables to a CPLEX file of format ".lp"
 */

void write_u_bounds(FILE *out)
{
  int i;

  for (i = 2; i <= n; i++) {
    fprintf(out, "2<=u%02d<=%d\n", i, n);
  }
}


/* ======================================================================
			      write_x_binary
   ====================================================================== */

/* Write binary variables x to a CPLEX file of format ".lp"
 */

void write_x_binary(FILE *out)
{
  int i, j;

  for (i = 1; i <= n; i++) {
    for (j = 1; j <= n; j++) {
      if (j % MAXLINE == 0) fprintf(out, "\n  ");
      if (i == j) {
        fprintf(out, "       ");
      } else {
        fprintf(out, "x%02d_%02d ", i, j);
      }
    }
    fprintf(out, "\n");
  }
} 


/* ======================================================================
			      write_u_integer
   ====================================================================== */

/* Write integer variables u to a CPLEX file of format ".lp"
 */

void write_u_integer(FILE *out)
{
  int i;

  for (i = 1; i <= n; i++) {
    if (i % MAXLINE == 0) fprintf(out, "\n  ");
    fprintf(out, "u%02d ", i);
  }
  fprintf(out, "\n");
} 


/* ======================================================================
			      write_branches
   ====================================================================== */

/* Write the x variables in branches[][] to a CPLEX file of format ".lp"
 */

void write_branches(FILE *out)
{
  int i, j;

  for (i = 1; i <= n; i++) 
    for (j = 1; j <= n; j++) {
      if (i == j) continue;
      if (branches[i][j] == -1) continue;
      fprintf(out, "x%02d_%02d=%d\n", i, j, branches[i][j]);
    }
} 


/* ======================================================================
				make_cplex_file
   ====================================================================== */

/* Generate a CPLEX file of format ".lp". The output is written to
 * file "filename". Assignemnt constrants are always written to the
 * file. If mtz=TRUE then the MTZ constraints are added to the file. 
 * If subtour=TRUE then the sobtour constraints are added to the file.
 * If cutting=TRUE or branching=TRUE then the generated cuts from table
 * t[][] are added to the file. The x variables are written as integers
 * if intsolution=TRUE otherwise they are written as real numbers.
 */

void make_cplex_file(char *filename, boolean mtz, boolean subtour, 
                     boolean cutting, boolean branching, boolean intsolution)
{ 
  int S[MAXV];
  FILE *out;

  out = fopen(filename, "w");
  if (out == NULL) error("could not open file %s\n", filename);
  
  fprintf(out, "\nminimize\n");
  write_objective(out);

  fprintf(out, "\nsubject to\n");
  write_assignment_constraints(out);
  if (subtour) write_subtour_constraints(out, S, 1);
  if (mtz) write_mtz_constraints(out);
  if (cutting || branching) write_cutting_constraints(out);
  if (cutting || branching) write_branches(out);

  fprintf(out, "\nbounds\n");
  if (mtz) write_u_bounds(out);
  if (!intsolution) write_x_bounds(out);

  fprintf(out, "\nbinary\n");
  if (intsolution) write_x_binary(out);

  fprintf(out, "\ngeneral\n");
  if (mtz && intsolution) write_u_integer(out);

  fprintf(out, "\nend\n");
  fclose(out);
}


/* ======================================================================
  			          call_cplex
   ====================================================================== */

/* Call CPLEX. If intsolution=TRUE then the problem is solved to 
 * IP-optimality, othewise the problem is solved to LP-optimality.
 * The input file is given by infile. The found solution is returned
 * in the xlp[][] or the xip[][] matrix depending on the type of solution.
 * Notice that the variables returned from CPLEX follow the same order
 * as written to the infile. If changes are made to the objective
 * function, then also the following procedure should be updated.
 */

double call_cplex(char *infile, boolean intsolution)
{
  int       i, j, k;
  int       solstat, status, numrows, numcols;
  double    objval;
  CPXLPptr  lp = NULL;
  char      errmsg[1024];
  double    x1[MAXV*MAXV];

  /* Initialize the CPLEX environment */
  printf("cplex: getting license\n");
  for (;;) {
    env = CPXopenCPLEXdevelop(&status);
    if (status != CPX_NOLICENSE) break; 
    sleep(1); /* sleep one second */
    printf("cplex: waiting for license\n");
  }
  if (env == NULL) {
    printf("Could not open CPLEX environment.\n");
    CPXgeterrorstring(env, status, errmsg);
    error("%s\n", errmsg);
  }
  printf("cplex: license available\n");

  /* Create the problem, using the filename as the problem name */
  lp = CPXcreateprob(env, &status, infile);   
  /* A returned pointer of NULL may mean that not enough memory
     was available or there was some other problem. */
  if (lp == NULL) error("cplex: failed to create LP.\n");

  /* Now read the file, and copy the data into the created lp */
  printf("cplex: reading input file '%s'\n", infile);
  status = CPXreadcopyprob(env, lp, infile, NULL);
  if (status) error("cplex: failed to read and copy the problem data.\n");

   /* Set time limit for solving the problem */
   CPXsetdblparam(env, CPX_PARAM_TILIM, MAXTIME);

   /* Set relative tolerance on IP solution */
   CPXsetdblparam(env, CPX_PARAM_EPGAP, 1E-8);

  /* The size of the problem should be obtained by asking CPLEX */
  numrows = CPXgetnumrows(env, lp);
  numcols = CPXgetnumcols(env, lp);

  /* Optimize the problem */
  if (intsolution) {
    printf("cplex: solving to IP optimality, max time %d sec\n", MAXTIME);
    status = CPXmipopt(env, lp);
  } else {
    printf("cplex: solving to LP optimality, max time %d sec\n", MAXTIME);
    status = CPXlpopt(env, lp);
  }
  if (status) {
    if (status == CPXERR_PRESLV_INForUNBD) {
      ;
      //error("cplex: infeasible or unbounded\n");
    } else {
      error("cplex: failed to optimize, code %d\n", status); 
    }
  }

  /* See if process was stopped */
  solstat = CPXgetstat(env, lp);
  if (intsolution) {
    if (solstat != CPXMIP_OPTIMAL) {
      printf("cplex: returncode %d\n", solstat);
      if (solstat==CPXMIP_INFEASIBLE) error("cplex: infeasible solution\n");
      if (solstat==CPXMIP_TIME_LIM_FEAS) error("cplex: timeout, feasible");
      if (solstat==CPXMIP_TIME_LIM_INFEAS) error("cplex: timeout, no solution");
      CPXgeterrorstring(env, solstat, errmsg);
      printf("cplex: %s", errmsg);
      error("cplex: presumably syntax error in input file,\
             run CPLEX manually on the input file to debug\n");
    } 
  } else {
    if (solstat != CPX_OPTIMAL) {
      if (solstat==CPX_UNBOUNDED)  //error("cplex: unbounded solution\n");
        if (solstat==CPX_INFEASIBLE); //error("cplex: infeasible solution\n");
        else {
          printf("cplex: returncode %d\n", solstat);
          CPXgeterrorstring(env, solstat, errmsg);
          printf("cplex: %s", errmsg);
          error("cplex: presumably syntax error in input file,\
             run CPLEX manually on the input file to debug\n");
        }
    }
  }

  if (status!=CPXERR_PRESLV_INForUNBD && 
      solstat!=CPX_UNBOUNDED && 
      solstat!=CPX_INFEASIBLE) {
  /* Get objective value */
  if (intsolution) {
    status = CPXgetmipobjval(env, lp, &objval);
  } else {
    status = CPXgetobjval(env, lp, &objval);
  }
  if (status) error("cplex: failed to obtain objective value.\n"); 
 
  if (intsolution) {
    printf("cplex: IP-solution %.0lf\n", objval); 
  } else {
    printf("cplex: LP-solution %.3lf\n", objval); 
  }

  /* Get primal solution */
  if (intsolution) {
    status = CPXgetmipx(env, lp, x1, 0, numcols-1);
  } else {
    status = CPXgetx(env, lp, x1, 0, numcols-1);
  }
  if (status) error("cplex: failed to obtain primal solution\n");

#ifdef DEBUG
  printf("cplex: primal solution\n");
  for (i = 0; i < numcols; i++) {
    if (x1[i] > 0) printf("x[%d]=%1.0lf ", i, x1[i]);
  }
  printf("\n");
#endif

  /* copy back the primal solution. Notice that cplex returns the   */
  /* variables in x1[] in the same order as the original variables  */
  /* appear in the input file. If the objective function is changed */
  /* then the following lines must be changed also!                 */
  k = 0;
  for (i = 1; i <= n; i++) {
    for (j = 1; j <= n; j++) {
      if (i != j) { 
        if (intsolution) xip[i][j] = (int) (x1[k] + 0.5);
        else xlp[i][j] = x1[k];
        k++; 
      }
    }
  }
  }
  else objval = INF;

  /* Free up the problem as allocated by CPXcreateprob, if necessary */
  if (lp != NULL) {
    status = CPXfreeprob(env, &lp);
    if (status) error("cplex: CPXfreeprob failed, error code %d.\n", status);
  }

  /* Free up the CPLEX environment, if necessary */
  if (env != NULL) {
    printf("cplex: freeing license\n");
    status = CPXcloseCPLEX(&env);
    if (status) {
      printf("cplex: could not close CPLEX environment\n");
      CPXgeterrorstring(env, status, errmsg);
      error("%s", errmsg);
    }
  }
  
  return objval;
}


/* ======================================================================
                                tsp_branch
   ====================================================================== */
/* Recursive branch-and-bound algorithm using deepth-first search.
 * The current best ip solution is written in xip[][] and the current
 * lp solution in xlp[][]. rootnode=TRUE denotes if only the rootnode is
 * solved, i.e. no branching occurs. hamiltonian=TRUE indicates whether
 * an ip solution should be hamiltonian. If separatecuts=TRUE then cuts
 * are separated on the lp solutions.
 */
void tsp_branch(int i, int j, int branchvalue,
		boolean rootnode, boolean hamiltonian, boolean separatecuts)
{
  double fracvalue;
  int it, h, k, fraci, fracj;
  
  printf("node %d", bbnodes);

  if (!rootnode && i > 0 && j > 0) {
    branches[i][j] = branchvalue;
    printf(", added constraint x%02d_%02d = %d", i,j, branchvalue);
  }
  else {
    zip = INF;
    for (h = 1; h <= n; h++)
      for (k = 1; k<= n; k++) branches[h][k] = -1;
  }

  printf("\n");

  // solve lp to optimality
  int cuts, difcuts;
  difcuts = it = cuts = 0;
  for (;;) {
    printf("node %d, iteration %d of lp-cutting plane algorithm\n", bbnodes, it++);
    make_cplex_file(TMPFILE, FALSE, FALSE, TRUE, FALSE, FALSE);
    zlp = call_cplex(TMPFILE, FALSE);

    // prune node if worse than best ip solution
    if (zlp >= zip) {
      printf("Pruning node %d\n", bbnodes++);
      branches[i][j] = -1;
      fprintf(logfile, "node %d cuts %d iterations %d\n", bbnodes, difcuts, it);
      return;
    }

    // Separate cuts
    if(separatecuts) {
      difcuts = -maxt;
      separate_cuts_lp();
      difcuts += maxt;
      if (difcuts <= 0) break;
      cuts += difcuts;
    }
    else break;
  }
  printf("node %d cuts %d iterations %d\n", bbnodes, difcuts, it);
  fprintf(logfile, "node %d cuts %d iterations %d\n", bbnodes, difcuts, it);

  // return if ip solution
  if (integral() && (!hamiltonian || hamilton(FALSE)) ) {
    branches[i][j] = -1;
    if ((int) (zlp + 0.5) < zip) {
      zip = (int) (zlp + 0.5);
      printf("node %d, new best solution zip %d\n", bbnodes, zip);      
      for (h = 1; h <= n; h++)
	for (k = 1; k <= n; k++) xip[h][k] = (int) (xlp[h][k] + 0.5);
    }
    bbnodes++;
    return;
  }

  if(rootnode) return;

  // Finding most fractional variable to branch on
  fracvalue = 0;

  for (h = 1; h <= n; h++)
    for (k = 1; k <= n; k++) {
      if ((1 - xlp[h][k] > EPSILON) && (xlp[h][k] > EPSILON))
	if (xlp[h][k] > fracvalue) {
	  fracvalue = xlp[h][k];
	  fraci = h;
	  fracj = k;
	}
    }

  printf("node %d, branch on x%d_%d\n", bbnodes++, fraci, fracj);

  // branch on frac variable set to 0/1 
  tsp_branch(fraci, fracj, 1, FALSE, hamiltonian, separatecuts);
  tsp_branch(fraci, fracj, 0, FALSE, hamiltonian, separatecuts);
  branches[i][j] = -1;
}


/* ======================================================================
				tsp_solve
   ====================================================================== */

/* Solve asymmetric TSP problem. "filename" is the name of the instance.
 * "mtz" says whether the MTZ formulation should be used.
 * "subtour" says whether the subtour formulation should be used.
 * "cutting" says whether a cutting plane algorithm should be run.
 * "branching" says whether the ip cuttingplane should be run.
 */

void tsp_solve(char *filename, boolean mtz, boolean subtour, boolean cutting,
	       boolean branching, boolean intsolution, boolean cplex)
{
  double comptime;
  int i, j, h, k, it;

  printf("\n\nFILE %s mtz %s subtour %s cutting %s branching %s ip %s\n", 
                   filename, STR(mtz), STR(subtour), STR(cutting), STR(branching), STR(intsolution));
  cpu_time(NULL);
  read_instance(filename);

  if (cutting) {
    maxt = bbnodes = 0;
    zip = INF;
    for (i = 1; i <= n; i++)
      for (j = 1; j <= n; j++) branches[i][j] = -1;
    
    if (intsolution) tsp_branch(0,0,0,FALSE,TRUE,TRUE);
    else tsp_branch(0,0,0,TRUE,TRUE,TRUE);
  } 
  else if (branching) {
    maxt = bbnodes = 0;
    it = 0;
    do {
      printf("iteration %d of ip-cutting plane algorithm\n", it++);

      if (cplex) {
        for (h = 1; h <= n; h++)
          for (k = 1; k<= n; k++) branches[h][k] = -1;
        make_cplex_file(TMPFILE, FALSE, FALSE, TRUE, FALSE, TRUE);
        zip = (int) (call_cplex(TMPFILE, TRUE) + 0.5);
      }
      else tsp_branch(0,0,0,FALSE,FALSE,FALSE);

      if (hamilton(TRUE)) break;
      separate_cuts_ip();
      printf("\n");
    }
    while (intsolution);
  }
  else {
    make_cplex_file(TMPFILE, mtz, subtour, FALSE, FALSE, intsolution);
    zlp = call_cplex(TMPFILE, intsolution);
    if (intsolution) zip = (int) (zlp + 0.5);
  }

  cpu_time(&comptime);

  if (intsolution) {
     check_objective(zip, intsolution);
    if ((mtz || subtour || cutting || branching) && !hamilton(TRUE)) 
      error("not hamilton cycle\n"); 
    printf("\ntime %.2lf solution %d\n", comptime, zip);
    fprintf(logfile, "\ntime %.2lf solution %d\n", comptime, zip);
    if (branching || cutting) {
      printf("bbnodes %d ", bbnodes);
      fprintf(logfile, "bbnodes %d ", bbnodes);
      printf("cuts %d\n", maxt);
      fprintf(logfile, "cuts %d\n", maxt);
    }
  }
  else {
    check_objective(zlp, intsolution);
    printf("\ntime %.2lf solution %.3lf\n", comptime, zlp);
    fprintf(logfile, "\ntime %.2lf solution %.3lf\n", comptime, zlp);
  }

  /* remove file after use */
  remove(TMPFILE);
}


/* ======================================================================
				main
   ====================================================================== */

/* Main program reads a file name and a solution method.
 */

int main(int argc, char *argv[])
{
  char infile[100], model[100], solution[100], usecplex[100];
  boolean intsolution, cplex;

  if (argc >= 3) {
    strcpy(infile, argv[1]);
    strcpy(model, argv[2]);
    if (argc >= 4) {
      strcpy(solution, argv[3]);
      if (strcmp(solution, "CPLEX") == 0)
	strcpy(usecplex, "CPLEX");
      else if (argc >= 5) strcpy(usecplex, argv[4]);
    }
  } else {
    printf("file = ");
    scanf("%s", infile);
    printf("model = ");
    scanf("%s", model);
    printf("[lp = ]");
    scanf("%s", solution);
    printf("[cplex = ]");
    scanf("%s", usecplex);
  }
  printf("\n\nRunning TSP %s model %s\n", infile, model);

  logfile = fopen("logfile", "a");
  fprintf(logfile, "\nInstance %s model %s\n", infile, model);

  strcmp(solution, "LP") == 0 ? intsolution = FALSE : intsolution = TRUE;
  strcmp(usecplex, "CPLEX") == 0 ? cplex = TRUE : cplex = FALSE;

  if (strcmp(model, "ASSIGN") == 0) {
    tsp_solve(infile, FALSE, FALSE, FALSE, FALSE, intsolution, FALSE);
  }
  if (strcmp(model, "MTZ") == 0) {
    tsp_solve(infile, TRUE, FALSE, FALSE, FALSE, intsolution, FALSE);
  }
  if (strcmp(model, "SUBTOUR") == 0) {
    tsp_solve(infile, FALSE, TRUE, FALSE, FALSE, intsolution, FALSE);
  }
  if (strcmp(model, "CUTTING") == 0) {
    tsp_solve(infile, FALSE, FALSE, TRUE, FALSE, intsolution, FALSE);
  }
  if (strcmp(model, "BRANCHING") == 0) {
    tsp_solve(infile, FALSE, FALSE, FALSE, TRUE, intsolution, cplex);
  }
  fclose(logfile);
  
  return 0;
}

