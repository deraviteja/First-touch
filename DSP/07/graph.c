#include "graph.h"

AdLNode* newAdjListNode(int des, int wght){
    AdLNode* newNode = malloc(sizeof(AdLNode));
    newNode->fin = des;
    newNode->wgt = wght;
    return newNode;
}

graph* createGraph(int V){	
    int i;
    graph* newgrph = malloc(sizeof(graph));
    newgrph->Vert = V;
    newgrph->array = malloc(V * sizeof(Adlist));
    return newgrph;
}

void addEdge(graph* newgrph, int src, int dest, int weight){
    
	AdLNode* newNode = newAdjListNode(dest, weight);
    newNode->next = newgrph->array[src].head;
    newgrph->array[src].head = newNode;
    newNode = newAdjListNode(src, weight);
    newNode->next = newgrph->array[dest].head;
    newgrph->array[dest].head = newNode;
}

void SSShortestPath(graph* grph, int src, int *pathLength){
    int V = grph->Vert;
    int dist[V];    
 	int i;
    mh* minHeap = createMinHeap(V);
    
	for ( i = 0; i < V; i++){
        dist[i] = INT_MAX;
        minHeap->array[i] = newMinHeapNode(i, dist[i]);
        minHeap->pos[i] = i;
    }
 
    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->pos[src]   = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);
 
    minHeap->size = V;
 
    while (!isEmpty(minHeap))
    {
        mhp* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v; 
        AdLNode* pCrawl = grph->array[u].head;
        while (pCrawl != NULL)
        {
            int v = pCrawl->fin;
 
            if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX && pCrawl->wgt + dist[u] < dist[v]){
                dist[v] = dist[u] + pCrawl->wgt;
                decreaseKey(minHeap, v, dist[v]);
            }
            pCrawl = pCrawl->next;
        }
    }
 	pathLength = dist;
}
 
