/* The AStar algorithm with the functions used on it */
#ifndef functions_astar_nb_h
#define functions_astar_nb_h

/* ------------------ Program for reading the graph stored in binary file ----------------------------- */
int binary_reader(node *nodes, segmentinfo *succesor){
    FILE *graph;

    if((graph = fopen("../data/graph.bin","rb")) == NULL){
        printf("The file doesn't exist or not possible to open");
        return 100;
    }

    // + fread(&total_succ, sizeof(long int), 1, graph) != 2
    // Reading the number of nodes and total succesors
    long int size_nodes, total_succ;
    if(fread(&size_nodes, sizeof(long int), 1, graph) != 1 ){
        printf("Error when reading the header of the binary data file");
        return 101;
    }
    // Reserving memory for the required data
    if((nodes = (node *) malloc(size_nodes*sizeof(node))) == NULL){
        printf("Error when allocating memory for the nodes vector");
        return 102;
    }
    
    printf("%ld, %f, %f, %d, \n",nodes[20000].id, nodes[20000].latitude, nodes[20000].longitude, nodes[20000].numbersegments);
    /*
    if((allsuccessors = (segmentinfo *) malloc(total_succ*sizeof(segmentinfo))) == NULL){
        printf("Error when allocating memory for the edges vector");
        return 103;
    }
    */

    // Reading all data from the binary file
    if(fread(nodes, sizeof(node), size_nodes, graph) != size_nodes ){
        printf("Error when reading nodes from the binary data file");
        return 104;
    }

    /*
    for(int i=0; i<size_nodes; i++){
        printf("FLAG");
        if(nodes[i].numbersegments > 0){
            for(int j=0; j<nodes[i].numbersegments; j++){
                printf("FLAG2");
                if(fread(&nodes[i].segment[j], sizeof(segmentinfo), 1, graph) != 1){
                    printf("Error when reading sucessors from the binary data file");
                    return 105;
                }
            }
        }
    }
    */

    fclose(graph);
    return 0;

    /*
    if(fread(allsuccessors, sizeof(segmentinfo), total_succ, graph) != total_succ){
        printf("Error when reading sucessors from the binary data file");
        return 105;
    }

    fclose(graph);
    
    // Setting pointers to successors 
    for(int i=0; i < size_nodes; i++) {
        if(nodes[i].numbersegments) {
            nodes[i].segment = allsuccessors;
            allsuccessors += nodes[i].numbersegments;
        }
    }
    */


}

/* ---------------- implementation of binary search ------------------- */
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

/* -------------------------------------- Functions for computing the weights ----------------------------- */
double deg2rad(double deg) {
  return (deg * pi / 180);
}

double rad2deg(double rad) {
  return (rad * 180 / pi);
}
double weight(float lat1, float lon1, float lat2, float lon2){
    double theta, dist;
    if ((lat1 == lat2) && (lon1 == lon2)) {
        return 0;
    } else{
        theta = lon1 - lon2;
        dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
        dist = acos(dist);
        dist = rad2deg(dist);
        dist = dist * 60 * 1.1515;
        dist = dist * 1.609344;
        return dist;
    }
}

/* ------------------------------- AStar functions ------------------------ */
/* The heuristic used */
float heuristic(node *Graph, unsigned vertex, unsigned goal) { // Returns the minimum distance of all the vertexes.
    register unsigned short i;

    if (vertex == goal)
        return 0.0;

    float initial_weigth;
    initial_weigth = weight(Graph[vertex].latitude, Graph[vertex].longitude, Graph[Graph[vertex].segment[0].nodeposition].latitude, Graph[Graph[vertex].segment[0].nodeposition].longitude);

    float minw = initial_weigth;

    float seg_weight;
    unsigned suc_node;
    for (i = 1; i < Graph[vertex].numbersegments; i++){
        suc_node = Graph[vertex].segment[i].nodeposition;
        seg_weight = weight(Graph[vertex].latitude, Graph[vertex].longitude, Graph[suc_node].latitude, Graph[suc_node].longitude);
        if (seg_weight < minw)
            minw = seg_weight;
    }
    return minw;
}

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
    q->seg = aux; // No idea what will do with these updated 'aux' and 'q'.
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
bool AStar(node *Graph, AStarPath *PathData, unsigned GrOrder, unsigned node_start, unsigned node_goal) {
    //Registers are faster than memory to access, so the variables which are most frequently used in a C program can be put in registers using register keyword
    register unsigned i;
    PriorityQueue Open = NULL; // Open EmptyPriorityQueue
    AStarControlData *Q; 
    
    if ((Q = (AStarControlData *) malloc(GrOrder*sizeof(AStarControlData))) == NULL){
        printf("Error when allocating memory for the AStar Control Data vector");
        return 13;
    }
    
    for (i = 0; i < GrOrder; i++) { // All node distances set to infinit and no-one is opened yet.
        PathData[i].g = 100000.999; //ULONG_MAX == The maximum value for an object of type unsigned long int.
        Q[i].IsOpen = false;
    }
    PathData[node_start].g = 0.0;
    PathData[node_start].parent = 1000000000;
    Q[node_start].f = heuristic(Graph, node_start, node_goal);

    if (!add_with_priority(node_start, &Open, Q)) // si tenemos &Open en una función es para modificarlo
        return -1;
    
    while (!IsEmpty(Open)) {
        unsigned node_curr;
        if ((node_curr = extract_min(&Open)) == node_goal) {
            free(Q);
            return true;
        }
        for (i = 0; i < Graph[node_curr].numbersegments; i++) {
            unsigned node_succ = Graph[node_curr].segment[i].nodeposition;
            float suc_weight = weight(Graph[node_curr].latitude, Graph[node_curr].longitude, Graph[node_succ].latitude, Graph[node_succ].longitude);
            float g_curr_node_succ = PathData[node_curr].g + suc_weight;
            
            if (g_curr_node_succ < PathData[node_succ].g) { // It always enter the first time.
                PathData[node_succ].parent = node_curr;
                Q[node_succ].f = g_curr_node_succ + ((PathData[node_succ].g == 100000.999) ? heuristic(Graph, node_succ, node_goal) : (Q[node_succ].f - PathData[node_succ].g));
                PathData[node_succ].g = g_curr_node_succ;
                
                if (!Q[node_succ].IsOpen) {
                    if (!add_with_priority(node_succ, &Open, Q))
                        return -1;
                }
                else
                    requeue_with_priority(node_succ, &Open, Q);
            }
        }
        Q[node_curr].IsOpen = false; // Cerrar nodo, ya hemos visitado
    }
    return false;
}

#endif