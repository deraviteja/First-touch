#include "minpriorityqueue.h"


mhp* newMinHeapNode(int ver, int dist){
    mhp* minHpNode = malloc(sizeof(mhp));
    minHpNode->v = ver;
    minHpNode->dist = dist;
    return minHpNode;
}

mh* createMinHeap(int capacity){
    mh* minHp = malloc(sizeof(mh));
    minHp->pos = (int *)malloc(capacity * sizeof(int));
    minHp->size = 0;
    minHp->capacity = capacity;
    minHp->array =(mhp**) malloc(capacity * sizeof(mhp*));
    return minHp;
}
 
void swapmhp(mhp** a, mhp** b){
    mhp* t = *a;
    *a = *b;
    *b = t;
}
 
void minHeapify(mh* minHeap, int idx){
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;
 
    if (left < minHeap->size &&
        minHeap->array[left]->dist < minHeap->array[smallest]->dist )
      smallest = left;
 
    if (right < minHeap->size &&
        minHeap->array[right]->dist < minHeap->array[smallest]->dist )
      smallest = right;
 
    if (smallest != idx)
    {
       mhp *smallestNode = minHeap->array[smallest];
       mhp* idxNode = minHeap->array[idx];
 
       minHeap->pos[smallestNode->v] = idx;
       minHeap->pos[idxNode->v] = smallest;
 
       swapmhp(&minHeap->array[smallest], &minHeap->array[idx]);
       minHeapify(minHeap, smallest);
    }
}
 
int isEmpty(mh* minHeap){
    return minHeap->size == 0;
}
 
mhp* extractMin(mh* minHeap){
    if (isEmpty(minHeap))
        return NULL;

    mhp* root = minHeap->array[0];
 
    mhp* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;
	minHeap->pos[root->v] = minHeap->size-1;
    minHeap->pos[lastNode->v] = 0;
    minHeap->size = minHeap->size-1;
    minHeapify(minHeap, 0);
 
    return root;
}
 
void decreaseKey(mh* minHeap, int v, int dist){
    int i = minHeap->pos[v];
    minHeap->array[i]->dist = dist;
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist){
        minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
        swapmhp(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

bool isInMinHeap(mh *minHeap, int v){
   if (minHeap->pos[v] < minHeap->size)
     return true;
   return false;
}

void printArr(int dist[], int n){	
    int i;
    printf("Vertex   Distance from Source\n");
    for ( i = 0; i < n; ++i)
    printf("%d \t\t %d\n", i, dist[i]);
}
