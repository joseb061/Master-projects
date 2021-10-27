//
//  main.c
//  AStar_algorithm
//



#include "main_functions.h"
#define GraphOrder 21


// Here the AStar's algorithm is defined. It goes iteratively to the nearest neighbour, and if the distance trough the node plus the expeted one to the expected node is smaller than the other options, it saves it as a new edge of the sapanning tree. Once it finishes one brach, it goes through the previous non-expanded node.

int main(int argc, char *argv[]) {
    graph_vertex Graph[GraphOrder] = {
        {'A', 3, { {1, 0.528}, {2, 0.495}, {3, 0.471} }},
        {'B', 2, { {0, 0.528}, {3, 0.508} }},
        {'C', 4, { {0, 0.495}, {3, 3.437}, {5, 12.033}, {15, 34.852} }},
        {'D', 4, { {0, 0.471}, {1, 0.508}, {2, 3.437}, {4, 23.155} }},
        {'E', 4, { {3, 23.155}, {5, 6.891}, {6, 4.285}, {7, 0.520} }},
        {'F', 2, { {2, 12.033}, {4, 6.8910} }},
        {'G', 3, { {4, 4.285}, {7, 0.630}, {8, 17.406} }},
        {'H', 2, { {4, 0.520}, {6, 0.630} }},
        {'I', 5, { {6, 17.406}, {9, 6.657}, {10, 15.216}, {11, 10.625}, {12, 17.320} }},
        {'J', 2, { {8, 6.657}, {12, 16.450} }},
        {'K', 2, { {8, 15.216}, {14, 12.373} }},
        {'L', 2, { {8, 10.625}, {12, 3.618} }},
        {'M', 3, { {8, 17.320}, {9, 16.450}, {11, 3.618} }},
        {'N', 2, { {14, 4.450}, {19, 6.450} }},
        {'O', 4, { {10, 12.373}, {13, 4.450}, {15, 16.178}, {19, 5.203} }},
        {'P', 5, { {2, 34.852}, {14, 16.178}, {16, 4.818}, {18, 3.877}, {19, 19.131} }},
        {'Q', 3, { {15, 4.818}, {17, 3.199}, {18, 2.976} }},
        {'R', 2, { {16, 3.199}, {18, 20.832} }},
        {'S', 4, { {15, 3.877}, {16, 2.976}, {17, 20.832}, {20, 2.510} }},
        {'T', 4, { {13, 6.450}, {14, 5.203}, {15, 19.131}, {20, 13.313} }},
        {'U', 2, { {18, 2.510}, {19, 13.313} }}
    };
    
    AStarPath PathData[GraphOrder];
    unsigned node_start = 0U, node_goal = 20U;
    
    bool r = AStar(Graph, PathData, GraphOrder, node_start, node_goal);
    
    if (r == -1)
        ExitError("in allocating memory for the OPEN list in AStar", 21);
    else
        if (!r)
            ExitError("no solution found in AStar", 7);
    
    register unsigned v = node_goal, pv = PathData[v].parent, ppv;
    PathData[node_goal].parent = UINT_MAX;
    
    while (v != node_start) { // Reconstructs the path backwards.
        ppv = PathData[pv].parent;
        PathData[pv].parent = v;
        v = pv;
        pv = ppv;
    }
    
    printf("Optimal path found:\n");
    printf("Node name | Distance\n");
    printf("----------|---------\n");
    printf(" %c (%3.3u) | Source\n", Graph[node_start].name, node_start);
    
    for (v = PathData[node_start].parent; v != UINT_MAX; v = PathData[v].parent)
        printf(" %c (%3.3u) | %7.3f\n", Graph[v].name, v, PathData[v].g);
}
