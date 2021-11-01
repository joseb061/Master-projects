/* The AStar algorithm with the functions used on it */
#ifndef functions_astar_nb_h
#define functions_astar_nb_h

#include <math.h> // For computing the weigths of each segment
#include <limits.h> // Defines infinities.
# define pi 3.14159265358979323846


void ExitError(const char *miss, int errcode) {
    fprintf (stderr, "\nERROR: %s.\nStopping...\n\n", miss); exit(errcode);
}


/* ------------------- Functions for calculating the weights
                    https://www.geodatasource.com/developers/c 
                        LEY ESFERICA DEL COSENO               --------------------- */
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
    
    if ((Q = (AStarControlData *) malloc(GrOrder*sizeof(AStarControlData))) == NULL)
        ExitError("when allocating memory for the AStar Control Data vector", 73);
    
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