// Example for reading csv files of maps, in the following format:
// There is a file for Nodes where there is a line for each node with the fields:
// NodeId;Latitude;Longitude
// There is a file for Streets where there is a line for each street with the fields:
// Id=StreetId;IdNode_1;IdNode_2; ... ; IdNode_n
// where de number of nodes of each street depends on the street.

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

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
// nodes[0].id=255401013
// nodes[0].latitude=41.42836498
// nodes[0].longitude=2.433424
// nodes[0].numbersegments=3
// nodes[0].segment[0].nodeposition=45   <-- there is a segment between nodes[0] and nodes[45]
// nodes[0].segment[1].nodeposition=5    <-- there is a segment between nodes[0] and nodes[5]
// nodes[0].segment[2].nodeposition=17   <-- there is a segment between nodes[0] and nodes[17]
// ...
// nodes[45].id=1450697823
// nodes[45].segment[2].nodeposition=0  
//
// We need:
// unsigned nodesearch ( long int ident , node l [] , unsigned nnodes )
// such that if we do nodesearch(255401013, nodes, nnodes), it returns 0
// and if we do nodesearch(1450697823, nodes, nnodes), it returns 45


unsigned nodesearch(long int ident, node l[],int n);

int main(){
    FILE *nodesdata;
    node *nodes;
    int c,nnodes=0,i;
    
    nodesdata=fopen("SabadellNodes.csv","r");
    if(nodesdata==NULL)
    {
        printf("\nWe cannot acces to the nodes data file\n");
        return 1;
    }
    // We count the number of nodes
    while((c=fgetc(nodesdata))!=EOF)
    {
        if(c=='\n') nnodes++;
    }
    rewind(nodesdata);
    // We reserve the memory for the nodes
    if((nodes=(node *)malloc(nnodes*sizeof(node)))==NULL)
    {
        printf("Error in the memory allocation of nodes.\n");
        return 2;
    }
    // We read the data in the nodes file and store it in the vector nodes.
    for(i=0;i<nnodes;i++){
        fscanf(nodesdata,"%ld;",&(nodes[i].id));
        fscanf(nodesdata,"%lf;",&(nodes[i].latitude));
        fscanf(nodesdata,"%lf\n",&(nodes[i].longitude));
        nodes[i].numbersegments=0;
    }
    fclose(nodesdata);
    // Let's show the result
    printf("Printing the result of reading the nodes file:\n");
    for(i=0;i<nnodes;i++){
        printf("Id=%010ld Lat=%lf Long=%lf \n",nodes[i].id,nodes[i].latitude,nodes[i].longitude);
    }
    
    FILE *streetdata;
    streetdata=fopen("SabadellStreets.csv","r");
    if(streetdata==NULL)
    {
        printf("\nWe cannot access to the streets data file\n");
        return 3;
    }
    char streetid[12];
    long int nodeid,previousnodeid;
    unsigned previousnode,previousnodeposition;
    while(fgetc(streetdata) !=EOF){
        fscanf(streetdata,"d=%[0-9];",streetid);
        // printf("The street has id=%s\n",streetid);
        // Now we have to read the nodes of the street and keep two in memory: previousnodeid and nodeid.
        fscanf(streetdata,"%ld",&(previousnodeid));
        previousnodeposition=nodesearch(previousnodeid,nodes,nnodes);
        // printf("previousnodeposition=%d\n", previousnodeposition);
        while(fgetc(streetdata)!='\n'){
            fscanf(streetdata,"%ld",&(nodeid));
            previousnode=nodesearch(nodeid,nodes,nnodes);
            //printf("previousnodeposition=%d i previousnode=%d\n", previousnodeposition, previousnode);
            if((previousnodeposition!=UMAX) && (previousnode != UMAX)){
                 nodes[previousnodeposition].segment[nodes[previousnodeposition].numbersegments].nodeposition=previousnode;
                nodes[previousnodeposition].numbersegments++;
                 nodes[previousnode].segment[nodes[previousnode].numbersegments].nodeposition=previousnodeposition;
                nodes[previousnode].numbersegments++;
                previousnodeposition=previousnode;
            }
            // we use nodesearch to store the data to the corresponding node
            // printf("Hi té el node %ld a la posicio %d\n",nodeid,previousnode);
        }
    }
    // We print the nodes with at least one segment to control if it works
    printf("Printing the nodes with at least one segment:\n");
    for(i=0;i<nnodes;i++){
        if(nodes[i].numbersegments != 0){
            printf("Id=%010ld Lat=%lf Long=%lf Number_of_segments=%d\n",nodes[i].id,nodes[i].latitude,nodes[i].longitude,nodes[i].numbersegments);
        }
    }
    return 0;
}


// This is a sequential search, which can be replace by a binary search to optimize it.
unsigned nodesearch(long int ident, node l[],int n){
    unsigned i;
    for (i=0; i<n&&(l[i].id!=ident) ; i++);
    if(i<n) return i;
    return UMAX;
}
