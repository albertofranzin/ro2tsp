NOMEFILE = test.c

PAR = -g -O0 -lm
# -g  : debug symbols
# -O3 : badass optimization
# -lm : linking to math lib math.h

#OPT = -ftree-vectorizer-verbose=2 -finline-functions -std=c99

CHECK = -Winline
# -Winline : warn if any problem in including inline functions occurs

all: test clean

test: elist.o
	gcc elist.o $(NOMEFILE) -o elist $(PAR)

list.o: edge.h elist.h elist.c
	gcc -g -c elist.c

clean:
	rm ./*o
	rm ./*~