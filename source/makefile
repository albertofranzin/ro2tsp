NOMEFILE1 = branchAndBound1.c
NOMEFILE2 = branchAndBound2.c
PAR = -lm -g -O3

branchAndBound1: clean buildLib $(NOMEFILE1)
	gcc $(NOMEFILE1) -o $@ -L. -lro2ds $(PAR)

#branchAndBound2: clean buildLib $(NOMEFILE2)
#	gcc $(NOMEFILE2) -o $@ -L. -lro2ds $(PAR)

buildLib: set.o rbtree.o node.o edge.o matrixGraph.o utils.o plot.o branchBoundUtils.o
	ar r libro2ds.a *.o
	rm *.o

set.o: ds/set.h ds/set.c
	gcc -c ds/set.c

rbtree.o: ds/rbtree.h ds/rbtree.c
	gcc -c ds/rbtree.c

node.o: ds/node.h ds/node.c
	gcc -c ds/node.c

edge.o: ds/edge.h ds/edge.c
	gcc -c ds/edge.c

utils.o: ds/utils.h ds/utils.c
	gcc -c ds/utils.c

matrixGraph.o: ds/matrixGraph.h ds/matrixGraph.c
	gcc -c ds/matrixGraph.c

plot.o: ds/plot.h ds/plot.c
	gcc -c ds/plot.c

branchBoundUtils.o: ds/branchBoundUtils.h ds/branchBoundUtils.c
	gcc -c ds/branchBoundUtils.c

clean:
	clear
	-rm branchAndBound1
	-rm branchAndBound1.a
	-rm branchAndBound2
	-rm branchAndBound2.a
	-rm libro2ds.a
	-rm *.o
	-rm *.gch
	-rm ds/*.o
	-rm ds/*.gch
