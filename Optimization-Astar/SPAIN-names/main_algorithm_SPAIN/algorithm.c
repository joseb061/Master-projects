
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h> // For computing the weigths of each segment
# include <limits.h> // Defines infinities.


# include "structures.h"
# include "functions_astar_nb.h"

#define number_nodes_SPAIN 23895681
// #define inputfilename "/home/joseba/Master/Data/Spain.bin"


// typedef struct{
//     unsigned long id; // In 32 bits we need to replace it for long long int id;
//     char *name; // esto tendremos que arreglarlo
//     double latitude,longitude;
//     unsigned short numbersegments;
//     unsigned long *segment;
// }node;

// void ExitError(const char *miss, int errcode) {
//     fprintf (stderr, "\nERROR: %s.\nStopping...\n\n", miss); exit(errcode);
// }

int main(){
    
    node *nodes;
    unsigned long size_nodes = number_nodes_SPAIN;
	
    /* getting memory for all data */
    if((nodes = (node *) malloc(size_nodes* sizeof(node))) == NULL)
        ExitError("when allocating memory for the nodes vector", 13);
     
   
    if((binary_reader(nodes)) == 0) {
        printf("The graph has been readed and stored\n");
    }
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
    
    // Finaly we reconstruct the path and print it
    unsigned long v = node_goal_pos ,pv, ppv;
    //PathData[v].parent = 1000000000;
    pv = PathData[v].parent;
    
    while (v != node_start){ // Reconstructs the path backwards.
        ppv = PathData[binarysearch(pv,nodes,size_nodes)].parent; 
        PathData[binarysearch(pv,nodes,size_nodes)].parent = v;
        v = pv;
        pv = ppv;
    }
        
    //--------------- TXT FOR RESULTS -------------
    FILE *sol_save_txt;
    if ((sol_save_txt = fopen ("RESULTS.txt", "w")) == NULL)
        ExitError("the output binary data file cannot be opened", 31);
    
    for (v = PathData[node_start_pos].parent; v != node_goal_pos; v = PathData[binarysearch(v,nodes,size_nodes)].parent){
        fprintf(sol_save_txt,"\nNode Id: %3.3lu\t|\tDistance:  %7.3f\t|\tNode_latitude: %7.5f\t|\tNode_longitude:  %7.5f\t|\tNode_streets: %s", v, PathData[binarysearch(v,nodes,size_nodes)].g, nodes[binarysearch(v,nodes,size_nodes)].latitude, nodes[binarysearch(v,nodes,size_nodes)].longitude, nodes[binarysearch(v,nodes,size_nodes)].name);           
    }
     fclose(sol_save_txt); 
    
    
    
    /* --------------- BINARY FILE OF RESULTS -------------*/
    // FILE *sol;
    // if ((sol = fopen ("pathsolution.bin", "wb")) == NULL)
    //     ExitError("the output binary data file cannot be opened", 31);    
    
    // for (v = PathData[node_start_pos].parent; v != node_goal_pos; v = PathData[binarysearch(v,nodes,size_nodes)].parent){
    //     printf("(%3.3lu) | %7.3f | %7.5f | %7.5f\n", v, PathData[binarysearch(v,nodes,size_nodes)].g, nodes[binarysearch(v,nodes,size_nodes)].latitude, nodes[binarysearch(v,nodes,size_nodes)].longitude );
    //     if(fwrite(&nodes[binarysearch(v,nodes,size_nodes)].latitude, sizeof(unsigned long), 1, sol) + fwrite(&nodes[binarysearch(v,nodes,size_nodes)].longitude, sizeof(unsigned long), 1, sol) != 2 ){
    //         ExitError("when initializing the output binary data file", 32);}
    // }
    // fclose(sol);
    // printf("----------|---------|---------|---------\n");
    // printf("Node id | Distance | Latitude | Longitude\n");
    
    //--------------- TXT FILE OF RESULTS GOOGLE EARTH -------------

    // FILE *soltxt;

    // if ((soltxt = fopen ("pathsolution.csv", "w")) == NULL)
    //     ExitError("the output binary data file cannot be opened", 31);
    
    // for (v = PathData[node_start_pos].parent; v != node_goal_pos; v = PathData[binarysearch(v,nodes,size_nodes)].parent){
    //     fprintf(soltxt,"%lf,%lf\n", nodes[binarysearch(v,nodes,size_nodes)].latitude, nodes[binarysearch(v,nodes,size_nodes)].longitude);
    // }
    // fclose(soltxt);
        
    return 0;

}