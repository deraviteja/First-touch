//all avl functions
#include "node.h"
#include "BST.h"

#ifndef __try__
#define __try__

typedef struct newoneAVL{
Node * root;
} Avl;

int updateheight(Node *k, Avl* L);
Node* insertAVL(Avl *L, int element);
int balance(Node* L);
Node* searchAvl(Avl *L, int element);
Avl* createAVL();

#endif
