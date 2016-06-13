//all AVL functions in descriptive way
#include "node.h"
#include "BST.h"
#include "AVL.h"

/////////////////////////////////////////////////////////////////////
 
Node* insertAVL(Avl *T, int element){

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
	 N->height = 1;
     
	 if(N->parent == NULL){             // Updating the root node if this is the first element being inserted.
         T->root = N;
         return N;
     }	
	 
     if(L->key < element) 
         L->right = N;
     else 
         L->left = N;	




	updateheight(N->parent, T);
	return N;
	}

////////////////////////////////////////////////////////////////////
	
int updateheight(Node *k, Avl* L){ // FUNCTION UPDATES HEIGHTS OF EACH TREE AND 
     int x =0;
  
         if(k == NULL)  
           return 0;

	 Node *k1,*k2;

	 if(k->right != NULL)
	     x = k->right->height;
	 
	 if(k->left != NULL){
	     if(x < k->left->height)
	     x = k->left->height;
		}
	 
	 if(k->height == (1+ x))// if there has been no change in height of child node, then no change in ancestor nodes.
	   return 0;
	   
	 k->height = 1 + x;                       // updating height

	                                         // UPDATING TREE TO BE BALANCED
//------------------Left---Start------------------------------	

	 if(balance(k) == 2){                           // RIGHT LEFT, RIGHT RIGHT
	    if(balance(k->left) == -1){ //changed
		    k1 = k->left; // 
            k2 = k->left->right;//
			
            k->left = k2; //
            k2->parent = k; //1 k2 parent was not NULL
			
            k1->right = k2->left; 
            if(k2->left != NULL)
            k2->left->parent = k1; //2 

            k2->left = k1;
            k1->parent = k2; //3 k1 parent was not NULL
 
            k2->height = k1->height;
            k1->height = k1->height - 1;

        }
        
		k1 = k->left;
		k2 = k1->left; //
		
        k->left = k1->right; //
		if(k1->right != NULL)
		k1->right->parent = k;
		
        k1->right = k; //
        k1->parent = k->parent;
        k->parent = k1;
        
        k->height= k->height - 2; 

		
		
		if(k1->parent == NULL)
           	L->root = k1;
			
        else{
            if(k1->parent->left == k)
                k1->parent->left = k1;
            else 
                k1->parent->right = k1;
            }				
        
        return 0;
        }

//------------------Left---end------------CHECK-1.0---------------------	

//------------------Right---start------------------------------		
	if(balance(k) == -2){                           // RIGHT LEFT, RIGHT RIGHT
	    if(balance(k->right) == 1){ //changed
		    k1 = k->right; // 
            k2 = k->right->left;//
            k->right = k2; //
            k2->parent = k; //1
			
            k1->left = k2->right;

            if(k2->right != NULL)
            k2->right->parent = k1; //2

            k2->right = k1;
            k1->parent = k2; //3
 
            k2->height = k1->height;
            k1->height = k1->height -1;

        }
        
		k1 = k->right;
		k2 = k1->right; //
		
        k->right = k1->left; //
		
		if(k1->left != NULL)
		k1->left->parent = k;
		
        k1->left = k; //
        k1->parent = k->parent;
        k->parent = k1;
        
        k->height= k->height - 2;

		
		
		if(k1->parent == NULL)
           	L->root = k1;
			
        else{
            if(k1->parent->left == k)
                k1->parent->left = k1;
            else 
                k1->parent->right = k1;
            }

        return 0;
        }

//------------------Right---end------------------------------	
		
    updateheight(k->parent, L);
    return 0;	
		
	}

//////////////////////////////////////////////////////////////////////////////
int balance(Node *k){
    
    int x1 = 0;
    int x2 = 0;
    if(k->right != NULL)
      x1 = k->right->height;
    if(k->left != NULL)
      x2 = k->left->height;

    return (x2-x1); 	  
	
	}

////////START://searchAvl/////////////////////////////////////////////////////////// 

Node* searchAvl(Avl *T, int element){
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

//////END://searchAvl ///////////////////////////////////////////////////////////////


Avl* createAVL(){                                // CREATING A NEW BST
     Avl* nAVL = malloc(sizeof(Avl));
	 return nAVL; 
     }

///////////////////////////////////////////////////////////////////////////
