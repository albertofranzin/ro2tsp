NOMEFILE = test_cplex.c

PAR = -lm
# -lm : linking to math lib math.h
#
#  #OPT = -ftree-vectorizer-verbose=2 -finline-functions -std=c99

CFLAGS = 
# debug options will be appended when asked by using 'make debug'
# release optimization will be added then command is 'make release'

CHECK = -Winline -Wall
# -Winline : warn if any problem in including inline functions occurs

CPLEX_COMPILER_OPT = -I/opt/ibm/ILOG/CPLEX_Studio125/cplex/include -DIL_STD

CPLEX_LINKER_OPT = -L/opt/ibm/ILOG/CPLEX_Studio125/cplex/lib/x86-64_sles10_4.1/static_pic -lcplex -lm -lpthread

all: clean test_cplex

test_cplex: cleanComp buildLib $(NOMEFILE)
	gcc $(NOMEFILE) -o $@ -L. -lro2ds $(CFLAGS) $(CHECK) $(CPLEX_COMPILER_OPT) $(CPLEX_LINKER_OPT) $(PAR)

release: CFLAGS += -O3
release: clean test_cplex
# -O3 : badass optimization

debug: CFLAGS += -g -DDEBUG
debug: clean test_cplex
# -g      : debug symbols
# -DDEBUG : declare DEBUG macro


# link libraries
buildLib: list.o edgelist.o graph.o egraph.o tree.o onetree.o cycle.o cplex_table.o \
		  compute_ot.o compute_upper_bound.o compute_lagrange.o compute_mst.o \
		  base.o utils.o \
		  solve_tsp.o cplex_solver.o
	ar r libro2ds.a *.o
	rm *.o

# data structures
list.o: data/list.h data/list.c
	gcc $(CFLAGS) $(PAR) -c data/list.c

edgelist.o: data/edge.h data/edgelist.h data/edgelist.c
	gcc $(CFLAGS) $(PAR) -c data/edgelist.c

graph.o: data/graph.h data/graph.c
	gcc $(CFLAGS) $(PAR) -c data/graph.c

egraph.o: data/egraph.h data/egraph.c
	gcc $(CFLAGS) $(PAR) -c data/egraph.c

tree.o: data/tree.h data/tree.c
	gcc $(CFLAGS) $(PAR) -c data/tree.c

onetree.o: data/onetree.h data/onetree.c
	gcc $(CFLAGS) $(PAR) -c data/onetree.c

cycle.o: data/cycle.h data/cycle.c
	gcc $(CFLAGS) $(PAR) -c data/cycle.c

cplex_table.o: data/cplex_table.h data/cplex_table.c
	gcc $(CFLAGS) $(PAR) -c data/cplex_table.c

# algorithms
compute_mst.o: algos/compute_mst.h algos/compute_mst.c
	gcc $(CFLAGS) $(PAR) -c algos/compute_mst.c

compute_ot.o: algos/compute_ot.h algos/compute_ot.c
	gcc $(CFLAGS) $(PAR) -c algos/compute_ot.c

compute_upper_bound.o: algos/compute_upper_bound.h algos/compute_upper_bound.c
	gcc $(CFLAGS) $(PAR) -c algos/compute_upper_bound.c

compute_lagrange.o: algos/compute_lagrange.h algos/compute_lagrange.c
	gcc $(CFLAGS) $(PAR) -c algos/compute_lagrange.c

# solvers
solve_tsp.o: solvers/solve_tsp.h solvers/solve_tsp.c
	gcc $(CFLAGS) $(PAR) -c solvers/solve_tsp.c

cplex_solver.o: solvers/cplex_solver.h solvers/cplex_solver.c
	gcc $(CFLAGS) $(CPLEX_COMPILER_OPT) $(CPLEX_LINKER_OPT) $(PAR) -c solvers/cplex_solver.c

# base (common utilities)
base.o: base/base.h base/base.c
	gcc $(CFLAGS) $(PAR) -c base/base.c

utils.o: base/utils.h base/utils.c
	gcc $(CFLAGS) $(PAR) -c base/utils.c

# cleanup
cleanComp:
	clear
	-rm test_cplex
	-rm test_cplex.a
	-rm *.gch

clean:
	clear
	-rm test_cplex
	-rm test_cplex.a
	-rm *.o
	-rm *.gch
