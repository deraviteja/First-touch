// BTree.h includes all functions to be used by Btree
#include <stdio.h>
#include <stdlib.h>
#include "Node.h"

#ifndef __isatree__
#define __isatree__

typedef struct betree{
Node* root;
int order;
} BTree;

BTree* createBtree(int k); //1.0
int insertBtree(int key, BTree* T);//2.0
int firstinsert(keys* k, Node* L, BTree *T); //3.0
int inserthere(keys* k, Node* L, BTree* T);//4.0
int splitfullnode(Node* L,BTree* T );//5.0
int pos_in_node(Node* L, int key); //6.0
int deletenode(Node* L);// 7.0
int searchintree(int keyring, BTree* T);//8.0
int searchnode(int keyring, Node* L);//9.0


#endif

