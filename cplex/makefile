NOMEFILE = test_cplex.c

PAR = -g -O3 -lm
# -g  : debug symbols
#  # -O3 : badass optimization
# -lm : linking to math lib math.h
#
#  #OPT = -ftree-vectorizer-verbose=2 -finline-functions -std=c99

CHECK = -Winline
# -Winline : warn if any problem in including inline functions occurs

CPLEX_COMPILER_OPT = -I/home/ludo/Programmi/ibm/ILOG/CPLEX_Studio125/cplex/include -DIL_STD

CPLEX_LINKER_OPT = -L/home/ludo/Programmi/ibm/ILOG/CPLEX_Studio125/cplex/lib/x86-64_sles10_4.1/static_pic -lcplex -lm -lpthread

test_cplex: clean buildLib $(NOMEFILE)
	gcc $(NOMEFILE) -o $@ -L. -lro2ds $(CHECK) $(CPLEX_COMPILER_OPT) $(CPLEX_LINKER_OPT) $(PAR)

# link libraries
buildLib: list.o edgelist.o graph.o egraph.o tree.o onetree.o compute_mst.o \
		  compute_ot.o compute_nearest_neighbour.o compute_upper_bound.o \
		  compute_lagrange.o solve_tsp.o utils.o cycle.o \
		  cplex_solver.o
	ar r libro2ds.a *.o
#	rm *.o

list.o: list.h list.c
	gcc -g -c list.c

edgelist.o: edge.h edgelist.h edgelist.c
	gcc -g -c edgelist.c

graph.o: graph.h graph.c
	gcc -g -c graph.c

egraph.o: egraph.h egraph.c
	gcc -g -c egraph.c

tree.o: tree.h tree.c
	gcc -g -c tree.c

onetree.o: onetree.h onetree.c
	gcc -g -c onetree.c

compute_mst.o: compute_mst.h compute_mst.c
	gcc -g -c compute_mst.c

compute_ot.o: compute_ot.h compute_ot.c
	gcc -g -c compute_ot.c

compute_nearest_neighbour.o: compute_nearest_neighbour.h compute_nearest_neighbour.c
	gcc -g -c compute_nearest_neighbour.c

compute_upper_bound.o: compute_upper_bound.h compute_upper_bound.c
	gcc -g -c compute_upper_bound.c

compute_lagrange.o: compute_lagrange.h compute_lagrange.c
	gcc -g -c compute_lagrange.c

solve_tsp.o: solve_tsp.h solve_tsp.c
	gcc -g -c solve_tsp.c

utils.o: utils.h utils.c
	gcc -g -c utils.c

cycle.o: cycle.h cycle.c
	gcc -g -c cycle.c

cplex_solver.o: cplex_solver.h cplex_solver.c
	gcc $(CPLEX_COMPILER_OPT) $(CPLEX_LINKER_OPT) -g -c cplex_solver.c

clean:
	clear
	-rm test_cplex
	-rm test_cplex.a
	-rm *.o
	-rm *.gch