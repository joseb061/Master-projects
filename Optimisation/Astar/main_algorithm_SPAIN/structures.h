# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h> // For computing the weigths of each segment
# include <limits.h> // Defines infinities.


#ifndef structures_h
#define structures_h


typedef enum { false, true } bool; // Declaring boolean variables.

// Structure with value of g and parent.
typedef struct {
    float g;
    unsigned long parent;
} AStarPath;

typedef struct QueueElementstruct {
    unsigned v;
    struct QueueElementstruct *seg;
} QueueElement;

typedef QueueElement * PriorityQueue;

//To save memory we only store the f-values separately. Tha value of h = f -g will then have to be computed from h and g.
typedef struct {float f; bool IsOpen;} AStarControlData;

#endif