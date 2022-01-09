# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h> // For computing the weigths of each segment
# include <limits.h> // Defines infinities.
# include <float.h>

# include "rk78/RKF78.c"

#define HMAX 1.0
#define HMIN 1.e-6
#define RKTOL 1.e-8
#define ElliotSigmoidSCALE 1000
#define TwoElliotSigmoidSCALE 2000
#define ZeRoParsThreshold 0.00001
#define nYears 12
const double z[] = {15329.,14177.,13031.,9762,11271,8688.,7571.,6983.,4778.,2067.,1586.,793.};

typedef struct {
    double x0;
    double phi; 
    double beta;
    double lambda;
    double mu;
    double sigma;
    double delta;
} ODE_Parameters;

typedef struct{
    unsigned long int x0;
    unsigned long int phi;
    unsigned long int lambda;
    unsigned long int mu;
    unsigned long int sigma;
    unsigned long int delta;
    double beta;
    double predicted_values[nYears];
    double fitness; // the fitness of the chromosome
    double p;       // chromosome selection probability
    double q;       // acumulated prob. with respect of the other chromosomes
} Type_chromosome;

double ElliotSigmoid(double x, double sigma, double delta) {
    x = sigma*(x-delta);
    return x/(ElliotSigmoidSCALE + fabs(x));
}

double Psi(double x, double mu, double sigma, double delta){
        //if(fabs(sigma) < ZeRoParsThreshold) return 1.0; //What is this?
        double ES = ElliotSigmoid(x, sigma, delta);
        sigma *= delta;
		if(x < delta) {
        	x /= delta;
        	ES = ES * (x + (mu*(1.0-x)*(sigma + ElliotSigmoidSCALE)) / (sigma + TwoElliotSigmoidSCALE));
        }
        return ((1 - ES)*(sigma + TwoElliotSigmoidSCALE)) / (sigma*(1+mu) + TwoElliotSigmoidSCALE);
    }

void MigrationODE(double t, double x, double *der, void *Params){
    ODE_Parameters *par = (ODE_Parameters *) Params; // Pointer cast to save typing and thinking
    *der = par->phi * x - par->beta*x*x - par->lambda*Psi(x, par->mu, par->sigma, par->delta);
}

//Resolve the EDO using RK78 
int Generate_EDO_Prediction(double *xt, double x0,
							unsigned short number_of_years,
							ODE_Parameters *pars){
	register unsigned ty;
	xt[0] = x0;
	for (ty=1; ty<number_of_years;ty++) xt[ty] = 0.0;
	double t = 0.0, err, h = 1.e-3;
	for(ty=1; ty<number_of_years;ty++){int status;
		while(t+h<ty){
			status = RKF78(&t,&x0,&h,&err,HMIN,HMAX,RKTOL,pars,MigrationODE);
			if(status){ 
				if (ty == (number_of_years-1)) printf("Failed int at year %i\n",ty);
				return status;
			}
		} 
	
		h = ty-t;
		status = RKF78(&t,&x0,&h,&err,HMIN,HMAX,RKTOL,pars,MigrationODE);
		if(status){ 
			if (ty == (number_of_years-1)) printf("Failed int at year %i\n",ty);
			return status;
		}
		xt[ty] = x0;
	}
	return 0;
}

