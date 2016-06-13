// BST.c
// all function details
#include "node.h"
#include "BST.h"


////////////////////////////////////////////////////////////////////////////////

Node* insertbst(Bst * T, int element){            // INSERTING AN ELEMENT
    
     Node* L = T->root;
	 Node* N = L;
	 
     while(N != NULL){
     L = N;
	 
	 if(L->key == element)
	  return L;
	  
     if(N->key < element)
         N = N->right;
     else
         N = N->left;
	 }
 
     N = createNode(); // creating new node to insert the new element
     N->key = element; // assigning element to the key data type  of the node
     N->parent = L; // L always contains the parent of L
     
	 if(N->parent == NULL){             // Updating the root node if this is the first element being inserted.
         T->root = N;
         return N;
     }	
	 
     if(L->key < element) 
         L->right = N;
     else 
         L->left = N;	

     return N;
 }
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
Node* searchbst(Bst* T, int element){              // SEARCHING FOR A GIVEN ELEMENT
     Node* L = T->root;
	 Node* N = L;
	 
     while(N->key!=element){
     if(N->key<element)
	     N = N->right;
     else
         N = N->left;
	 }
	 
     return N;
}	

////////////////////////////////////////////////////////////////////////////////

Bst* createBST(){                                // CREATING A NEW BST
     Bst* nBST = malloc(sizeof(Bst));
	 return nBST; 
     }
 
////////////////////////////////////////////////////////////////////////////////

int printBST(Node* L){                             // PRINTING THE WHOLE BST IN A SORTED ORDER
     if(L != NULL){
     printBST(L->left);
	 printf("%d \n", L->key);
	 printBST(L->right);
     }
     return 0;
}	 
 	  
////////////////////////////////////////////////////////////////////////////////   
  
 
  
   
   
