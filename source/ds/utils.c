#include "utils.h"

void initializeRandom(long seed) {
	srand(seed);
	printf("utils.c :: initializeRandom :: srand done\n");
}

void initializeRandomWithTime() {
	time_t now = time ( 0 ); 
	unsigned char *p = (unsigned char *)&now;
	unsigned seed = 0;
	size_t i;

	for ( i = 0; i < sizeof now; i++ ) 
		seed = seed * ( UCHAR_MAX + 2U ) + p[i];

	initializeRandom(seed);
}
