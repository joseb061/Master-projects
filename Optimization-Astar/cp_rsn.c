/*This programme aims to store the
nodes in a vector of node structures of length 23895681
this structure will contain all the info about the node.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

# define MAXSEGMENTS 15
# define UMAX 432444444

typedef struct{
    char carrer[12]; // Street Id (could be the name)
    unsigned nodeposition; // Position in nodes vector of the destination of the segment
}segmentinfo;

typedef struct{
    long int id; // In 32 bits we need to replace it for long long int id;
    double latitude,longitude;
    int numbersegments;
    segmentinfo segment[MAXSEGMENTS];
}node;

//nodesearch(ident, nodes, nnodes) it returns the idex of the node given. 
unsigned nodesearch(long int ident, node l[], int n){
    unsigned i;
    for (i=0; i<n && (l[i].id!=ident); i++);
        if (i<n) return i;
    return UMAX;
}

unsigned nodesearch(long int ident, node l[],int n);

int main(){
    FILE *data; 
    node *nodes;
    // Setting number of nodes and edges
    char *row = NULL, *ptr, *token;
    size_t row_size = 0;
    long int size_nodes=23895681; // real_number of characters.;
    int nnodes = 0, nways = 0, prueba =2895681 ;
    
    data=fopen("spain.csv","r");
    if(data==NULL)
    {
        printf("\nWe cannot acces to the nodes data file\n");
        return 1;
    }

    // We reserve the memory for the nodes
    if((nodes=(node *)malloc(prueba*sizeof(node)))==NULL)
    {
        printf("Error in the memory allocation of nodes.\n");
        return 2;
    }

    getline(&row,&row_size,data);   
    for (long int h=0; h< prueba; h++){
        token = strsep(&row, "|");
        if(strcmp(token,"node")==0){
            nodes[nnodes].id  = strtoul(strsep(&row,"|"), &ptr, 10);
            for(int j=0;j<7;j++){
                strsep(&row,"|");
            }
            nodes[nnodes].latitude = atof(strsep(&row,"|"));
            nodes[nnodes].longitude = atof(strsep(&row,"|"));
            //printf("%ld\n",nodes[nnodes].id);
            //printf("%f\n",nodes[nnodes].latitude);
            //printf("%f\n",nodes[nnodes].longitude);
            nnodes++;
        } 
        else if(strcmp(token,"way")==0){
            nways++;
        }
        
        row = NULL;
        //printf("%d\n", nnodes);
        getline(&row,&row_size,data);
    }
    fclose(data);
    printf("%ld\n",nodes[23].id);
}
