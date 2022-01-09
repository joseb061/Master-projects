#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>  // define FLT_MAX ...
#include <limits.h> // Defines infinities.
#include <math.h>

#include "Randombits.h"
#include "EDO_solver.h"
//#include "binary.h"


#define BETA 0.000024382635446
#define xo_value 21
#define phi_value 31
#define lamnda_value 25
#define mu_value 25
#define sigma_value 17
#define delta_value 15
#define POP_SIZE 100 // number of chromosomes.
#define nYears 12

const double real[] = {15329., 14177., 13031., 9762, 11271, 8688., 7571., 6983., 4778., 2067., 1586., 793.};

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

void ExitError(const char *miss, int errcode) {
    fprintf (stderr, "\nERROR: %s.\nStopping...\n\n", miss); exit(errcode);
}

void save_results(Type_chromosome chromosome[], ODE_Parameters pheno_params[],
        FILE *sol_save_fitnes, FILE *sol_save_years,FILE *sol_save_params, unsigned int k){

        
        fprintf(sol_save_fitnes, "%f\n", chromosome[0].fitness);
        fprintf(sol_save_years, "%f %f %f %f %f %f %f %f %f %f %f %f\n", chromosome[0].predicted_values[0], chromosome[0].predicted_values[1], chromosome[0].predicted_values[2],
               chromosome[0].predicted_values[3], chromosome[0].predicted_values[4], chromosome[0].predicted_values[5], chromosome[0].predicted_values[6],
               chromosome[0].predicted_values[7], chromosome[0].predicted_values[8], chromosome[0].predicted_values[9], chromosome[0].predicted_values[10], chromosome[0].predicted_values[11]);
        fprintf(sol_save_params, "%f %f %f %f %f %f\n", pheno_params[0].sigma, pheno_params[0].phi, pheno_params[0].delta, pheno_params[0].lambda, pheno_params[0].mu, pheno_params[0].x0);
    
        printf("-------------------EPOCH%d-------------------------\n", k);
        printf("Fitnes: %f\n", chromosome[0].fitness);
        printf("Sigma: %f Phi: %f delta: %f lamda= %f mu= %f xo= %f\n", pheno_params[0].sigma, pheno_params[0].phi, pheno_params[0].delta, pheno_params[0].lambda, pheno_params[0].mu, pheno_params[0].x0);
        printf("%f %f %f %f %f %f %f %f %f %f %f %f\n\n", chromosome[0].predicted_values[0], chromosome[0].predicted_values[1], chromosome[0].predicted_values[2],
               chromosome[0].predicted_values[3], chromosome[0].predicted_values[4], chromosome[0].predicted_values[5], chromosome[0].predicted_values[6],
               chromosome[0].predicted_values[7], chromosome[0].predicted_values[8], chromosome[0].predicted_values[9], chromosome[0].predicted_values[10], chromosome[0].predicted_values[11]);
}

void fill_random_chromo(Type_chromosome chromosome[], unsigned int N){
    int i;
    for (i = 0; i < N; i++){
        chromosome[i].x0 = N_random(xo_value);
        chromosome[i].phi = N_random(phi_value);
        chromosome[i].beta = BETA;
        chromosome[i].lambda = N_random(lamnda_value);
        chromosome[i].mu = N_random(mu_value);
        chromosome[i].sigma = N_random(sigma_value);
        chromosome[i].delta = N_random(delta_value);
    }
}

void fill_chromosomes(Type_chromosome chromosome[]){
    int i, j;
    double F = 0., NORM = 0.;

    //sum of all the fitnes
    for (i = 0; i < POP_SIZE; i++){
        F += chromosome[i].fitness;
    }

    for (i = 0; i < POP_SIZE; i++){
        NORM += F / chromosome[i].fitness;
    }

    // define accumulative probability.
    for (i = 0; i < POP_SIZE; i++){
        chromosome[i].p = (F / chromosome[i].fitness) / NORM;
        for (j = 0; j <= i; j++){
            chromosome[i].q += chromosome[j].p;
        }
    }
}

void Geno_to_pheno(ODE_Parameters output[], Type_chromosome input[], unsigned int N){
    int i;

    for (i = 0; i < N; i++){

        output[i].x0 = (16600.0 / ((1U << 21) - 1.0)) * (input[i].x0);
        output[i].phi = (100.35 / ((1U << 31) - 1.0)) * input[i].phi - 100;
        output[i].beta = input[i].beta;
        output[i].lambda = (3000.0 / ((1U << 25) - 1.0)) * (input[i].lambda);
        output[i].mu = (20.0 / ((1U << 25) - 1.0)) * (input[i].mu);
        output[i].sigma = (1000.0 / ((1U << 17) - 1.0)) * (input[i].sigma);
        output[i].delta = (25000.0 / ((1U << 15) - 1.0)) * (input[i].delta);
    }
}

void chromosome_copy(Type_chromosome destination[], Type_chromosome origin[], unsigned int N){
    int i;
    for (i = 0; i < N; i++){
        destination[i] = origin[i];
    }
}

int tournament_selection_2(Type_chromosome chromosome[], unsigned int t, unsigned int N){
    int i, j, best_index, max_random;
    unsigned int index;
    double best_fitnes = DBL_MAX, first_election;

    first_election = uniform();

    if (first_election < 0.80){      
        max_random = N * 0.2;
        
        for (j = 0; j < t; j++){
            index = uniform() * (max_random - 1);
            if (chromosome[index].fitness < best_fitnes){

                best_index = index;
                best_fitnes = chromosome[index].fitness;
            }
        }
        return best_index;
    }

    else{
        max_random = (0.2 + uniform() * 0.8) * N;
        for (j = 0; j < t; j++){
            index = uniform() * (max_random - 1);
            if (chromosome[index].fitness < best_fitnes){

                best_index = index;
                best_fitnes = chromosome[index].fitness;
            }
        }
        return best_index;
    }
}

void cross_over(Type_chromosome old_params[],
                           Type_chromosome new_params[], unsigned int keep,unsigned int N){
    int i, j;
    unsigned long child1, child2;
    unsigned int index1, index2;

    //emptying_structure(new_params, N);

    for (i = 0; i < N/keep; i++){
        new_params[i] = old_params[i];
    }

    for (i = N/keep; i < N - 1; i = i + 2){

        index1 = tournament_selection_2(old_params, N *.9, N);
        index2 = tournament_selection_2(old_params, N *.9, N);

        OnePointCrossover(old_params[index1].delta, old_params[index2].delta, &child1, &child2, delta_value);
        new_params[i].delta = child1;
        new_params[i + 1].delta = child2;

        OnePointCrossover(old_params[index1].lambda, old_params[index2].lambda, &child1, &child2, lamnda_value);
        new_params[i].lambda = child1;
        new_params[i + 1].lambda = child2;

        OnePointCrossover(old_params[index1].mu, old_params[index2].mu, &child1, &child2, mu_value);
        new_params[i].mu = child1;
        new_params[i + 1].mu = child2;

        OnePointCrossover(old_params[index1].phi, old_params[index2].phi, &child1, &child2, phi_value);
        new_params[i].phi = child1;
        new_params[i + 1].phi = child2;

        OnePointCrossover(old_params[index1].sigma, old_params[index2].sigma, &child1, &child2, sigma_value);
        new_params[i].sigma = child1;
        new_params[i + 1].sigma = child2;

        OnePointCrossover(old_params[index1].x0, old_params[index2].x0, &child1, &child2, xo_value);
        new_params[i].x0 = child1;
        new_params[i + 1].x0 = child2;
    }
}

void mutation(Type_chromosome chromosome[], double prob, unsigned int N){
    int i; 

    for (i = 0; i < POP_SIZE; i++){
        BitFlipMutation(&chromosome[i].x0, prob, xo_value);
        BitFlipMutation(&chromosome[i].phi, prob, phi_value);
        BitFlipMutation(&chromosome[i].lambda, prob, lamnda_value);
        BitFlipMutation(&chromosome[i].mu, prob, mu_value);
        BitFlipMutation(&chromosome[i].sigma, prob, sigma_value);
    }
}

void sort_by_fitnes(Type_chromosome chromo[], unsigned int N){
    Type_chromosome temp;
    int i, j;

    //Sort the array in ascending order
    for (int i = 0; i < N; i++){
        for (int j = i + 1; j < N; j++){
            if (chromo[i].fitness > chromo[j].fitness){
                temp = chromo[i];
                chromo[i] = chromo[j];
                chromo[j] = temp;
            }
        }
    }
}

void fitnesfunction(Type_chromosome chromosome[], unsigned int nyears, unsigned int N){
    //Casting pointers
    unsigned int i;
    unsigned int j;

    for (i = 0; i < N; i++){
        chromosome[i].fitness = 0.0;
        
        for (j = 0; j < nyears; j++){
            if (pow(real[j] - chromosome[i].predicted_values[j], 2) > chromosome[i].fitness){
                chromosome[i].fitness = pow(real[j] - chromosome[i].predicted_values[j], 2);
            }
        }
    }
}