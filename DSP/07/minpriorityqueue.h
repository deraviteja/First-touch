#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
 
#ifndef __er__
#define __er__

typedef struct MinHeapNode{
    int  v;
    int dist;
} mhp;
 
typedef struct MinHeap{
    int size;      // Number of heap nodes present currently
    int capacity;  // Capacity of min heap
    int *pos;     // This is needed for decreaseKey()
    mhp **array;
}mh;
 

mhp* newMinHeapNode(int v, int dist);
mh* createMinHeap(int capacity);
void swapmhp(mhp** a, mhp** b);
void minHeapify(mh* minHeap, int idx);
int isEmpty(mh* minHeap);
mhp* extractMin(mh* minHeap);
void decreaseKey(mh* minHeap, int v, int dist);
bool isInMinHeap(mh* minHeap, int v);
void printArr(int dist[], int n);

 #endif

