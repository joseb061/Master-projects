# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h> // For computing the weigths of each segment
# include <limits.h> // Defines infinities.

// Modified crossover to take into account width of parents and children
void OnePointCrossover(unsigned long int p1, unsigned long int p2, unsigned long int *f1, unsigned long int *f2,unsigned  char width){
	/* d \in [1, 8*sizeof(unsigned int)-1] */
	unsigned char d = uniform()*(width-1) + 1;
	unsigned long int mask = 0xFFFFFFFFFFFFFFFFU << d;
	*f1 = (p1 & mask) | (p2 & ~mask);
	*f2 = (p2 & mask) | (p1 & ~mask);
}

void TwoPointCrossover(unsigned int p1, unsigned int p2, unsigned int *f1, unsigned int *f2){
    unsigned char len = 8*sizeof(unsigned int);
    unsigned char p = uniform()*(len-2) + 1; /* p \in [1, len-2] */
    /* We want a mask with p 0's at the beginning; len-q zeros at the end;
    * and 1's between the positions p and q inclusive
    * where q \in [p+1, len-1].
    * CONSISTENCY NOTE: p <= len-2 ==> p+1 <= len-1
    * THEN: mask = (0xFFFFFFFFU >> (len-q+p)) << p;
    * OBSERVE THAT: p+1 <= len-q+p <= len-1 is a random number
    * THUS: len-q+p = uniform()*(len-p-1) + p+1; */
    unsigned int mask = (0xFFFFFFFFU >> (((unsigned char) uniform()*(len-p-1)) + p+1)) << p;
    *f1 = (p1 & mask) | (p2 & ~mask);
    *f2 = (p2 & mask) | (p1 & ~mask);
}

void UniformCrossover(unsigned int p1, unsigned int p2, unsigned int *f1, unsigned int *f2, double prob){
    unsigned char len = 8*sizeof(*f1);
    unsigned int mask = 0U;
    register unsigned char i;
    for(i=0; i < len; i++) if(uniform() < prob) mask = mask | (1U << i);
    *f1 = (p1 & mask) | (p2 & ~mask);
    *f2 = (p2 & mask) | (p1 & ~mask);
}

void BitFlipMutation(unsigned long int *f, double prob, unsigned char width){
	register unsigned char i;
	//Only allowed to flip bits for the given width. Otherwise it will change bits that are out of range for a given parameter
	for(i=0; i < width; i++) if(uniform() < prob) *f = (*f)^(1U << i);
}



