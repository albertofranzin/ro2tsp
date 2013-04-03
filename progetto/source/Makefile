NOMEFILE = test.c

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

# link libraries
buildLib: node.o edge.o compute_ot.o utils.o solve_tsp.o compute_path.o \
		  compute_mst.o graph.o heuristics.o lagrangian.o tree.o subgradient.o
	ar r libro2ds.a *.o
#	rm *.o

compute_ot.o: compute_ot.h compute_ot.c
	gcc -g -c compute_ot.c

node.o: node.h
	gcc -g -c node.h

edge.o: edge.h
	gcc -g -c edge.h

utils.o: utils.h utils.c
	gcc -g -c utils.c

solve_tsp.o: solve_tsp.h solve_tsp.c
	gcc -g -c solve_tsp.c

graph.o: graph.h graph.c
	gcc -g -c graph.c

compute_path.o: compute_path.h compute_path.c
	gcc -g -c compute_path.c

compute_mst.o: compute_mst.h compute_mst.c
	gcc -g -c compute_mst.c

heuristics.o: heuristics.h heuristics.c
	gcc -g -c heuristics.c

lagrangian.o: lagrangian.h lagrangian.c
	gcc -g -c lagrangian.c

tree.o: tree.h tree.c
	gcc -g -c tree.c

subgradient.o : subgradient.h subgradient.c
	gcc -g -c subgradient.c

clean:
	clear
	-rm test
	-rm test.a
	-rm *.o
	-rm *.gch
