NOMEFILE = test.c

PAR = -g -O0 -lm
# -g  : debug symbols
# -O3 : badass optimization
# -lm : linking to math lib math.h

#OPT = -ftree-vectorizer-verbose=2 -finline-functions -std=c99

CHECK = -Winline
# -Winline : warn if any problem in including inline functions occurs

all: test clean

test: tree.o
	gcc tree.o $(NOMEFILE) -o tree $(PAR)

tree.o: tree.h tree.c
	gcc -g -c tree.c

clean:
	rm ./*o
	rm ./*~