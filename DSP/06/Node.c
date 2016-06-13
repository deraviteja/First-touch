// Btree functions in complete
#include "Node.h"
#include <stdio.h>
#include <stdlib.h>

extern int m;

Node* createNode(){
     Node* L;
     L = malloc(sizeof(Node) + (m+1)*sizeof(keys));
	 if(L==NULL)
	 printf("CULPRIT %d\n",m);
     return L;
};

keys* createElem(){
     keys* k;	 
     k = malloc(sizeof(keys));
	 if(k==NULL)
	 printf("CULPRIT \n");
     return k;
}
