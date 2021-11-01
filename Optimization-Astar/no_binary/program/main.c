/* Code for reading the csv file, create the node and store it in a binary file
    Joseba Hernández Bravo y Jorge Vicente Puig 
    Ultima version 30/10 11:00 */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>

# include "structures.h"
# include "graph_construction_functions.h"
# include "functions_astar_nb.h"

int main(){
    /* ---------------------- Graph Construction ---------------------- */
	// Execution time
	clock_t tic = clock();

	// Creating and opening the file
	FILE *data;
	data=fopen("../data/cataluna.csv","r");
    if(data==NULL)
    {
        printf("\nWe cannot acces to the nodes data file\n");
        return 1;
    } else printf("\n Data opened \n");

    // Reserving memory for the nodes
    long int size_nodes = 3472620; // Nodes of the cataluna.csv
    node *nodes;
    if((nodes=(node *)malloc(size_nodes*sizeof(node)))==NULL)
    {
        printf("Error in the memory allocation of nodes.\n");
        return 2;
    } else printf("\n Memory reserved for the data. \n");

    // Reading the nodes and edges for constructing the graph structure
    file_reader(nodes, data, size_nodes);
    fclose(data);
    printf("\n Data closed.\n");

    /*
        falta aqui guardar el archivo en binario
    */


    /* ---------------------------- AStar ------------------------------------------ */
        
    /*
        falta aqui leer el archivo en binario
    */

    // Defining structures and variables to use in the AStar program
    AStarPath *PathData; // Empty way to use in the AStar
    if((PathData = (AStarPath *)malloc(size_nodes*sizeof(AStarPath)))==NULL){
        printf("error in the memory allocation of the path data.\n");
        return 2;
    }
    unsigned long node_start = 771979683 , node_goal = 429854583; // Id of the nodes
    unsigned node_start_pos = binarysearch(node_start, nodes, size_nodes);
    unsigned node_goal_pos = binarysearch(node_goal, nodes, size_nodes);

    bool r = AStar(nodes, PathData, size_nodes, node_start_pos, node_goal_pos);
    
    if (r == -1)
        ExitError("in allocating memory for the OPEN list in AStar", 21);
    else if (!r)
        ExitError("no solution found in AStar", 7);
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

