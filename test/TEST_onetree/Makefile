NOMEFILE = test.c

PAR = -g -O0 -lm
# -g  : debug symbols
# -O3 : badass optimization
# -lm : linking to math lib math.h

#OPT = -ftree-vectorizer-verbose=2 -finline-functions -std=c99

CHECK = -Winline
# -Winline : warn if any problem in including inline functions occurs

all: test clean

test: onetree.o
	gcc onetree.o $(NOMEFILE) -o onetree $(PAR)

onetree.o: onetree.h onetree.c
	gcc -g -c onetree.c

clean:
	rm ./*o
	rm ./*~