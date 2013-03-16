NOMEFILE = branchAndBound1.c
PAR = -lm -g -O3

branchAndBound1: clean buildLib $(NOMEFILE)
	gcc $(NOMEFILE) -o $@ -L. -lro2ds $(PAR)

buildLib: itemset.o set.o rbtree.o node.o edge.o matrixGraph.o utils.o plot.o
	ar r libro2ds.a *.o
	rm *.o

itemset.o: ds/itemset.h ds/itemset.c
	gcc -c ds/itemset.c

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

clean:
	clear
	-rm branchAndBound1
	-rm branchAndBound1.a
	-rm libro2ds.a
	-rm *.o
	-rm *.gch
	-rm ds/*.o
	-rm ds/*.gch
