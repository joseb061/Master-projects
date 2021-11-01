
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
# include <math.h> // For computing the weigths of each segment
# include <limits.h> // Defines infinities.
# define pi 3.14159265358979323846

# include "structures.h"
# include "functions_astar_nb.h"

int main(){
    long int size_nodes = 3472620; // Nodes of the cataluna.csv

	// Execution time
	clock_t tic = clock();

	// Reading the binary file
	node *nodes;
	segmentinfo *succesor;
	if(binary_reader(nodes, succesor) == 0) printf("The graph has been readed and stored");

	/* ---------------------- AStar ---------------------- */
    AStarPath *PathData; // Empty way to use in the AStar
    if((PathData = (AStarPath *)malloc(size_nodes*sizeof(AStarPath)))==NULL){
        printf("error in the memory allocation of the path data.\n");
        return 2;
    }
    unsigned long node_start = 771979683 , node_goal = 429854583; // Id of the nodes
    unsigned node_start_pos = binarysearch(node_start, nodes, size_nodes);
    unsigned node_goal_pos = binarysearch(node_goal, nodes, size_nodes);

    bool r = AStar(nodes, PathData, size_nodes, node_start_pos, node_goal_pos);
    
    if (r == -1){
        printf("in allocating memory for the OPEN list in AStar");
        return 21;
    }
    else if (!r){
        printf("no solution found in AStar");
    	return 7;
	}
    else printf("\n AStar completed \n");

    // Priting the results
    printf("\n Optimal path found:\n");
    printf("\n ----------|---------\n");
    printf("\n Source: %ld\n", nodes[node_start_pos].id);
    printf("\n Goal: %ld\n", nodes[node_goal_pos].id);
    printf("\n Total distance: %f\n", PathData[node_goal_pos].g);

    clock_t tac = clock();
    printf("\n Execution time: %f\n", (double)(tac-tic)/CLOCKS_PER_SEC);
    return 0;
}