/*  Code for reading the csv file, create the node and store it in a binary file
    Joseba Hernández Bravo y Jorge Vicente Puig 
    Last version 8/11 01:02 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define _GNU_SOURCE

#define outputfilename "/path_to_the_file:/Spain.bin"
#define inputfilename "/path_to_the_file:/spain.csv"



typedef enum { false, true } bool; 

typedef struct{
    unsigned long id;
    char *name; 
    double latitude,longitude;
    unsigned short numbersegments;
    unsigned long *segment;
}node;

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

void ExitError(const char *miss, int errcode) {
    fprintf (stderr, "\nERROR: %s.\nStopping...\n\n", miss); exit(errcode);
}

int binarysearch(long int ident, node l[],int n){
    int first = 0, last = n-1, middle;
    middle = (first + last)/2;

    while(first <= last){
        if(l[middle].id < ident){
            first = middle + 1;
        } 
        else if(l[middle].id == ident){
            return middle;
        } 
        else
        last = middle - 1;
        middle = (first + last)/2;
    }
    return -1;
    
}

int main(){
    FILE *data, *fin;
    node *nodes; 
    size_t row_size = 0;
    bool two_directions = false;
    char *data_string = NULL, *token, *row, *oneway, *streetname; 
    int node_num = 0, way_num=0, nsuccesors =0 ,actualnodeposition, nextnodeposition;
    long unsigned int number_nodes = 23895681, streetid, actualnodeid, nextnodeid;
    register unsigned short i = 0;
    unsigned int N_tokens = 11, aux;

    data = fopen(inputfilename,"r");
    if (data == NULL){
        printf("Can't read the data file");
        return 1;
    }
    
    if ((nodes = (node *)malloc(number_nodes*sizeof(node))) == NULL ){
        printf("Can't allocate memory for the nodes");
        return 2;
    }
    
    while(getline(&data_string,&row_size,data) != -1){
    
        row = data_string;
        token = strsep(&row,"|");
        if (token == NULL)
            break; 
        
        if (strcmp(token , "node") == 0 ){
            nodes[node_num].id = atoi(strsep(&row,"|"));
            nodes[node_num].numbersegments = 0;
            
            //memory for the nodes.succesors
            if ((nodes[node_num].segment = (unsigned long *)malloc(0*sizeof(unsigned long))) == NULL){
                printf("Can't allocate memory for the succesors");
                return 3;
            }

            if ((nodes[node_num].name = (char *)malloc(1*sizeof(char))) == NULL){
                printf("Can't allocate memory for the streetnames");
                return 3;
            }
            

            for (int r = 0; r < (N_tokens-2); r++){
                token = strsep(&row,"|");
                if (r == 7){nodes[node_num].latitude = atof(token);}
                if (r == 8){nodes[node_num].longitude = atof(token);}
            }
            node_num++;         
        }
        
        if (strcmp(token , "way") == 0 ){
            
            streetid = atoi(strsep(&row, "|"));            
            streetname = strsep(&row,"|");
        
            //printf("streetname: %s\n", streetname);

            for (int r= 0; r < 5; r++){
                token = strsep(&row,"|");
            }
            
            if (strlen(token) == 0){two_directions = true;}
            if (strlen(token) != 0){two_directions = false;}
            
            strsep(&row,"|");
            actualnodeid = atoi(strsep(&row,"|"));
            actualnodeposition = binarysearch(actualnodeid,nodes,number_nodes);
            
            //search the first and the each succesor nodes. 
            while (actualnodeposition == -1){
                token = strsep(&row,"|") ;
                aux = binarysearch(atoi(token),nodes,number_nodes);
                actualnodeposition = aux;
                actualnodeid = atoi(token);
            }
            
            while ( (token = strsep(&row, "|")) != NULL){
                
                nextnodeid = atoi(token);
                nextnodeposition = binarysearch(nextnodeid,nodes,number_nodes);
                 
                if (nextnodeposition < 0){continue;}
                
                else{
                    
                    if (two_directions){

                        if ((nodes[actualnodeposition].segment = (unsigned long *)realloc(nodes[actualnodeposition].segment,((nodes[actualnodeposition].numbersegments)+1)* sizeof(unsigned long))) == NULL){
                            printf("Can't allocate more memory for the succesors in bpth ways\n");
                            return 4;
                        }

                        if((nodes[nextnodeposition].segment = (unsigned long *)realloc(nodes[nextnodeposition].segment,((nodes[nextnodeposition].numbersegments)+1)* sizeof(unsigned long))) == NULL){
                            printf("Can't allocate more memory for the succesors in bpth ways\n");
                            return 5;    
                        }

                        nodes[actualnodeposition].segment[nodes[actualnodeposition].numbersegments]=nextnodeposition;
                        nodes[nextnodeposition].segment[nodes[nextnodeposition].numbersegments]=actualnodeposition;
                        nodes[actualnodeposition].numbersegments++;
                        nodes[nextnodeposition].numbersegments++;
                        
                        if ( strlen(streetname) != 0 && strlen(nodes[actualnodeposition].name) > 0){
                            if ((nodes[actualnodeposition].name = (char *)realloc(nodes[actualnodeposition].name, (strlen(nodes[actualnodeposition].name) + strlen(streetname)+2)*sizeof(char))) == NULL){
                                printf("Can't allocate more memory for the names in both ways");
                                return 6;
                            }
                            strcat(nodes[actualnodeposition].name,"|");
                            strcat(nodes[actualnodeposition].name,streetname);      
                        }

                        if(strlen(streetname) != 0 && strlen(nodes[actualnodeposition].name) == 0) {
                            if ((nodes[actualnodeposition].name = (char *)realloc(nodes[actualnodeposition].name, (strlen(nodes[actualnodeposition].name) + strlen(streetname)+1)*sizeof(char))) == NULL){
                                printf("Can't allocate more memory for the names in both ways");
                                return 6;
                                }
                            strcpy(nodes[actualnodeposition].name,streetname);
                        }
                        
                        if (strlen(streetname) == 0){
                            if ((nodes[actualnodeposition].name = (char *)realloc(nodes[actualnodeposition].name, (strlen(nodes[actualnodeposition].name)+2)*sizeof(char))) == NULL){
                                printf("Can't allocate more memory for the names in both ways");
                                return 6;
                                }
                            strcat(nodes[actualnodeposition].name,"|");
                        }                    
                        actualnodeposition=nextnodeposition;                      
                    }

                    if (two_directions == false){
                        
                        if ((nodes[actualnodeposition].segment = (unsigned long *)realloc(nodes[actualnodeposition].segment,((nodes[actualnodeposition].numbersegments)+1)* sizeof(unsigned long))) == NULL){
                            printf("Can't allocate more memory for the succesors");
                            return 8;
                        }

                        nodes[actualnodeposition].segment[nodes[actualnodeposition].numbersegments] = nextnodeposition;
                        nodes[actualnodeposition].numbersegments ++;

                        if (strlen(streetname) != 0 & strlen(nodes[actualnodeposition].name) > 0){
                            if ((nodes[actualnodeposition].name = (char *)realloc(nodes[actualnodeposition].name, (strlen(nodes[actualnodeposition].name) +strlen(streetname)+2)*sizeof(char))) == NULL){
                                printf("Can't allocate more memory for the names in oneway");
                                return 9;
                            }       
                            strcat(nodes[actualnodeposition].name,"|");
                            strcat(nodes[actualnodeposition].name,streetname);
                           
                        }

                        if (strlen(streetname) != 0 & strlen(nodes[actualnodeposition].name) == 0) {
                            if ((nodes[actualnodeposition].name = (char *)realloc(nodes[actualnodeposition].name, (strlen(nodes[actualnodeposition].name) +strlen(streetname)+1)*sizeof(char))) == NULL){
                                printf("Can't allocate more memory for the names in oneway");
                                return 9;
                            }       
                            strcpy(nodes[actualnodeposition].name,streetname);
                        }

                        if (strlen(streetname) == 0){
                            if ((nodes[actualnodeposition].name = (char *)realloc(nodes[actualnodeposition].name, (strlen(nodes[actualnodeposition].name)+2)*sizeof(char))) == NULL){
                                printf("Can't allocate more memory for the names in both ways");
                                return 6;
                                }
                            strcat(nodes[actualnodeposition].name,"|");
                        }
                        
                        actualnodeposition = nextnodeposition;
                    }

                }
            }
            way_num++; 
        }
        else {continue;} 
    }
    fclose(data);

    /* Computing the total number of successors and char with all the separators*/
    unsigned long ntotnsucc=0, ntotchar=0;
    for(int i=0; i < number_nodes; i++) {
        ntotnsucc += nodes[i].numbersegments;
        ntotchar += strlen(nodes[i].name) + 1;
    }
    printf("nsuc: %lu nchar: %lu\n", ntotnsucc, ntotchar);
    if ((fin = fopen (outputfilename, "wb")) == NULL)
        ExitError("the output binary data file cannot be opened", 31);

    /* Global data −−− header */
    if( fwrite(&number_nodes, sizeof(unsigned long), 1, fin) + fwrite(&ntotnsucc, sizeof(unsigned long), 1, fin) != 2 )
        ExitError("when initializing the output binary data file", 32);

    /* Writing all nodes */
    if( fwrite(nodes, sizeof(node), number_nodes, fin) != number_nodes )
        ExitError("when writing nodes to the output binary data file", 32);
    
    /* Writing sucessors in blocks */
    for(int i=0; i < number_nodes; i++) if(nodes[i].numbersegments) {
        if( fwrite(nodes[i].segment, sizeof(unsigned long), nodes[i].numbersegments, fin) != nodes[i].numbersegments)
            ExitError("when writing edges to the output binary data file", 32);
        }
    
    for(int i=0; i < number_nodes; i++){
        fwrite(nodes[i].name, sizeof(char),strlen(nodes[i].name),fin);
        fwrite("@", sizeof(char),1,fin);
    }

    fclose(fin);
    
    printf("The graph was written in binary");
}



    
