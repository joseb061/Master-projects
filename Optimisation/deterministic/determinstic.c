#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

const double sigmavec[] = {0.0001,0.001,0.0023,0.01,0.03,0.06,0.09,0.1,0.13,0.16,0.26,0.36, 0.49,
0.491,0.492,0.493,0.494,0.495,0.497,0.496, 0.5};
const double rhovec[] = {0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.89,0.99,0.991,0.992,0.993,0.994,0.995, 0.996, 0.997};

#define outputresultsGradient_init "results/data/gradient_resuts.txt"
#define outputresultsConjugate_init "results/data/conjugate_resuts.txt"

#define outputresultsGradient_modif "results/data/gradient_resuts_modif.txt"
#define outputresultsConjugate_modif "results/data/conjugate_resuts_modif.txt"

#define bestresultsGradient "results/data/gradient_iter_resuts.txt"
#define bestresultsConjugate "results/data/conjugate_iter_resuts.txt"

/*---------------------------------------------------*/
/*              ROSENBROC FUNCTION                   */
/*---------------------------------------------------*/
double evalRonsenbroc(double x[]){
    double f; 

    f = 100*pow(x[1]-pow(x[0],2),2) + pow(1 -x[0],2);
    return f;
}

void jacobianRonsenbroc(double x[], double g[]){
    
    g[0] = 2.*(200*pow(x[0],3) -200*x[0]*x[1]+x[0]-1);
    g[1] =  200.*(x[1]-pow(x[0],2));
    
    // module
    g[2] = sqrt( pow(g[0],2) +  pow(g[1],2) );
}

/*---------------------------------------------------*/
/*                    METHODS                        */
/*---------------------------------------------------*/

void ExitError(const char *miss, int errcode) {
    fprintf (stderr, "\nERROR: %s.\nStopping...\n\n", miss); exit(errcode);
}

int directionGradient(double x[], double g[],double d[]){
    double minumumValue = 0.000000001;

    jacobianRonsenbroc(x,g);
    //printf("gradiente %.10lf\n", g[2]);
    if (g[2] < minumumValue){
       return 22; 
    }
    else{
        d[0] = -g[0];
        d[1] = -g[1];
    }
    return 0; 
}

int directionConjugate(double x[], double g[],double d[]){
    double minumumValue = 0.000000001;
    double beta, gold, gnew;

    gold = g[2];
    jacobianRonsenbroc(x,g);
    gnew = g[2];
    beta =  pow(gnew/gold,2);

    if (g[2] < minumumValue){
       return 22; 
    }
    else{
        d[0] = -g[0] + beta*d[0];
        d[1] = -g[1] + beta*d[1];
    }
    return 0; 
}

double SteepestDescent(double x[],double jacobian[], double d[],double rho, double sigma){
    double alpha=1.;
    //double sigma = SIGMA, rho = RHO; 
    double xaux[2];
    double fk, fknext, product;
    

    xaux[0] = x[0];
    xaux[1] = x[1];

    fk = evalRonsenbroc(x);
    
    x[0] = x[0] + alpha*d[0];
    x[1] = x[1] + alpha*d[1];
    fknext = evalRonsenbroc(x);
    
    product = jacobian[0]*d[0] + jacobian[1]*d[1];
    while(fknext > fk + sigma*alpha*product){
        alpha = rho*alpha;
        x[0] = xaux[0] + alpha*d[0];
        x[1] = xaux[1] + alpha*d[1];
        fknext = evalRonsenbroc(x);
    }

    return alpha;
}

int main(){
    FILE *sol_save_conjugate, *sol_save_txt;
    FILE *iter_save_txt, *iter_save_conjugate;
    double alpha;
    int epsilonR, i;
    double x[2], d[2], jacobian[3];
    double sigma, rho;

    // Real initial point. 
    x[0] = -1.5;
    x[1] = -1;

    // Arbitrary initial point.
    // x[0] = 3.5;
    // x[1] = 1.3;

    epsilonR = directionGradient(x,jacobian,d);
    
    // // Gradient method.
    // sigma = 0.5;
    // rho = 0.994;

    // if ((sol_save_txt = fopen (outputresultsGradient_modif, "w")) == NULL)
    //     ExitError("the output gradient data file cannot be opened", 31);
    // fprintf(sol_save_txt, "%lf %lf %lf\n", x[0], x[1], evalRonsenbroc(x));

    // while (epsilonR != 22){
    //     alpha = SteepestDescent(x,jacobian,d,rho, sigma);
    //     printf("x:%.8lf\t y:%.8lf\t alpha %.8lf\n", x[0], x[1], alpha);
    //     fprintf(sol_save_txt, "%lf %lf %lf\n", x[0], x[1], evalRonsenbroc(x));
    //     epsilonR = directionGradient(x,jacobian,d);  
    //     i++;   
    // }
  
    // Gradient conjugate method.
    sigma = 0.495;
    rho = 0.991;
    
    if ((sol_save_conjugate = fopen (outputresultsConjugate_modif, "w")) == NULL)
        ExitError("the output conjugate data file cannot be opened", 31);

    fprintf(sol_save_conjugate, "%lf %lf %lf\n", x[0], x[1], evalRonsenbroc(x));
    
    while (epsilonR != 22){
        alpha = SteepestDescent(x,jacobian,d,rho, sigma);
        printf("x:%.8lf\t y:%.8lf\t alpha %.8lf\n", x[0], x[1], alpha);
        fprintf(sol_save_conjugate, "%lf %lf %lf\n", x[0], x[1], evalRonsenbroc(x));
        epsilonR = directionConjugate(x,jacobian,d);
        i++;   
    }
    printf("\nnumber of iter: %d\n", i);
    // // // count iterations
    // if ((iter_save_txt = fopen (bestresultsGradient, "w")) == NULL)
    //     ExitError("the output gradient data file cannot be opened", 31);

    // if ((iter_save_conjugate = fopen (bestresultsConjugate, "w")) == NULL)
    //     ExitError("the output conjugate data file cannot be opened", 31);

    // for (int j=0; j<16; j++){
    //     rho = rhovec[j];
    //     for (int r=0; r< 21; r++){
    //         sigma = sigmavec[r];

    //         i = 0;
    //         x[0] = -1.5;
    //         x[1] = -1;
    //         epsilonR = directionGradient(x,jacobian,d);
    //         while (epsilonR != 22){
    //             alpha = SteepestDescent(x,jacobian,d, rho, sigma);
    //             epsilonR = directionGradient(x,jacobian,d);  
    //             i++;   
    //         }
    //         fprintf(iter_save_txt, "%lf %lf %d\n", rho,sigma,i);
            
    //         i = 0;
    //         x[0] = -1.5;
    //         x[1] = -1;
    //         epsilonR = directionGradient(x,jacobian,d);
    //         while (epsilonR != 22){
    //             alpha = SteepestDescent(x,jacobian,d,rho,sigma);
    //             epsilonR = directionConjugate(x,jacobian,d);
    //             i++;   
    //         }
    //         fprintf(iter_save_conjugate, "%lf %lf %d\n", rho, sigma,i);
    //     }
    // }

    return 0;
}

