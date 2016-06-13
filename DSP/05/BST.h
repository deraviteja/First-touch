// BST.h
// file having objects and operation functions
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "node.h"

#ifndef __lis2__
#define __lis2__ 

//typedef struct newnode Node;
typedef struct newoneBST{
Node * root;
} Bst;

Node* insertbst(Bst * T, int element);
Node* searchbst(Bst * T, int element);
Bst* createBST();
int printBST(Node* L);


//node* isleft(node* K);
//node* isright(node* K);
//int deletebst(BST* T, int element);
//Node* maximum(BST* T);
//Node* minimum(BST* T);
//Node* successor(BST* T);

#endif
