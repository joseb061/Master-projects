/* Code for reading the csv file, create the node and store it in a binary file
    Joseba Hernández Bravo y Jorge Vicente Puig 
    Ultima version 30/10 11:00 */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>

# include "structures.h"
# include "graph_construction_functions.h"

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

    segmentinfo *succesors;
    if(binary_writting(nodes, size_nodes, succesors) == 0) printf("\n The graph has been written in binary \n");

    clock_t tac = clock();
    printf("\n Execution time: %f\n", (double)(tac-tic)/CLOCKS_PER_SEC);
    return 0;
}

