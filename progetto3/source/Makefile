NOMEFILE = test.c
HEUR2OPT = heur2opt.c

PAR = -g -O3 -lm
# -g  : debug symbols
# -O3 : badass optimization
# -lm : linking to math lib math.h

#OPT = -ftree-vectorizer-verbose=2 -finline-functions -std=c99

CHECK = -Winline
# -Winline : warn if any problem in including inline functions occurs

# compile main
test: clean buildLib $(NOMEFILE)
	gcc $(NOMEFILE) -o $@ -L. -lro2ds $(CHECK) $(PAR)

# test for 2 opt
test2opt: clean buildLib $(HEUR2OPT)
	gcc $(HEUR2OPT) -o $@ -L. -lro2ds $(CHECK) $(PAR)

# link libraries
buildLib: list.o edgelist.o graph.o egraph.o tree.o onetree.o compute_mst.o \
		  compute_ot.o compute_nearest_neighbour.o compute_upper_bound.o \
		  compute_lagrange.o solve_tsp.o utils.o cycle.o
	ar r libro2ds.a *.o
#	rm *.o

list.o: list.h list.c
	gcc $(PAR) -c list.c

edgelist.o: edge.h edgelist.h edgelist.c
	gcc $(PAR) -c edgelist.c

graph.o: graph.h graph.c
	gcc $(PAR) -c graph.c

egraph.o: egraph.h egraph.c
	gcc $(PAR) -c egraph.c

tree.o: tree.h tree.c
	gcc $(PAR) -c tree.c

onetree.o: onetree.h onetree.c
	gcc $(PAR) -c onetree.c

compute_mst.o: compute_mst.h compute_mst.c
	gcc $(PAR) -c compute_mst.c

compute_ot.o: compute_ot.h compute_ot.c
	gcc $(PAR) -c compute_ot.c

compute_nearest_neighbour.o: compute_nearest_neighbour.h compute_nearest_neighbour.c
	gcc $(PAR) -c compute_nearest_neighbour.c

compute_upper_bound.o: compute_upper_bound.h compute_upper_bound.c
	gcc $(PAR) -c compute_upper_bound.c

compute_lagrange.o: compute_lagrange.h compute_lagrange.c
	gcc $(PAR) -c compute_lagrange.c

solve_tsp.o: solve_tsp.h solve_tsp.c
	gcc $(PAR) -c solve_tsp.c

utils.o: utils.h utils.c
	gcc $(PAR) -c utils.c

cycle.o: cycle.h cycle.c
	gcc $(PAR) -c cycle.c

clean:
	clear
	-rm test
	-rm test.a
	-rm test2opt
	-rm *.o
	-rm *.gch
