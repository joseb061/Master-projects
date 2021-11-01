#ifndef structures_h
#define structures_h

# define MAXSEGMENTS 9

// We define the boolean
typedef char bool;
enum {false, true};

// Here are all the structures used:
//Graph structure
typedef struct{
    unsigned long id; // Street Id
    char *name; // Street name
    unsigned nodeposition; // Position in nodes vector of the destination of the segment
}segmentinfo;

typedef struct{
    unsigned long id; // In 32 bits we need to replace it for long long int id;
    double latitude,longitude;
    int numbersegments;
    segmentinfo segment[MAXSEGMENTS];
}node;

// Structure with value of g and parent.
typedef struct {
    float g;
    unsigned parent;
} AStarPath;

typedef struct QueueElementstruct {
    unsigned v;
    struct QueueElementstruct *seg;
} QueueElement;

typedef QueueElement * PriorityQueue;

//To save memory we only store the f-values separately. Tha value of h = f -g will then have to be computed from h and g.
typedef struct {float f; bool IsOpen;} AStarControlData;

#endif