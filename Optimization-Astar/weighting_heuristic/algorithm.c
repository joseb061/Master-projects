# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h> 
# include <limits.h>
# include <time.h>
# include "structures.h"
# include "functions_astar_nb.h"

# define number_nodes_SPAIN 23895681

void principal_func(node *nodes, float lambda, float *dist, unsigned long *counter, float *time){

    AStarPath *PathData; // Empty way to use in the AStar
    if((PathData = (AStarPath *)malloc(number_nodes_SPAIN*sizeof(AStarPath)))==NULL){
        printf("error in the memory allocation of the path data.\n");
    } 

    unsigned long node_start = 240949599 , node_goal = 195977239  ; // Id of the nodes
    unsigned node_start_pos = binarysearch(node_start, nodes, number_nodes_SPAIN);
    unsigned node_goal_pos = binarysearch(node_goal, nodes, number_nodes_SPAIN);

    /* ---------------------- AStar ---------------------- */
    clock_t tic = clock();
    *counter = AStar(nodes, PathData, number_nodes_SPAIN, node_start_pos, node_goal_pos, lambda);
    clock_t toc = clock();
    float measured_time = (double)(toc - tic) / CLOCKS_PER_SEC;

    *time = measured_time;
    *dist = PathData[node_goal_pos].g;
    free(PathData);
}

int main(){
    float lambda_vec[101];
    float dist_vect[101];
    float time_vector[101];
    unsigned long counter_vect[101];

    node *nodes;
    unsigned long size_nodes = number_nodes_SPAIN;
    
    // Saving memory for nodes data
    if((nodes = (node *) malloc(size_nodes* sizeof(node))) == NULL)
        ExitError("when allocating memory for the nodes vector", 13);

    if((binary_reader(nodes)) == 0) printf("The graph has been readed and stored\n");

    // Doing 101 computations with different lambda values and saving the results
    for(int k=0; k<101;k++){
        lambda_vec[k] = 0.8 + (0.004)*k;
        principal_func(nodes, lambda_vec[k], &dist_vect[k], &counter_vect[k], &time_vector[k]);
        printf("%f;%f;%u;%f \n", lambda_vec[k], dist_vect[k], counter_vect[k], time_vector[k]);
    }

    // Saving results
    FILE *sol_save_txt;
    if ((sol_save_txt = fopen ("lambda_resuts.txt", "w")) == NULL)
        ExitError("the output binary data file cannot be opened", 31);
    
    for(int k=0; k<100;k++){
        fprintf(sol_save_txt, "%f;%f;%u;%f \n", lambda_vec[k], dist_vect[k], counter_vect[k], time_vector[k]);
    }
    fclose(sol_save_txt);   

    return 0;
}