
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h> // For computing the weigths of each segment
# include <limits.h> // Defines infinities.


#include "structures.h"
# include "functions_astar_nb.h"

#define number_nodes_SPAIN 23895681

int main(){
    
    node *nodes;
    unsigned long size_nodes = number_nodes_SPAIN;
	
    /* getting memory for all data */
    if((nodes = (node *) malloc(size_nodes* sizeof(node))) == NULL)
        ExitError("when allocating memory for the nodes vector", 13);

    if((binary_reader(nodes)) == 0) printf("The graph has been readed and stored\n");
    

	/* ---------------------- AStar ---------------------- */
    AStarPath *PathData; // Empty way to use in the AStar
    if((PathData = (AStarPath *)malloc(size_nodes*sizeof(AStarPath)))==NULL){
        printf("error in the memory allocation of the path data.\n");
        return 2;
    }
    
    
    unsigned long node_start = 240949599 , node_goal = 195977239  ; // Id of the nodes
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
    printf("\n Source: %lu Source_lat: %lf Source_lon: %lf \n", nodes[node_start_pos].id,nodes[node_start_pos].latitude,nodes[node_start_pos].longitude );
    printf("\n Goal: %lu Goal_lat: %lf Goal_lon: %lf \n", nodes[node_goal_pos].id, nodes[node_goal_pos].latitude, nodes[node_goal_pos].longitude);
    printf("\n Total distance: %f\n", PathData[node_goal_pos].g);

    return 0;
}