#ifndef graph_construction_functions_h
#define graph_construction_functions_h

/* ---------------------- strsep function for splitting the lines --------------------- */
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

/* -------------------------- implementation of binary search ------------------------------- */
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
        return 0;
    }
}

/* -------------------- function for reading and storing the file information ----------------------- */
void file_reader(node *nodes, FILE *data, long int size_nodes){
    int node_num = 0, nways = 0;
    char *take_row = NULL, *token, *ptr, *row;
    size_t row_size = 0;
    // For saving the edges information
    char *streetname, *oneway;
    long int streetid;
    unsigned long previousnodeid, actualnodeid;
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
        } else if(strcmp(token,"way")==0){ // Taking edges information
            nways++;
            
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
    if((node_num==3472620) & (nways==201235)) printf("\n All nodes are readed. Number of nodes: %d, number of ways %d.\n", node_num, nways);
    else printf("Warning: Not all nodes or ways are readed from the file");
}


#endif

