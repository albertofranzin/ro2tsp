NOMEFILE = test.c
PAR = -g -O3 -lm

test: clean buildLib $(NOMEFILE)
	gcc $(NOMEFILE) -o $@ -L. -lro2ds $(PAR)

buildLib: set.o rbtree.o node.o edge.o matrixGraph.o utils.o plot.o branchBoundUtils.o tspHeuristics.o
	ar r libro2ds.a *.o
#	rm *.o

set.o: set.h set.c
	gcc -c set.c

rbtree.o: rbtree.h rbtree.c
	gcc -c rbtree.c

node.o: node.h node.c
	gcc -c node.c

edge.o: edge.h edge.c
	gcc -c edge.c

utils.o: utils.h utils.c
	gcc -c utils.c

matrixGraph.o: matrixGraph.h matrixGraph.c
	gcc -c matrixGraph.c

plot.o: plot.h plot.c
	gcc -c plot.c

branchBoundUtils.o: branchBoundUtils.h branchBoundUtils.c
	gcc -c branchBoundUtils.c

tspHeuristics.o: tspHeuristics.h tspHeuristics.c
	gcc -c tspHeuristics.c

clean:
	clear
	-rm test
	-rm makefile~
	-rm test.a
	-rm *.o
	-rm *.gch
