# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h> // For computing the weigths of each segment
# include <limits.h> // Defines the maxim unsigned long.
# include <float.h> // Defines the maximum float

# include "structures.h"
/* The AStar algorithm with the functions used on it */
#ifndef functions_astar_nb_h
#define functions_astar_nb_h


#define inputfilename "../SPAIN-FUN-NONAME/construct_graph_SPAIN/Spain.bin"
# define pi 3.14159265358979323846
# define rad_earth 6371.0

/* ------------------ Program for reading the graph stored in binary file ----------------------------- */
void ExitError(const char *miss, int errcode) {
fprintf (stderr, "\nERROR: %s.\nStopping...\n\n", miss); exit(errcode);
}


typedef struct{
    unsigned long id; // In 32 bits we need to replace it for long long int id;
    //char *name; // esto tendremos que arreglarlo
    double latitude,longitude;
    unsigned short numbersegments;
    unsigned long *segment;
}node;

int binary_reader(node *nodes){ 
        FILE *graph;
        unsigned long number_nodes =  23895681, ntotnsucc = 47685269;
        unsigned long *allsuccessors;
        
        if ((graph = fopen (inputfilename, "rb")) == NULL)
            ExitError("the data file does not exist or cannot be opened", 11);


        /* Global data −−− header */
        if( fread(&number_nodes, sizeof(unsigned long), 1, graph) + fread(&ntotnsucc, sizeof(unsigned long), 1, graph) != 2 )
            ExitError("when reading the header of the binary data file", 12);

        /* getting memory for all data */
        if( ( allsuccessors = (unsigned long *) malloc(ntotnsucc* sizeof(unsigned long))) == NULL)
            ExitError("when allocating memory for the edges vector", 15);

        /* Reading all data from file */
        if( fread(nodes, sizeof(node), number_nodes, graph) != number_nodes ){
            ExitError("when reading nodes from the binary data file", 17);}
        
        if(fread(allsuccessors, sizeof(unsigned long), ntotnsucc, graph) != ntotnsucc){
            ExitError("when reading sucessors from the binary data file", 18);}

        fclose(graph);

        /* Setting pointers to successors */
        for(long unsigned int i=0; i < number_nodes; i++) if(nodes[i].numbersegments) {
            nodes[i].segment = allsuccessors; allsuccessors += nodes[i].numbersegments;
        }
        return 0;
    
}

/* --------------------------- Implementation of binary search --------------------------- */
unsigned binarysearch(long unsigned int ident, node l[],int n){
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





/* -------------------------------------- Functions for computing the weights ----------------------------- */
double deg2rad(double deg) {
  return (deg * pi / 180);
}

double rad2deg(double rad) {
  return (rad * 180 / pi);
}

/* --------------------------------- Law of spherical cosines --------------------------- */
double cos_weight(float lat1, float lon1, float lat2, float lon2){
    double theta, dist;
    if ((lat1 == lat2) && (lon1 == lon2)) {
        return 0;
    } else{
        theta = lon1 - lon2;
        dist = rad2deg(acos(sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta))))*60.0* 1.1515*1.609344;

        //dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
        //dist = acos(dist);
        //dist = rad2deg(dist);
        //dist = dist * 60 * 1.1515;
        //dist = dist * 1.609344;
        return dist;
    }
}

/* ------------------------------------ Equirectangular projection  ------------------------ */
double equirectangular_dist(float lat1, float lon1, float lat2, float lon2){
    if ((lat1==lat2) & (lon1==lon2))
    {
        return 0;
    } else{
        float x = (deg2rad(lat2-lat1))*cos((deg2rad(lon1+lon2))/2);
        float y = deg2rad(lon2-lon1);
        return rad_earth*(sqrt((x*x)+(y*y)));
    }
}


/* -------------------------------------- Harversine distance -------------------------------- 
double harversine_distance(float lat1, float lon1, float lat2, float lon2){
    double u,v, dist;
    if ((lat1 == lat2) && (lon1 == lon2)) {
        return 0;
    } else{
        u = sin((deg2rad(lat2)-deg2rad(lat1))/2);
        v = sin((deg2rad(lon2)-deg2rad(lon1))/2);
        dist = 2.0*rad_earth*asin((u*u)+(cos(deg2rad(lat1))*cos(deg2rad(lat2))*v*v));
        return dist;
    }
}
*/





/* ------------------------------- Heuristic functions ------------------------ */

/* -------------------------------- The heuristic given -------------------------- 
float heuristic(node *Graph, unsigned vertex, unsigned goal) { // Returns the minimum distance of all the vertexes.
    register unsigned short i;
   float initial_weigth, seg_weight;
  unsigned suc_node;

     if (vertex == goal)
         return 0.0;

     initial_weigth = weight(Graph[vertex].latitude, Graph[vertex].longitude, Graph[Graph[vertex].segment[0]].latitude, Graph[Graph[vertex].segment[0]].longitude);
     float minw = initial_weigth;

     for (i = 1; i < Graph[vertex].numbersegments; i++){
         suc_node = Graph[vertex].segment[i];
         seg_weight = weight(Graph[vertex].latitude, Graph[vertex].longitude, Graph[suc_node].latitude, Graph[suc_node].longitude);
         if (seg_weight < minw)
             minw = seg_weight;
     }
     return minw;
 }
*/

/* ---------------------------------- Our heuristic ---------------------------- 
float heuristic(node *Graph, unsigned vertex, unsigned goal) { // Returns the minimum distance of all the vertexes.
    register unsigned short i;
    float minw, seg_weight, seg_weight_next, seg_weight_goal, initial_weigth;
    if (vertex == goal)
        return 0.0;

    initial_weigth = cos_weight(Graph[vertex].latitude, Graph[vertex].longitude, Graph[Graph[vertex].segment[0]].latitude, Graph[Graph[vertex].segment[0]].longitude);
    initial_weigth += cos_weight(Graph[goal].latitude, Graph[goal].longitude, Graph[Graph[vertex].segment[0]].latitude, Graph[Graph[vertex].segment[0]].longitude);
    minw = initial_weigth;

    unsigned suc_node;
    for (i = 1; i < Graph[vertex].numbersegments; i++){
        suc_node = Graph[vertex].segment[i];
        seg_weight_next = cos_weight(Graph[vertex].latitude, Graph[vertex].longitude, Graph[suc_node].latitude, Graph[suc_node].longitude);
        seg_weight_goal = cos_weight(Graph[goal].latitude, Graph[goal].longitude, Graph[suc_node].latitude, Graph[suc_node].longitude);
        seg_weight = seg_weight_goal + seg_weight_next;
        if (seg_weight < minw)
            minw = seg_weight;
    }
    return minw;
}
*/
/* -------------------------------- Heuristic simplest (mejor resultado por el momento) -------------------------- */
float heuristic(node *Graph, unsigned vertex, unsigned goal) { // Returns the minimum distance of all the vertexes.
    register unsigned short i;
   float initial_weigth, seg_weight;
  unsigned suc_node;

     if (vertex == goal)
         return 0.0;

     initial_weigth = cos_weight(Graph[goal].latitude, Graph[goal].longitude, Graph[Graph[vertex].segment[0]].latitude, Graph[Graph[vertex].segment[0]].longitude);
     float minw = initial_weigth;

     for (i = 1; i < Graph[vertex].numbersegments; i++){
         suc_node = Graph[vertex].segment[i];
         seg_weight = cos_weight(Graph[goal].latitude, Graph[goal].longitude, Graph[suc_node].latitude, Graph[suc_node].longitude);
         if (seg_weight < minw)
             minw = seg_weight;
     }
     return minw;
 }

/* -------------------------------- Heuristic simplest (falla en precision, encuentra un camino muy largo) -------------------------- 
float heuristic(node *Graph, unsigned vertex, unsigned goal) { // Returns the minimum distance of all the vertexes.
    register unsigned short i;
   float initial_weigth, seg_weight;
  unsigned suc_node;

     if (vertex == goal)
         return 0.0;

     initial_weigth = equirectangular_dist(Graph[goal].latitude, Graph[goal].longitude, Graph[Graph[vertex].segment[0]].latitude, Graph[Graph[vertex].segment[0]].longitude);
     float minw = initial_weigth;

     for (i = 1; i < Graph[vertex].numbersegments; i++){
         suc_node = Graph[vertex].segment[i];
         seg_weight = equirectangular_dist(Graph[goal].latitude, Graph[goal].longitude, Graph[suc_node].latitude, Graph[suc_node].longitude);
         if (seg_weight < minw)
             minw = seg_weight;
     }
     return minw;
 } */




/* ------------------------------------------------ Queue functions -------------------------------------------- */
bool IsEmpty(PriorityQueue Pq) { // Returns true or false.
    return ((bool) (Pq == NULL));
}

unsigned extract_min(PriorityQueue *Pq) { // Takes the first element (minumum) and displaces the list to the right).
    PriorityQueue first = *Pq;
    unsigned v = first->v;
    *Pq = (*Pq)->seg;
    free(first);
    return v;
}

bool add_with_priority(unsigned w, PriorityQueue *Pq, AStarControlData * Q) {
    register QueueElement * q;
    QueueElement *aux = (QueueElement *) malloc(sizeof(QueueElement));
    if (aux == NULL)
        return false;
    aux->v = w;
    float costw = Q[w].f;
    Q[w].IsOpen = true; // The vertex is expanded.
    if (*Pq == NULL || !(costw > Q[(*Pq)->v].f)) { // Moves 'Pq' to the right.
        aux->seg = *Pq;
        *Pq = aux;
        return true;
    }
    for (q = *Pq; q->seg && Q[q->seg->v].f < costw; q = q->seg);
    aux->seg = q->seg;
    q->seg = aux;
    return true;
}

void requeue_with_priority(unsigned v, PriorityQueue *Pq, AStarControlData * Q){
    if ((*Pq)->v == v) // Veryfying that v it not the prior one.
        return;
    
    register QueueElement * prepv;
    for (prepv = *Pq; prepv->seg->v != v; prepv = prepv->seg);
    QueueElement * pv = prepv->seg;
    prepv->seg = pv->seg;
    free(pv);
    add_with_priority(v, Pq, Q);
}

/* ------------------------------- AStar main ------------------------ */
unsigned long AStar(node *Graph, AStarPath *PathData, unsigned GrOrder, unsigned node_start, unsigned node_goal, float lambda) {
    //Registers are faster than memory to access, so the variables which are most frequently used in a C program can be put in registers using register keyword
    register unsigned i;
    unsigned long counter=0;
    PriorityQueue Open = NULL; // Open EmptyPriorityQueue
    AStarControlData *Q; 
    
    if ((Q = (AStarControlData *) malloc(GrOrder*sizeof(AStarControlData))) == NULL){
        printf("Error when allocating memory for the AStar Control Data vector");
        return 13;
    }

    for (i = 0; i < GrOrder; i++) { // All node distances set to ingraphit and no-one is opened yet.
        PathData[i].g = 21474836.0; // Maximum float 
        Q[i].IsOpen = false;
    }
    PathData[node_start].g = 0.0;
    PathData[node_start].parent = ULONG_MAX;
    Q[node_start].f = heuristic(Graph, node_start, node_goal);

    if (!add_with_priority(node_start, &Open, Q)) // si tenemos &Open en una función es para modificarlo
        return -1;
    
    while (!IsEmpty(Open)) {
        counter++;
        unsigned node_curr;
        if ((node_curr = extract_min(&Open)) == node_goal) {
            free(Q);
            printf(" Number of iterations: %u \n", counter);
            return counter;
        }

        for (i = 0; i < Graph[node_curr].numbersegments; i++) {
            unsigned node_succ = Graph[node_curr].segment[i];
            if(Graph[node_succ].numbersegments == 0) {continue;}
            // Set new cost from start to current node
            float suc_weight = cos_weight(Graph[node_curr].latitude, Graph[node_curr].longitude, Graph[node_succ].latitude, Graph[node_succ].longitude);
            float g_curr_node_succ = PathData[node_curr].g + suc_weight;

            // Check for better path found
            if (g_curr_node_succ < PathData[node_succ].g) { // It always enter the first time.
                PathData[node_succ].parent = Graph[node_curr].id;

                // We can make fast computation avoiding recomputing the heuristic
                Q[node_succ].f = g_curr_node_succ + ((PathData[node_succ].g == 21474836.0) ? (lambda)*heuristic(Graph, node_succ, node_goal) : (Q[node_succ].f - PathData[node_succ].g));

                PathData[node_succ].g = g_curr_node_succ;
                if (!Q[node_succ].IsOpen) {
                    if (!add_with_priority(node_succ, &Open, Q)){
                        return -1;
                    }
                }
                else{
                    requeue_with_priority(node_succ, &Open, Q);
                    }
            }
        }
        Q[node_curr].IsOpen = false; // Cerrar nodo, ya hemos visitado
    }
    return 0;
}

#endif