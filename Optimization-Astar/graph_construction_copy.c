/* Code for reading the csv file, create the node and store it in a binary file
    Joseba Hernández Bravo y Jorge Vicente Puig 
    Ultima version 29/10 15:16 */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>

# define MAXSEGMENTS 3
# define UMAX 432444444

typedef struct{
    int id; // Street Id
    char *name; // Street name
    unsigned nodeposition; // Position in nodes vector of the destination of the segment
}segmentinfo;

typedef struct{
    long int id; // In 32 bits we need to replace it for long long int id;
    double latitude,longitude;
    int numbersegments;
    segmentinfo segment[MAXSEGMENTS];
}node;

char *strsep(char **stringp, const char *delim) {
    char *rv = *stringp;
    if (rv) {
        *stringp += strcspn(*stringp, delim);
        if (**stringp)
            *(*stringp)++ = '\0';
        else
            *stringp = 0; }
    return rv;
}

/*
//nodesearch(ident, nodes, nnodes) it returns the idex of the node given. 
unsigned nodesearch(long int ident, node l[],int n){
    unsigned i;
    for (i=0; i<n&&(l[i].id!=ident) ; i++);
    if(i<n) return i;
    return UMAX;
}
*/

unsigned binarysearch(long int ident, node l[],int n){
	int first = 0, last = n-1, middle;
	middle = (first + last)/2;

	while(first <= last){
		if(l[middle].id < ident){
			first = middle + 1;
		} else if(l[middle].id == ident){
			return middle;
		} else
		last = middle - 1;
		middle = (first + last)/2;
	}
	if(first > last){
	   //printf("The node %ld doesn't exist! \n", ident);
        return 0;
	}

}


int binary_writting(node *nodes){
    FILE *fin;
    unsigned long nnodes = 3472620;

    // Computing the total number of successors 
    unsigned long ntotnsucc=0UL;
    for(int i=0; i < nnodes; i++) ntotnsucc += nodes[i].numbersegments; 

    // Setting the name of the binary file 
    if ((fin = fopen ("graph.bin", "w")) == NULL){
        printf("the output binary data file cannot be opened");
        return 100;
    }
    
    // Global data −−− header 
    if( fwrite(&nnodes, sizeof(unsigned long), 1, fin) + fwrite(&ntotnsucc, sizeof(unsigned long), 1, fin) != 2 ){
        printf("when initializing the output binary data file");
        return 101;
    }
    
    // Writing all nodes 
    if( fwrite(nodes, sizeof(node), nnodes, fin) != nnodes ){
        printf("Error when writing nodes to the output binary data file");
        return 102;
    }
    
    // Writing sucessors in blocks 
    for(int i=0; i < nnodes; i++) if(nodes[i].numbersegments) {
        if( fwrite(nodes[i].segment, sizeof(unsigned long), nodes[i].numbersegments, fin) != nodes[i].numbersegments ){
            printf("Error when writing edges to the output binary data file");
            return 103;
        }
    }

    printf("The graph has been written in binary");
    fclose(fin);
    return 0;
}


int main(){
	// Execution time
	clock_t tic = clock();

	// Creating and opening the file
	FILE *data;
	data=fopen("cataluna.csv","r");
    if(data==NULL)
    {
        printf("\nWe cannot acces to the nodes data file\n");
        return 1;
    } else printf("\n Data opened \n");

    // Reserving memory for the nodes
    long int total_size_file = 3+3472620+201235; // Lines of cataluna.csv 
    long int size_nodes = 3472620; // Nodes of the cataluna.csv
    node *nodes;
    if((nodes=(node *)malloc(size_nodes*sizeof(node)))==NULL)
    {
        printf("Error in the memory allocation of nodes.\n");
        return 2;
    } else printf("\n Memory reserved for the data. \n");

    // Reading the nodes and edges for constructing the graph structure
    // For reading the lines
    int node_num = 0, nways = 0;
    char *take_row = NULL, *token, *ptr, *row;
    size_t row_size = 0;
    // For saving the edges information
    char *streetname, *oneway;
    long int streetid;
    long int previousnodeid, actualnodeid;
    unsigned previousnodeposition, actualnodeposition;

    while(getline(&take_row,&row_size,data) != -1){
        row = take_row;
        token = strsep(&row, "|");
        // Taking nodes information
        if(strcmp(token,"node")==0){
            nodes[node_num].id  = strtoul(strsep(&row,"|"), &ptr, 10);
            for(int j=0;j<7;j++){
                strsep(&row,"|");
            }
            nodes[node_num].latitude = atof(strsep(&row,"|"));
            nodes[node_num].longitude = atof(strsep(&row,"|"));
            node_num++; 
            if(node_num==3472620) printf("\n All nodes are readed. \n");
        } else if(strcmp(token,"way")==0){ // Taking edges information
        	nways++;
            if(nways==201235) printf("\n All ways are readed. \n");
            
            // Information about the way
            streetid = strtoul(strsep(&row, "|"),&ptr,10);
            streetname = strsep(&row, "|");
            for(int k=0;k<4;k++){
                strsep(&row, "|");
            }
            oneway = strsep(&row, "|");
            strsep(&row,"|");
            // Linking the nodes
            previousnodeid = strtoul(strsep(&row,"|"), &ptr, 10);
            previousnodeposition = binarysearch(previousnodeid,nodes,size_nodes);
            if(previousnodeposition != 0){
	            while((actualnodeid = strtoul(strsep(&row,"|"), &ptr, 10)) != 0){
	            	actualnodeposition = binarysearch(actualnodeid,nodes,size_nodes);
	                if (strcmp(oneway,"oneway") & (actualnodeposition != 0)){
	                    nodes[previousnodeposition].segment[nodes[previousnodeposition].numbersegments].nodeposition=actualnodeposition;
	                    //nodes[previousnodeposition].segment[nodes[previousnodeposition].numbersegments].id=streetid;
	                    //nodes[previousnodeposition].segment[nodes[previousnodeposition].numbersegments].name=streetname;
	                    nodes[previousnodeposition].numbersegments++;
	                    previousnodeposition=actualnodeposition;
	                } else{
	                    //nodes[previousnodeposition].segment[nodes[previousnodeposition].numbersegments].id=streetid;
	                    //nodes[previousnodeposition].segment[nodes[previousnodeposition].numbersegments].name=streetname;
	                    nodes[previousnodeposition].segment[nodes[previousnodeposition].numbersegments].nodeposition=actualnodeposition;
	                    nodes[previousnodeposition].numbersegments++;
	                    nodes[actualnodeposition].segment[nodes[actualnodeposition].numbersegments].nodeposition=previousnodeposition;
	                    nodes[actualnodeposition].numbersegments++;
	                    previousnodeposition=actualnodeposition;
                    }
	            }
	        }
	    } 
    }
    fclose(data);
    printf("\n The data is closed.\n");


    printf("\n Number of nodes: %d     Number of ways %d\n", node_num, nways);

    binary_writting(nodes);

    clock_t tac = clock();
    printf("\n Execution time: %f\n", (double)(tac-tic)/CLOCKS_PER_SEC);

    return 0;
}

