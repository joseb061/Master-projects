#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

#define POP_SIZE 100 // number of chromosomes.
#define MAXGENS 5000   // number of iterations.


#define nYears 12

#define outputresults "results/fitnes_resuts.txt"
#define outputparams "results/params_resuts.txt"
#define outputyears "results/years_resuts.txt"


int main()
{
    Type_chromosome chromosome[POP_SIZE], new_population[POP_SIZE];
    ODE_Parameters pheno_params[POP_SIZE]; // only for ODE
    FILE *sol_save_txt, *sol_save_params, *sol_save_years;
    unsigned int list[POP_SIZE], keep;
    unsigned long child1, child2;
    int i, j, k, best_index;

    if ((sol_save_txt = fopen (outputresults, "w")) == NULL)
        ExitError("the output binary data file cannot be opened", 31);
    
    if ((sol_save_params = fopen (outputparams, "w")) == NULL)
        ExitError("the output binary data file cannot be opened", 31);
    
    if ((sol_save_years = fopen (outputyears, "w")) == NULL)
        ExitError("the output binary data file cannot be opened", 31);

    fill_random_chromo(chromosome, POP_SIZE);
    for (k = 0; k < MAXGENS; k++){

       keep = 8;     
        Geno_to_pheno(pheno_params, chromosome, POP_SIZE);
        for (i = 0; i < POP_SIZE; i++){
            Generate_EDO_Prediction(chromosome[i].predicted_values,
                                    pheno_params[i].x0, nYears, &pheno_params[i]);
        }

        fitnesfunction(chromosome, nYears, POP_SIZE);
        sort_by_fitnes(chromosome, POP_SIZE);
        Geno_to_pheno(pheno_params, chromosome, POP_SIZE);
        save_results(chromosome, pheno_params,sol_save_txt,sol_save_years,sol_save_params,k);
        cross_over(chromosome, new_population, keep,POP_SIZE);
        chromosome_copy(chromosome, new_population, POP_SIZE);
        mutation(chromosome, 0.01, POP_SIZE);  
    }

    fclose(sol_save_txt);
    fclose(sol_save_years);
    fclose(sol_save_params);

    return 0;
}