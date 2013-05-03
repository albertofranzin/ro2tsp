NOMEFILE = test_cplex.c

PAR = -O3 -lm
# -O3 : badass optimization
# -lm : linking to math lib math.h
#
#  #OPT = -ftree-vectorizer-verbose=2 -finline-functions -std=c99

CFLAGS = 
# debug options will be appended when asked by using 'make debug'

CHECK = -Winline
# -Winline : warn if any problem in including inline functions occurs

CPLEX_COMPILER_OPT = -I/opt/ibm/ILOG/CPLEX_Studio125/cplex/include -DIL_STD

CPLEX_LINKER_OPT = -L/opt/ibm/ILOG/CPLEX_Studio125/cplex/lib/x86-64_sles10_4.1/static_pic -lcplex -lm -lpthread

all: clean test_cplex

test_cplex: cleanComp buildLib $(NOMEFILE)
	gcc $(NOMEFILE) -o $@ -L. -lro2ds $(CFLAGS) $(CHECK) $(CPLEX_COMPILER_OPT) $(CPLEX_LINKER_OPT) $(PAR)

debug: CFLAGS += -g -DDEBUG
debug: clean test_cplex
# -g      : debug symbols
# -DDEBUG : declare DEBUG macro


# link libraries
buildLib: list.o edgelist.o graph.o egraph.o tree.o onetree.o compute_mst.o \
		  compute_ot.o compute_nearest_neighbour.o compute_upper_bound.o \
		  compute_lagrange.o solve_tsp.o utils.o cycle.o \
		  cplex_solver.o
	ar r libro2ds.a *.o
#	rm *.o

list.o: list.h list.c
	gcc $(CFLAGS) $(PAR) -c list.c

edgelist.o: edge.h edgelist.h edgelist.c
	gcc $(CFLAGS) $(PAR) -c edgelist.c

graph.o: graph.h graph.c
	gcc $(CFLAGS) $(PAR) -c graph.c

egraph.o: egraph.h egraph.c
	gcc $(CFLAGS) $(PAR) -c egraph.c

tree.o: tree.h tree.c
	gcc $(CFLAGS) $(PAR) -c tree.c

onetree.o: onetree.h onetree.c
	gcc $(CFLAGS) $(PAR) -c onetree.c

compute_mst.o: compute_mst.h compute_mst.c
	gcc $(CFLAGS) $(PAR) -c compute_mst.c

compute_ot.o: compute_ot.h compute_ot.c
	gcc $(CFLAGS) $(PAR) -c compute_ot.c

compute_nearest_neighbour.o: compute_nearest_neighbour.h compute_nearest_neighbour.c
	gcc $(CFLAGS) $(PAR) -c compute_nearest_neighbour.c

compute_upper_bound.o: compute_upper_bound.h compute_upper_bound.c
	gcc $(CFLAGS) $(PAR) -c compute_upper_bound.c

compute_lagrange.o: compute_lagrange.h compute_lagrange.c
	gcc $(CFLAGS) $(PAR) -c compute_lagrange.c

solve_tsp.o: solve_tsp.h solve_tsp.c
	gcc $(CFLAGS) $(PAR) -c solve_tsp.c

utils.o: utils.h utils.c
	gcc $(CFLAGS) $(PAR) -c utils.c

cycle.o: cycle.h cycle.c
	gcc $(CFLAGS) $(PAR) -c cycle.c

cplex_solver.o: cplex_solver.h cplex_solver.c
	gcc $(CFLAGS) $(CPLEX_COMPILER_OPT) $(CPLEX_LINKER_OPT) $(PAR) -c cplex_solver.c

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
