#include "list.h"
#include "04ravitejastack.h"

KeyList* Key(ListItem * item){
        KeyList* K=NULL;
        K=malloc(sizeof(KeyList)+sizeof(int)*1);
        K->totalKeys=1;
        memmove(&K->keys[0],&item->Data[0],sizeof(int));
        return K; 
}

void itemToString(ListItem* item,char *buffer,unsigned int maxbufferSize){
        char s[10];
        int i,l;
        memmove(&i,item->Data,sizeof(int));
        sprintf(s,"%d",i);
        s[9]='\0';
        l=strlen(s);
        l=(l>maxbufferSize)? maxbufferSize:l;
        memmove(buffer,s,l+1);
        buffer[maxbufferSize]='\0';
}

List* Newstack(){
return createList(&itemToString,&Key);
}

int pop(List *L){
    return  delete(L, 0);
}

int push(List *L, char x){
    ListItem *L1 = createListItem(1);
	*(L1->Data) = (unsigned char)x;
	return insert(L,L1,0);
}

ListItem* top(List *L){
return L->Head;
}

int isEmpty(List *L){
if(top(L)==NULL)
return 1;
else 
return 0;
}


 

