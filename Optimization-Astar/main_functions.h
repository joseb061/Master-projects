//
//  main_functions.h
//  AStar_algorithm
// joseba and jorge


#ifndef main_functions_h
#define main_functions_h


#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // Defines infinities.


typedef char bool;
enum {false, true};
// these two structures are used to create the graf. Unsigned is used to hold integers between [0,inf), e.g. only positive int + 0  
typedef struct {unsigned vertexto; float weight; } weighted_arrow;
typedef struct {char name; unsigned arrows_num; weighted_arrow arrow[5];} graph_vertex;
// Structure with value of g and parent.
typedef struct {float g; unsigned parent; } AStarPath;
typedef struct QueueElementstruct {unsigned v; struct QueueElementstruct *seg;} QueueElement;
typedef QueueElement * PriorityQueue;
//To save memory we only store the f-values separately. Tha value of h = f -g will then have to be computed from h and g.
typedef struct {float f; bool IsOpen;} AStarControlData;
bool AStar(graph_vertex *, AStarPath *, unsigned, unsigned, unsigned);


// Here are all the needed functions defined:

void ExitError(const char *miss, int errcode) {
    fprintf (stderr, "\nERROR: %s.\nStopping...\n\n", miss); exit(errcode);
}

float heuristic(graph_vertex *Graph, unsigned vertex, unsigned goal) { // Returns the minimum distance of all the vertexes.
    register unsigned short i;
    
    if (vertex == goal)
        return 0.0;
    
    float minw = Graph[vertex].arrow[0].weight;
    
    for (i = 1; i < Graph[vertex].arrows_num; i++){
        if (Graph[vertex].arrow[i].weight < minw)
            minw = Graph[vertex].arrow[i].weight;
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

bool add_with_priority(unsigned v, PriorityQueue *Pq, AStarControlData * Q) {
    register QueueElement * q;
    QueueElement *aux = (QueueElement *) malloc(sizeof(QueueElement));
    
    if (aux == NULL)
        return false;
    
    aux->v = v;
    float costv = Q[v].f;
    Q[v].IsOpen = true; // The vertex is expanded.
    
    if (*Pq == NULL || !(costv > Q[(*Pq)->v].f)) { // Moves 'Pq' to the right.
        aux->seg = *Pq;
        *Pq = aux;
        return true;
    }
    for (q = *Pq; q->seg && Q[q->seg->v].f < costv; q = q->seg);
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

bool AStar(graph_vertex *Graph, AStarPath *PathData, unsigned GrOrder, unsigned node_start, unsigned node_goal) {
    //Registers are faster than memory to access, so the variables which are most frequently used in a C program can be put in registers using register keyword
    register unsigned i;
    PriorityQueue Open = NULL; // Open EmptyPriorityQueue
    AStarControlData *Q; 
    
    if ((Q = (AStarControlData *) malloc(GrOrder*sizeof(AStarControlData))) == NULL)
        ExitError("when allocating memory for the AStar Control Data vector", 73);
    
    for (i = 0; i < GrOrder; i++) { // All node distances set to infinit and no-one is opened yet.
        PathData[i].g = ULONG_MAX; //ULONF_MAX == The maximum value for an object of type unsigned long int.
        Q[i].IsOpen = false;
    }
    PathData[node_start].g = 0.0;
    PathData[node_start].parent = UINT_MAX;
    Q[node_start].f = heuristic(Graph, node_start, node_goal);
    
    if (!add_with_priority(node_start, &Open, Q)) // si tenemos &Open en una función es para modificarlo
        return -1;
    
    while (!IsEmpty(Open)) {
        unsigned node_curr;
        
        if ((node_curr = extract_min(&Open)) == node_goal) {
            free(Q);
            return true;
        }
        for (i = 0; i < Graph[node_curr].arrows_num; i++) {
            unsigned node_succ = Graph[node_curr].arrow[i].vertexto;
            float g_curr_node_succ = PathData[node_curr].g + Graph[node_curr].arrow[i].weight;
            
            if (g_curr_node_succ < PathData[node_succ].g) { // It always enter the first time.
                PathData[node_succ].parent = node_curr;
                Q[node_succ].f = g_curr_node_succ + ((PathData[node_succ].g == ULONG_MAX) ? heuristic(Graph, node_succ, node_goal) : (Q[node_succ].f - PathData[node_succ].g));
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


#endif /* main_functions_h */
