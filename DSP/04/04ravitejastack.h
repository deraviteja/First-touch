#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#ifndef __04raviteja__
#define __04raviteja__ 

KeyList* Key(ListItem * item);
void itemToString(ListItem* item,char *buffer,unsigned int maxbufferSize);
int pop(List *L);
int push(List *L, char x);
ListItem* top(List *L);
List* Newstack(); 
int isEmpty(List *L);

#endif
