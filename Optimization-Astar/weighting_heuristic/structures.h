#ifndef structures_h
#define structures_h

typedef struct{
    unsigned long id; 
    char *name; 
    double latitude,longitude;
    unsigned short numbersegments;
    unsigned long *segment;
}node;

typedef enum { false, true } bool; // Declaring boolean variables.

typedef struct {
    float g;
    unsigned long parent;
} AStarPath;

typedef struct QueueElementstruct {
    unsigned v;
    struct QueueElementstruct *seg;
} QueueElement;

typedef QueueElement * PriorityQueue;

typedef struct {float f; bool IsOpen;} AStarControlData;

#endif