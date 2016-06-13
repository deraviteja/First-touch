#include "minpriorityqueue.h"

#ifndef __dw__
#define __dw__

typedef struct AdNode{
    int fin;
    int wgt;
    struct AdNode* next;
} AdLNode;
 

typedef struct AdList{
    AdLNode *head;
} Adlist;

typedef struct grh{
    int Vert;
    Adlist* array;
}graph;

AdLNode* newAdjListNode(int dest, int weight);
graph* createGraph(int V);
void addEdge(graph*, int shr, int dest, int weight);
void SSShortestPath(graph*, int shr,int *pathLength);

#endif
