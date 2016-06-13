// Btree functions in complete
#include "Node.h"
#include <stdio.h>
#include <stdlib.h>
#include "BTree.h"

extern int m;
//1.0--------------------------------
BTree* createBtree(int k){
	     //printf("1.0 \n");
             BTree* T;
             T = malloc(sizeof(BTree));
             T->order = k;
	     m = k;  // Fetching the value of the order of the tree and assigning to the global variable m
	     return T;
	 }
// ------------------------------

//2.0--------------------------------------------------
int insertBtree(int key, BTree* T){
             keys* k = createElem(); // element is created
             Node* L ;
	     L = T->root;
             k->key1 = key;
             firstinsert(k, L, T);
             //printf("root %d \n",T->root->keez[0]->key1);   
                      return 0; 	
      }  
//-----------------------------------------------------

//3.0---------------------------------------------------------	
// Function to find the leaf node where insertion is supposed to take place	for the first time  
int firstinsert(keys* k, Node* L, BTree *T){
                 int x = k->key1;
                 //int r = 0; 
                 int p;
		 Node* innode;
		 innode= L;
		 Node* leaf ;
		 leaf = L;
		 
         while(leaf != NULL ){
             p =pos_in_node(leaf,x);
             innode = leaf;
            // if(leaf == innode->keez[p-1]->rightchild)
              //printf("Havoc \n");
			 if(p==0)
                 leaf = innode->keez[p]->leftchild;
             else
                 leaf = innode->keez[p-1]->rightchild;
             
         }
         
        
         leaf =innode;
         inserthere(k, leaf, T); // insert in the leaf node and then recursively go up if required .. Do this in the invoked function itself
               
		 return 0;
	}
//-------------------------------------------------		 

//4.0-----------------------------------------------------------------
// function for inserting an element (key)  anywhere in the given internal node L
int inserthere(keys* k, Node* L, BTree* T){
            // printf("4.000 \n");
         int x = k->key1;
         int p; 
		 int y;
		 
		 if(L==NULL){  // If its a root node that it entered previously.. This is the root node now
		     L = createNode();
		     L->keez[0]= k;
		     k->mynode = L;
			 T->root = L;
			 return 0;
			 }
		 
		 p = pos_in_node(L, x); // Get the position on where to insert
		 y = p; // Store this position in y
                 //printf("inserting at %d \n",p);
                
                 //if(L->keez[p]== NULL)
                  //printf("hereiam \n");
         while(L->keez[p] !=NULL)
             p++;
		
         while(y!=p){ // Shift the rest of the keys greater than 'key' by 1 step to create space for new key
            L->keez[p] = L->keez[p-1];
            p=p-1; // Keys can be till NULL pointer or m-1
             }

		 L->keez[y]= k;
        	 if(y != 0 ){
              L->keez[y-1]->rightchild = k->leftchild; // If inserted is first element then nobody else shares the left child
                     
		     L->keez[y]->mynode = L->keez[y-1]->mynode; // Tell the key which node it is in 
         	 }

         if(y<m){
		 if(L->keez[y+1] != NULL){
		     L->keez[y+1]->leftchild = k->rightchild;	// If inserted is last element then nobody else shares the right child
                     //printf("hellothere11 \n");
		     L->keez[y]->mynode = L->keez[y+1]->mynode; // Tell the key which node it is in 
                     //printf("Yahoo23 \n");
		 }
		 }
         if(L->keez[m] != NULL){  
                  splitfullnode(L,T);
                }      
	 return 0;
     }	
//--------------------------------------------------------------------------------------------
	 
//5.0---------------------------------------------------------------------------------------------	
int splitfullnode(Node* L, BTree* T){
                // printf("5.0 /n");
	         Node* L1;
             int i =0;  
			 keys* k;
	         L1 = createNode(); // CREATION OF NEW NODE OCCURS
             int en  = (m)/2 ;
		     
                         for(i=0; i<en; i++){
		         L1->keez[i] = L->keez[i]; 
			     L1->keez[i]->mynode =  L1; 
				 }
				 for(i=0;i<en; i++){
     	         L->keez[i] = L->keez[en+1+i]; 
			     L->keez[i]->mynode =  L; 
 			     }
		     
			 k = L->keez[en];
             L->keez[en] = NULL;			 // The chosen key is in between
			 
			 if(m%2){ 
	             L->keez[en] = L->keez[m];
                 L->keez[en]->mynode = L;
				 L->keez[m] = NULL;
                      }  // Consider for cases of m odd or even. If even then second one has more 
                 
				 for(i=0;i<en; i++){
     	         L->keez[en+1+i] = NULL;  
			     }
		                     
             
             k->rightchild = L; // Attaching the bigger node to right
             k->leftchild = L1; // Attaching the smaller node to left

			  if(L->parent != NULL)		  
                 k->mynode = L->parent->mynode; // Tell the key where k will be from now on, if the split was not in the root node		  
		     else
		         k->mynode = NULL; // The full node was a root node
    	     L->parent = k;    
	         L1->parent = k; // Map the parent key	to the chosen key
		  
             
		  

             inserthere(k, k->mynode, T); // Send k to the Node where the earlier parent was
		 
		 return 0;	 
	 }
//-------------------------------------------------------------------------------------	
	
//----------------------------------------------------------------------------------	
// Gives the position in a given Node. All keys< key 
// Any node has at most m-1 keys
//6.0:::::::::::::::::::::::::::::::::::::::::::::::::
int pos_in_node(Node* L, int key){
          //printf("always\n");
         int position =0;
	     //int i=0;
          int start,end,mid;
             start=0;
             end = m-1;
             
	     //keys* elem =L->keez[i];
              keys* elem;
          while(start < end-1){
             mid = (start +end)/2;
            elem = L->keez[mid];
               
             if(elem == NULL){
               end = mid;
               }
              else{
                  if(elem->key1 == key)
                       return position;
                  else if(elem->key1 >key)
                         start = mid;
                 else
                   end = mid;
                 }
             } 

         if(L->keez[start] == NULL)
              return 0;
         if(L->keez[end] == NULL){            
         if( L->keez[start]->key1 >= key)
            return start;
          else
              return end;
          }
         else{
             if( L->keez[start]->key1 >= key) 
                   return start;
             else if ( L->keez[end]->key1 >= key)
           return (end+1);
              else
                return end;
            }
         /*if(elem != NULL){
         while((key) > (elem->key1) && i!=m ){
             position ++;
	         i++;
		     elem = L->keez[i];
		     if(elem == NULL)
             break;		// If node has only k keys, then break if reached k+1.
		 }
                 }
	     return position;*/
	 }	 
//-----------------------------------------------------------------------------------	 

// 7.0---------------------------------------------------------------------------------
int deletenode(Node* L){
	     free(L);
             return 0;
	 }
//-----------------------------------------------------------------------------------	 

// 8.0---------------------------------------------------------------------------------
int searchintree(int keyring, BTree* T){
if(T == NULL) return 0;
searchnode(keyring, T->root);
//printf("here8.2 \n");
return 0;
}
//-----------------------------------------------------------------------------------	 

// 9.0---------------------------------------------------------------------------------

int searchnode(int keyring, Node* L){

int x;
//int r = 0;
while(L != NULL ){

x = pos_in_node(L,keyring);
printf("%d\n",x);
if(L->keez[x]==NULL){
printf("flag1 \n");
L = L->keez[x-1]->rightchild;
}
else if( L->keez[x]->key1 == keyring ){
L=NULL;
return 0;
}
else{
L = L->keez[x]->leftchild;
}
//r++;
}
return 0;
}


	 
