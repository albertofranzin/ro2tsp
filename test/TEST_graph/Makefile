NOMEFILE = test.c

PAR = -g -O0 -lm
# -g  : debug symbols
# -O3 : badass optimization
# -lm : linking to math lib math.h

#OPT = -ftree-vectorizer-verbose=2 -finline-functions -std=c99

CHECK = -Winline
# -Winline : warn if any problem in including inline functions occurs

all: test clean

test: graph.o
	gcc graph.o $(NOMEFILE) -o test $(PAR)

graph.o: graph.h graph.c
	gcc -g -c graph.c

clean:
	rm ./*o
	rm ./*~