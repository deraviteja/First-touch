#include "list.h"
#include "04ravitejastack.h"

int main(int argv, char **argc){   
	 List *L = NULL;
         int i=1,j=1; 
         int x = argv;
         char *st;
         L = Newstack(); 

 
        while(x!=1)
        { 
         st = *(argc+argv-x+1);
	       while(((int)(*st))!= 0)
	        {
	          if((*st) == '(')
	           j = push(L,*st); ////to check if operation was successful
		  
		  else if((*st) ==')'){
		  j= pop(L); //to check if operation was successful
           }
            if(j==0){
             printf("NO \n");
             return 0;
             }			 
		  st = st+1;
		  i=i+1;
	        }
         x = x-1;
        }
		if(isEmpty(L) == 1)
	      printf("YES \n");
	    else
              printf("NO \n");
 

    return 0;
}

