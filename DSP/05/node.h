// node.h file
//Functions: createNode, createBST, createAVL

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#ifndef __lis1__
#define __lis1__ 


struct newnode{
int key;
int height;
struct newnode * parent;
struct newnode * right;
struct newnode * left;
};

typedef struct newnode Node;


Node * createNode();

#endif
