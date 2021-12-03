# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h> // For computing the weigths of each segment
# include <limits.h> // Defines infinities.
# include <time.h>
# include "costable_0_0001.h"

# define MAX_ITER 10000000
# define N_iter 5
# define pi 3.14159265358979323846
# define CONST_2PI 6.28318530718
# define lerp(w, v1, v2) ((1.0 - (w)) * (v1) + (w) * (v2))
#define modd(x, y) ((x) - (int)((x) / (y)) * (y))

double absd(double a) { *((unsigned long *)&a) &= ~(1ULL << 63); return a; }

double cos_table_0_0001_LERP(double x)
{
    x = absd(x);
    x = modd(x, CONST_2PI);
    double i = x * 100.0;
    int index = (int)i;
    return lerp(i - index,        /* weight      */
        costable_0_0001[index],     /* lower value */
        costable_0_0001[index + 1]  /* upper value */
        );
}

double deg2rad(double deg) {
  return (deg * pi / 180);
}

double rad2deg(double rad) {
  return (rad * 180 / pi);
}

/* --------------------------------- Law of spherical cosines --------------------------- */
double cos_weight_imp1(float lat1, float lon1, float lat2, float lon2){
    double theta, dist;
    if ((lat1 == lat2) && (lon1 == lon2)) {
        return 0;
    } else{
        theta = lon1 - lon2;
        dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
        dist = acos(dist);
        dist = rad2deg(dist);
        dist = dist * 60 * 1.1515;
        dist = dist * 1.609344;
        return dist;
    }
}

double cos_weight_imp2(float lat1, float lon1, float lat2, float lon2){
    double theta, dist;
    if ((lat1 == lat2) && (lon1 == lon2)) {
        return 0;
    } else{
        theta = lon1 - lon2;
        dist = 57.2957795131*(acos(sin(0.01745329251*(lat1)) * sin(0.01745329251*(lat2)) + cos(0.01745329251*(lat1)) * cos(0.01745329251*(lat2)) * cos(0.01745329251*(theta))))*111.18957696;
        return dist;
    }
}

double cos_weight_imp3(float lat1, float lon1, float lat2, float lon2){
    double theta, dist;
    if ((lat1 == lat2) && (lon1 == lon2)) {
        return 0;
    } else{
        theta = lon1 - lon2;
        dist = 57.2957795131*(acos(sin(0.01745329251*(lat1)) * sin(0.01745329251*(lat2)) + cos_table_0_0001_LERP(0.01745329251*(lat1)) * cos_table_0_0001_LERP(0.01745329251*(lat2)) * cos_table_0_0001_LERP(0.01745329251*(theta))))*111.18957696;
        return dist;
    }
}


int main(){
	register unsigned long k;
	double dist;
	float measured_time1=0, measured_time2=0, measured_time3=0;
	float lat1=38.6048094, lat2=40.4202342, lon1=-0.0489952, lon2=-3.6877944;

	for(int l=0; l<N_iter;l++){
		clock_t tic1 = clock();
		for( k=0; k<MAX_ITER; k++){
			cos_weight_imp1( lat1,  lon1,  lat2,  lon2);
		}
	    clock_t toc1 = clock();
	    measured_time1 += (double)(toc1 - tic1) / CLOCKS_PER_SEC;

		clock_t tic2 = clock();
		for( k=0; k<MAX_ITER; k++){
			cos_weight_imp2( lat1,  lon1,  lat2,  lon2);
		}
	    clock_t toc2 = clock();
	    measured_time2 += (double)(toc2 - tic2) / CLOCKS_PER_SEC;

		clock_t tic3 = clock();
		for( k=0; k<MAX_ITER; k++){
			cos_weight_imp3( lat1,  lon1,  lat2,  lon2);
		}
	    clock_t toc3 = clock();
	    measured_time3 += (double)(toc3 - tic3) / CLOCKS_PER_SEC;
	}

	// Printing the results
    printf("Execution time implementation 1 %f \n", measured_time1/N_iter);
	printf("Execution time implementation 2 %f \n", measured_time2/N_iter);
	printf("Execution time implementation 3 %f \n", measured_time3/N_iter);

	return 0;
}

