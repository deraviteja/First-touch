//
#include <stdio.h>
#include <stdlib.h>

#ifndef __isanode__
#define __isanode__

typedef struct element{
     int key1;
     struct newnode* rightchild; // Whats in right
     struct newnode* leftchild ; // Whats in left
     struct newnode* mynode; // Which node am I in
     } keys; 

typedef struct newnode{
     keys* parent; //Whose my parent
     keys* keez[0]; // What all do i have
	 } Node;

Node* createNode();
keys* createElem();	 	 

#endif


