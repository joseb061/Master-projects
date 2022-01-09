#include <stdio.h>
#include <time.h>
#include <limits.h>
# include <limits.h> // Defines infinities.
#include <math.h>

#define USHRT_WIDTH 16
#define UINT_WIDTH 32

/*********************************
 * ran1 from "Numerical Recipes" *
 * note #undef's at end of file  *
**********************************/
#define IA 16807
#define IM 2147483647L
#define AM (1.0/IM)
#define IQ 127773L
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

float ran1(long *idum){
    int j; long k;
    static long iy=0;
    static long iv[NTAB];
    float temp;

    if (*idum <= 0 || !iy) {
        if (-(*idum) < 1) *idum=1; else *idum = -(*idum);
        for (j=NTAB+7;j>=0;j--) {
            k=(*idum)/IQ;
            *idum=IA*(*idum-k*IQ)-IR*k;
            if (*idum < 0) *idum += IM;
            if (j < NTAB) iv[j] = *idum;
        }
        iy=iv[0];
    }
    k=(*idum)/IQ;
    *idum=IA*(*idum-k*IQ)-IR*k;
    if (*idum < 0) *idum += IM;
    j=(int) (iy/(long)NDIV);
    iy=iv[j];
    iv[j] = *idum;
    if ((temp=AM*iy) > RNMX) return RNMX;
    else return temp;
} /* ran1 */

#undef IA
#undef IM
#undef AM
#undef IQ
#undef IR
#undef NTAB
#undef NDIV
#undef EPS
#undef RNMX

/* Utility functions to simplify the use of ran1 and idum */

long idum;

void randomize(void) {idum = time(NULL); if(idum > 0) idum = -idum;}

float uniform(void) {return ran1(&idum);} // between 0.0 and 1.0

// /* Based on von-neuman observation ; rather inefficient; */
unsigned char random_bit(void){ 
    unsigned char f, s;
    do { f = 2*ran1(&idum); s = 2*ran1(&idum); } while (f == s);
    return f;
}

long aleatorio(long min, long max){
    return min + ran1(&idum)*(max-min);
}

unsigned long intRandoms(int lower, int upper){
    unsigned long upp, num;
    upp = pow(2,upper)-1;
    num = (rand() % (upp - lower + 1)) + lower;
    return num;
}

unsigned long intRandoms2(int lower, int upper){
    unsigned int num;
    num = (rand() % (upper - lower + 1)) + lower;
    return num;
}

unsigned UINTran(double width_number){ 
    register unsigned char i;
    unsigned oneUL = 1UL, base = 0UL;
    for(i=0; i < width_number; i++) if(random_bit()) { base = oneUL; break; }
    for(i++; i < width_number; i++){ base = base << 1;
        if(random_bit()) base = base | oneUL;
    }
    return base;
}

unsigned long long int N_random(int N){
    register unsigned char i;
    unsigned long long int oneUL = 1UL, base = 0UL;

    for (i = 0; i < N; i++)
        if (random_bit()) {
            base = oneUL;
            break;
        }
    for (i++; i < N; i++) {
        base = base << 1;
        if (random_bit())
            base = base | oneUL;
    }
    return base;
}






