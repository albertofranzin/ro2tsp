NOMEFILE = test.c
PAR = -g -O3 -lm

test: clean buildLib $(NOMEFILE)
	gcc $(NOMEFILE) -o $@ -L. -lro2ds $(PAR)

buildLib: node.o edge.o compute_ot.o utils.o solve_tsp.o compute_lagrange.o compute_path.o compute_mst.o graph.o heuristics.o
	ar r libro2ds.a *.o
#	rm *.o

compute_ot.o: compute_ot.h compute_ot.c
	gcc -c compute_ot.c

node.o: node.h
	gcc -c node.h

edge.o: edge.h
	gcc -c edge.h

utils.o: utils.h utils.c
	gcc -c utils.c

solve_tsp.o: solve_tsp.h solve_tsp.c
	gcc -c solve_tsp.c

compute_lagrange.o: compute_lagrange.h compute_lagrange.c
	gcc -c compute_lagrange.c

graph.o: graph.h graph.c
	gcc -c graph.c

compute_path.o: compute_path.h compute_path.c
	gcc -c compute_path.c

compute_mst.o: compute_mst.h compute_mst.c
	gcc -c compute_mst.c

heuristics.o: heuristics.h heuristics.c
	gcc -c heuristics.c

clean:
	clear
	-rm test
	-rm test.a
	-rm *.o
	-rm *.gch
